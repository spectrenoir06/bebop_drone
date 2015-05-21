

local socket = require "socket"
local struct = require "struct"

new = {}

function new._object(class, o)
   local o = o or {}
   setmetatable(o, { __index = class })
   return o
end

new._mt = {}
function new._mt.__index(table, key)
   local class = _G[key]
   return type(class.initialize) == 'function' and class.initialize or function() return new._object(class) end
end
setmetatable(new,new._mt)

---------------------------------

bebop = {}
function bebop.initialize()
   local o = {
       x        = 1280/2,
       y        = 720/2,
       front    = 50,
       left     = 0,
       right    = 0,
       bottom   = 0,
       largeur  = 30,
       img      = love.graphics.newImage("bebop.jpg"),
       roll     = 0,
       pitch    = 0,
       yaw      = 0,
       gaz      = 0,
       psi      = 0
    }
   return new._object(bebop, o)
end

function bebop:draw()
    love.graphics.setColor( 255, 255, 255, 255)
    love.graphics.draw( self.img,self.x, self.y,  math.rad(90), 1, 1, self.img:getWidth() / 2, self.img:getHeight() / 2)
    if self.front < 75 then
        love.graphics.setColor( 255, 0, 0, 255)
    else
        love.graphics.setColor( 0, 255, 0, 255)
    end
    love.graphics.print( math.ceil(self.gaz).." %", 10, 720/2 - 50)
    love.graphics.rectangle( 'fill', 10, (720/2) - (self.gaz * 5), 50, 10 + (self.gaz * 5))
    love.graphics.circle( 'line', 1280/6 , 350, 200, 100 )
    love.graphics.circle( 'fill', 1280/6 - (self.roll * 1.5), 350 - (self.pitch * 1.5), 50, 100 )
end

-----------------------------------

----------------------------------------------------------------


function love.load()

    drone = new.bebop()
    love.graphics.setBackgroundColor(255, 255, 255)

    udpSocket = assert(socket.udp())
    udpSocket:settimeout(0)
    udpSocket:setsockname('*', 54321)

end

i = 0
j = 0

function love.update(dt)
	i = i + dt
	j = j + dt
    data, msg_or_ip, port_or_nil = udpSocket:receivefrom()
	if data then
        tipe,
        buffer_id,
        sequence,
        size,
        id_project,
        class_piloting,
        cmd,
        flag,
        drone.roll,
        drone.pitch,
        drone.yaw,
        drone.gaz,
        drone.psi = struct.unpack("bbbi4bbi2bbbbbf", data)
        print(
            drone.roll,
            drone.pitch,
            drone.yaw,
            drone.gaz,
            drone.psi
        )
    else
        drone.roll = 0
        drone.pitch = 0
        drone.yaw = 0
        drone.gaz = drone.gaz - dt
        drone.ps = 0
    end


end

function love.draw()
    drone:draw()
end

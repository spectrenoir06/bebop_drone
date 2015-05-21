

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
       img      = love.graphics.newImage("bebop.jpg")
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
    love.graphics.print( math.ceil(self.front).." cm", 1280/2 - 200, 720/2 - 50)
    love.graphics.rectangle( 'fill', 1280/2 - self.img:getWidth() / 2 - self.front + 100, 720/2 - self.largeur / 2 , self.front, self.largeur)
end

function bebop:send(roll, pitch, yaw, gaz)

    s = struct.pack("bbbi4bbi2bbbbbf",
        0x02, -- frame_type
        0x0a, -- buffer_id
        42, -- sequence
        0x14, -- size
        0x01, -- id_project
        0x00, -- class_piloting
        0x02, -- cmd
        0x01, -- flag
        roll, -- roll
        pitch, -- pitch
        yaw, -- yaw
        gaz, -- gaz
        0x00 -- psi
        )
    udpSocket:send(s)
    s = struct.pack("bbbi4bbi2bbbbbf",
        0x02, -- frame_type
        0x0a, -- buffer_id
        42 - 10, -- sequence
        0x14, -- size
        0x01, -- id_project
        0x00, -- class_piloting
        0x02, -- cmd
        0x01, -- flag
        roll, -- roll
        pitch, -- pitch
        yaw, -- yaw
        gaz, -- gaz
        0x00 -- psi
        )
    udpSocket:send(s)

end

-----------------------------------

rserial=io.open("/dev/ttyACM0","r")

if not rserial then
    error("Pas de port serie ouvert")
end

while chaine==nil do
        chaine=rserial:read()
        rserial:flush()
end

----------------------------------------------------------------


function love.load()

    drone = new.bebop()
    love.graphics.setBackgroundColor(255, 255, 255)

    udpSocket = assert(socket.udp())
    udpSocket:settimeout(0)
    udpSocket:setpeername("localhost", 54321)

end

i = 0
j = 0

function love.update(dt)
    j = j + dt

    drone.front = rserial:read('*n')
    rserial:flush()

    if j > 0.02 then
        if (drone.front < 75) then
            drone:send(0, -100, 0, 0)
        end
        j = 0
    end
end

function love.draw()
    drone:draw()
end

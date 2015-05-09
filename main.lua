
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
    love.graphics.setColor( 255, 0, 0, 255)
    love.graphics.print( math.ceil(self.front).." cm", 1280/2 - 200, 720/2 - 50)
    love.graphics.rectangle( 'fill', 1280/2 - self.img:getWidth() / 2 - self.front + 100, 720/2 - self.largeur / 2 , self.front, self.largeur)
end

rserial=io.open("/dev/ttyACM0","r")
while chaine==nil do
        chaine=rserial:read()
        rserial:flush()
end
print(chaine)

----------------------------------------------------------------


function love.load()

    drone = new.bebop()
    love.graphics.setBackgroundColor(255, 255, 255)
end

i = 0

function love.update(dt)
    i = i + dt
    --if (i > 0.10) then
        chaine = rserial:read()
        rserial:flush()
        if chaine then
            drone.front = tonumber(chaine)
        end
        print(chaine)
        i = 0
    --end

end

function love.draw()
    drone:draw()
end

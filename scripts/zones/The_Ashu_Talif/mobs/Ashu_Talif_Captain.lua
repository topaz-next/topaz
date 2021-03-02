-----------------------------------
-- Area: The Ashu Talif (The Black Coffin)
--   NM: Ashu Talif Captain
-----------------------------------
local ID = require("scripts/zones/The_Ashu_Talif/IDs")
-----------------------------------
local entity = {}

entity.onMobSpawn = function(mob)
    mob:setUnkillable(true)
end

entity.onMobFight = function(mob, target)
    -- The captain gives up at <= 20% HP. Everyone disengages
    local instance = mob:getInstance()
    if mob:getHPP() <= 20 and instance:completed() == false then
        instance:complete()
    end

    mob:addListener("WEAPONSKILL_STATE_ENTER", "WS_START_MSG", function(m, skillID)
        -- Vulcan Shot
        if skillID == 254 then
            m:showText(m, ID.text.FOR_EPHRAMAD)
            m:timer(3000, function(mTimer)
                mobTimer:showText(mTimer, ID.text.TROUBLESOME_SQUABS)
            end)
        -- Circle Blade
        elseif skillID == 938 then
            m:showText(m, ID.text.FOR_THE_BLACK_COFFIN)
        end
    end)
end

entity.onMobRoam = function(mob)
    local jumped = mob:getLocalVar("jump")
    local ready = mob:getLocalVar("ready")

    -- Becomes ready when the Crew is engaged. Jump down!
    if ready == 1 and jumped == 0 then
        mob:showText(mob, ID.text.OVERPOWERED_CREW)
        mob:hideName(true)
        mob:entityAnimationPacket("jmp0")
        mob:timer(2000, function(m)
            m:setPos(0, -22, 13, 192)
            m:entityAnimationPacket("jmp1")
            m:showText(mob, ID.text.TEST_YOUR_BLADES)
            m:timer(2000, function(mAnimation)
                mAnimation:hideName(false)
                mAnimation:untargetable(false)
            end)
        end)
        mob:setLocalVar("jump", 1)
    end
end

entity.onMobDisengage = function(mob, target)
end

entity.onMobDeath = function(mob, player, isKiller)
end

return entity

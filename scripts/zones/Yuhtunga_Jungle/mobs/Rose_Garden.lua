-----------------------------------
-- Area: Yuhtunga Jungle
--  Mob: Rose Garden
-----------------------------------
local ID = require("scripts/zones/Yuhtunga_Jungle/IDs")
-----------------------------------
local entity = {}

entity.onMobSpawn = function(mob)
    mob:setLocalVar("timeToGrow", os.time() + math.random(36000, 37800)) -- 10:00:00 to 10:30:00
end

entity.onMobDisengage = function(mob)
    mob:setLocalVar("timeToGrow", os.time() + math.random(36000, 37800)) -- 10:00:00 to 10:30:00
end

entity.onMobRoam = function(mob)
    -- Rose Garden has been left alone for 10.25 hours
    if os.time() >= mob:getLocalVar("timeToGrow") then
        DisallowRespawn(ID.mob.ROSE_GARDEN, true)
        DespawnMob(ID.mob.ROSE_GARDEN)
        DisallowRespawn(ID.mob.VOLUPTUOUS_VILMA, false)
        pos = mob:getPos()
        SpawnMob(ID.mob.VOLUPTUOUS_VILMA):setPos(pos.x, pos.y, pos.z, pos.rot)
    end
end

entity.onMobDeath = function(mob, player, isKiller)
end

entity.onMobDespawn = function(mob)
    if os.time() < mob:getLocalVar("timeToGrow") then
        DisallowRespawn(ID.mob.ROSE_GARDEN, true)
        DisallowRespawn(ID.mob.ROSE_GARDEN_PH, false)
        GetMobByID(ID.mob.ROSE_GARDEN_PH):setRespawnTime(GetMobRespawnTime(ID.mob.ROSE_GARDEN_PH))
    end
end

return entity

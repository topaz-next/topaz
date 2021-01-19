-----------------------------------
-- Area: Buburimu Peninsula
--   NM: Wake Warder Wanda
-----------------------------------
require("scripts/globals/hunts")
require("scripts/globals/status")
-----------------------------------
local entity = {}

entity.onMobInitialize = function(mob)
    mob:setMobMod(tpz.mobMod.MAGIC_COOL, 50) -- just one spell to spam
end

entity.onMobEngaged = function(mob, target)
    mob:setMod(tpz.mod.REGAIN, 25)
end

entity.onMobDisengage = function(mob)
    mob:setMod(tpz.mod.REGAIN, 0)
end

entity.onMobDeath = function(mob, player, isKiller)
    tpz.hunts.checkHunt(mob, player, 260)
end

entity.onMobDespawn = function(mob)
    UpdateNMSpawnPoint(mob:getID())
    mob:setRespawnTime(math.random(3600, 4200)) -- repop 60-70min
end

return entity

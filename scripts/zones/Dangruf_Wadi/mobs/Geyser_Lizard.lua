-----------------------------------
-- Area: Dangruf Wadi (191)
--  Mob: Geyser Lizard
-----------------------------------
require("scripts/globals/hunts")
-----------------------------------
local entity = {}

entity.onMobInitialize = function(mob)
    mob:setMobMod(tpz.mobMod.IDLE_DESPAWN, 180)
end

function onMobSpawn(mob)
    DespawnMob(mob:getID(), 600)
end

entity.onMobDeath = function(mob, player, isKiller)
    tpz.hunts.checkHunt(mob, player, 225)
end

function onMobDespawn(mob)
    mob:setLocalVar("pop", os.time() + (math.random(45, 75) * 60))
end

return entity

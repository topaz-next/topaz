-----------------------------------
-- Area: Castle Oztroja (151)
--  Mob: Cutter
-----------------------------------
local entity = {}

function onMobSpawn(mob)
    mob:setMobMod(tpz.mobMod.CHARMABLE, 1)
end

entity.onMobDeath = function(mob, player, isKiller)
end

return entity

-----------------------------------
-- Area: Horlais Peak
--  Mob: Cottontail
-- BCNM: Tails of Woe
-----------------------------------
require("scripts/globals/status")
-----------------------------------
local entity = {}

entity.onMobSpawn = function(mob)
    mob:setMobMod(tpz.mobMod.CHARMABLE, 1)
end

entity.onMobDeath = function(mob, player, isKiller)
end

return entity

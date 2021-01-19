-----------------------------------
-- Area: Waughroon Shrine
--  Mob: Flayer Franz
-- BCNM: The Worm's Turn
-----------------------------------
require("scripts/globals/status")
-----------------------------------
local entity = {}

function onMobSpawn(mob)
    mob:setMobMod(tpz.mobMod.DRAW_IN, 1)
end

entity.onMobDeath = function(mob, player, isKiller)
end

return entity

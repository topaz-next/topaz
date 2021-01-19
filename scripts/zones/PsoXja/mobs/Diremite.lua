-----------------------------------
-- Area: Pso'Xja
--  Mob: Diremite
-----------------------------------
local ID = require("scripts/zones/PsoXja/IDs")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
end

entity.onMobDespawn = function(mob)
    tpz.mob.phOnDespawn(mob, ID.mob.GYRE_CARLIN_PH, 5, 1800) -- 30 minutes.
end

return entity

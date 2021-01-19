-----------------------------------
-- Area: Bostaunieux Oubliette (167)
--  Mob: Gespenst
-- Note: PH for Manes
-----------------------------------
local ID = require("scripts/zones/Bostaunieux_Oubliette/IDs")
require("scripts/globals/mobs")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
end

entity.onMobDespawn = function(mob)
    tpz.mob.phOnDespawn(mob, ID.mob.MANES_PH, 5, 3600) -- 1 hour
end

return entity

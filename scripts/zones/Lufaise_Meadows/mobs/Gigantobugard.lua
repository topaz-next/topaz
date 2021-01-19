-----------------------------------
-- Area: Lufaise_Meadows
--  Mob: Gigantobugard
-- Note: PH for Megalobugard
-----------------------------------
local ID = require("scripts/zones/Lufaise_Meadows/IDs")
require("scripts/globals/mobs")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
end

entity.onMobDespawn = function(mob)
    tpz.mob.phOnDespawn(mob, ID.mob.MEGALOBUGARD_PH, 10, 3600) -- 1 hour
end

return entity

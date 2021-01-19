-----------------------------------
-- Area: Pashhow Marshlands [S]
--  Mob: Lou Carcolh
-- Note: PH for Nommo
-----------------------------------
local ID = require("scripts/zones/Pashhow_Marshlands_[S]/IDs")
require("scripts/globals/mobs")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
end

entity.onMobDespawn = function(mob)
    tpz.mob.phOnDespawn(mob, ID.mob.NOMMO_PH, 5, 3600) -- 1 hour
end

return entity

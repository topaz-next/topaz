-----------------------------------
-- Area: East Ronfaure [S]
--  Mob: Battrap
-- Note: PH for Goblintrap
-----------------------------------
local ID = require("scripts/zones/East_Ronfaure_[S]/IDs")
require("scripts/globals/mobs")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
end

entity.onMobDespawn = function(mob)
    tpz.mob.phOnDespawn(mob, ID.mob.GOBLINTRAP_PH, 5, 3600) -- 1 hour
end

return entity

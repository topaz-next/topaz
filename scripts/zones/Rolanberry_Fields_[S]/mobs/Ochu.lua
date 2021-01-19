-----------------------------------
-- Area: Rolanberry Fields [S]
--  Mob: Ochu
-- Note: PH for Delicieuse Delphine
-----------------------------------
local ID = require("scripts/zones/Rolanberry_Fields_[S]/IDs")
require("scripts/globals/mobs")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
end

entity.onMobDespawn = function(mob)
    tpz.mob.phOnDespawn(mob, ID.mob.DELICIEUSE_DELPHINE_PH, 10, 3600) -- 1 hour
end

return entity

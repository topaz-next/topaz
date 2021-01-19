-----------------------------------
-- Area: La Vaule [S]
--  Mob: Orcish Augur
-- Note: PH for Ashmaker Gotblut
-----------------------------------
local ID = require("scripts/zones/La_Vaule_[S]/IDs")
require("scripts/globals/mobs")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
end

entity.onMobDespawn = function(mob)
    tpz.mob.phOnDespawn(mob, ID.mob.ASHMAKER_GOTBLUT_PH, 10, 3600) -- 1 hour
end

return entity

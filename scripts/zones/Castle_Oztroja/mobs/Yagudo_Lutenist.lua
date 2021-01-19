-----------------------------------
-- Area: Castle Oztroja (151)
--  Mob: Yagudo Lutenist
-- Note: PH for Yaa Haqa the Profane
-----------------------------------
local ID = require("scripts/zones/Castle_Oztroja/IDs")
require("scripts/globals/mobs")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
end

entity.onMobDespawn = function(mob)
    tpz.mob.phOnDespawn(mob, ID.mob.YAA_HAQA_THE_PROFANE_PH, 5, 3600) -- 1 hour
end

return entity

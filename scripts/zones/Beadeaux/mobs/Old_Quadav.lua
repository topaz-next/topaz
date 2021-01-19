-----------------------------------
-- Area: Beadeaux
--  Mob: Old Quadav
-- PH for Ge'Dha Evileye
-----------------------------------
local ID = require("scripts/zones/Beadeaux/IDs")
require("scripts/globals/mobs")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
end

entity.onMobDespawn = function(mob)
    tpz.mob.phOnDespawn(mob, ID.mob.GE_DHA_EVILEYE_PH, 25, math.random(3600, 7200)) -- 1 to 2 hours
end

return entity

-----------------------------------
-- Area: Konschtat Highlands
--  Mob: Mad Sheep
-- Note: Place holder Stray Mary
-----------------------------------
local ID = require("scripts/zones/Konschtat_Highlands/IDs")
require("scripts/globals/mobs")
require("scripts/quests/tutorial")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
    tpz.tutorial.onMobDeath(player)
end

entity.onMobDespawn = function(mob)
    tpz.mob.phOnDespawn(mob, ID.mob.STRAY_MARY_PH, 5, math.random(300, 3600)) -- 5-60 minutes
end

return entity

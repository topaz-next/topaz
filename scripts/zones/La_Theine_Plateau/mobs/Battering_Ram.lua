-----------------------------------
-- Area: La Theine Plateau
--  Mob: Battering Ram
-----------------------------------
local ID = require("scripts/zones/La_Theine_Plateau/IDs")
-----------------------------------
require("scripts/globals/mobs")
require("scripts/quests/tutorial")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
    tpz.tutorial.onMobDeath(player)
end

entity.onMobDespawn = function(mob)
    if not tpz.mob.phOnDespawn(mob, ID.mob.BLOODTEAR_PH, 10, math.random(75600, 86400)) then -- 21-24 hours
        tpz.mob.phOnDespawn(mob, ID.mob.LUMBERING_LAMBERT_PH, 10, 1200) -- 20 min
    end
end

return entity

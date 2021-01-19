-----------------------------------
-- Area: Dynamis - Buburimu
--  Mob: Vanguard Chanter
-----------------------------------
mixins =
{
    require("scripts/mixins/dynamis_beastmen"),
    require("scripts/mixins/job_special")
}
local ID = require("scripts/zones/Dynamis-Buburimu/IDs")
require("scripts/globals/mobs")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
end

entity.onMobDespawn = function(mob)
    tpz.mob.phOnDespawn(mob, ID.mob.REE_NATA_THE_MELOMANIC_PH, 10, 1200) -- 20 minutes
end

return entity

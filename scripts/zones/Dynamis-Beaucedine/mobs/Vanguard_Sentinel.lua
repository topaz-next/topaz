-----------------------------------
-- Area: Dynamis - Beaucedine
--  Mob: Vanguard Sentinel
-----------------------------------
mixins =
{
    require("scripts/mixins/dynamis_beastmen"),
    require("scripts/mixins/job_special")
}
local ID = require("scripts/zones/Dynamis-Beaucedine/IDs")
require("scripts/globals/mobs")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
end

entity.onMobDespawn = function(mob)
    tpz.mob.phOnDespawn(mob, ID.mob.XAA_CHAU_THE_ROCTALON_PH, 10, 1200) -- 20 minutes
end

return entity

-----------------------------------
-- Area: Castle Oztroja (151)
--  Mob: Yagudo Theologist
-- Note: PH for Moo Ouzi the Swiftblade
-----------------------------------
local ID = require("scripts/zones/Castle_Oztroja/IDs")
require("scripts/globals/mobs")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
end

entity.onMobDespawn = function(mob)
    tpz.mob.phOnDespawn(mob, ID.mob.MOO_OUZI_THE_SWIFTBLADE_PH, 5, 3600) -- 1 hour
end

return entity

-----------------------------------
-- Area: Temple of Uggalepih
--  Mob: Torama
-- Note: PH for Flauros
-----------------------------------
local ID = require("scripts/zones/Temple_of_Uggalepih/IDs")
require("scripts/globals/mobs")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
end

entity.onMobDespawn = function(mob)
    tpz.mob.phOnDespawn(mob, ID.mob.FLAUROS_PH, 10, 3600) -- 1 hour
end

return entity

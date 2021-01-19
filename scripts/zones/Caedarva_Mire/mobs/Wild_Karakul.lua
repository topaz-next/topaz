-----------------------------------
-- Area: Caedarva Mire
--  Mob: Wild Karakul
-- Note: PH for Peallaidh
-----------------------------------
local ID = require("scripts/zones/Caedarva_Mire/IDs")
require("scripts/globals/mobs")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
end

entity.onMobDespawn = function(mob)
    tpz.mob.phOnDespawn(mob, ID.mob.PEALLAIDH_PH, 5, 3600) -- 1 hour
end

return entity

-----------------------------------
-- Area: Pashhow Marshlands
--  Mob: Veteran Quadav
-- Note: PH for Ni'Zho Bladebender
-----------------------------------
local ID = require("scripts/zones/Pashhow_Marshlands/IDs")
require("scripts/globals/regimes")
require("scripts/globals/mobs")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
    tpz.regime.checkRegime(player, mob, 60, 1, tpz.regime.type.FIELDS)
end

entity.onMobDespawn = function(mob)
    tpz.mob.phOnDespawn(mob, ID.mob.NI_ZHO_BLADEBENDER_PH, 10, 3600) -- 1 hour
end

return entity

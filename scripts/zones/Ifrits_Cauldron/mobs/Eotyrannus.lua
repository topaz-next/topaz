-----------------------------------
-- Area: Ifrit's Cauldron
--  Mob: Eotyrannus
-- Note: PH for Lindwurm
-----------------------------------
local ID = require("scripts/zones/Ifrits_Cauldron/IDs")
require("scripts/globals/regimes")
require("scripts/globals/mobs")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
    tpz.regime.checkRegime(player, mob, 758, 1, tpz.regime.type.GROUNDS)
end

entity.onMobDespawn = function(mob)
    tpz.mob.phOnDespawn(mob, ID.mob.LINDWURM_PH, 5, 3600) -- 1 hour
end

return entity

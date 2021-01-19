-----------------------------------
-- Area: Rolanberry Fields
--  Mob: Ochu
-- Note: PH for Drooling Daisy
-----------------------------------
local ID = require("scripts/zones/Rolanberry_Fields/IDs")
require("scripts/globals/regimes")
require("scripts/globals/mobs")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
    tpz.regime.checkRegime(player, mob, 88, 1, tpz.regime.type.FIELDS)
end

entity.onMobDespawn = function(mob)
    tpz.mob.phOnDespawn(mob, ID.mob.DROOLING_DAISY_PH, 10, 3600) -- 1 hour
end

return entity

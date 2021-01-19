-----------------------------------
-- Area: Eastern Altepa Desert
--  Mob: Sand Beetle
-- Note: PH for Donnergugi
-----------------------------------
local ID = require("scripts/zones/Eastern_Altepa_Desert/IDs")
require("scripts/globals/regimes")
require("scripts/globals/mobs")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
    tpz.regime.checkRegime(player, mob, 110, 3, tpz.regime.type.FIELDS)
end

entity.onMobDespawn = function(mob)
    tpz.mob.phOnDespawn(mob, ID.mob.DONNERGUGI_PH, 10, 3600) -- 1 hour
end

return entity

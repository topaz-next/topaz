-----------------------------------
-- Area: Gustav Tunnel
--  Mob: Antares
-- Note: Place holder Amikiri
-----------------------------------
local ID = require("scripts/zones/Gustav_Tunnel/IDs")
require("scripts/globals/regimes")
require("scripts/globals/mobs")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
    tpz.regime.checkRegime(player, mob, 768, 2, tpz.regime.type.GROUNDS)
end

entity.onMobDespawn = function(mob)
    tpz.mob.phOnDespawn(mob, ID.mob.AMIKIRI_PH, 5, math.random(25200, 32400)) -- 7 to 9 hours
end

return entity

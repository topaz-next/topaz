-----------------------------------
-- Area: Crawlers' Nest (197)
--  Mob: Wespe
-- Note: PH for Demonic Tiphia
-----------------------------------
local ID = require("scripts/zones/Crawlers_Nest/IDs")
require("scripts/globals/regimes")
require("scripts/globals/mobs")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
    tpz.regime.checkRegime(player, mob, 691, 2, tpz.regime.type.GROUNDS)
end

entity.onMobDespawn = function(mob)
    tpz.mob.phOnDespawn(mob, ID.mob.DEMONIC_TIPHIA_PH, 5, math.random(7200, 28800)) -- 2 to 8 hours
end

return entity

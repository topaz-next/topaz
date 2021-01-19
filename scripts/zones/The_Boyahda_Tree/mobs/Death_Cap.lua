-----------------------------------
-- Area: The Boyahda Tree
--  Mob: Death Cap
-- Note: PH for Ellyllon
-----------------------------------
local ID = require("scripts/zones/The_Boyahda_Tree/IDs")
require("scripts/globals/regimes")
require("scripts/globals/mobs")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
    tpz.regime.checkRegime(player, mob, 719, 1, tpz.regime.type.GROUNDS)
end

entity.onMobDespawn = function(mob)
    tpz.mob.phOnDespawn(mob, ID.mob.ELLYLLON_PH, 10, math.random(7200, 10800)) -- 2 to 3 hours
end

return entity

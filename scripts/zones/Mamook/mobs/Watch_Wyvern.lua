-----------------------------------
-- Area: Mamook
--  Mob: Watch Wyvern
-- Note: PH for Firedance Magmaal Ja
-----------------------------------
local ID = require("scripts/zones/Mamook/IDs")
require("scripts/globals/mobs")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
end

entity.onMobDespawn = function(mob)
    tpz.mob.phOnDespawn(mob, ID.mob.FIREDANCE_MAGMAAL_JA_PH, 5, 3600) -- 1 hour
end

return entity

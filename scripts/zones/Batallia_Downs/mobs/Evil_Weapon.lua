-----------------------------------
-- Area: Batallia Downs
--  Mob: Evil Weapon
-- Note: PH for Prankster Maverix
-----------------------------------
local ID = require("scripts/zones/Batallia_Downs/IDs")
require("scripts/globals/mobs")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
end

entity.onMobDespawn = function(mob)
    tpz.mob.phOnDespawn(mob, ID.mob.PRANKSTER_MAVERIX_PH, 5, 3600) -- 1 hour
end

return entity

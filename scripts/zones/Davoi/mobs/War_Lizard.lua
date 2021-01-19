-----------------------------------
-- Area: Davoi
--  Mob: War Lizard
-- Note: PH for Tigerbane Bakdak
-----------------------------------
local ID = require("scripts/zones/Davoi/IDs")
require("scripts/globals/mobs")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
end

entity.onMobDespawn = function(mob)
    tpz.mob.phOnDespawn(mob, ID.mob.TIGERBANE_BAKDAK_PH, 10, 3600) -- 1 hour
end

return entity

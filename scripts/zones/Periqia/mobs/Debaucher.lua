-----------------------------------
-- Area: Periqia (Seagull Grounded)
--  Mob: Debaucher
-----------------------------------
require("scripts/globals/status")
-----------------------------------
local entity = {}

entity.onMobInitialize = function(mob)
    mob:addImmunity(tpz.immunity.DARKSLEEP)
end

entity.onMobDeath = function(mob, player, isKiller)
end

return entity

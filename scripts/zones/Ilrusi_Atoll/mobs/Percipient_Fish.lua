-----------------------------------
-- Area: Ilrusi Atoll
--  Mob: Imp
-----------------------------------
require("scripts/globals/status")
-----------------------------------
local entity = {}

entity.onMobInitialize = function(mob)
    mob:setMobMod(tpz.mobMod.SOUND_RANGE, 4)
end

entity.onMobDeath = function(mob, player, isKiller)
end

return entity

-----------------------------------
-- Area: Ilrusi Atoll
--  MOB: Imp
-----------------------------------
require("scripts/globals/status")
-----------------------------------

function onMobInitialize(mob)
    mob:setMobMod(dsp.mobMod.SOUND_RANGE, 4)
end

function onMobDeath(mob, player, isKiller)
end
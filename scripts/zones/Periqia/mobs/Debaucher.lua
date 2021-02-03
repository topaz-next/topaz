-----------------------------------
-- Area: Periqia (Seagull Grounded)
--  MOB: Debaucher
-----------------------------------
require("scripts/globals/status")
-----------------------------------

function onMobInitialize(mob)
    mob:addImmunity(dsp.immunity.DARKSLEEP)
end

function onMobDeath(mob, player, isKiller)
end

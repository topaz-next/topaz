-----------------------------------
-- Area: Lebros Cavern
--  MOB: Crimson Eruca
-----------------------------------
require("scripts/globals/status")
-----------------------------------

function onMobSpawn(mob)
	mob:addResist({ dsp.resist.ENFEEBLING_LIGHTSLEEP, 25, 0 })
    mob:addResist({ dsp.resist.ENFEEBLING_DARKSLEEP, 25, 0 })
    mob:addResist({ dsp.resist.ENFEEBLING_GRAVITY, 25, 0 })
    mob:addResist({ dsp.resist.ENFEEBLING_BIND, 25, 0 })
end


function onMobDeath(mob, player, isKiller, firstCall)
end

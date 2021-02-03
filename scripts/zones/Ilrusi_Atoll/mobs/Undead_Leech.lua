-----------------------------------
-- Area: Ilrusi Atoll (Extermination)
--  MOB: Undead Leech
-----------------------------------
require("scripts/globals/utils/assault")
-----------------------------------
function onMobSpawn(mob)
    assaultUtil.adjustMobLevel(mob, mob:getID())
end

function onMobDeath(mob, player, isKiller, firstCall)
    if firsCall then
        local instance = mob:getInstance()
        instance:setProgress(instance:getProgress() + 1)
    end
end

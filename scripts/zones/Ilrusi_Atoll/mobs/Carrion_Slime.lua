-----------------------------------
-- Area: Ilrusi Atoll (Extermination)
--  MOB: Carrion Slime
-----------------------------------
local ID = require("scripts/zones/Ilrusi_Atoll/IDs")
require("scripts/zones/Ilrusi_Atoll/globals")
require("scripts/globals/missions")
-----------------------------------

function onMobDeath(mob, player, isKiller, firstCall)
    local instance = mob:getInstance()

    if firstCall then
        exterminationRandomSpawn(mob, ID.mob[EXTERMINATION].NMS.SLIME)
        instance:setProgress(instance:getProgress() + 1)
    end
end

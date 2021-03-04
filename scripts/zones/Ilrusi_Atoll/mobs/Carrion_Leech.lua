-----------------------------------
-- Area: Ilrusi Atoll (Extermination)
--  Mob: Carrion Leech
-----------------------------------
local ID = require("scripts/zones/Ilrusi_Atoll/IDs")
local zoneUtil = require("scripts/zones/Ilrusi_Atoll/globals/zoneUtil")
require("scripts/globals/missions")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller, firstCall)
    local instance = mob:getInstance()

    if firstCall then
        zoneUtil.exterminationRandomSpawn(mob, ID.mob[EXTERMINATION].NMS.LEECH)
        instance:setProgress(instance:getProgress() + 1)
    end
end

return entity

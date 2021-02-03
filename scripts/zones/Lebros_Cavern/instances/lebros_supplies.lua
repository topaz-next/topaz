-----------------------------------
-- Assault: Lebros Supplies
-- Your mission is to deliver rations to each member of the advance unit.
-----------------------------------
local ID = require("scripts/zones/Lebros_Cavern/IDs")
require("scripts/globals/instance")
require("scripts/globals/items")
require("scripts/globals/missions")
require("scripts/globals/status")
require("scripts/globals/utils/assault")
require("scripts/globals/zone")
-----------------------------------

function afterInstanceRegister(player)
    assaultUtil.afterInstanceRegister(player, 5345, ID.text, ID.mob)
end

function onInstanceCreated(instance)
    if math.random(1,100) >= 50 then
        instance:getEntity(bit.band(17035306, 0xFFF), dsp.objType.MOB):setSpawn(-304.151, -8.470, -180.556, 250)
    end
    if math.random(1,100) >= 50 then
        instance:getEntity(bit.band(17035307, 0xFFF), dsp.objType.MOB):setSpawn(-555.575, -10.872, -149.561, 201)
    end
    if math.random(1,100) >= 50 then
        instance:getEntity(bit.band(17035308, 0xFFF), dsp.objType.MOB):setSpawn(-291.425, -8.264, -137.649, 142)
    end
    instance:getEntity(bit.band(ID.npc.RUNE_OF_RELEASE, 0xFFF), dsp.objType.NPC):setPos(-330, -10, -262, 128)
    instance:getEntity(bit.band(ID.npc.ANCIENT_LOCKBOX, 0xFFF), dsp.objType.NPC):setPos(-330, -10, -265, 128)
end

function onInstanceTimeUpdate(instance, elapsed)
    updateInstanceTime(instance, elapsed, ID.text)
end

function onInstanceFailure(instance)
    assaultUtil.onInstanceFailure(instance, 102, ID.text)
end

function onInstanceProgressUpdate(instance, progress)
    if progress >= 12 then
        instance:complete()
    end
end

function onInstanceComplete(instance)
    assaultUtil.onInstanceComplete(player, instance, 5, 10, ID.text, ID.npc)
end

function onEventUpdate(player, csid, option)
end

function onEventFinish(player, csid, option)
    assaultUtil.instanceOnEventFinish(player, csid, 102, dsp.zone.MOUNT_ZHAYOLM)
end

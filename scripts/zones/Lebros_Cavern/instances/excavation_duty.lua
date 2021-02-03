-----------------------------------
-- Assault: Excavation Duty
-- The Imperial Army plans to traverse Lebros Cavern as part of their attack on Halvung,
-- however the tunnels are blocked with rocks. You must remove the obstructions.
-----------------------------------
local ID = require("scripts/zones/Lebros_Cavern/IDs")
require("scripts/globals/instance")
require("scripts/globals/missions")
require("scripts/globals/utils/assault")
require("scripts/globals/zone")
-----------------------------------

function afterInstanceRegister(player)
    assaultUtil.afterInstanceRegister(player, 5345, ID.text, ID.mob)
end

function onInstanceCreated(instance)
    instance:getEntity(bit.band(ID.npc.RUNE_OF_RELEASE, 0xFFF), dsp.objType.NPC):setPos(49.999,-40.837,96.999,0)
    instance:getEntity(bit.band(ID.npc.ANCIENT_LOCKBOX, 0xFFF), dsp.objType.NPC):setPos(50.000,-40.070,99.999,0)
end

function onInstanceTimeUpdate(instance, elapsed)
    updateInstanceTime(instance, elapsed, ID.text)
end

function onInstanceFailure(instance)
    assaultUtil.onInstanceFailure(instance, 102, ID.text)
end

function onInstanceProgressUpdate(instance, progress)
    if progress >= 5 then
        instance:complete()
    end
end

function onInstanceComplete(instance)
    assaultUtil.onInstanceComplete(player, instance, 5, 10, ID.text, ID.npc)
end

function onEventUpdate(player, csid, option)
end

function onEventFinish(player, csid, option)
    assaultUtil.instanceOnEventFinish(player, 102, dsp.zone.MOUNT_ZHAYOLM)
end

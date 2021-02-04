-----------------------------------
--
-- Assault: Preemptive Strike
--
-----------------------------------
local ID = require("scripts/zones/Mamool_Ja_Training_Grounds/IDs")
require("scripts/globals/instance")
require("scripts/globals/missions")
require("scripts/globals/assault")
require("scripts/globals/zone")
-----------------------------------

function afterInstanceRegister(player)
    assaultUtil.afterInstanceRegister(player, 5344, ID.text, ID.mob)
end

function onInstanceCreated(instance)
    instance:getEntity(bit.band(ID.npc.RUNE_OF_RELEASE, 0xFFF), dsp.objType.NPC):setPos(-55.000, 1.323, -103.000, 128)
    instance:getEntity(bit.band(ID.npc.ANCIENT_LOCKBOX, 0xFFF), dsp.objType.NPC):setPos(-58.000, 1.519, -103.000, 128)
end

function onInstanceTimeUpdate(instance, elapsed)
    updateInstanceTime(instance, elapsed, ID.text)
end

function onInstanceFailure(instance)
    assaultUtil.onInstanceFailure(instance, 102, ID.text)
end

function onInstanceProgressUpdate(instance, progress)
    if progress >= 13 then
        instance:complete()
    end
end

function onInstanceComplete(instance)
    assaultUtil.onInstanceComplete(player, instance, 8, 8, ID.text, ID.npc)
end

function onEventUpdate(player,csid,option)
end

function onEventFinish(player,csid,option)
    assaultUtil.instanceOnEventFinish(player, 102, dsp.zone.BHAFLAU_THICKETS)
end

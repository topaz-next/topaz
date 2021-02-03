-----------------------------------
--
-- Assault: Extermination
--
-----------------------------------
local ID = require("scripts/zones/Ilrusi_Atoll/IDs")
require("scripts/globals/instance")
require("scripts/globals/utils/assault")
require("scripts/globals/zone")
-----------------------------------

function afterInstanceRegister(player)
    assaultUtil.afterInstanceRegister(player, 5347, ID.text, ID.mob)
end

function onInstanceCreated(instance)
    instance:getEntity(bit.band(ID.npc.RUNE_OF_RELEASE, 0xFFF), dsp.objType.NPC):setPos(290.857,-3.424,132.339,148)
    instance:getEntity(bit.band(ID.npc.ANCIENT_LOCKBOX, 0xFFF), dsp.objType.NPC):setPos(293.637,-3.376,130.364,148)
end

function onInstanceTimeUpdate(instance, elapsed)
    updateInstanceTime(instance, elapsed, ID.text)
end

function onInstanceFailure(instance)
    assaultUtil.onInstanceFailure(instance, 102, ID.text)
end

function onInstanceProgressUpdate(instance, progress)
    if progress == 20 then
        instance:complete()
    end
end

function onInstanceComplete(instance)
    assaultUtil.onInstanceComplete(player, instance, 8, 8, ID.text, ID.npc) -- verify
end

function onEventUpdate(player,csid,option)
end

function onEventFinish(player,csid,option)
    assaultUtil.instanceOnEventFinish(player, 102, dsp.zone.ARRAPAGO_REEF)
end

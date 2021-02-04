-----------------------------------
-- Assault: Imperial Agent Rescue
-- An agent sent to spy on the secret training grounds of the Mamool Ja has been captured.
-- Rescue him before he is interrogated for Imperial secrets.
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
    instance:getEntity(bit.band(ID.npc.RUNE_OF_RELEASE, 0xFFF), dsp.objType.NPC):setPos(220.000, 1.465, -504.999, 0)
    instance:getEntity(bit.band(ID.npc.ANCIENT_LOCKBOX, 0xFFF), dsp.objType.NPC):setPos(220.000, 1.619, -502.999, 0)

    instance:setProgress(math.random(ID.npc.POT_HATCH, ID.npc.POT_HATCH + 2))
end

function onInstanceTimeUpdate(instance, elapsed)
    updateInstanceTime(instance, elapsed, ID.text)
end

function onInstanceFailure(instance)
    assaultUtil.onInstanceFailure(instance, 102, ID.text)
end

function onInstanceProgressUpdate(instance, progress)
end

function onInstanceComplete(instance)
    assaultUtil.onInstanceComplete(player, instance, 9, 8, ID.text, ID.npc)
end

function onEventUpdate(player,csid,option)
end

function onEventFinish(player,csid,option)
    assaultUtil.instanceOnEventFinish(player, 102, dsp.zone.BHAFLAU_THICKETS)
end

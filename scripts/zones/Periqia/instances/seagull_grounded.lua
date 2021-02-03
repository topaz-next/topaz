-----------------------------------
-- Assault: Seagull Grounded
-- The Immortals have captured a member of the Seagull Phratrie, a rebel organization.
-- You are to escort the prisoner safely to a holding area.
-----------------------------------
local ID = require("scripts/zones/Periqia/IDs")
require("scripts/globals/instance")
require("scripts/globals/missions")
require("scripts/globals/utils/assault")
require("scripts/globals/zone")
require("scripts/zones/Periqia/mobs/Excaliace")
-----------------------------------

function afterInstanceRegister(player)
    assaultUtil.afterInstanceRegister(player, 5346, ID.text, ID.mob)
end

function onInstanceCreated(instance)
    instance:getEntity(bit.band(ID.npc.RUNE_OF_RELEASE, 0xFFF), dsp.objType.NPC):setPos(-495.000,-9.695,-72.000,0)
    instance:getEntity(bit.band(ID.npc.ANCIENT_LOCKBOX, 0xFFF), dsp.objType.NPC):setPos(-490.000,-9.900,-72.000,0)

    instance:getEntity(bit.band(ID.npc._1K6, 0xFFF), dsp.objType.NPC):setAnimation(8)
    instance:getEntity(bit.band(ID.npc._1KX, 0xFFF), dsp.objType.NPC):setAnimation(8)
    instance:getEntity(bit.band(ID.npc._1KZ, 0xFFF), dsp.objType.NPC):setAnimation(8)
    instance:getEntity(bit.band(ID.npc._JK1, 0xFFF), dsp.objType.NPC):setAnimation(8)
    instance:getEntity(bit.band(ID.npc._JK3, 0xFFF), dsp.objType.NPC):setAnimation(8)

end

function onInstanceTimeUpdate(instance, elapsed)
    local mob = instance:getEntity(bit.band(ID.mob[SEAGULL_GROUNDED].MOBS_START.EXCALIAC, 0xFFF), dsp.objType.MOB)
    if mob ~= nil then
        onTrack(mob)
    end
    updateInstanceTime(instance, elapsed, ID.text)
end

function onInstanceFailure(instance)
    assaultUtil.onInstanceFailure(instance, 102, ID.text)
end

function onInstanceProgressUpdate(instance, progress)

    if progress > 0 then
        instance:complete()
    end

end

function onInstanceComplete(instance)
    assaultUtil.onInstanceComplete(player, instance, 8, 8, ID.text, ID.npc)
end

function onEventUpdate(player, csid, option)
end

function onEventFinish(player, csid, option)
    assaultUtil.instanceOnEventFinish(player, 102, dsp.zone.CAEDARVA_MIRE)
end

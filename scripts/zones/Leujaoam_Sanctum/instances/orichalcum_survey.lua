-----------------------------------
-- Assault: Orichalcum Survey
-- General Afkaam is to inspect Leujaoam Sanctum. Destroy all creatures in the area that may pose a threat to the general.
-----------------------------------
local ID = require("scripts/zones/Leujaoam_Sanctum/IDs")
require("scripts/globals/instance")
require("scripts/globals/items")
require("scripts/globals/missions")
require("scripts/globals/status")
require("scripts/globals/utils/assault")
require("scripts/globals/zone")
-----------------------------------

function afterInstanceRegister(player)
    assaultUtil.afterInstanceRegister(player, dsp.items.CAGE_OF_AZOUPH_FIREFLIES, ID.text, ID.mob)
end

function onInstanceCreated(instance)
    local miningPoints = utils.shuffle(ID.mob[ORICHALCUM_SURVEY].MINING_POINTS)
    local points = 5
    while points > 0 do
        for _, point in pairs(miningPoints) do
            if instance:getEntity(bit.band(point, 0xFFF), dsp.objType.NPC):getStatus() == dsp.status.DISAPPEAR then
                instance:getEntity(bit.band(point, 0xFFF), dsp.objType.NPC):setStatus(dsp.status.NORMAL)
                instance:getEntity(bit.band(point, 0xFFF), dsp.objType.NPC):setLocalVar("Mined", math.random(5,10))
                points = points - 1
                break
            end
        end
    end

    instance:getEntity(bit.band(ID.npc.RUNE_OF_RELEASE, 0xFFF), dsp.objType.NPC):setPos(-432.000, -27.627, 169.000, 129)
    instance:getEntity(bit.band(ID.npc.ANCIENT_LOCKBOX, 0xFFF), dsp.objType.NPC):setPos(-432.000, -27.588, 167.000, 129)
end

function onInstanceTimeUpdate(instance, elapsed)
    updateInstanceTime(instance, elapsed, ID.text)
end

function onInstanceFailure(instance)
    assaultUtil.onInstanceFailure(instance, 102, ID.text)
end

function onInstanceProgressUpdate(instance, progress)
    if progress >= 1 then
        instance:complete()
    end
end

function onInstanceComplete(instance)
    assaultUtil.onInstanceComplete(player, instance, 7, 8, ID.text, ID.npc)
end

function onEventUpdate(player, csid, option)
end

function onEventFinish(player, csid, option)
    assaultUtil.instanceOnEventFinish(player, csid, 102, dsp.zone.CAEDARVA_MIRE)
end

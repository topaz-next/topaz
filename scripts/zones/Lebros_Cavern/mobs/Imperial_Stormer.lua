-----------------------------------
-- Area: Lebros Cavern
--  MOB: Imperial Stormer
-----------------------------------
local ID = require("scripts/zones/Lebros_Cavern/IDs")
require("scripts/globals/items")
require("scripts/globals/status")
-----------------------------------

local assaultFood =
{
    {item = dsp.items.SERVING_OF_BISON_STEAK, point = 5},
    {item = dsp.items.COEURL_SUB, point = 4},
    {item = dsp.items.STRIP_OF_BISON_JERKY, point = 3},
    {item = dsp.items.BOWL_OF_PEA_SOUP, point = 2},
    {item = dsp.items.LOAF_OF_WHITE_BREAD, point = 1},
}

function foodPoints(player, mob)

    for _, v in pairs(assaultFood) do
        if player:hasItem(v.item, dsp.inventoryLocation.TEMPITEMS) then
            mob:setLocalVar("foodEaten", mob:getLocalVar("foodEaten") + v.point)
            player:setLocalVar("foodGiven", 0)
            player:delItem(v.item, 1, dsp.inventoryLocation.TEMPITEMS)
            return v.point
        end
    end
    return 0
end

function onMobSpawn(mob)
	mob:setStatus(dsp.status.NPC)
end

function onTrigger(player, mob)
	local instance = mob:getInstance()
	local points = foodPoints(player, mob)
	local MOB = instance:getEntity(bit.band(mob:getID(), 0xFFF), dsp.objType.MOB)


	if points > 0 then
		if mob:getLocalVar("foodEaten") >= 7 and mob:getLocalVar("complete") == 0 then
			instance:setProgress(instance:getProgress() + 1)
			mob:setLocalVar("complete", 1)
			player:showText(mob, ID.text.FULL_HUNGRY)
		elseif mob:getLocalVar("complete") == 1 then
			player:showText(mob, ID.text.FULL_FED)
		else
			player:showText(mob, ID.text.STILL_HUNGRY_FED)
		end
	else
		if mob:getLocalVar("foodEaten") >= 7 then
			player:showText(mob, ID.text.FULL_HUNGRY)
		else
			player:showText(mob, ID.text.STILL_HUNGRY_TRIGGER)
		end
	end
end

function onMobDeath(mob, player, isKiller, firstCall)
end

-----------------------------------
-- Area: Leujaoam Sanctum
-- NPC: Mulwahah
-- Assault: Orichalcum Survey
-----------------------------------
local ID = require("scripts/zones/Leujaoam_Sanctum/IDs")
require("scripts/globals/items")
require("scripts/globals/status")
-----------------------------------

function onTrigger(player, npc)
    local instance = npc:getInstance()

    if player:hasItem(dsp.items.CHUNK_OF_ORICHALCUM_ORE, dsp.inventoryLocation.TEMPITEMS) and instance:getStage() == 1 and instance:getProgress() == 0 then
        player:messageSpecial(ID.text.MULWAHAH_FINISH)
        player:delItem(dsp.items.CHUNK_OF_ORICHALCUM_ORE, 1, dsp.inventoryLocation.TEMPITEMS)
        player:timer(3000, function(player) player:messageSpecial(ID.text.MULWAHAH_FINISH + 1, dsp.items.CHUNK_OF_ORICHALCUM_ORE) end)
        player:timer(6000, function(player) player:messageSpecial(ID.text.MULWAHAH_FINISH + 2) end)
        player:timer(7000, function(player) instance:setProgress(1) end)
    elseif not player:hasItem(dsp.items.PICKAXE, dsp.inventoryLocation.TEMPITEMS) then
        player:messageSpecial(ID.text.MULWAHAH_TAKE_THIS, dsp.items.CHUNK_OF_ORICHALCUM_ORE)
        player:addTempItem(dsp.items.PICKAXE)
        player:messageSpecial(ID.text.OBTAINED_TEMP_ITEM, dsp.items.PICKAXE)
    elseif instance:getStage() == 1 and instance:getProgress() == 1 then
        player:messageSpecial(ID.text.MULWAHAH_TAKE_THIS + 2)
    else
        player:messageSpecial(ID.text.MULWAHAH_TAKE_THIS - 1, dsp.items.PICKAXE)
    end
end

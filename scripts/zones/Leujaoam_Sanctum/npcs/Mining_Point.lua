-----------------------------------
-- Area: Leujaoam Sanctum
-- NPC: Mining Point
-- Assault: Orichalcum Survey
-- IDs 17060016 - 17060025
-----------------------------------
local ID = require("scripts/zones/Leujaoam_Sanctum/IDs")
require("scripts/globals/items")
require("scripts/globals/npc_util")
require("scripts/globals/status")
-----------------------------------

function onTrigger(player, npc)
    local instance = npc:getInstance()
    local chance = math.random(1,1000)
    local pickBreak = math.random(1,100)
    local mineralEater = npc:getID() - 152
    local currentTime = os.time()

    if npc:checkDistance(player) > 3 then
        player:messageSpecial(ID.text.MINE_TOO_FAR)
        return
    elseif not player:hasItem(dsp.items.PICKAXE, dsp.inventoryLocation.TEMPITEMS) then
        player:messageSpecial(ID.text.MINE_NO_PICK, dsp.items.PICKAXE)
        return
    elseif instance:getEntity(bit.band(mineralEater, 0xFFF), dsp.objType.MOB):isSpawned() or npc:getLocalVar("Wait") >= currentTime then
        player:messageSpecial(ID.text.CANT_MINE)
        return
    end

    if chance >= 30 then
        player:sendEmote(npc, dsp.emote.EXCAVATION, dsp.emoteMode.MOTION, true)
        npc:setLocalVar("Wait", currentTime + 3)
        if chance > 900 then -- items
            if chance > 990 then
                player:timer(3000, function(player)
                player:addTempItem(dsp.items.CHUNK_OF_ORICHALCUM_ORE)
                player:messageSpecial(ID.text.MINE_SUCCESS, dsp.items.CHUNK_OF_ORICHALCUM_ORE)
                instance:setStage(1) end)
            else
                player:timer(3000, function(player)
                player:addTreasure(dsp.items.PEBBLE, instance:getEntity(bit.band(17060016, 0xFFF), dsp.objType.NPC)) end)
            end
        elseif chance < 100 then
            player:timer(3000, function(player)
            player:messageSpecial(ID.text.MINE_FAIL)
            SpawnMob(mineralEater, instance):updateEnmity(player) end)
        else
            player:timer(3000, function(player)
            player:messageSpecial(ID.text.MINE_FAIL) end)
        end
        if pickBreak >= 900 then
            player:timer(3000, function(player)
            player:messageSpecial(ID.text.BREAK_PICKAXE, dsp.items.PICKAXE)
            player:delItem(dsp.items.PICKAXE, 1, dsp.inventoryLocation.TEMPITEMS) end)
        end
    else
        SpawnMob(mineralEater, instance):updateEnmity(player)
    end
    handleMiningPoint(npc)
end

function handleMiningPoint(npc)
    local instance = npc:getInstance()
    local Mined = npc:getLocalVar("Mined")
    npc:setLocalVar("Mined", Mined - 1)

    if Mined <= 0 then
        local miningPoints = utils.shuffle(ID.mob[ORICHALCUM_SURVEY].MINING_POINTS)

        for _, point in pairs(miningPoints) do
            if instance:getEntity(bit.band(point, 0xFFF), dsp.objType.NPC):getStatus() == dsp.status.DISAPPEAR then
                instance:getEntity(bit.band(point, 0xFFF), dsp.objType.NPC):setStatus(dsp.status.NORMAL)
                instance:getEntity(bit.band(point, 0xFFF), dsp.objType.NPC):setLocalVar("Mined", math.random(5,10))
                break
            end
        end
        npc:setStatus(dsp.status.DISAPPEAR)
    end
end

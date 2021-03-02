-----------------------------------
-- Area: Windurst Waters
--  NPC: Baren-Moren
-- Starts and Finishes Quest: Hat in Hand
-- !pos -66 -3 -148 238
-----------------------------------
local ID = require("scripts/zones/Windurst_Waters/IDs")
require("scripts/globals/settings")
require("scripts/globals/keyitems")
require("scripts/globals/npc_util")
require("scripts/globals/quests")
require("scripts/globals/titles")
require("scripts/globals/utils")
-----------------------------------
local entity = {}

entity.onTrade = function(player, npc, trade)
    local aFeatherInOnesCap = player:getQuestStatus(tpz.quest.log_id.WINDURST, tpz.quest.id.windurst.A_FEATHER_IN_ONE_S_CAP)

    if
        (
            aFeatherInOnesCap == QUEST_ACCEPTED or
            player:getCharVar("QuestFeatherInOnesCap_var") == 1
        ) and
        npcUtil.tradeHas(trade, {{842, 3}})
    then
        player:startEvent(79, 1500) -- Quest Turn In
    end
end

entity.onTrigger = function(player, npc)
    local hatInHand = player:getQuestStatus(tpz.quest.log_id.WINDURST, tpz.quest.id.windurst.HAT_IN_HAND)
    local aFeatherInOnesCap = player:getQuestStatus(tpz.quest.log_id.WINDURST, tpz.quest.id.windurst.A_FEATHER_IN_ONE_S_CAP)
    local pfame = player:getFameLevel(WINDURST)

    if hatInHand == QUEST_AVAILABLE then
        player:startEvent(48) -- Quest Offered
    elseif player:hasKeyItem(tpz.ki.NEW_MODEL_HAT) then
        local count = player:getCharVar("QuestHatInHand_count")

        if count >= 8 then
            player:startEvent(52, 80) -- 80 = HAT + FULL REWARD = 8 NPCS
            player:setLocalVar("hatRewardTier", 5)
        elseif count >= 6 then
            player:startEvent(52, 50) -- 50 = HAT + GOOD REWARD >= 6-7 NPCS
            player:setLocalVar("hatRewardTier", 4)
        elseif count >= 4 then
            player:startEvent(52, 30) -- 30 = PARTIAL REWARD >= 4-5 NPCS
            player:setLocalVar("hatRewardTier", 3)
        elseif count >= 2 then
            player:startEvent(52, 20) -- 20 = POOR REWARD >= 2-3 NPCS
            player:setLocalVar("hatRewardTier", 2)
        else
            player:startEvent(52) -- 0 = NO REWARD >= 0-1 NPCS
            player:setLocalVar("hatRewardTier", 1)
        end
    elseif hatInHand == QUEST_COMPLETED and aFeatherInOnesCap == QUEST_AVAILABLE and pfame >= 3 and not player:needToZone() then
        player:startEvent(75, 0, 842) -- Quest "Feather In One's Cap" offered
    elseif aFeatherInOnesCap == QUEST_ACCEPTED or player:getCharVar("QuestFeatherInOnesCap_var") == 1 then
        player:startEvent(78, 0, 842) -- Quest Objective Reminder
    elseif aFeatherInOnesCap == QUEST_COMPLETED and not player:needToZone() then
        player:startEvent(75, 0, 842) -- Repeatable Quest "A Feather In One's Cap" offered

    -- default dialog
    else
        local rand = math.random(1, 6)

        if rand == 1 then
            player:startEvent(42) -- Standard Conversation 1
        elseif rand == 2 then
            player:startEvent(44) -- Standard Conversation 2
        elseif rand == 3 then
            player:startEvent(45) -- Standard Conversation 3
        elseif rand == 4 then
            player:startEvent(46) -- Standard Conversation 4
        elseif rand == 5 then
            player:startEvent(47) -- Standard Conversation 5
        elseif rand == 6 then
            player:startEvent(1022) -- Standard Conversation 6
        end
    end
end

entity.onEventUpdate = function(player, csid, option)
end

entity.onEventFinish = function(player, csid, option)
    local aFeatherInOnesCap = player:getQuestStatus(tpz.quest.log_id.WINDURST, tpz.quest.id.windurst.A_FEATHER_IN_ONE_S_CAP)

    if csid == 48 and option == 1 then
        player:addQuest(tpz.quest.log_id.WINDURST, tpz.quest.id.windurst.HAT_IN_HAND)
        npcUtil.giveKeyItem(player, tpz.ki.NEW_MODEL_HAT)
    elseif csid == 52 and option >= 1 then
        local rewardTier = player:getLocalVar("hatRewardTier")
        local rewards = {fame = 75, var = {"QuestHatInHand_var", "QuestHatInHand_count"}}

        if rewardTier == 5 then
            rewards.gil = 500
            rewards.item = 12543
        elseif rewardTier == 4 then
            rewards.gil = 400
            rewards.item = 12543
        elseif rewardTier == 3 then
            rewards.gil = 300
        elseif rewardTier == 2 then
            rewards.gil = 150
        elseif rewardTier == 1 then
            rewards.gil = 300
        end

        if npcUtil.completeQuest(player, WINDURST, tpz.quest.id.windurst.HAT_IN_HAND, rewards) then
            player:delKeyItem(tpz.ki.NEW_MODEL_HAT)
            player:needToZone(true)
        end
    elseif csid == 75 and option == 1 then
        if aFeatherInOnesCap == QUEST_AVAILABLE then
            player:addQuest(tpz.quest.log_id.WINDURST, tpz.quest.id.windurst.A_FEATHER_IN_ONE_S_CAP)
        elseif aFeatherInOnesCap == QUEST_COMPLETED then
            player:setCharVar("QuestFeatherInOnesCap_var", 1)
        end
    elseif csid == 79 then
        if aFeatherInOnesCap == QUEST_ACCEPTED then
            npcUtil.completeQuest(player, WINDURST, tpz.quest.id.windurst.A_FEATHER_IN_ONE_S_CAP, {fame = 75})
        else
            player:addFame(WINDURST, 8)
            player:setCharVar("QuestFeatherInOnesCap_var", 0)
        end
        player:addGil(GIL_RATE * 1500)
        player:confirmTrade()
        player:needToZone(true)
    end
end

return entity

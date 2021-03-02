-----------------------------------
-- Area: Windurst Waters
--  NPC: Kyume-Romeh
--  Involved In Quest: Making Headlines, Hat in Hand
-- !pos -58 -4 23 238
-----------------------------------
local ID = require("scripts/zones/Windurst_Waters/IDs")
require("scripts/globals/keyitems")
require("scripts/globals/npc_util")
require("scripts/globals/quests")
require("scripts/globals/utils")
-----------------------------------
local entity = {}

entity.onTrade = function(player, npc, trade)
end

entity.onTrigger = function(player, npc)
    local makingHeadlines = player:getQuestStatus(tpz.quest.log_id.WINDURST, tpz.quest.id.windurst.MAKING_HEADLINES)
    local lureOfTheWildcat = player:getQuestStatus(tpz.quest.log_id.WINDURST, tpz.quest.id.windurst.LURE_OF_THE_WILDCAT)
    local wildcatWindurst = player:getCharVar("WildcatWindurst")

    if player:getCurrentMission(COP) == tpz.mission.id.cop.THE_ROAD_FORKS and player:getCharVar("MEMORIES_OF_A_MAIDEN_Status") == 4 then
        player:startEvent(873)
    elseif lureOfTheWildcat == QUEST_ACCEPTED and not utils.mask.getBit(wildcatWindurst, 14) then
        player:startEvent(939)
    elseif player:hasKeyItem(tpz.ki.NEW_MODEL_HAT) and not utils.mask.getBit(player:getCharVar("QuestHatInHand_var"), 4) then
        player:messageSpecial(ID.text.YOU_SHOW_OFF_THE, 0, tpz.ki.NEW_MODEL_HAT)
        player:startEvent(60)
    elseif makingHeadlines == QUEST_ACCEPTED then
        -- bitmask of progress: 0 = Kyume-Romeh, 1 = Yuyuju, 2 = Hiwom-Gomoi, 3 = Umumu, 4 = Mahogany Door
        local prog = player:getCharVar("QuestMakingHeadlines_var")

        if not utils.mask.getBit(prog, 0) then
            player:startEvent(668) -- Quest progress
        else
            player:startEvent(669) -- Quest not furthered
        end
    else
        if math.random(1, 2) == 1 then
            player:startEvent(604) -- Standard Conversation
        else
            player:startEvent(393) -- Standard Conversation
        end
    end
end

entity.onEventUpdate = function(player, csid, option)
end

entity.onEventFinish = function(player, csid, option)
    if csid == 668 then
        npcUtil.giveKeyItem(player, tpz.ki.WINDURST_WATERS_SCOOP)
        player:setCharVar("QuestMakingHeadlines_var", utils.mask.setBit(player:getCharVar("QuestMakingHeadlines_var"), 0, true))
    elseif csid == 60 then
        player:setCharVar("QuestHatInHand_var", utils.mask.setBit(player:getCharVar("QuestHatInHand_var"), 4, true))
        player:addCharVar("QuestHatInHand_count", 1)
    elseif csid == 873 then
        player:setCharVar("MEMORIES_OF_A_MAIDEN_Status", 5)
    elseif csid == 939 then
        player:setCharVar("WildcatWindurst", utils.mask.setBit(player:getCharVar("WildcatWindurst"), 14, true))
    end
end

return entity

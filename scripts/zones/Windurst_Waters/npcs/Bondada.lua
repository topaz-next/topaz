-----------------------------------
-- Area: Windurst Waters
--  NPC: Bondada
-- Involved in Quests: Hat in Hand
-- !pos -66 -3 -148 238
-----------------------------------
local ID = require("scripts/zones/Windurst_Waters/IDs")
require("scripts/globals/keyitems")
require("scripts/globals/quests")
require("scripts/globals/utils")
-----------------------------------
local entity = {}

entity.onTrade = function(player, npc, trade)
end

entity.onTrigger = function(player, npc)
    local hatInHand = player:getQuestStatus(tpz.quest.log_id.WINDURST, tpz.quest.id.windurst.HAT_IN_HAND)
    local hatMask = player:getCharVar("QuestHatInHand_var")

    if player:hasKeyItem(tpz.ki.NEW_MODEL_HAT) and not utils.mask.getBit(hatMask, 7) then
        player:messageSpecial(ID.text.YOU_SHOW_OFF_THE, 0, tpz.ki.NEW_MODEL_HAT)

        if utils.mask.isFull(hatMask, 7) then
            player:startEvent(61) -- Show Off Hat (She buys one)
        else
            player:startEvent(53) -- Show Off Hat (She does not buy one)
        end
    else
        player:startEvent(43) -- Standard Conversation
    end
end

entity.onEventUpdate = function(player, csid, option)
end

entity.onEventFinish = function(player, csid, option)
    if csid == 61 then
        player:setCharVar("QuestHatInHand_var", utils.mask.setBit(player:getCharVar("QuestHatInHand_var"), 7, true))
        player:addCharVar("QuestHatInHand_count", 1)
    end
end

return entity

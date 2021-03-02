-----------------------------------
-- Area: Windurst Waters
--  NPC: Clais
-- Involved In Quest: Hat in Hand
-- !pos -31 -3 11 238
-----------------------------------
local ID = require("scripts/zones/Windurst_Waters/IDs")
require("scripts/globals/keyitems")
require("scripts/globals/utils")
-----------------------------------
local entity = {}

entity.onTrade = function(player, npc, trade)
end

entity.onTrigger = function(player, npc)
    if player:hasKeyItem(tpz.ki.NEW_MODEL_HAT) and not utils.mask.getBit(player:getCharVar("QuestHatInHand_var"), 3) then
        player:messageSpecial(ID.text.YOU_SHOW_OFF_THE, 0, tpz.ki.NEW_MODEL_HAT)
        player:startEvent(57)
    else
        player:startEvent(602) -- Standard Conversation
    end
end

entity.onEventUpdate = function(player, csid, option)
end

entity.onEventFinish = function(player, csid, option)
    if csid == 57 then
        player:setCharVar("QuestHatInHand_var", utils.mask.setBit(player:getCharVar("QuestHatInHand_var"), 3, true))
        player:addCharVar("QuestHatInHand_count", 1)
    end
end

return entity

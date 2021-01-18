-----------------------------------
-- Area: Appolyon
-- Name: Central Apollyon
-----------------------------------
require("scripts/globals/limbus")
require("scripts/globals/battlefield")
require("scripts/globals/keyitems")
local ID = require("scripts/zones/Apollyon/IDs")
-----------------------------------
local battlefield_object = {}

battlefield_object.onBattlefieldInitialise = function(battlefield)
    battlefield:setLocalVar("loot", 1)
    battlefield:setLocalVar("podReady", 1)
    SetServerVariable("[Central_Apollyon]Time", battlefield:getTimeLimit()/60)
    tpz.limbus.setupArmouryCrates(battlefield:getID())
end

battlefield_object.onBattlefieldTick = function(battlefield, tick)
    if battlefield:getRemainingTime() % 60 == 0 then
        SetServerVariable("[Central_Apollyon]Time", battlefield:getRemainingTime()/60)
    end
    tpz.battlefield.onBattlefieldTick(battlefield, tick)
end

battlefield_object.onBattlefieldRegister = function(player, battlefield)
end

battlefield_object.onBattlefieldEnter = function(player, battlefield)
    player:delKeyItem(tpz.ki.COSMOCLEANSE)
    player:setCharVar("Cosmo_Cleanse_TIME", os.time())
    if player:getCharVar("ApollyonEntrance") == 0 then
        player:delKeyItem(tpz.ki.BLACK_CARD)
    else
        player:delKeyItem(tpz.ki.RED_CARD)
    end
end

battlefield_object.onBattlefieldDestroy = function(battlefield)
    SetServerVariable("[Central_Apollyon]Time", 0)
end

battlefield_object.onBattlefieldLeave = function(player, battlefield, leavecode)
    player:messageSpecial(ID.text.HUM+1)
    if leavecode == tpz.battlefield.leaveCode.WON then
        local name, clearTime, partySize = battlefield:getRecord()
        player:startEvent(32001, battlefield:getArea(), clearTime, partySize, battlefield:getTimeInside(), 1, battlefield:getLocalVar("[cs]bit"), 0)
    elseif leavecode == tpz.battlefield.leaveCode.LOST then
        player:startEvent(32002)
    end
end
return battlefield_object

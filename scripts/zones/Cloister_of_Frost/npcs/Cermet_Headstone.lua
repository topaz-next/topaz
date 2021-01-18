-----------------------------------
-- Area: Cloister of Frost
--  NPC: Cermet Headstone
-- Involved in Mission: ZM5 Headstone Pilgrimage (Ice Fragment)
-- !pos 566 0 606 203
-----------------------------------
require("scripts/globals/keyitems")
require("scripts/globals/titles")
require("scripts/globals/missions")
local ID = require("scripts/zones/Cloister_of_Frost/IDs")
-----------------------------------
local entity = {}

entity.onTrade = function(player, npc, trade)
end

entity.onTrigger = function(player, npc)

    if (player:getCurrentMission(ZILART) == tpz.mission.id.zilart.HEADSTONE_PILGRIMAGE) then
        if (player:hasKeyItem(tpz.ki.ICE_FRAGMENT) == false) then
            player:startEvent(200,tpz.ki.ICE_FRAGMENT);
        elseif
            player:hasKeyItem(tpz.ki.FIRE_FRAGMENT) and
            player:hasKeyItem(tpz.ki.ICE_FRAGMENT) and
            player:hasKeyItem(tpz.ki.WIND_FRAGMENT) and
            player:hasKeyItem(tpz.ki.EARTH_FRAGMENT) and
            player:hasKeyItem(tpz.ki.LIGHTNING_FRAGMENT) and
            player:hasKeyItem(tpz.ki.WATER_FRAGMENT) and
            player:hasKeyItem(tpz.ki.LIGHT_FRAGMENT) and
            player:hasKeyItem(tpz.ki.DARK_FRAGMENT)
        then
            player:messageSpecial(ID.text.ALREADY_HAVE_ALL_FRAGS)
        elseif (player:hasKeyItem(tpz.ki.ICE_FRAGMENT)) then
            player:messageSpecial(ID.text.ALREADY_OBTAINED_FRAG, tpz.ki.ICE_FRAGMENT)
        end
    elseif (player:hasCompletedMission(tpz.mission.log_id.ZILART, tpz.mission.id.zilart.HEADSTONE_PILGRIMAGE)) then
        player:messageSpecial(ID.text.ZILART_MONUMENT)
    else
        player:messageSpecial(ID.text.CANNOT_REMOVE_FRAG)
    end

end

entity.onEventUpdate = function(player, csid, option)
end

entity.onEventFinish = function(player, csid, option)

    if (csid == 200 and option == 1) then
        player:addKeyItem(tpz.ki.ICE_FRAGMENT)
        -- Check and see if all fragments have been found (no need to check ice and dark frag)
        if
            player:hasKeyItem(tpz.ki.FIRE_FRAGMENT) and
            player:hasKeyItem(tpz.ki.WIND_FRAGMENT) and
            player:hasKeyItem(tpz.ki.EARTH_FRAGMENT) and
            player:hasKeyItem(tpz.ki.LIGHTNING_FRAGMENT) and
            player:hasKeyItem(tpz.ki.WATER_FRAGMENT) and
            player:hasKeyItem(tpz.ki.LIGHT_FRAGMENT)
        then
            player:messageSpecial(ID.text.FOUND_ALL_FRAGS, tpz.ki.ICE_FRAGMENT)
            player:addTitle(tpz.title.BEARER_OF_THE_EIGHT_PRAYERS)
            player:completeMission(tpz.mission.log_id.ZILART, tpz.mission.id.zilart.HEADSTONE_PILGRIMAGE)
            player:addMission(tpz.mission.log_id.ZILART, tpz.mission.id.zilart.THROUGH_THE_QUICKSAND_CAVES)
        else
            player:messageSpecial(ID.text.KEYITEM_OBTAINED, tpz.ki.ICE_FRAGMENT)
        end
    end

end

return entity

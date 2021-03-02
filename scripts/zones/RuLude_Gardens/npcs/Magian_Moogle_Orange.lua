-----------------------------------
-- Area: Ru'Lude Gardens
--  NPC: Magian Moogle (Orange Bobble)
-- Type: Magian Trials NPC (Weapon/Empyrean Armor)
-- !pos -11 2.453 118 64
-----------------------------------
require("scripts/globals/magiantrials")
require("scripts/globals/status")
-----------------------------------
local entity = {}
local EVENT_IDS = {
    [1] = 10121,
    [2] = 10122,
    [3] = 10123,
    [4] = 10124,
    [5] = 10125,
    [6] = 10129,
}

entity.onTrade = function(player, npc, trade)
    if ENABLE_MAGIAN_TRIALS ~= 1 then
        return
    end

    tpz.magian.magianOnTrade(player, npc, trade, tpz.itemType.WEAPON, EVENT_IDS)
end

entity.onTrigger = function(player, npc)
    if ENABLE_MAGIAN_TRIALS ~= 1 then
        return
    end

    tpz.magian.magianOnTrigger(player, npc, EVENT_IDS)
end

entity.onEventUpdate = function(player, csid, option)
    tpz.magian.magianEventUpdate(player, itemId, csid, option, EVENT_IDS)
end

entity.onEventFinish = function(player, csid, option)
    tpz.magian.magianOnEventFinish(player, itemId, csid, option, EVENT_IDS)
end

return entity

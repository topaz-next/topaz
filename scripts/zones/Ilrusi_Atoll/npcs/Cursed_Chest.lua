-----------------------------------
-- Area: Ilrusi Atoll
--  NPC: Cursed Chest
-----------------------------------
local ID = require("scripts/zones/Ilrusi_Atoll/IDs")
-----------------------------------
local entity = {}

entity.onTrade = function(player, npc, trade)
end

entity.onTrigger = function(player, npc)
    player:messageSpecial(ID.text.CHEST)

    local npcID = npc:getID()
    local instance = npc:getInstance()
    local figureheadChest = instance:getProgress()

    if npcID == figureheadChest then
        player:messageSpecial(ID.text.GOLDEN)
        instance:complete()
        for i, v in pairs(ID.mob[2]) do
            DespawnMob(v, instance)
        end
    else
        SpawnMob(npcID, instance):updateClaim(player)
    end
end

entity.onEventUpdate = function(player, csid, option)
end

entity.onEventFinish = function(player, csid, option)
end

return entity

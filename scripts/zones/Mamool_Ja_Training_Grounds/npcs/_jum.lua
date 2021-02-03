-----------------------------------
-- Area: Mamool Ja Training Grounds
-- Npc: Pot Hatch
-- !pos 267 0 -582
-----------------------------------
local ID = require("scripts/zones/Mamool_Ja_Training_Grounds/IDs")
require("scripts/globals/status")
require("scripts/zones/Mamool_Ja_Training_Grounds/globals/zoneUtil")
-----------------------------------

function onTrade(player,npc,trade)
end

function onTrigger(player,npc)
    zoneUtil.ImperialAgent_PotHatch(player, npc, 269, -582, 124)
end

function onEventUpdate(player,csid,option)
end

function onEventFinish(player,csid,option)
end

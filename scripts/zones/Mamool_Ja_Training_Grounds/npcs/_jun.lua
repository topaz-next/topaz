-----------------------------------
-- Area: Mamool Ja Training Grounds
-- Npc: Pot Hatch
-- !pos 221 0 -415
-----------------------------------
local ID = require("scripts/zones/Mamool_Ja_Training_Grounds/IDs")
require("scripts/globals/status")
require("scripts/zones/Mamool_Ja_Training_Grounds/globals/zoneUtil")
-----------------------------------

function onTrade(player,npc,trade)
end

function onTrigger(player,npc)
    zoneUtil.ImperialAgent_PotHatch(player, npc, 220, -415, 15)
end

function onEventUpdate(player,csid,option)
end

function onEventFinish(player,csid,option)
end

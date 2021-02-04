-----------------------------------
-- Area: Mamool Ja Training Grounds
-- Npc: Pot Hatch
-- !pos 183 0 -581
-----------------------------------
local ID = require("scripts/zones/Mamool_Ja_Training_Grounds/IDs")
local zoneUtil = require("scripts/zones/Mamool_Ja_Training_Grounds/globals/zoneUtil")
require("scripts/globals/status")
-----------------------------------

function onTrade(player,npc,trade)
end

function onTrigger(player,npc)
    zoneUtil.ImperialAgent_PotHatch(player, npc, 184, -583, 185)
end

function onEventUpdate(player,csid,option)
end

function onEventFinish(player,csid,option)
end

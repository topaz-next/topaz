-----------------------------------
-- ID: 5349
-- Cutter Fireflies
-- Transports the user to Arrapago Reef
-----------------------------------
require("scripts/globals/teleports")
require("scripts/globals/status")
require("scripts/globals/zone")
-----------------------------------
local item_object = {}

item_object.onItemCheck = function(target)
    if target:getZoneID() == tpz.zone.THE_ASHU_TALIF then
        return 0
    end
    return 56
end

item_object.onItemUse = function(target)
    target:addStatusEffectEx(tpz.effect.TELEPORT, 0, tpz.teleport.id.CUTTER, 0, 1)
end

return item_object

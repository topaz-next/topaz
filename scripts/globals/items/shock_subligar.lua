-----------------------------------
-- ID: 15650
-- Item: shock subligar
-- Item Effect: Shock Spikes
-----------------------------------
require("scripts/globals/settings")
require("scripts/globals/status")
-----------------------------------
local item_object = {}

item_object.onItemCheck = function(target)
    return 0
end

item_object.onItemUse = function(target)
    target:addStatusEffect(tpz.effect.SHOCK_SPIKES, 7, 0, 180)
end

return item_object

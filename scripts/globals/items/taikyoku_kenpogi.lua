-----------------------------------
-- ID: 14541
-- Item: taikyoku_kenpogi
-- Item Effect: Eva +3
-- Duration: 30 Minutes
-----------------------------------
require("scripts/globals/status")
-----------------------------------
local item_object = {}

item_object.onItemCheck = function(target)
    local effect = target:getStatusEffect(tpz.effect.ENCHANTMENT)
    if effect ~= nil and effect:getSubType() == 14541 then
        target:delStatusEffect(tpz.effect.ENCHANTMENT)
    end
    return 0
end

item_object.onItemUse = function(target)
    target:addStatusEffect(tpz.effect.ENCHANTMENT, 0, 0, 1800, 14541)
end

item_object.onEffectGain = function(target, effect)
    target:addMod(tpz.mod.EVA, 3)
end

item_object.onEffectLose = function(target, effect)
    target:delMod(tpz.mod.EVA, 3)
end

return item_object

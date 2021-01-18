-----------------------------------
-- ID: 14531
-- Item: bannaret_mail
-- Item Effect: HP +15, Enmity +2
-- Duration: 30 Minutes
-----------------------------------
require("scripts/globals/status")
-----------------------------------
local item_object = {}

item_object.onItemCheck = function(target)
    local effect = target:getStatusEffect(tpz.effect.ENCHANTMENT)
    if effect ~= nil and effect:getSubType() == 14531 then
        target:delStatusEffect(tpz.effect.ENCHANTMENT)
    end
    return 0
end

item_object.onItemUse = function(target)
    target:addStatusEffect(tpz.effect.ENCHANTMENT, 0, 0, 1800, 14531)
end

item_object.onEffectGain = function(target, effect)
    target:addMod(tpz.mod.HP, 15)
    target:addMod(tpz.mod.ENMITY, 2)
end

item_object.onEffectLose = function(target, effect)
    target:delMod(tpz.mod.HP, 15)
    target:delMod(tpz.mod.ENMITY, 2)
end

return item_object

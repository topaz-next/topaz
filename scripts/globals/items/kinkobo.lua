-----------------------------------
-- ID: 17592
-- Item: Kinkobo
-- Enchantment: Subtle Blow
-- Duration: 60 Mins
-----------------------------------
require("scripts/globals/status")
require("scripts/globals/settings")
-----------------------------------
local item_object = {}

item_object.onItemCheck = function(target)
    return 0
end

item_object.onItemUse = function(target)
    if (target:addStatusEffect(tpz.effect.ENCHANTMENT) == false) then
        target:addStatusEffect(tpz.effect.ENCHANTMENT, 0, 0, 3600, 17592)
    end
end

item_object.onEffectGain = function(target, effect)
    target:addMod(tpz.mod.SUBTLE_BLOW, 20)
end

item_object.onEffectLose = function(target, effect)
    target:delMod(tpz.mod.SUBTLE_BLOW, 20)
end

return item_object

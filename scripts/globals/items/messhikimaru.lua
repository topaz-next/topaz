-----------------------------------
-- ID: 17826
-- Item: Messhikimaru
-- Enchantment: Arcana Killer
-- Durration: 10 Mins
-----------------------------------
require("scripts/globals/settings")
require("scripts/globals/status")
-----------------------------------
local item_object = {}

item_object.onItemCheck = function(target)
    result = 0
end

item_object.onItemUse = function(target)
    if (target:hasStatusEffect(tpz.effect.ENCHANTMENT) == false) then
        target:addStatusEffect(tpz.effect.ENCHANTMENT, 0, 0, 600, 17826)
    end
end

item_object.onEffectGain = function(target, effect)
    target:addMod(tpz.mod.ARCANA_KILLER, 20)
end

item_object.onEffectLose = function(target, effect)
    target:delMod(tpz.mod.ARCANA_KILLER, 20)
end

return item_object

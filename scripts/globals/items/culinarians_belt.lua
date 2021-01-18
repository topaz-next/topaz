-----------------------------------
-- ID: 15451
-- Item: Culinarian's Belt
-- Enchantment: Synthesis image support
-- 2Min, All Races
-----------------------------------
-- Enchantment: Synthesis image support
-- Duration: 2Min
-- Alchemy Skill +3
-----------------------------------
require("scripts/globals/status")
-----------------------------------
local item_object = {}

item_object.onItemCheck = function(target)
    local result = 0
    if (target:hasStatusEffect(tpz.effect.COOKING_IMAGERY) == true) then
        result = 243
    end
    return result
end

item_object.onItemUse = function(target)
    target:addStatusEffect(tpz.effect.COOKING_IMAGERY, 3, 0, 120)
end

item_object.onEffectGain = function(target, effect)
    target:addMod(tpz.mod.COOK, 1)
end

item_object.onEffectLose = function(target, effect)
    target:delMod(tpz.mod.COOK, 1)
end

return item_object

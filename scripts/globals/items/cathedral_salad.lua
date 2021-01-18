-----------------------------------
-- ID: 5679
-- Item: cathedral_salad
-- Food Effect: 4 Hrs, All Races
-----------------------------------
-- MP 15% Cap 90
-- Agility 7
-- Mind 7
-- Strength -5
-- Vitality -5
-- Ranged Accuracy +17
-----------------------------------
require("scripts/globals/status")
require("scripts/globals/msg")
-----------------------------------
local item_object = {}

item_object.onItemCheck = function(target)
    local result = 0
    if (target:hasStatusEffect(tpz.effect.FOOD)) then
        result = tpz.msg.basic.IS_FULL
    end
    return result
end

item_object.onItemUse = function(target)
    target:addStatusEffect(tpz.effect.FOOD, 0, 0, 14400, 5679)
end

item_object.onEffectGain = function(target, effect)
    target:addMod(tpz.mod.FOOD_MPP, 15)
    target:addMod(tpz.mod.FOOD_MP_CAP, 90)
    target:addMod(tpz.mod.AGI, 7)
    target:addMod(tpz.mod.MND, 7)
    target:addMod(tpz.mod.STR, -5)
    target:addMod(tpz.mod.VIT, -5)
    target:addMod(tpz.mod.RACC, 17)
end

item_object.onEffectLose = function(target, effect)
    target:delMod(tpz.mod.FOOD_MPP, 15)
    target:delMod(tpz.mod.FOOD_MP_CAP, 90)
    target:delMod(tpz.mod.AGI, 7)
    target:delMod(tpz.mod.MND, 7)
    target:delMod(tpz.mod.STR, -5)
    target:delMod(tpz.mod.VIT, -5)
    target:delMod(tpz.mod.RACC, 17)
end

return item_object

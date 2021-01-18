-----------------------------------
-- ID: 5773
-- Item: mushroom_crepe
-- Food Effect: 30Min, All Races
-----------------------------------
-- Mind 2
-- MP % 10 (cap 30)
-- Magic Accuracy +10
-- Magic Def. Bonus +5
-----------------------------------
require("scripts/globals/status")
require("scripts/globals/msg")
-----------------------------------
local item_object = {}

item_object.onItemCheck = function(target)
    local result = 0
    if target:hasStatusEffect(tpz.effect.FOOD) or target:hasStatusEffect(tpz.effect.FIELD_SUPPORT_FOOD) then
        result = tpz.msg.basic.IS_FULL
    end
    return result
end

item_object.onItemUse = function(target)
    target:addStatusEffect(tpz.effect.FOOD, 0, 0, 1800, 5773)
end

item_object.onEffectGain = function(target, effect)
    target:addMod(tpz.mod.MND, 2)
    target:addMod(tpz.mod.FOOD_MPP, 10)
    target:addMod(tpz.mod.FOOD_MP_CAP, 30)
    target:addMod(tpz.mod.MACC, 10)
    target:addMod(tpz.mod.MDEF, 5)
end

item_object.onEffectLose = function(target, effect)
    target:delMod(tpz.mod.MND, 2)
    target:delMod(tpz.mod.FOOD_MPP, 10)
    target:delMod(tpz.mod.FOOD_MP_CAP, 30)
    target:delMod(tpz.mod.MACC, 10)
    target:delMod(tpz.mod.MDEF, 5)
end

return item_object

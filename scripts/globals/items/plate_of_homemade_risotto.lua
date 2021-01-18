-----------------------------------
-- ID: 5229
-- Item: plate_of_homemade_risotto
-- Food Effect: 30Min, All Races
-----------------------------------
-- Mind 1
-- Accuracy +12% (cap 80)
-- Attack +10% (cap 40)
-- Ranged Accuracy +12% (cap 80)
-- Ranged Attack +10% (cap 40)
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
    target:addStatusEffect(tpz.effect.FOOD, 0, 0, 1800, 5229)
end

item_object.onEffectGain = function(target, effect)
    target:addMod(tpz.mod.MND, 1)
    target:addMod(tpz.mod.FOOD_ACCP, 12)
    target:addMod(tpz.mod.FOOD_ACC_CAP, 80)
    target:addMod(tpz.mod.FOOD_ATTP, 10)
    target:addMod(tpz.mod.FOOD_ATT_CAP, 40)
    target:addMod(tpz.mod.FOOD_RACCP, 12)
    target:addMod(tpz.mod.FOOD_RACC_CAP, 80)
    target:addMod(tpz.mod.FOOD_RATTP, 10)
    target:addMod(tpz.mod.FOOD_RATT_CAP, 40)
end

item_object.onEffectLose = function(target, effect)
    target:delMod(tpz.mod.MND, 1)
    target:delMod(tpz.mod.FOOD_ACCP, 12)
    target:delMod(tpz.mod.FOOD_ACC_CAP, 80)
    target:delMod(tpz.mod.FOOD_ATTP, 10)
    target:delMod(tpz.mod.FOOD_ATT_CAP, 40)
    target:delMod(tpz.mod.FOOD_RACCP, 12)
    target:delMod(tpz.mod.FOOD_RACC_CAP, 80)
    target:delMod(tpz.mod.FOOD_RATTP, 10)
    target:delMod(tpz.mod.FOOD_RATT_CAP, 40)
end

return item_object

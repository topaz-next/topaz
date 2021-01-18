-----------------------------------
-- ID: 6462
-- Item: bowl_of_salt_ramen
-- Food Effect: 30Min, All Races
-----------------------------------
-- DEX +5
-- VIT +5
-- AGI +5
-- Accuracy +5% (cap 90)
-- Ranged Accuracy +5% (cap 90)
-- Evasion +5% (cap 90)
-- Resist Slow +10
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
    target:addStatusEffect(tpz.effect.FOOD, 0, 0, 1800, 6462)
end

item_object.onEffectGain = function(target, effect)
    target:addMod(tpz.mod.DEX, 5)
    target:addMod(tpz.mod.VIT, 5)
    target:addMod(tpz.mod.AGI, 5)
    target:addMod(tpz.mod.FOOD_ACCP, 5)
    target:addMod(tpz.mod.FOOD_ACC_CAP, 90)
    target:addMod(tpz.mod.FOOD_RACCP, 5)
    target:addMod(tpz.mod.FOOD_RACC_CAP, 90)
    -- target:addMod(tpz.mod.FOOD_EVAP, 5)
    -- target:addMod(tpz.mod.FOOD_EVA_CAP, 90)
    target:addMod(tpz.mod.SLOWRES, 10)
end

item_object.onEffectLose = function(target, effect)
    target:delMod(tpz.mod.DEX, 5)
    target:delMod(tpz.mod.VIT, 5)
    target:delMod(tpz.mod.AGI, 5)
    target:delMod(tpz.mod.FOOD_ACCP, 5)
    target:delMod(tpz.mod.FOOD_ACC_CAP, 90)
    target:delMod(tpz.mod.FOOD_RACCP, 5)
    target:delMod(tpz.mod.FOOD_RACC_CAP, 90)
    -- target:delMod(tpz.mod.FOOD_EVAP, 5)
    -- target:delMod(tpz.mod.FOOD_EVA_CAP, 90)
    target:delMod(tpz.mod.SLOWRES, 10)
end

return item_object

-----------------------------------
-- ID: 4502
-- Item: Marron Glace
-- Food Effect: 180Min, All Races
-----------------------------------
-- Magic % 13
-- Magic Cap 85
-- Magic Regen While Healing 1
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
    target:addStatusEffect(tpz.effect.FOOD, 0, 0, 10800, 4502)
end

item_object.onEffectGain = function(target, effect)
    target:addMod(tpz.mod.FOOD_MPP, 13)
    target:addMod(tpz.mod.FOOD_MP_CAP, 85)
    target:addMod(tpz.mod.MPHEAL, 1)
end

item_object.onEffectLose = function(target, effect)
    target:delMod(tpz.mod.FOOD_MPP, 13)
    target:delMod(tpz.mod.FOOD_MP_CAP, 85)
    target:delMod(tpz.mod.MPHEAL, 1)
end

return item_object

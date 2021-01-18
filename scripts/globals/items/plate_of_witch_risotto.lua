-----------------------------------
-- ID: 4330
-- Item: witch_risotto
-- Food Effect: 4hours, All Races
-----------------------------------
-- Magic Points 35
-- Strength -1
-- Vitality 3
-- Mind 3
-- MP Recovered While Healing 2
-- Enmity -4
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
    target:addStatusEffect(tpz.effect.FOOD, 0, 0, 14400, 4330)
end

item_object.onEffectGain = function(target, effect)
    target:addMod(tpz.mod.MP, 35)
    target:addMod(tpz.mod.STR, -1)
    target:addMod(tpz.mod.VIT, 3)
    target:addMod(tpz.mod.MND, 3)
    target:addMod(tpz.mod.MPHEAL, 2)
    target:addMod(tpz.mod.ENMITY, -4)
end

item_object.onEffectLose = function(target, effect)
    target:delMod(tpz.mod.MP, 35)
    target:delMod(tpz.mod.STR, -1)
    target:delMod(tpz.mod.VIT, 3)
    target:delMod(tpz.mod.MND, 3)
    target:delMod(tpz.mod.MPHEAL, 2)
    target:delMod(tpz.mod.ENMITY, -4)
end

return item_object

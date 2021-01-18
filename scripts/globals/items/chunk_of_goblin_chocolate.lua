-----------------------------------
-- ID: 4495
-- Item: chunk_of_goblin_chocolate
-- Food Effect: 3Min, All Races
-----------------------------------
-- Health Regen While Healing 5
-- Lizard Killer 10
-- Petrify resistance +10
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
    target:addStatusEffect(tpz.effect.FOOD, 0, 0, 180, 4495)
end

item_object.onEffectGain = function(target, effect)
    target:addMod(tpz.mod.HPHEAL, 5)
    target:addMod(tpz.mod.LIZARD_KILLER, 5)
    target:addMod(tpz.mod.PETRIFYRES, 5)
end

item_object.onEffectLose = function(target, effect)
    target:delMod(tpz.mod.HPHEAL, 5)
    target:delMod(tpz.mod.LIZARD_KILLER, 5)
    target:delMod(tpz.mod.PETRIFYRES, 5)
end

return item_object

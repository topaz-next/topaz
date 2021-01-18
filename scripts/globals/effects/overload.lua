-----------------------------------
-- tpz.effect.OVERLOAD
-----------------------------------
require("scripts/globals/status")
-----------------------------------
local effect_object = {}

effect_object.onEffectGain = function(target, effect)
    local pet = target:getPet()
    if pet then
        pet:setLocalVar("overload", 1)
        pet:addMod(tpz.mod.HASTE_MAGIC, -5000)
        pet:addMod(tpz.mod.MOVE, -50)
        pet:addMod(tpz.mod.EVA, -10)
    end
end

effect_object.onEffectTick = function(target, effect)
end

effect_object.onEffectLose = function(target, effect)
    local pet = target:getPet()
    if pet and pet:getLocalVar("overload") ~= 0 then
        pet:setLocalVar("overload", 0)
        pet:delMod(tpz.mod.HASTE_MAGIC, -5000)
        pet:delMod(tpz.mod.MOVE, -50)
        pet:delMod(tpz.mod.EVA, -10)
    end
end

return effect_object

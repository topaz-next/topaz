-----------------------------------
-- ID: 4155
-- Item: Remedy
-- Item Effect: This potion remedies status ailments.
-- Works on paralysis, silence, blindness, poison, and disease.
-----------------------------------
require("scripts/globals/status")
-----------------------------------
local item_object = {}

item_object.onItemCheck = function(target)
    return 0
end

item_object.onItemUse = function(target)

    if (target:hasStatusEffect(tpz.effect.SILENCE) == true) then
        target:delStatusEffect(tpz.effect.SILENCE)
    end
    if (target:hasStatusEffect(tpz.effect.BLINDNESS) == true) then
        target:delStatusEffect(tpz.effect.BLINDNESS)
    end
    if (target:hasStatusEffect(tpz.effect.POISON) == true) then
        target:delStatusEffect(tpz.effect.POISON)
    end
    if (target:hasStatusEffect(tpz.effect.PARALYSIS) == true) then
        target:delStatusEffect(tpz.effect.PARALYSIS)
    end

    local rDisease = math.random(1, 2) -- Disease is not garunteed to be cured, 1 means removed 2 means fail. 50% chance
    if (rDisease == 1 and target:hasStatusEffect(tpz.effect.DISEASE) == true) then
        target:delStatusEffect(tpz.effect.DISEASE)
    end
end


return item_object

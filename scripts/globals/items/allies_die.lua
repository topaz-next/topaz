-----------------------------------
-- ID: 5502
-- Allies' Die
-- Teaches the job ability Allies Roll
-----------------------------------
local item_object = {}

item_object.onItemCheck = function(target)
    return target:canLearnAbility(tpz.jobAbility.ALLIES_ROLL)
end

item_object.onItemUse = function(target)
    target:addLearnedAbility(tpz.jobAbility.ALLIES_ROLL)
end

return item_object

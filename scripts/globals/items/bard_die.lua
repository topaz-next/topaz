-----------------------------------
-- ID: 5486
-- Bard Die
-- Teaches the job ability Choral Roll
-----------------------------------
local item_object = {}

item_object.onItemCheck = function(target)
    return target:canLearnAbility(tpz.jobAbility.CHORAL_ROLL)
end

item_object.onItemUse = function(target)
    target:addLearnedAbility(tpz.jobAbility.CHORAL_ROLL)
end

return item_object

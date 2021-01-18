-----------------------------------
-- ID: 5495
-- Dancer Die
-- Teaches the job ability Dancer's Roll
-----------------------------------
local item_object = {}

item_object.onItemCheck = function(target)
    return target:canLearnAbility(tpz.jobAbility.DANCERS_ROLL)
end

item_object.onItemUse = function(target)
    target:addLearnedAbility(tpz.jobAbility.DANCERS_ROLL)
end

return item_object

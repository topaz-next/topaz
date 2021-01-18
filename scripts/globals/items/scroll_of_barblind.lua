-----------------------------------
-- ID: 4683
-- Scroll of Barblind
-- Teaches the white magic Barblind
-----------------------------------
local item_object = {}

item_object.onItemCheck = function(target)
    return target:canLearnSpell(75)
end

item_object.onItemUse = function(target)
    target:addSpell(75)
end

return item_object

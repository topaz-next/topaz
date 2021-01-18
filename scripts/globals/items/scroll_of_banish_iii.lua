-----------------------------------
-- ID: 4638
-- Scroll of Banish III
-- Teaches the white magic Banish III
-----------------------------------
local item_object = {}

item_object.onItemCheck = function(target)
    return target:canLearnSpell(30)
end

item_object.onItemUse = function(target)
    target:addSpell(30)
end

return item_object

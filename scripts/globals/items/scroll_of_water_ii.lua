-----------------------------------
-- ID: 4778
-- Scroll of Water II
-- Teaches the black magic Water II
-----------------------------------
local item_object = {}

item_object.onItemCheck = function(target)
    return target:canLearnSpell(170)
end

item_object.onItemUse = function(target)
    target:addSpell(170)
end

return item_object

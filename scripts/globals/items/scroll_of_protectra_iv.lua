-----------------------------------
-- ID: 4736
-- Scroll of Protectra IV
-- Teaches the white magic Protectra IV
-----------------------------------
local item_object = {}

item_object.onItemCheck = function(target)
    return target:canLearnSpell(128)
end

item_object.onItemUse = function(target)
    target:addSpell(128)
end

return item_object

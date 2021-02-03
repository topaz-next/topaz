-----------------------------------------
-- Teaches Blizzara II
-----------------------------------------

function onItemCheck(target)
    return target:canLearnSpell(831)
end

function onItemUse(target)
    target:addSpell(831)
end
-----------------------------------------
-- Indi-Poison
-- Teaches Indi-Poison
-----------------------------------------

function onItemCheck(target)
    return target:canLearnSpell(769)
end

function onItemUse(target)
    target:addSpell(769)
end
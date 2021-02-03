-----------------------------------------
-- Indi-Fend
-- Teaches Indi-Fend
-----------------------------------------

function onItemCheck(target)
    return target:canLearnSpell(782)
end

function onItemUse(target)
    target:addSpell(782)
end
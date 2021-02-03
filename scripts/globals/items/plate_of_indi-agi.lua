-----------------------------------------
-- Indi-AGI
-- Teaches Indi-AGI
-----------------------------------------

function onItemCheck(target)
    return target:canLearnSpell(775)
end

function onItemUse(target)
    target:addSpell(775)
end
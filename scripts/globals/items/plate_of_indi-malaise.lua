-----------------------------------------
-- Indi-Malaise
-- Teaches Indi-Malaise
-----------------------------------------

function onItemCheck(target)
    return target:canLearnSpell(790)
end

function onItemUse(target)
    target:addSpell(790)
end
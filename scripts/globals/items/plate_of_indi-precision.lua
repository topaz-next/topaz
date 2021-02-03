----------------------------------------
-- Indi-Precision
-- Teaches Indi-Precision
-----------------------------------------

function onItemCheck(target)
    return target:canLearnSpell(783)
end

function onItemUse(target)
    target:addSpell(783)
end
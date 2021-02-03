-----------------------------------------
-- Indi-Gravity
-- Teaches Indi-Gravity
-----------------------------------------

function onItemCheck(target)
    return target:canLearnSpell(797)
end

function onItemUse(target)
    target:addSpell(797)
end
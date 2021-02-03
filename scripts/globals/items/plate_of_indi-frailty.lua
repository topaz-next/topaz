-----------------------------------------
-- Indi-frailty
-- Teaches Indi-frailty
-----------------------------------------

function onItemCheck(target)
    return target:canLearnSpell(788)
end

function onItemUse(target)
    target:addSpell(788)
end
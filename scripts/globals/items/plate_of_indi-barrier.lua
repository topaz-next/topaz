-----------------------------------------
-- Indi-Barrier
-- Teaches Indi-Barrier
-----------------------------------------

function onItemCheck(target)
    return target:canLearnSpell(780)
end

function onItemUse(target)
    target:addSpell(780)
end
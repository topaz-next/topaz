-----------------------------------------
-- Indi-Focus
-- Teaches Indi-Focus
-----------------------------------------

function onItemCheck(target)
    return target:canLearnSpell(785)
end

function onItemUse(target)
    target:addSpell(785)
end
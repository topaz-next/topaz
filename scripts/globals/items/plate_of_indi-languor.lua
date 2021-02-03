-----------------------------------------
-- Indi-languor
-- Teaches Indi-Languor
-----------------------------------------

function onItemCheck(target)
    return target:canLearnSpell(794)
end

function onItemUse(target)
    target:addSpell(794)
end
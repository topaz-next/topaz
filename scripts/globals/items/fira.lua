-----------------------------------------
-- Teaches Fira
-----------------------------------------

function onItemCheck(target)
    return target:canLearnSpell(828)
end

function onItemUse(target)
    target:addSpell(828)
end
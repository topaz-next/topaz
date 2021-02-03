-----------------------------------------
-- Teaches Watera
-----------------------------------------

function onItemCheck(target)
    return target:canLearnSpell(838)
end

function onItemUse(target)
    target:addSpell(838)
end
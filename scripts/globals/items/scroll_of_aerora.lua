-----------------------------------------
-- Teaches Aerora
-----------------------------------------

function onItemCheck(target)
    return target:canLearnSpell(832)
end

function onItemUse(target)
    target:addSpell(832)
end
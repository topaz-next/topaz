-----------------------------------------
-- Teaches Watera II
-----------------------------------------

function onItemCheck(target)
    return target:canLearnSpell(839)
end

function onItemUse(target)
    target:addSpell(839)
end
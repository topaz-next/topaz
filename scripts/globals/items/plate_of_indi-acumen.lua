-----------------------------------------
-- Indi-Acumen
-- Teaches Indi-Acumen
-----------------------------------------

function onItemCheck(target)
    return target:canLearnSpell(781)
end

function onItemUse(target)
    target:addSpell(781)
end
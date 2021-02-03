-----------------------------------------
-- Teaches Indi-Voidance	
-----------------------------------------

function onItemCheck(target)
    return target:canLearnSpell(784)
end

function onItemUse(target)
	target:addSpell(784)
end
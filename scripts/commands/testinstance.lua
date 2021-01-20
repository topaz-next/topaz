-----------------------------------
-- func: !testinstance <instance_id>
-- desc: Load an instance and take you there
-----------------------------------

cmdprops =
{
    permission = 1,
    parameters = "i"
}

function error(player, msg)
    player:PrintToPlayer(msg)
    player:PrintToPlayer("!testinstance <instance_id>")
end

function onTrigger(player, instance_id)
    player:createInstance(instance_id)
    player:PrintToPlayer(string.format("Going to instance %d in 5 seconds...", instance_id))
    player:timer(5000, function(player)
        local instance = player:getInstance()
        player:PrintToPlayer(string.format("Going to %s...", instance:getName()))
        player:instanceEntry(player, 1)
    end)
end

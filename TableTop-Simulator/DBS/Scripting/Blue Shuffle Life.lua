function onLoad()
    self.UI.setXml([[
        <Panel width="664" height="175" position="0 0 -50" id="BlueLifeShuffle" active="false">


            <Button id="submitButton" onClick="onSubmit" color="#ffffff" image="https://steamusercontent-a.akamaihd.net/ugc/2465231303189235030/405B37287E9359C9EFD20265C24326DC33D2CE0F/" />

        </Panel>
    ]])
    self.interactable = false
end


function onSubmit(player, value, id)

    -- Correctly call the global function with the color parameter
    Global.call("shuffleDeck", {DeckType = "Life", Color = "Blue"})

    -- Correctly set the UI attribute to make the panel inactive
    self.UI.setAttribute("BlueLifeShuffle", "active", "false")
    
    -- Adjust the position of the checker object
    local position = self.getPosition()
    self.setPosition({position.x, position.y + 100, position.z})
end


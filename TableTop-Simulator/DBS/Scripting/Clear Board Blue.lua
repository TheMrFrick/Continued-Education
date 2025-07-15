function onLoad()
    self.UI.setXml([[
        <Panel width="1500" height="250" position="0 0 -50" color="#00000000">
            <VerticalLayout spacing="5">

                <Button text="CLEAR" fontSize="200" id="submitButton" onClick="onSubmitScoop" color="Blue" >
                    <Image image="https://steamusercontent-a.akamaihd.net/ugc/2365040797438966890/99E0647551CFFB04A4B880BE768209AAD0A41FC6/" />
                </Button>
            </VerticalLayout>
        </Panel>
    ]])
    self.interactable = false
end

function onSubmitScoop(player, value, id)
    local params = {
        color="Blue",
        removeCards=true
    }
    Global.call("scoopCards", params)
end




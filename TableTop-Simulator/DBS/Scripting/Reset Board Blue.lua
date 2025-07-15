function onLoad()
    self.UI.setXml([[
        <Panel width="1500" height="250" position="0 0 -50" color="#00000000">
            <VerticalLayout spacing="5">

                <Button text="RESET" fontSize="200" id="submitButton" onClick="onSubmitScoop" color="Blue" >
                    <Image image="https://steamusercontent-a.akamaihd.net/ugc/2365040797438966990/A8EE75A4AE7F546F4CB5A1D70D9726DA135214FF/" />
                </Button>
            </VerticalLayout>
        </Panel>
    ]])
    self.interactable = false
end

function onSubmitScoop(player, value, id)
    local params = {
        color="Blue",
        removeCards=false
    }
    Global.call("scoopCards", params)
end




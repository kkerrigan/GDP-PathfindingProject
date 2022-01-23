--create the master command group that 
--will hold all the commands/command groups
createCommandGroup("MasterGroup", false)

    --move the camera to the back of the ship
    createCommandGroup("CameraMove", true)
    addCommandGroupToGroup("CameraMove","MasterGroup")
    cameraFollowEntity(
            "CameraMove",     -- command group
            7,                -- Entity to follow
            0.0,0.0,0.0,     -- follow distance
            5.0,            -- min distance
            50.0,            -- max speed distance
            72.0,            -- max speed
            -25.0,-40.0,-550.0,-- offset
            0.0,            -- easeIn
            1.0,            -- easeOut
            5.0            -- time to follow for
        )
	orientCameraToEntity(
				"CameraMove", -- command group
				6,			  -- Entity to look at
				5.0,		  -- speed
				1.0,		  -- Ease-in
				1.0			  -- Ease-out
		)
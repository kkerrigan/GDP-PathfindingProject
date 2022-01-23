	-- Start moving the attacking ship
	createCommandGroup("MoveAttacking", false)
	addCommandGroupToGroup("MoveAttacking","MasterGroup")
	moveToTime(
			"MoveAttacking", --command group
			7, 					--entity to move
			-34.0, 125, -50, 		--position to move to 
			15, 					--duration
			4.0,				--ease in 
			0.0					--ease out
		)
	-- Orient attacking ship to the defending ship
	orientToEntity( 
			"MoveAttacking",
			7, 		-- Entity to rotate
			6,		-- Entity to look at
			1.0,	-- Speed
			0.0, 	-- Ease-in
			0.0		-- Ease-out
		)
		
	-- Have the camera follow the attacking ship
	cameraFollowEntity(
            "MoveAttacking",     -- command group
            7,                -- Entity to follow
            0.0,0.0,0.0,     -- follow distance
            5.0,            -- min distance
            50.0,            -- max speed distance
            40.0,            -- max speed
            -80.0,14.0,10.0,-- offset
            0.0,            -- easeIn
            1.0,            -- easeOut
            15.0            -- time to follow for
        )
		
	-- Orient the camera to the defending ship
	orientCameraToEntity(
				"MoveAttacking", -- command group
				6,			  -- Entity to look at
				5.0,		  -- speed
				1.0,		  -- Ease-in
				1.0			  -- Ease-out
		)
			
	createCommandGroup("ElapseTime", false)
    addCommandGroupToGroup("ElapseTime","MoveAttacking")
        wait("ElapseTime", 6.0)
    
	-- Start moving the defending plane
    createCommandGroup("MoveDefending", false)
	addCommandGroupToGroup("MoveDefending","MoveAttacking")
	moveToTime(
			"MoveDefending", --command group
			6, 					--entity to move
			-5, 125, -50, 		--position to move to 
			4, 					--duration
			0.0,				--ease in 
			0					--ease out
		)
	
	orientToEntity( 
			"MoveDefending",
			6, 		-- Entity to rotate
			7,		-- Entity to look at
			1.0,	-- Speed
			0.0, 	-- Ease-in
			0.0		-- Ease-out
		)
		
	-- Ship rotations start here
	createCommandGroup("ShipsRotate",false)
	addCommandGroupToGroup("ShipsRotate","MoveAttacking")
		entityFollowCurve(
			"ShipsRotate", 	--Command group
			7, 				--entity
			-44, 125, -50,	--destination 
			5,				--time 
			-46, 125, -55.0,	--control point
			0.0,			--easeIn
			1.0			--easeOut
		)
		
		entityFollowCurve(
			"ShipsRotate", 	--Command group
			6, 				--entity
			-20, 125, -65,	--destination 
			5,				--time 
			-10, 125, -50.0,	--control point
			0.0,			--easeIn
			1.0			--easeOut
		)
		
		cameraMoveToTime(
			"ShipsRotate", 	-- command group
			-50, 130, -130,	-- position	
			20,				-- time
			0.0,			-- easeIn
			2.0				-- easeOut
		)
		
		orientCameraToEntity(
				"ShipsRotate", -- command group
				7,			  -- Entity to look at
				20.0,		  -- duration
				1.0,		  -- Ease-in
				1.0			  -- Ease-out
		)
createCommandGroup("AttackScene", false)
	addCommandGroupToGroup("AttackScene","MasterGroup")
	
	-- Add fireCannons command for first ship
	fireCannons(
			"AttackScene", --command group
			6, 			   --Attacker
			7 			   --Target 
		)
		
	-- Add fireCannons command for first ship
	fireCannons(
			"AttackScene", --command group
			7, 			   --Attacker
			6 			   --Target 
		)
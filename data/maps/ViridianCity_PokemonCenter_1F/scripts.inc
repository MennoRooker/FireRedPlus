ViridianCity_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, ViridianCity_PokemonCenter_1F_OnTransition
	.byte 0

ViridianCity_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_VIRIDIAN_CITY
	end

ViridianCity_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse
	release
	end

ViridianCity_PokemonCenter_1F_EventScript_Boy::
	msgbox ViridianCity_PokemonCenter_1F_Text_PokeCenterInEveryTown, MSGBOX_NPC
	end

ViridianCity_PokemonCenter_1F_EventScript_Gentleman::
	msgbox ViridianCity_PokemonCenter_1F_Text_FeelFreeToUsePC, MSGBOX_NPC
	end

ViridianCity_PokemonCenter_1F_EventScript_Youngster::
	msgbox ViridianCity_PokemonCenter_1F_Text_PokeCentersHealMons, MSGBOX_NPC
	end

ViridianCity_PokemonCenter_1F_EventScript_ExplainPC::
	lock
	faceplayer
	signmsg
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox ViridianCity_PokemonCenter_1F_Text_HoldAButtonForInstaBox
	setvar VAR_VIRIDIAN_POKECENTER_NPC_TRIGGER, 1
	release
	end

ViridianCity_PokemonCenter_1F_EventScript_ExplainPCTrigger::
	lock
    applymovement LOCALID_PC_NERD, ViridianCity_PokemonCenter_1F_Movement_FaceLeft
	waitmovement 0
    applymovement OBJ_EVENT_ID_PLAYER, ViridianCity_PokemonCenter_1F_Movement_FaceRight
    waitmovement 0
	signmsg
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox ViridianCity_PokemonCenter_1F_Text_HoldAButtonForInstaBox
	setvar VAR_VIRIDIAN_POKECENTER_NPC_TRIGGER, 1
	release
	end

ViridianCity_PokemonCenter_1F_Movement_FaceRight::
	face_right
	step_end

ViridianCity_PokemonCenter_1F_Movement_FaceLeft::
	face_left
	step_end

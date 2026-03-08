RockTunnel_B1F_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, RockTunnel_B1F_MapScripts_MAP_SCRIPTS_ON_FRAME_TABLE
	.byte 0

RockTunnel_B1F_MapScripts_MAP_SCRIPTS_ON_FRAME_TABLE::
	map_script_2 VAR_MAP_SCENE_ROCK_TUNNEL_B1F, 0, RockTunnel_B1F_EventScript_MachokeRockSmash
	.2byte 0

RockTunnel_B1F_EventScript_MachokeRockSmash::
	lockall
	fadenewbgm MUS_FOLLOW_ME
	applymovement OBJ_EVENT_ID_PLAYER, RockTunnel_B1F_Movement_LooksRight
	waitmovement 0
	delay 30
	special SpawnCameraObject
	applymovement LOCALID_CAMERA, RockTunnel_B1F_Movement_CameraPanRight2
	waitmovement 0
	delay 40
	playmoncry SPECIES_MACHOKE, CRY_MODE_NORMAL
	message RockTunnel_B1F_Text_Machoke
	waitmoncry
	waitmessage
	delay 20
	closemessage
	delay 10
	playse SE_M_ROCK_THROW
	applymovement 16, RockTunnel_B1F_Movement_BreakRock
	waitmovement 0
	removeobject 16
	delay 40
	applymovement LOCALID_MACHOKE, RockTunnel_B1F_Movement_LooksLeft
	waitmovement 0
	delay 10
	playse SE_PIN
	applymovement LOCALID_MACHOKE, Common_Movement_ExclamationMark
	waitmovement 0
	delay 50
	applymovement LOCALID_CAMERA, RockTunnel_B1F_Movement_CameraPanLeft2
	waitmovement 0
	delay 20
	applymovement LOCALID_MACHOKE, RockTunnel_B1F_Movement_WalksLeft
	waitmovement 0
	delay 10
	playmoncry SPECIES_MACHOKE, CRY_MODE_NORMAL
	message RockTunnel_B1F_Text_Machoke
	waitmoncry
	waitmessage
	delay 20
	closemessage
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RockTunnel_B1F_Text_MachokeIsHoldingSomething
	giveitem_msg RockTunnel_B1F_Text_ObtainedHM06FromMachoke, ITEM_HM06, 1, MUS_OBTAIN_KEY_ITEM
	setflag FLAG_GOT_HM06
	setvar VAR_MAP_SCENE_ROCK_TUNNEL_B1F, 1
	special RemoveCameraObject
	fadedefaultbgm
	delay 30
	releaseall
	end

RockTunnel_B1F_Movement_CameraPanRight2::
	walk_right
	walk_right
	step_end

RockTunnel_B1F_Movement_CameraPanLeft2::
	walk_left
	walk_left
	step_end

RockTunnel_B1F_Movement_LooksRight::
	face_right
	step_end

RockTunnel_B1F_Movement_LooksLeft::
	face_left
	step_end

RockTunnel_B1F_Movement_WalksLeft::
	walk_left
	step_end

RockTunnel_B1F_Movement_BreakRock::
	rock_smash_break
	step_end

RockTunnel_B1F_EventScript_Machoke::
	playmoncry SPECIES_MACHOKE, CRY_MODE_NORMAL
	msgbox RockTunnel_B1F_Text_Machoke, MSGBOX_NPC
	waitmoncry
	end

RockTunnel_B1F_EventScript_Sofia::
	trainerbattle_single TRAINER_PICNICKER_SOFIA, RockTunnel_B1F_Text_SofiaIntro, RockTunnel_B1F_Text_SofiaDefeat
	msgbox RockTunnel_B1F_Text_SofiaPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Martha::
	trainerbattle_single TRAINER_PICNICKER_MARTHA, RockTunnel_B1F_Text_MarthaIntro, RockTunnel_B1F_Text_MarthaDefeat
	msgbox RockTunnel_B1F_Text_MarthaPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Dudley::
	trainerbattle_single TRAINER_HIKER_DUDLEY, RockTunnel_B1F_Text_DudleyIntro, RockTunnel_B1F_Text_DudleyDefeat
	msgbox RockTunnel_B1F_Text_DudleyPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Allen::
	trainerbattle_single TRAINER_HIKER_ALLEN, RockTunnel_B1F_Text_AllenIntro, RockTunnel_B1F_Text_AllenDefeat
	msgbox RockTunnel_B1F_Text_AllenPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Eric::
	trainerbattle_single TRAINER_HIKER_ERIC, RockTunnel_B1F_Text_EricIntro, RockTunnel_B1F_Text_EricDefeat
	msgbox RockTunnel_B1F_Text_EricPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Cooper::
	trainerbattle_single TRAINER_POKEMANIAC_COOPER, RockTunnel_B1F_Text_CooperIntro, RockTunnel_B1F_Text_CooperDefeat
	msgbox RockTunnel_B1F_Text_CooperPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Steve::
	trainerbattle_single TRAINER_POKEMANIAC_STEVE, RockTunnel_B1F_Text_SteveIntro, RockTunnel_B1F_Text_SteveDefeat
	msgbox RockTunnel_B1F_Text_StevePostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Winston::
	trainerbattle_single TRAINER_POKEMANIAC_WINSTON, RockTunnel_B1F_Text_WinstonIntro, RockTunnel_B1F_Text_WinstonDefeat
	msgbox RockTunnel_B1F_Text_WinstonPostBattle, MSGBOX_AUTOCLOSE
	end

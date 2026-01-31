ViridianForest_MapScripts::
# 2 "data/maps/ViridianForest/scripts.pory"
	map_script MAP_SCRIPT_ON_TRANSITION, ViridianForest_OnTransition
# 3 "data/maps/ViridianForest/scripts.pory"
	map_script MAP_SCRIPT_ON_FRAME_TABLE, ViridianForest_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE
	.byte 0

ViridianForest_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE:
# 4 "data/maps/ViridianForest/scripts.pory"
	map_script_2 VAR_BUGSY_VIRIDIAN_FOREST_SCENE, 1, ViridianForest_EventScript_BugsyCutscene
	.2byte 0


ViridianForest_OnTransition::
# 9 "data/maps/ViridianForest/scripts.pory"
	setworldmapflag FLAG_WORLD_MAP_VIRIDIAN_FOREST
	return


ViridianForest_EventScript_BugsyCutscene::
# 13 "data/maps/ViridianForest/scripts.pory"
	lockall
# 14 "data/maps/ViridianForest/scripts.pory"
	applymovement LOCALID_BUGSY_VIRIDIAN_FOREST, ViridianForest_Movement_BugsyLooksAround
# 15 "data/maps/ViridianForest/scripts.pory"
	waitmovement 0
# 16 "data/maps/ViridianForest/scripts.pory"
	textcolor NPC_TEXT_COLOR_MALE
# 17 "data/maps/ViridianForest/scripts.pory"
	msgbox ViridianForest_Text_BugsyLooksForNet, MSGBOX_NPC
# 18 "data/maps/ViridianForest/scripts.pory"
	applymovement LOCALID_BUGSY_VIRIDIAN_FOREST, ViridianForest_Movement_BugsyFacesPlayer
# 19 "data/maps/ViridianForest/scripts.pory"
	waitmovement 0
# 20 "data/maps/ViridianForest/scripts.pory"
	msgbox ViridianForest_Text_BugsySendsPlayerToRoute22, MSGBOX_NPC
# 21 "data/maps/ViridianForest/scripts.pory"
	applymovement LOCALID_BUGSY_VIRIDIAN_FOREST, ViridianForest_Movement_BugsyLeaves
# 22 "data/maps/ViridianForest/scripts.pory"
	waitmovement 0
# 23 "data/maps/ViridianForest/scripts.pory"
	removeobject LOCALID_BUGSY_VIRIDIAN_FOREST
# 24 "data/maps/ViridianForest/scripts.pory"
	setvar VAR_BUGSY_VIRIDIAN_FOREST_SCENE, 0
# 25 "data/maps/ViridianForest/scripts.pory"
	setflag FLAG_HIDE_BUGSY_VIRIDIAN_FOREST
# 26 "data/maps/ViridianForest/scripts.pory"
	releaseall
	return


# 29 "data/maps/ViridianForest/scripts.pory"
ViridianForest_Movement_BugsyLeaves:
# 30 "data/maps/ViridianForest/scripts.pory"
	walk_left
# 31 "data/maps/ViridianForest/scripts.pory"
	walk_up
# 32 "data/maps/ViridianForest/scripts.pory"
	walk_up
	step_end

# 35 "data/maps/ViridianForest/scripts.pory"
ViridianForest_Movement_BugsyFacesPlayer:
# 36 "data/maps/ViridianForest/scripts.pory"
	delay_16
# 37 "data/maps/ViridianForest/scripts.pory"
	face_up
	step_end

# 40 "data/maps/ViridianForest/scripts.pory"
ViridianForest_Movement_BugsyLooksAround:
# 41 "data/maps/ViridianForest/scripts.pory"
	face_right
# 42 "data/maps/ViridianForest/scripts.pory"
	delay_16
# 43 "data/maps/ViridianForest/scripts.pory"
	face_down
# 44 "data/maps/ViridianForest/scripts.pory"
	delay_16
# 45 "data/maps/ViridianForest/scripts.pory"
	face_left
# 46 "data/maps/ViridianForest/scripts.pory"
	delay_16
# 47 "data/maps/ViridianForest/scripts.pory"
	face_down
# 48 "data/maps/ViridianForest/scripts.pory"
	delay_16
# 49 "data/maps/ViridianForest/scripts.pory"
	delay_16
	step_end

# 52 "data/maps/ViridianForest/scripts.pory"

# 53 "data/maps/ViridianForest/scripts.pory"
ViridianForest_EventScript_Youngster::
# 54 "data/maps/ViridianForest/scripts.pory"
	msgbox ViridianForest_Text_FriendsItchingToBattle, MSGBOX_NPC
# 55 "data/maps/ViridianForest/scripts.pory"
	end
# 56 "data/maps/ViridianForest/scripts.pory"

# 57 "data/maps/ViridianForest/scripts.pory"
ViridianForest_EventScript_Boy::
# 58 "data/maps/ViridianForest/scripts.pory"
	msgbox ViridianForest_Text_RanOutOfPokeBalls, MSGBOX_NPC
# 59 "data/maps/ViridianForest/scripts.pory"
	end
# 60 "data/maps/ViridianForest/scripts.pory"

# 61 "data/maps/ViridianForest/scripts.pory"
ViridianForest_EventScript_TrainerTips1::
# 62 "data/maps/ViridianForest/scripts.pory"
	msgbox ViridianForest_Text_AvoidGrassyAreasWhenWeak, MSGBOX_SIGN
# 63 "data/maps/ViridianForest/scripts.pory"
	end
# 64 "data/maps/ViridianForest/scripts.pory"

# 65 "data/maps/ViridianForest/scripts.pory"
ViridianForest_EventScript_TrainerTips2::
# 66 "data/maps/ViridianForest/scripts.pory"
	msgbox ViridianForest_Text_UseAntidoteForPoison, MSGBOX_SIGN
# 67 "data/maps/ViridianForest/scripts.pory"
	end
# 68 "data/maps/ViridianForest/scripts.pory"

# 69 "data/maps/ViridianForest/scripts.pory"
ViridianForest_EventScript_TrainerTips3::
# 70 "data/maps/ViridianForest/scripts.pory"
	msgbox ViridianForest_Text_ContactOakViaPCToRatePokedex, MSGBOX_SIGN
# 71 "data/maps/ViridianForest/scripts.pory"
	end
# 72 "data/maps/ViridianForest/scripts.pory"

# 73 "data/maps/ViridianForest/scripts.pory"
ViridianForest_EventScript_TrainerTips4::
# 74 "data/maps/ViridianForest/scripts.pory"
	msgbox ViridianForest_Text_CantCatchOwnedMons, MSGBOX_SIGN
# 75 "data/maps/ViridianForest/scripts.pory"
	end
# 76 "data/maps/ViridianForest/scripts.pory"

# 77 "data/maps/ViridianForest/scripts.pory"
ViridianForest_EventScript_TrainerTips5::
# 78 "data/maps/ViridianForest/scripts.pory"
	msgbox ViridianForest_Text_WeakenMonsBeforeCapture, MSGBOX_SIGN
# 79 "data/maps/ViridianForest/scripts.pory"
	end
# 80 "data/maps/ViridianForest/scripts.pory"

# 81 "data/maps/ViridianForest/scripts.pory"
ViridianForest_EventScript_ExitSign::
# 82 "data/maps/ViridianForest/scripts.pory"
	msgbox ViridianForest_Text_LeavingViridianForest, MSGBOX_SIGN
# 83 "data/maps/ViridianForest/scripts.pory"
	end
# 84 "data/maps/ViridianForest/scripts.pory"

# 85 "data/maps/ViridianForest/scripts.pory"
ViridianForest_EventScript_Rick::
# 86 "data/maps/ViridianForest/scripts.pory"
	trainerbattle_single TRAINER_BUG_CATCHER_RICK, ViridianForest_Text_RickIntro, ViridianForest_Text_RickDefeat
# 87 "data/maps/ViridianForest/scripts.pory"
	msgbox ViridianForest_Text_RickPostBattle, MSGBOX_AUTOCLOSE
# 88 "data/maps/ViridianForest/scripts.pory"
	end
# 89 "data/maps/ViridianForest/scripts.pory"

# 90 "data/maps/ViridianForest/scripts.pory"
ViridianForest_EventScript_Doug::
# 91 "data/maps/ViridianForest/scripts.pory"
	trainerbattle_single TRAINER_BUG_CATCHER_DOUG, ViridianForest_Text_DougIntro, ViridianForest_Text_DougDefeat
# 92 "data/maps/ViridianForest/scripts.pory"
	msgbox ViridianForest_Text_DougPostBattle, MSGBOX_AUTOCLOSE
# 93 "data/maps/ViridianForest/scripts.pory"
	end
# 94 "data/maps/ViridianForest/scripts.pory"

# 95 "data/maps/ViridianForest/scripts.pory"
ViridianForest_EventScript_Sammy::
# 96 "data/maps/ViridianForest/scripts.pory"
	trainerbattle_single TRAINER_BUG_CATCHER_SAMMY, ViridianForest_Text_SammyIntro, ViridianForest_Text_SammyDefeat
# 97 "data/maps/ViridianForest/scripts.pory"
	msgbox ViridianForest_Text_SammyPostBattle, MSGBOX_AUTOCLOSE
# 98 "data/maps/ViridianForest/scripts.pory"
	end
# 99 "data/maps/ViridianForest/scripts.pory"

# 100 "data/maps/ViridianForest/scripts.pory"
ViridianForest_EventScript_Anthony::
# 101 "data/maps/ViridianForest/scripts.pory"
	trainerbattle_single TRAINER_BUG_CATCHER_ANTHONY, ViridianForest_Text_AnthonyIntro, ViridianForest_Text_AnthonyDefeat
# 102 "data/maps/ViridianForest/scripts.pory"
	msgbox ViridianForest_Text_AnthonyPostBattle, MSGBOX_AUTOCLOSE
# 103 "data/maps/ViridianForest/scripts.pory"
	end
# 104 "data/maps/ViridianForest/scripts.pory"

# 105 "data/maps/ViridianForest/scripts.pory"
ViridianForest_EventScript_Charlie::
# 106 "data/maps/ViridianForest/scripts.pory"
	trainerbattle_single TRAINER_BUG_CATCHER_CHARLIE, ViridianForest_Text_CharlieIntro, ViridianForest_Text_CharlieDefeat
# 107 "data/maps/ViridianForest/scripts.pory"
	msgbox ViridianForest_Text_CharliePostBattle, MSGBOX_AUTOCLOSE
# 108 "data/maps/ViridianForest/scripts.pory"
	end

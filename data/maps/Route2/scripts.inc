Route2_MapScripts::
# 2 "data/maps/Route2/scripts.pory"
	map_script MAP_SCRIPT_ON_TRANSITION, Route2_OnTransition
# 3 "data/maps/Route2/scripts.pory"
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Route2_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE
	.byte 0

Route2_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE:
# 4 "data/maps/Route2/scripts.pory"
	map_script_2 VAR_BUGSY_ROUTE2_SCENE, 0, Route2_EventScript_BugsyCutscene
	.2byte 0


Route2_OnTransition::
# 9 "data/maps/Route2/scripts.pory"
	goto_if_unset FLAG_BUGSY_ITEM_EXCHANGED, Route2_OnTransition_1
	return

Route2_OnTransition_1:
# 10 "data/maps/Route2/scripts.pory"
	getplayerxy VAR_TEMP_1, VAR_TEMP_2
# 11 "data/maps/Route2/scripts.pory"
	compare VAR_TEMP_2, 15
	goto_if_ge Route2_OnTransition_3
# 14 "data/maps/Route2/scripts.pory"
	setvar VAR_BUGSY_ROUTE2_SCENE, 0
	return

Route2_OnTransition_3:
# 12 "data/maps/Route2/scripts.pory"
	setvar VAR_BUGSY_ROUTE2_SCENE, 1
	return


Route2_EventScript_BugsyCutscene::
# 21 "data/maps/Route2/scripts.pory"
	lockall
# 22 "data/maps/Route2/scripts.pory"
	applymovement LOCALID_BUGSY_ROUTE2, Route2_Movement_BugsyFacesPlayer
# 23 "data/maps/Route2/scripts.pory"
	waitmovement 0
# 24 "data/maps/Route2/scripts.pory"
	playse SE_PIN
# 25 "data/maps/Route2/scripts.pory"
	applymovement LOCALID_BUGSY_ROUTE2, Common_Movement_ExclamationMark
# 26 "data/maps/Route2/scripts.pory"
	waitmovement 0
# 27 "data/maps/Route2/scripts.pory"
	applymovement LOCALID_BUGSY_ROUTE2, Common_Movement_Delay48
# 28 "data/maps/Route2/scripts.pory"
	waitmovement 0
# 29 "data/maps/Route2/scripts.pory"
	applymovement LOCALID_BUGSY_ROUTE2, Route2_Movement_BugsyApproach
# 30 "data/maps/Route2/scripts.pory"
	waitmovement 0
# 31 "data/maps/Route2/scripts.pory"
	msgbox Route2_Text_BugsyAsksForNet, MSGBOX_NPC
# 33 "data/maps/Route2/scripts.pory"
	checkitem ITEM_BUGSYS_NET, 1
# 35 "data/maps/Route2/scripts.pory"
	compare VAR_RESULT, FALSE
	goto_if_eq Route2_EventScript_BugsyCutscene_2
# 40 "data/maps/Route2/scripts.pory"
	msgbox Route2_Text_BugsyNetInInventory, MSGBOX_NPC
# 41 "data/maps/Route2/scripts.pory"
	removeitem ITEM_BUGSYS_NET, 1
# 42 "data/maps/Route2/scripts.pory"
	giveitem ITEM_REPEL, 1
# 43 "data/maps/Route2/scripts.pory"
	msgbox Route2_Text_ExplainRepel, MSGBOX_DEFAULT
# 44 "data/maps/Route2/scripts.pory"
	msgbox Route2_Text_BugsySaysGoodbye, MSGBOX_NPC
# 45 "data/maps/Route2/scripts.pory"
	closemessage
# 46 "data/maps/Route2/scripts.pory"
	setvar VAR_BUGSY_ROUTE2_SCENE, 1
# 47 "data/maps/Route2/scripts.pory"
	setflag FLAG_BUGSY_ITEM_EXCHANGED
# 48 "data/maps/Route2/scripts.pory"
	applymovement LOCALID_BUGSY_ROUTE2, Route2_Movement_BugsyLeaves
# 49 "data/maps/Route2/scripts.pory"
	waitmovement 0
# 50 "data/maps/Route2/scripts.pory"
	removeobject LOCALID_BUGSY_ROUTE2
Route2_EventScript_BugsyCutscene_1:
# 52 "data/maps/Route2/scripts.pory"
	releaseall
	return

Route2_EventScript_BugsyCutscene_2:
# 36 "data/maps/Route2/scripts.pory"
	msgbox Route2_Text_BugsyNetNotInInventory, MSGBOX_NPC
# 37 "data/maps/Route2/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, Route2_Movement_PlayerLeaves
# 38 "data/maps/Route2/scripts.pory"
	warp MAP_ROUTE2_VIRIDIAN_FOREST_NORTH_ENTRANCE, 7, 2
	goto Route2_EventScript_BugsyCutscene_1


# 55 "data/maps/Route2/scripts.pory"
Route2_Movement_BugsyFacesPlayer:
# 56 "data/maps/Route2/scripts.pory"
	face_down
	step_end

# 59 "data/maps/Route2/scripts.pory"
Route2_Movement_BugsyApproach:
# 60 "data/maps/Route2/scripts.pory"
	walk_down
# 61 "data/maps/Route2/scripts.pory"
	walk_down
	step_end

# 64 "data/maps/Route2/scripts.pory"
Route2_Movement_PlayerLeaves:
# 65 "data/maps/Route2/scripts.pory"
	walk_down
# 66 "data/maps/Route2/scripts.pory"
	walk_down
	step_end

# 69 "data/maps/Route2/scripts.pory"
Route2_Movement_BugsyLeaves:
# 70 "data/maps/Route2/scripts.pory"
	walk_up
# 71 "data/maps/Route2/scripts.pory"
	walk_up
# 72 "data/maps/Route2/scripts.pory"
	walk_right
# 73 "data/maps/Route2/scripts.pory"
	walk_right
# 74 "data/maps/Route2/scripts.pory"
	walk_right
# 75 "data/maps/Route2/scripts.pory"
	walk_right
# 76 "data/maps/Route2/scripts.pory"
	walk_right
# 77 "data/maps/Route2/scripts.pory"
	walk_up
# 78 "data/maps/Route2/scripts.pory"
	walk_up
# 79 "data/maps/Route2/scripts.pory"
	walk_up
# 80 "data/maps/Route2/scripts.pory"
	walk_up
# 81 "data/maps/Route2/scripts.pory"
	walk_up
	step_end

# 84 "data/maps/Route2/scripts.pory"

# 85 "data/maps/Route2/scripts.pory"
Route2_EventScript_RouteSign::
# 86 "data/maps/Route2/scripts.pory"
	msgbox Route2_Text_RouteSign, MSGBOX_SIGN
# 87 "data/maps/Route2/scripts.pory"
	end
# 88 "data/maps/Route2/scripts.pory"

# 89 "data/maps/Route2/scripts.pory"
Route2_EventScript_DiglettsCaveSign::
# 90 "data/maps/Route2/scripts.pory"
	msgbox Route2_Text_DiglettsCave, MSGBOX_SIGN
# 91 "data/maps/Route2/scripts.pory"
	end

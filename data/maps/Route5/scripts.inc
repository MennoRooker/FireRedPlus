Route5_MapScripts::
	.byte 0

Route5_EventScript_UndergroundPathSign::
	msgbox Route5_Text_UndergroundPathSign, MSGBOX_SIGN
	end

Route5_EventScript_Thomas::
	trainerbattle_single TRAINER_YOUNGSTER_THOMAS, Route5_Text_ThomasIntro, Route5_Text_ThomasDefeat
	msgbox Route5_Text_ThomasPostBattle, MSGBOX_AUTOCLOSE
	end

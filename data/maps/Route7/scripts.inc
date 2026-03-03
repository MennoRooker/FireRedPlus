Route7_MapScripts::
	.byte 0

Route7_EventScript_UndergroundPathSign::
	msgbox Route7_Text_UndergroundPathSign, MSGBOX_SIGN
	end

Route7_EventScript_James::
	lockall
	textcolor NPC_TEXT_COLOR_MALE
	msgbox Route7_Text_JessieAndJames
	closemessage
	release
	end

Route7_EventScript_Jessie::
	lockall
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox Route7_Text_JessieAndJames
	closemessage
	release
	end

EventScript_PickUpLetter::
	goto_if_eq VAR_LETTERS_FOUND, 2, EventScript_ThirdLetter
	goto_if_eq VAR_LETTERS_FOUND, 1, EventScript_SecondLetter
	goto EventScript_FirstLetter

EventScript_FirstLetter::
	signmsg
	msgbox Text_FoundALetter
	normalmsg
	msgbox Text_First_Letter
	addvar VAR_LETTERS_FOUND, 1
	return

EventScript_SecondLetter::
	signmsg
	msgbox Text_FoundAnotherLetter
	normalmsg
	msgbox Text_Second_Letter
	addvar VAR_LETTERS_FOUND, 1
	return

EventScript_ThirdLetter::
	signmsg
	msgbox Text_FoundAnotherLetter
	normalmsg
	msgbox Text_Third_Letter
	addvar VAR_LETTERS_FOUND, 1
	return

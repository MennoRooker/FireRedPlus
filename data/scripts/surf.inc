EventScript_UseSurf::
	goto_if_questlog EventScript_UseSurfEnd
	lockall
	goto_if_unset FLAG_BADGE06_GET, EventScript_CantSurf
	goto_if_unset FLAG_GOT_HM03, EventScript_CantSurf
	checkpartymove MOVE_SURF
	goto_if_eq VAR_RESULT, PARTY_SIZE, EventScript_UseSurfWithLapras
	setfieldeffectargument 0, VAR_RESULT
	dofieldeffect FLDEFF_USE_SURF
	releaseall
EventScript_UseSurfEnd:
	end

EventScript_UseSurfWithLapras::
	setfieldeffectargument 0, PARTY_SIZE
	dofieldeffect FLDEFF_USE_SURF
	releaseall
	end

EventScript_CantSurf::
	msgbox Text_WantToSurf, MSGBOX_SIGN
	releaseall
	end

EventScript_CurrentTooFast::
	lockall
	msgbox Text_CurrentTooFast
	releaseall
	end

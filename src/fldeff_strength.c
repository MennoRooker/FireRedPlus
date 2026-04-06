#include "global.h"
#include "field_player_avatar.h"
#include "field_effect.h"
#include "party_menu.h"
#include "event_data.h"
#include "script.h"
#include "fldeff.h"
#include "event_scripts.h"
#include "overworld.h"
#include "event_object_movement.h"
#include "constants/event_objects.h"
#include "constants/event_object_movement.h"
#include "constants/species.h"

static void FieldCB_UseStrength(void);
static void ShowMonCB_UseStrength(void);
static void Task_StrengthWithTauros(u8 taskId);

bool8 SetUpFieldMove_Strength(void)
{
    if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING) || CheckObjectGraphicsInFrontOfPlayer(OBJ_EVENT_GFX_PUSHABLE_BOULDER) != TRUE)
    {
        return FALSE;
    }
    else
    {
        gSpecialVar_Result = GetCursorSelectionMonId();
        gFieldCallback2 = FieldCallback_PrepareFadeInFromMenu;
        gPostMenuFieldCallback = FieldCB_UseStrength;
        return TRUE;
    }
}
static void FieldCB_UseStrength(void)
{
    gFieldEffectArguments[0] = GetCursorSelectionMonId();
    ScriptContext_SetupScript(EventScript_FldEffStrength);
}

bool8 FldEff_UseStrength(void)
{
    u8 taskId;

    if (gFieldEffectArguments[0] == PARTY_SIZE)
        taskId = CreateTask(Task_StrengthWithTauros, 8);
    else
        taskId = CreateFieldEffectShowMon();

    FLDEFF_SET_FUNC_TO_DATA(ShowMonCB_UseStrength);

    if (gFieldEffectArguments[0] != PARTY_SIZE)
        GetMonNickname(&gPlayerParty[gFieldEffectArguments[0]], gStringVar1);

    return FALSE;
}

static void ShowMonCB_UseStrength(void)
{
    FieldEffectActiveListRemove(FLDEFF_USE_STRENGTH);
    ScriptContext_Enable();
}

static void Task_StrengthWithTauros(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    u8 mapObjId = gPlayerAvatar.objectEventId;

    switch (task->data[0])
    {
    case 0:
        LockPlayerFieldControls();
        gPlayerAvatar.preventStep = TRUE;
        if (!ObjectEventIsMovementOverridden(&gObjectEvents[mapObjId])
         || ObjectEventClearHeldMovementIfFinished(&gObjectEvents[mapObjId]))
        {
            gFieldEffectArguments[0] = SPECIES_TAUROS | 0x80000000;
            gFieldEffectArguments[1] = 0x12345678;
            gFieldEffectArguments[2] = 0x79ABCDEF;

            StartPlayerAvatarSummonMonForFieldMoveAnim();
            ObjectEventSetHeldMovement(&gObjectEvents[mapObjId], MOVEMENT_ACTION_START_ANIM_IN_DIRECTION);
            FieldEffectStart(FLDEFF_FIELD_MOVE_SHOW_MON);
            task->data[0]++;
        }
        break;
    case 1:
        if (!FieldEffectActiveListContains(FLDEFF_FIELD_MOVE_SHOW_MON))
        {
            gFieldEffectArguments[1] = GetPlayerFacingDirection();
            if (gFieldEffectArguments[1] == DIR_SOUTH)
                gFieldEffectArguments[2] = 0;
            if (gFieldEffectArguments[1] == DIR_NORTH)
                gFieldEffectArguments[2] = 1;
            if (gFieldEffectArguments[1] == DIR_WEST)
                gFieldEffectArguments[2] = 2;
            if (gFieldEffectArguments[1] == DIR_EAST)
                gFieldEffectArguments[2] = 3;

            ObjectEventSetGraphicsId(&gObjectEvents[gPlayerAvatar.objectEventId], GetPlayerAvatarGraphicsIdByCurrentState());
            StartSpriteAnim(&gSprites[gPlayerAvatar.spriteId], gFieldEffectArguments[2]);
            FieldEffectActiveListRemove(FLDEFF_FIELD_MOVE_SHOW_MON);

            FLDEFF_CALL_FUNC_IN_DATA();
            gPlayerAvatar.preventStep = FALSE;
            DestroyTask(taskId);
        }
        break;
    }
}

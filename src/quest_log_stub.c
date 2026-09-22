#include "global.h"
#include "quest_log.h"
#include "quest_log_player.h"
#include "quest_log_objects.h"
#include "event_data.h"

COMMON_DATA u8 gQuestLogState = 0;
COMMON_DATA u8 gQuestLogPlaybackState = 0;
COMMON_DATA struct FieldInput gQuestLogFieldInput = {0};
EWRAM_DATA struct QuestLogRepeatEventTracker gQuestLogRepeatEventTracker = {0};
static EWRAM_DATA u16 sQuestLogDummyRecord = 0;
EWRAM_DATA u16 *gQuestLogDefeatedWildMonRecord = &sQuestLogDummyRecord;
EWRAM_DATA u16 *gQuestLogRecordingPointer = &sQuestLogDummyRecord;
EWRAM_DATA u16 gQuestLogCurActionIdx = 0;

void QuestLogRecordPlayerAvatarGfxTransition(u8 state)
{
    (void)state;
}

void SetQuestLogEvent(u16 eventId, const u16 *data)
{
    (void)eventId;
    (void)data;
}

void SetQLPlayedTheSlots(void)
{
}

void QuestLog_RecordEnteredMap(u16 mapLayoutId)
{
    (void)mapLayoutId;
}

u8 QL_GetPlaybackState(void)
{
    return 0;
}

bool8 QL_AvoidDisplay(void (*func)(void))
{
    if (func != NULL)
        func();

    return FALSE;
}

void QuestLog_BackUpPalette(u16 offset, u16 size)
{
    (void)offset;
    (void)size;
}

void CommitQuestLogWindow1(void)
{
}

void QuestLog_DrawPreviouslyOnQuestHeaderIfInPlaybackMode(void)
{
}

void ResetQuestLog(void)
{
    gQuestLogState = 0;
    gQuestLogPlaybackState = 0;
    gQuestLogCurActionIdx = 0;
    gQuestLogDefeatedWildMonRecord = &sQuestLogDummyRecord;
    gQuestLogRecordingPointer = &sQuestLogDummyRecord;
}

void TryStartQuestLogPlayback(u8 taskId)
{
    (void)taskId;
}

void SaveQuestLogData(void)
{
}

void QuestLog_CutRecording(void)
{
}

void ResetDeferredLinkEvent(void)
{
}

void QL_FinishRecordingScene(void)
{
}

void QuestLogEvents_HandleEndTrainerBattle(void)
{
}

void *QuestLogGetFlagOrVarPtr(bool8 isFlag, u16 idx)
{
    if (isFlag)
    {
        if (idx < FLAGS_COUNT)
            return &gSaveBlock1Ptr->flags[idx >> 3];
    }
    else
    {
        if (idx < VARS_COUNT)
            return &gSaveBlock1Ptr->vars[idx];
    }

    return NULL;
}

void QuestLogSetFlagOrVar(bool8 isFlag, u16 idx, u16 value)
{
    void *ptr = QuestLogGetFlagOrVarPtr(isFlag, idx);

    if (ptr == NULL)
        return;

    if (isFlag)
    {
        u8 *flagByte = ptr;
        u8 mask = 1 << (idx & 7);

        if (value)
            *flagByte |= mask;
        else
            *flagByte &= ~mask;
    }
    else
    {
        *(u16 *)ptr = value;
    }
}

void QL_AddASLROffset(void *oldSaveBlockPtr)
{
    (void)oldSaveBlockPtr;
}

void QL_UpdateObject(struct Sprite *sprite)
{
    (void)sprite;
}

void QuestLogRecordNPCStep(u8 a0, u8 a1, u8 a2, u8 a3)
{
    (void)a0;
    (void)a1;
    (void)a2;
    (void)a3;
}

bool8 QL_IsTrainerSightDisabled(void)
{
    return FALSE;
}

void QuestLog_OnEscalatorWarp(u8 direction)
{
    (void)direction;
}

void QuestLogRecordPlayerAvatarGfxTransitionWithDuration(u8 movementActionId, u8 duration)
{
    (void)movementActionId;
    (void)duration;
}

void QuestLogRecordPlayerStep(u8 movementActionId)
{
    (void)movementActionId;
}

void QuestLogRecordPlayerStepWithDuration(u8 movementActionId, u8 duration)
{
    (void)movementActionId;
    (void)duration;
}

void QuestLogRecordNPCStepWithDuration(u8 localId, u8 mapNum, u8 mapGroup, u8 movementActionId, u8 duration)
{
    (void)localId;
    (void)mapNum;
    (void)mapGroup;
    (void)movementActionId;
    (void)duration;
}

void QL_AfterRecordFishActionSuccessful(void)
{
}

void QL_ResetDefeatedWildMonRecord(void)
{
    sQuestLogDummyRecord = 0;
}

void QL_RestoreMapLayoutId(void)
{
}

void QL_RecordFieldInput(struct FieldInput *fieldInput)
{
    (void)fieldInput;
}

void QL_TryRunActions(void)
{
}

void RunQuestLogCB(void)
{
}

void QL_HandleInput(void)
{
}

bool8 QuestLogScenePlaybackIsEnding(void)
{
    return FALSE;
}

void SetQuestLogEvent_Arrived(void)
{
}

bool8 QuestLog_ShouldEndSceneOnMapChange(void)
{
    return FALSE;
}

void QuestLog_AdvancePlayhead_(void)
{
}

void QuestLog_InitPalettesBackup(void)
{
}

void QL_InitSceneObjectsAndActions(void)
{
}

u8 GetQuestLogStartType(void)
{
    return 0;
}

void QL_CopySaveState(void)
{
}

void QL_ResetPartyAndPC(void)
{
}

void QL_StartRecordingAction(u16 eventId)
{
    (void)eventId;
}

bool8 QL_IsRoomToSaveAction(const void *cursor, size_t size)
{
    (void)cursor;
    (void)size;
    return TRUE;
}

bool8 QL_IsRoomToSaveEvent(const void *cursor, size_t size)
{
    (void)cursor;
    (void)size;
    return TRUE;
}

void QL_ResetEventStates(void)
{
}

void QL_ResetRepeatEventTracker(void)
{
    gQuestLogRepeatEventTracker.id = 0;
    gQuestLogRepeatEventTracker.numRepeats = 0;
    gQuestLogRepeatEventTracker.counter = 0;
}

u16 *QL_RecordAction_SceneEnd(u16 *cursor)
{
    return cursor;
}

u16 *QL_LoadAction_Wait(u16 *cursor, struct QuestLogAction *action)
{
    (void)action;
    return cursor;
}

u16 *QL_RecordAction_Input(u16 *cursor, struct QuestLogAction *action)
{
    (void)action;
    return cursor;
}

u16 *QL_LoadAction_Input(u16 *cursor, struct QuestLogAction *action)
{
    (void)action;
    return cursor;
}

u16 *QL_RecordAction_MovementOrGfxChange(u16 *cursor, struct QuestLogAction *action)
{
    (void)action;
    return cursor;
}

u16 *QL_LoadAction_MovementOrGfxChange(u16 *cursor, struct QuestLogAction *action)
{
    (void)action;
    return cursor;
}

void QL_EnableRecordingSteps(void)
{
}

u16 *QL_SkipCommand(u16 *cursor, u16 **nextCursor)
{
    if (nextCursor != NULL)
        *nextCursor = cursor;
    return cursor;
}

void QL_UpdateLastDepartedLocation(const u16 *eventData)
{
    (void)eventData;
}

u16 *QL_LoadAction_SceneEnd(u16 *cursor, struct QuestLogAction *action)
{
    (void)action;
    return cursor;
}

bool8 QL_LoadEvent(const u16 *eventData)
{
    (void)eventData;
    return FALSE;
}

bool8 QL_TryRepeatEvent(const u16 *eventData)
{
    (void)eventData;
    return FALSE;
}

void QL_RecordWait(u16 duration)
{
    (void)duration;
}

void TrySetQuestLogBattleEvent(void)
{
}

void TrySetQuestLogLinkBattleEvent(void)
{
}

void QuestLog_StartRecordingInputsAfterDeferredEvent(void)
{
}

void GetQuestLogState(void)
{
    gSpecialVar_Result = 0;
}

void QuestLogUpdatePlayerSprite(u8 state)
{
    (void)state;
}

bool32 QuestLogTryRecordPlayerAvatarGfxTransition(u8 state)
{
    (void)state;
    return FALSE;
}

void QuestLogCallUpdatePlayerSprite(u8 state)
{
    (void)state;
}

void QL_RecordObjects(struct QuestLogScene *scene)
{
    (void)scene;
}

void QL_LoadObjects(struct QuestLogScene *scene, struct ObjectEventTemplate *templates)
{
    (void)scene;
    (void)templates;
}

void QL_TryStopSurfing(void)
{
}

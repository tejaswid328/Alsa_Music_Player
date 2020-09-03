#include "music_player.h"

RET_STATUS mp_seek_forward()
{
    MP_INT32 return_value = MP_SUCCESS;
    if( mp_check_state() == MP_STATE_PLAY ||  mp_check_state() == MP_STATE_PAUSE )
    {
        if( sPlayerStatus->sfileHeader->fDuration <= sPlayerStatus->timer+SEEK_FORWARD_VAL)
        {
            return_value = MP_SEEK_OVERFLOW;
        }
        if( sPlayerStatus->ireadflag == 0 )
        {
            sPlayerStatus->timer += SEEK_FORWARD_VAL;
            sPlayerStatus->key_operation = MP_KEY_SEEK;
            sPlayerStatus->ireadflag = 1;
        }
    }
    return return_value;
}

RET_STATUS mp_seek_backward()
{
    if( mp_check_state() == MP_STATE_PLAY ||  mp_check_state() == MP_STATE_PAUSE )
    {
        if( sPlayerStatus->ireadflag == 0 )
        {
            sPlayerStatus->timer -= SEEK_FORWARD_VAL;
            sPlayerStatus->key_operation = MP_KEY_SEEK;
            sPlayerStatus->ireadflag = 1;
        }
    }
    return MP_SUCCESS;
}

RET_STATUS mp_seek_to(MP_UINT64 seconds)
{
    MP_INT32 return_value = MP_SUCCESS;

    if( mp_check_state() == MP_STATE_PLAY ||  mp_check_state() == MP_STATE_PAUSE )
    {
        if( sPlayerStatus->sfileHeader->fDuration <= seconds)
        {
            return_value = MP_SEEK_OVERFLOW;
        }
        if( sPlayerStatus->ireadflag == 0 )
        {
            sPlayerStatus->timer = seconds;
            sPlayerStatus->key_operation = MP_KEY_SEEK;
            sPlayerStatus->ireadflag = 1;
        }
    }
    return return_value;
}

MP_INT32 time_to_seconds(MP_INT32 arr[], MP_INT32 size)
{
    MP_INT32 retSec = 0;
    if( size == 1)
    {
        retSec = arr[0];
    }
    if( size == 2)
    {
        retSec = arr[0]*60 + arr[1];
    }
    if( size == 3)
    {
        retSec = arr[0]*3600 + arr[1]*60 + arr[2];
    }

    return retSec;
}

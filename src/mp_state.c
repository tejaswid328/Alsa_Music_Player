#include "music_player.h"

MP_STATE mp_check_state()
{ 
    MP_STATE return_value;
    if( sPlayerStatus == NULL )
    {
        return_value = MP_STATE_NULL;
    }
    else
    {
       return_value = sPlayerStatus->eCurrentState;
    }
    return return_value;
}

MP_VOID mp_change_state(MP_STATE estate)
{
    sPlayerStatus->eCurrentState = estate;
}

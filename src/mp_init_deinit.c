#include "music_player.h"

RET_STATUS mp_initialization()
{
    if( mp_check_state() != MP_STATE_EXIT )
    {
        sPlayerStatus = (struct MPPlayerStatus *) malloc(sizeof(struct MPPlayerStatus));
        sPlayerStatus->eCurrentState = MP_STATE_INIT;
        sPlayerStatus->sfileHeader = (HEADER *)malloc(sizeof(HEADER));
        sPlayerStatus->stalsaparams = (struct MPAlsaParams *)malloc(sizeof(struct MPAlsaParams));
        sPlayerStatus->ireadflag = 0;
        sPlayerStatus->key_operation = MP_KEY_PLAY;
        sPlayerStatus->ifilePointer = 0;
        sPlayerStatus->playback_tid = 0;
        sPlayerStatus->volume_tid = 0 ;
        sPlayerStatus->timer = 0;
        sPlayerStatus->buffer = NULL;
        sPlayerStatus->iIsPlaybackThreadAlive = MP_FALSE;
        sPlayerStatus->iIsVolumeThreadAlive = MP_FALSE;
        sPlayerStatus->iPlayNextTrack = MP_FALSE;
        sPlayerStatus->vol_thread_close = 1;
    }

    MP_INT32 fr;
    fr = pthread_create(&sPlayerStatus->volume_tid, NULL, mp_volume_thread, NULL );
    if (fr > 0)
    {
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    }
    else
    {
        //error
    }

    return 0;
	
}

MP_VOID mp_deinitialization()
{
    MP_INT32 i;
    sPlayerStatus->eCurrentState = MP_STATE_STOP;
    if(sPlayerStatus->iIsPlaybackThreadAlive == MP_TRUE)
    {
        snd_pcm_drop(sPlayerStatus->stalsaparams->pcmHandle);
        pthread_cancel( sPlayerStatus->playback_tid );
        pthread_cancel( sPlayerStatus->timer_tid );
        sPlayerStatus->iIsPlaybackThreadAlive = MP_FALSE;
    }
    if(sPlayerStatus->iIsVolumeThreadAlive == MP_TRUE)
    {
        sPlayerStatus->vol_thread_close = 0;
        pthread_cancel( sPlayerStatus->volume_tid );
        sPlayerStatus->iIsVolumeThreadAlive = MP_FALSE;
    }
    
    if( mp_check_state() == MP_STATE_EXIT )
    {
        for (i = 0; i < (MP_INT32)smpPlaylist.guinoOfPlaylist ; ++i)
        {
            free(smpPlaylist.gacinternalPlaylist[i]);
        }
        free(smpPlaylist.gacinternalPlaylist);
        free(smpPlaylist.gauiplayOrder);
    }
}

MP_VOID mp_exit()
{
    mp_change_state(MP_STATE_EXIT);
    mp_deinitialization();
    free(sPlayerStatus);
   // return MP_SUCCESS;
}

#include "music_player.h"

MP_INT32  mp_next_track()
{
    if( smpPlaylist.guicurrHeader < smpPlaylist.guinoOfPlaylist-1 )
    {
        if( sPlayerStatus->iIsPlaybackThreadAlive == MP_TRUE)
        {
           snd_pcm_drop(sPlayerStatus->stalsaparams->pcmHandle);
            pthread_cancel( sPlayerStatus->playback_tid );
            pthread_cancel( sPlayerStatus->timer_tid );
            sPlayerStatus->iIsPlaybackThreadAlive = MP_FALSE;
        }
        smpPlaylist.guicurrHeader++;
        if( smpPlaylist.guicurrHeader < 10000 )
        {    
            if( smpPlaylist.gauiplayOrder[smpPlaylist.guicurrHeader] < 10000 )
            {    
                mp_play_track( smpPlaylist.gacinternalPlaylist[smpPlaylist.gauiplayOrder[smpPlaylist.guicurrHeader]] );
            }
        }
    }
    else
    {
        mp_exit();
    } 
    return 0;   
}

RET_STATUS mp_previous_track()
{
    if( smpPlaylist.guicurrHeader > 0 )
    {
        if( sPlayerStatus->iIsPlaybackThreadAlive == MP_TRUE )
        {
            snd_pcm_drop(sPlayerStatus->stalsaparams->pcmHandle);
    	    pthread_cancel( sPlayerStatus->playback_tid );
            pthread_cancel( sPlayerStatus->timer_tid );
            sPlayerStatus->iIsPlaybackThreadAlive = MP_FALSE;
        }
        smpPlaylist.guicurrHeader--;
        if( smpPlaylist.guicurrHeader < 10000 )
        {    
            if( smpPlaylist.gauiplayOrder[smpPlaylist.guicurrHeader] < 10000 )
            {    
                mp_play_track( smpPlaylist.gacinternalPlaylist[smpPlaylist.gauiplayOrder[smpPlaylist.guicurrHeader]] );
            }
        }
    }
    return MP_SUCCESS;
}


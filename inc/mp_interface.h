#ifndef _MP_INTERFACE_H_
#define _MP_INTERFACE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>
#include <pthread.h>
#include <alsa/asoundlib.h>

#include "mp_types.h"
#include "mp_audiofile_header.h"
#include "mp_error_codes.h"



struct MPAlsaParams
{
    snd_pcm_t *pcmHandle;
    snd_pcm_hw_params_t *hwparams;

}sAlsaParams;

struct MPAlsaMixerParams
{
    snd_mixer_t *volhandle;
    snd_mixer_selem_id_t *sid;

}sAlsaMixerParams;

struct MPPlayerStatus
{
    MP_STATE eCurrentState;
    HEADER *sfileHeader;
    MP_CHAR *trackName;
    struct MPAlsaParams *stalsaparams;
    MP_INT32 ireadflag;
    MP_KEY_OPERATION key_operation;
    MP_INT32 ifilePointer;
    pthread_t playback_tid;
    pthread_t volume_tid;
   /* MP_INT32 (*callback)() ; */
    MP_UINT64 timer;
    pthread_t timer_tid;
    MP_CHAR *buffer;
    MP_INT32 iIsPlaybackThreadAlive;
    MP_INT32 iIsVolumeThreadAlive;
    MP_INT32 iPlayNextTrack;
    MP_INT32 vol_thread_close;
}*sPlayerStatus;

struct MPPlayerVolume
{
    MP_INT32 uiVolumeLevel;
    MP_INT32 ivolStatus;
}sPlayerVolume;

struct MPPlaylist
{ 
    MP_CHAR **gacinternalPlaylist;
    MP_UINT32 *gauiplayOrder;
    MP_UINT32 guicurrHeader;
    MP_UINT32 guinoOfPlaylist;
}smpPlaylist;

RET_STATUS mp_extract_header_info(MP_CHAR *filepath, HEADER *fileheader);
MP_VOID *mp_playback_thread(MP_VOID *args);
MP_VOID *timer_display(MP_VOID *args);
MP_INT32 time_to_seconds(MP_INT32 arr[], MP_INT32 size);
MP_VOID *mp_volume_thread(MP_VOID *args);
pthread_mutex_t mutexthread;

/*typedef struct TrackInfo
{ 	
    // sampling rate (blocks per second)
    MP_UINT32 uiSampleRate;

    // Duration in seconds
    MP_FLOAT fDuration;
};*/

#endif


#include "music_player.h"

MP_VOID *mp_volume_thread(MP_VOID *args)
{
    MP_INT64 min, max;
    sPlayerVolume.uiVolumeLevel=50;
    // MPAlsaMixerParams *volparams = (sAlsaMixerParams *)(args);
    const MP_CHAR *card = "default";
    const MP_CHAR *selem_name = "Master";
    snd_mixer_open(&(sAlsaMixerParams.volhandle), 0);
    snd_mixer_attach(sAlsaMixerParams.volhandle, card);
    snd_mixer_selem_register(sAlsaMixerParams.volhandle, NULL, NULL);
    snd_mixer_load(sAlsaMixerParams.volhandle);

    snd_mixer_selem_id_alloca(&(sAlsaMixerParams.sid));
    snd_mixer_selem_id_set_index(sAlsaMixerParams.sid, 0);
    snd_mixer_selem_id_set_name(sAlsaMixerParams.sid, selem_name);
    
    snd_mixer_elem_t* elem = snd_mixer_find_selem(sAlsaMixerParams.volhandle, sAlsaMixerParams.sid);
    snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
    //long sysvol;
  //  int fr;
      
    while(sPlayerStatus->vol_thread_close != 0)
    { 		
       switch(sPlayerVolume.ivolStatus)
       {
           case 1:
                  /*fr = (int)snd_mixer_selem_get_playback_volume(elem, 0, &sysvol) ;
                  if( fr < 0 ) 
                  {
                      snd_mixer_close(sAlsaMixerParams.volhandle);
                  }
                  sPlayerVolume.uiVolumeLevel = ((int) (float)sysvol/(float)max * 100) + 10;*/
                  sPlayerVolume.uiVolumeLevel += 10;
                  if(sPlayerVolume.uiVolumeLevel > 100)
                  {
		      sPlayerVolume.uiVolumeLevel = 100;
                  }      
                  
                  snd_mixer_selem_set_playback_volume_all(elem, sPlayerVolume.uiVolumeLevel*max/100);
                  sPlayerVolume.ivolStatus = 0;
                  break;
           case 2:
		  /*if(snd_mixer_selem_get_playback_volume(elem, 0, &sysvol) < 0) 
                  {
                      snd_mixer_close(sAlsaMixerParams.volhandle);
                  }
                  sPlayerVolume.uiVolumeLevel = ((int) (float)sysvol/(float)max * 100) - 10;*/
                  sPlayerVolume.uiVolumeLevel -= 10;
                  if(sPlayerVolume.uiVolumeLevel < 0)
                  {
		      sPlayerVolume.uiVolumeLevel = 0;
                  }      
                  
                  snd_mixer_selem_set_playback_volume_all(elem, sPlayerVolume.uiVolumeLevel*max/100);
                  sPlayerVolume.ivolStatus = 0;
       }
    }
   return MP_SUCCESS;   
}

long mp_volume_increase()
{
    sPlayerVolume.ivolStatus = 1;
    return sPlayerVolume.uiVolumeLevel;
}

long mp_volume_decrease()
{
    sPlayerVolume.ivolStatus = 2;
    return sPlayerVolume.uiVolumeLevel;
}

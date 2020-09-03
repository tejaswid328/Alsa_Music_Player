#define _LARGEFILE64_SOURCE

#include "mp_interface.h"
#include "music_player.h"

RET_STATUS mp_play_track(MP_CHAR *filepath)
{
 
    MP_INT32 fr;
    //HEADER sMusicfileHeader;
    if( mp_extract_header_info(filepath, sPlayerStatus->sfileHeader) == MP_SUCCESS )
    {

       // sPlayerStatus->sfileHeader = sMusicfileHeader;
        sPlayerStatus->eCurrentState = MP_STATE_PLAY;
        sPlayerStatus->ireadflag = 0;
        sPlayerStatus->trackName = filepath;
        //key_operation;
        //sPlayerStatus->stalsaparams = &sAlsaParams;
        sPlayerStatus->ifilePointer = open(filepath, O_RDONLY);      

        fr = pthread_create(&sPlayerStatus->playback_tid, NULL, mp_playback_thread, (MP_VOID *)sPlayerStatus);
        if (fr > 0)
        {
	    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
        }
        else
        {
            //error
        }
        //pthread_join(sPlayerStatus->playback_tid, NULL);
       // while(1);
    }
    else
    {
        //error
    }
   
    
   printf("mp_play_track end \n");
    return MP_SUCCESS;
}

MP_VOID *mp_playback_thread(MP_VOID *args)
{
    MP_INT32 dir, fr;
    snd_pcm_uframes_t frames;
    snd_pcm_format_t sMusicFormat = SND_PCM_FORMAT_S16_LE;
    
    MP_INT32 size, paused = 0,read_file = 0, endLoop = 1 ;
  //  struct MPPlayerStatus *sPlayerStatus = (struct MPPlayerStatus *)args;
 
  //  struct MPPlayerStatus *sPlayerStatus = sPlayerStatus;
    sPlayerStatus->stalsaparams = &sAlsaParams;

    sPlayerStatus->iIsPlaybackThreadAlive = MP_TRUE;
    //DEBUG
    printf("\nPlayback Thread\n");
    
    printf("Channels: %u \n", sPlayerStatus->sfileHeader->uiChannels);
    printf("Sample Rate: %u \n", sPlayerStatus->sfileHeader->uiSampleRate);
    
    fr = snd_pcm_open(&(sPlayerStatus->stalsaparams->pcmHandle), "default", SND_PCM_STREAM_PLAYBACK, 0);
    if( fr < 0 ) 
    {
        fprintf(stderr, "unable to open pcm device: %s\n", snd_strerror(fr));
        exit(1);
	
    }
    
    snd_pcm_hw_params_alloca(&(sPlayerStatus->stalsaparams->hwparams));
    snd_pcm_hw_params_any( sPlayerStatus->stalsaparams->pcmHandle, sPlayerStatus->stalsaparams->hwparams );
    snd_pcm_hw_params_set_access(sPlayerStatus->stalsaparams->pcmHandle, sPlayerStatus->stalsaparams->hwparams, SND_PCM_ACCESS_RW_INTERLEAVED);

    switch(sPlayerStatus->sfileHeader->uiBitsPerSample)
    {
        case 8:
            sMusicFormat = SND_PCM_FORMAT_U8;
            break;
        case 16:
            sMusicFormat = SND_PCM_FORMAT_S16_LE;
            break;
        case 24:
            sMusicFormat = SND_PCM_FORMAT_S24_3LE;
    }
    
    fr = snd_pcm_hw_params_set_format( sPlayerStatus->stalsaparams->pcmHandle, sPlayerStatus->stalsaparams->hwparams, sMusicFormat);
    if (fr < 0) 
    {
        fprintf(stderr, "unable to set format: %s\n", snd_strerror(fr));
        exit(1);
    }
    
    fr = snd_pcm_hw_params_set_channels(sPlayerStatus->stalsaparams->pcmHandle, sPlayerStatus->stalsaparams->hwparams, sPlayerStatus->sfileHeader->uiChannels);
    if (fr < 0) 
    {
        fprintf(stderr, "unable to set channels: %s\n", snd_strerror(fr));
        exit(1);
    }

    fr = snd_pcm_hw_params_set_rate_near(sPlayerStatus->stalsaparams->pcmHandle, sPlayerStatus->stalsaparams->hwparams, &(sPlayerStatus->sfileHeader->uiSampleRate), &dir);
    if (fr < 0) 
    {
        fprintf(stderr, "unable to set rate: %s\n", snd_strerror(fr));
        exit(1);
    }

    /* Write the parameters to the driver */
    fr = snd_pcm_hw_params(sPlayerStatus->stalsaparams->pcmHandle, sPlayerStatus->stalsaparams->hwparams);
    if (fr < 0) 
    {
        fprintf(stderr, "unable to set hw parameters: %s\n", snd_strerror(fr));
        exit(1);
    }
    
    /* Use a buffer large enough to hold one period */
    snd_pcm_hw_params_get_period_size(sPlayerStatus->stalsaparams->hwparams, &frames, &dir);
    size = frames * sPlayerStatus->sfileHeader->uiChannels * sPlayerStatus->sfileHeader->uiBitsPerSample/8;
    sPlayerStatus->buffer = (MP_CHAR *) malloc(size);
  
    sAlsaParams = *sPlayerStatus->stalsaparams;

    printf("now playing music...\n");

    sPlayerStatus->timer = 0;
    fr = pthread_create(&sPlayerStatus->timer_tid, NULL, timer_display, NULL);
    if (fr > 0)
    {
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    }

    

    while( endLoop == 1 )
    {
       // while(sPlayerStatus->ireadflag == 0);
      
	if( sPlayerStatus->ireadflag == 1 )
	{ 
                printf("%ld\n", sPlayerStatus->playback_tid);
                printf("\033[1A");
		switch( sPlayerStatus->key_operation )
		{
		    case MP_KEY_PAUSE : 
			                if(snd_pcm_pause(sPlayerStatus->stalsaparams->pcmHandle, 1) < 0)
					{
						printf("ERROR while pause");
                                                endLoop = 0;
				    	}
			                printf("PAUSED");
			                paused = 1;
			                break;
		    case MP_KEY_RESUME :
			                if(snd_pcm_pause(sPlayerStatus->stalsaparams->pcmHandle, 0) < 0)
					{
						printf("ERROR while resume");
                                                endLoop = 0;
				    	}
			                printf("RESUME");
			                paused = 0;
			                break;
                    case MP_KEY_NEXT:
                    case MP_KEY_PREV:
                                       // pthread_cancel( sPlayerStatus->timer_tid );
                                        endLoop = 0;
                                        break;
             
		    case MP_KEY_SEEK :
			                snd_pcm_drop(sPlayerStatus->stalsaparams->pcmHandle);
				        snd_pcm_prepare(sPlayerStatus->stalsaparams->pcmHandle);
				        fr = lseek64(sPlayerStatus->ifilePointer, sPlayerStatus->sfileHeader->uiByterate * (sPlayerStatus->timer) , SEEK_SET);
                                        if( fr < 0)
                                        {
                                            read_file = 1;
                                        }
                                        else
                                        {
			                    read_file = 0;
                                        }
			                break;

		}
	        sPlayerStatus->ireadflag = 0;
	}
            
        if( paused == 1 ) 
        {
            continue;
        }

        
        if(read_file == 0)
        {
		memset(sPlayerStatus->buffer, 0, size);
		// fr = fgets(buffer,size, mfp);
		MP_INT32 fr = read(sPlayerStatus->ifilePointer, sPlayerStatus->buffer, size);
		if (fr == 0) 
		{
		    fprintf(stderr, "end of file on input\n");
		    read_file = 1;
		 /*
		    while( 1 )
		    {
		        printf("%ld \n",snd_pcm_avail_update(sPlayerStatus->stalsaparams->pcmHandle));
		        sleep(1);
		    }
		  
		    break;*/
		} 
		else if (fr != size) 
		{
		    fprintf(stderr,"short read: read %d bytes\n", fr);
		}
		fr = snd_pcm_writei(sPlayerStatus->stalsaparams->pcmHandle, sPlayerStatus->buffer, frames);
		if (fr == -EPIPE) 
		{
		    /* EPIPE means underrun */
		    fprintf(stderr, "underrun occurred\n");
		    snd_pcm_prepare(sPlayerStatus->stalsaparams->pcmHandle);
		 } 
		 else if (fr < 0) 
		 {
		      fprintf(stderr, "error from writei: %s\n",snd_strerror(fr));
		 }
		 else if (fr != (MP_INT32) frames) 
		 {
		      fprintf(stderr,"short write, write %d frames\n", fr);
		 }
           }
           else if( snd_pcm_avail_update(sPlayerStatus->stalsaparams->pcmHandle) < 0 )
           {
                 break;
           }
         

     }
    
     printf("\nDone ...\n");
     snd_pcm_drop(sPlayerStatus->stalsaparams->pcmHandle);
     snd_pcm_close(sPlayerStatus->stalsaparams->pcmHandle);
     free(sPlayerStatus->buffer);
     sPlayerStatus->iPlayNextTrack = MP_TRUE;
    // sPlayerStatus->iIsPlaybackThreadAlive = MP_FALSE;
    //  (*sPlayerStatus->callback)();
    return MP_SUCCESS;
}

RET_STATUS mp_pause_resume_track()
{
    printf("mp_pause_resume_track \n");
    switch(mp_check_state())
    {
        case MP_STATE_PLAY:
                           if(sPlayerStatus->ireadflag == 0)
                           {
		                   printf("\nAPI PAUSE\n");
                                   pthread_mutex_lock(&mutexthread);
		                   mp_change_state(MP_STATE_PAUSE);
		                   sPlayerStatus->key_operation = MP_KEY_PAUSE;
                                   pthread_mutex_unlock(&mutexthread);
		                   sPlayerStatus->ireadflag = 1;
                                   
                           }
                           break;
        case MP_STATE_PAUSE:
                           if(sPlayerStatus->ireadflag == 0)
                           {
		                   printf("\nAPI RESUME\n");
                                   pthread_mutex_lock(&mutexthread);
		                   mp_change_state(MP_STATE_PLAY);
		                   sPlayerStatus->key_operation = MP_KEY_RESUME;
                                   pthread_mutex_unlock(&mutexthread);
		                   sPlayerStatus->ireadflag = 1;
                                   
                           }
                           break;
        
    }
   
    printf("mp_pause_resume_track end %d\n", sPlayerStatus->key_operation);
    printf("STATE %d\n",sPlayerStatus->eCurrentState);
    return MP_SUCCESS;
}

MP_VOID mp_play()
{
    if( mp_check_state() == MP_STATE_STOP )
    {
        smpPlaylist.guicurrHeader = 0;
        if( smpPlaylist.guicurrHeader < smpPlaylist.guinoOfPlaylist )
        {    
            if( smpPlaylist.gauiplayOrder[smpPlaylist.guicurrHeader] < smpPlaylist.guinoOfPlaylist )
            {    
                mp_play_track( smpPlaylist.gacinternalPlaylist[smpPlaylist.gauiplayOrder[smpPlaylist.guicurrHeader]] );
            }
        }
    }
}


RET_STATUS mp_stop()
{
    if( mp_check_state() == MP_STATE_PLAY || mp_check_state() == MP_STATE_PAUSE)
    {
        sPlayerStatus->eCurrentState = MP_STATE_STOP;
        // mp_deinitialization();
        if(sPlayerStatus->iIsPlaybackThreadAlive == MP_TRUE)
        {
            printf("mp_deinitialization thread cancel\n");
            snd_pcm_drop(sPlayerStatus->stalsaparams->pcmHandle);
            // snd_pcm_close(sPlayerStatus->stalsaparams->pcmHandle);
            // free(sPlayerStatus->buffer);
            pthread_cancel( sPlayerStatus->playback_tid );
            pthread_cancel( sPlayerStatus->timer_tid );
            sPlayerStatus->iIsPlaybackThreadAlive = MP_FALSE;
        }
        smpPlaylist.guicurrHeader = -1;
    }
    return 0;
}

void *timer_display(void *args)
{
    printf("\033[7A");
    printf("\nPlaying : %s             \n", sPlayerStatus->trackName);
    printf("Channels : %d              \n", sPlayerStatus->sfileHeader->uiChannels);
    printf("Sample Rate : %d           \n", sPlayerStatus->sfileHeader->uiSampleRate);
    printf("Total Duration : %d seconds \n\n\n",(int)sPlayerStatus->sfileHeader->fDuration );
    while( sPlayerStatus->eCurrentState != MP_STATE_STOP && sPlayerStatus->iIsPlaybackThreadAlive == MP_TRUE)
    {         
        if(sPlayerStatus->eCurrentState == MP_STATE_PLAY )
        {
            sPlayerStatus->timer++;
            printf("\033[1A");
            printf("Current Playback Time : %ld  \n", sPlayerStatus->timer);
        } 
        sleep(1);
    }
}



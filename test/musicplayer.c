#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/select.h>
#include "music_player.h"

int main(int argc, char** argv)
{
    int i, j, wavCount=0;
    FILE *fp;
    char ch;

    if(argc <= 1)
    {
        printf("NO ARGUMENTS\n");
        printf("Proper Usage : ./music_player [-shuffle] filename.wav/filename.txt ...\n");
	return 0;
    }
    else if( argc == 2 )
    {
        if( strcmp(argv[1],"-shuffle") == 0 ) 
	{
            printf("NO ARGUMENTS\n");
            printf("Proper Usage : ./music_player [-shuffle] filename.wav/filename.txt ...\n");
	    return 0;
        }
	else
        {
            if( strcmp(strrchr(argv[1],'.'),".wav") == 0)
            {
                if( mp_add_to_playlist(argv[1], wavCount) == 0)
                {
                    wavCount++;
                }
            }
            else if( strcmp(strrchr(argv[1],'.'),".txt") == 0 )
            {
                fp = fopen(argv[1], "r");
                char temp[256], c;
                j = 0;
                for (c = getc(fp); c != EOF; c = getc(fp)) 
                {
        	   if (c == '\n') 
                   {
                       temp[j++] = '\0';
                       if( mp_add_to_playlist(temp, wavCount) == 0)
                       {
                           wavCount++;
                       }
                       j=0;
                   } 
                   else
                   {
                       temp[j++] = c;
                   }
                } 
            }
        }
    }
    else
    {
        if( strcmp(argv[1],"-shuffle") == 0 ) 
	{
            i = 2;
        }
	else
        {
            i = 1;
        }
        for( ; i < argc ; i++)
	{
           if( strcmp(strrchr(argv[i],'.'),".wav") == 0)
            {
                if( mp_add_to_playlist(argv[i], wavCount) == 0)
                {
                    wavCount++;
                }
            }
            else if( strcmp(strrchr(argv[i],'.'),".txt") == 0 )
            {
                fp = fopen(argv[i], "r");
                char temp[256], c;
                j = 0;
                for (c = getc(fp); c != EOF; c = getc(fp)) 
                {
        	   if (c == '\n') 
                   {
                       temp[j++] = '\0';
                       if( mp_add_to_playlist(temp, wavCount) == 0)
                       {
                           wavCount++;
                       }
                       j=0;
                   } 
                   else
                   {
                       temp[j++] = c;
                   }
                }
            }
        }
    }  
 
    if( strcmp(argv[1],"-shuffle") == 0 ) 
    {
         mp_set_play_order(wavCount, 1);
    }
    else
    {
         mp_set_play_order(wavCount, 0);
    }
    for(i=0;i<smpPlaylist.guinoOfPlaylist;i++)
    {
        printf("%s\n", smpPlaylist.gacinternalPlaylist[smpPlaylist.gauiplayOrder[i]]);
    }

    if( mp_initialization() == 0 )
    {
         printf("Initialization Successfull\n");
         smpPlaylist.guicurrHeader=0;
         pthread_mutex_init(&mutexthread, NULL);
         mp_play_track(smpPlaylist.gacinternalPlaylist[smpPlaylist.gauiplayOrder[smpPlaylist.guicurrHeader]]);
    }
    else
    {
         printf("Initialization Unsuccessfull\n");
         exit(0);
    }  
    
    do 
    {
        if(kbhit())
        {   
            
            ch = getchar();
            switch(ch)
            {
                case '+':
                     mp_volume_increase();
                     break;   
                case '-':
                     mp_volume_decrease();
                     break;
                case ' ':  
                     mp_pause_resume_track();           
		     break;

	        case 67:
		    mp_next_track();
                    break;
                case 68:
                    mp_previous_track();
                    break;
                case 'S':
                case 's':
                    mp_stop();
                    break;
                case 'p':
                case 'P':
                    mp_play();
                    break;
                case 'J':
                case 'j':
                    while(!kbhit());
                    ch = getchar();
                    switch(ch)
                    {
                        case '+':
                                mp_seek_forward();
                                break;
                        case '-':
                                mp_seek_backward();
                                break;
                        default:
                                if(isdigit(ch))
                                {
                                    int seek_val[3] = {0}, i = 0, error_input = 0, seek_val_size = 0; 
                                    seek_val[i] = ( int ) ch - 48;

                                    while(1)
                                    {
                                        if( kbhit() )
                                        {
                                            ch = getchar();
                                            if(isdigit(ch))
                                            {
                                                seek_val[i] = seek_val[i]*10 + ((int) ch - 48);
                                                continue;
                                            }
                                            else if( ch == ':')
                                            {
                                                if( ++i > 2 )
                                                {
                                                    break;
                                                }
                                            }
                                            else if( ch == '\n')
                                            {
                                                ++i;
                                                break;
                                            }
                                            else
                                            {
                                                error_input = 1;
                                                break;
                                            }
                                        }
                                    }
                                    if( error_input == 0)
                                    {
                                        long sec = time_to_seconds(seek_val, i);
                                        mp_seek_to(sec);     
                                    } 
                                }
                    }
                    break;
                case 'q':
                case 'Q':
                    mp_change_state(MP_STATE_EXIT);
            }
         }
         if(sPlayerStatus->iPlayNextTrack == MP_TRUE)
         {
             mp_next_track();
             sPlayerStatus->iPlayNextTrack = MP_FALSE;
         }
    }while(mp_check_state() != MP_STATE_EXIT);
    
    pthread_mutex_destroy(&mutexthread);
    mp_exit();
    
    return 0;    
}












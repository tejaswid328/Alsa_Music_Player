#include "music_player.h"

MP_INT32 mp_add_to_playlist(MP_CHAR *pcfilename, MP_INT32 ipos)
{
    if( 0 == ipos )
    {
        smpPlaylist.gacinternalPlaylist = (MP_CHAR **) malloc(sizeof(MP_CHAR *)*1);
    }
    else
    {
        smpPlaylist.gacinternalPlaylist = realloc(smpPlaylist.gacinternalPlaylist, (ipos+1)*sizeof(MP_CHAR *));
    }
    smpPlaylist.guinoOfPlaylist++;
    if( ipos < 10000)
    {
        smpPlaylist.gacinternalPlaylist[ipos] = (MP_CHAR *) malloc(256*sizeof(MP_CHAR));
        strncpy(smpPlaylist.gacinternalPlaylist[ipos], pcfilename, strlen(pcfilename));
    }
    return 0;
}

MP_VOID mp_set_play_order(MP_UINT32 n, MP_UINT32 shuffle )
{
    MP_UINT32 i,j;
    smpPlaylist.gauiplayOrder = (MP_UINT32 *)malloc( sizeof(MP_UINT32) * n );
    for( i = 0; i < n; ++i)
    {
        smpPlaylist.gauiplayOrder[i] = i;
    }
    if(shuffle == 1)
    {
        srand (time(NULL)); 
        for (i = n - 1; i > 0; i--)  
        {  
            // Pick a random index from 0 to i  
            j = rand() % (i + 1);  
  
            // Swap arr[i] with the element  
            // at random index  
            if(j < 10000 && i < 10000)
            {
	        MP_UINT32 temp = smpPlaylist.gauiplayOrder[i];
                smpPlaylist.gauiplayOrder[i] = smpPlaylist.gauiplayOrder[j];
	        smpPlaylist.gauiplayOrder[j] = temp;
            }
        } 
    } 
}

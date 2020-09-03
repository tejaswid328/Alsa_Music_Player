#ifndef _MP_MUSIC_PLAYER_H_
#define _MP_MUSIC_PLAYER_H_

#include "mp_interface.h"

MP_INT32 kbhit(MP_VOID);
MP_INT32 mp_add_to_playlist(MP_CHAR* ,MP_INT32 );
MP_VOID mp_set_play_order(MP_UINT32, MP_UINT32 );
MP_INT32 mp_initialization();
MP_INT32 mp_next_track();
RET_STATUS mp_play_track(MP_CHAR *filepath);
MP_VOID mp_exit();
MP_VOID mp_deinitialization();
RET_STATUS mp_stop();
MP_VOID mp_play();
MP_STATE mp_check_state();
RET_STATUS mp_seek_forward();
RET_STATUS mp_seek_backward();
RET_STATUS mp_seek_to(MP_UINT64);
MP_VOID mp_change_state(MP_STATE);
RET_STATUS mp_pause_resume_track();
RET_STATUS mp_previous_track();
MP_INT64 mp_volume_increase();
MP_INT64 mp_volume_decrease();

#endif

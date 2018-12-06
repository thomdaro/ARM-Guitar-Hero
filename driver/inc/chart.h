#include <stm32f30x.h>
#include <ff.h>
/* functions */
#define QUOTE_REGEX "%*[^\"]%*c%[^\"]"
#define NUM_REGEX 	"%*[^=]%*2c%hd"

/* song data */
typedef struct song_t {
	TCHAR name[64];
	TCHAR artist[64];
	uint16_t offset;
	uint16_t resolution;
	TCHAR genre[64];
	TCHAR music_stream[128];
} song_t;

/* sync data
 * mod is one of:
 * -> B  - bitrate
 * -> TS - ?
 * -> A  - ?
 */
typedef struct sync_track_t {
	uint32_t time;
	TCHAR mod[3];			// mod string (B/TS/A)
	uint32_t val;
	struct sync_track_t * next;	// pointer to the next struct
} sync_track_t;

/* event data */
typedef struct event_t {
	uint32_t time;
	TCHAR message[1024];
	struct event_t * next;			// pointer to the next struct
} event_t;

/* level data
 * arg0 could be one of:
 * -> N followed by 2 numbers
 * -> S followed by 2 numbers
 * -> E followed by either T or *
 */
typedef struct single_t {
	uint32_t time;
	TCHAR arg0;
	TCHAR arg1;
	uint16_t num0;
	uint16_t num1;
	struct single_t * next;	// pointer to the next struct
} single_t;

TCHAR* f_gets(TCHAR* buff, int len, FIL* fp);
void init_song(FIL *, song_t *);
void init_sync_track(FIL *, sync_track_t *);
void init_event(FIL *, event_t *);
void init_single(FIL *, single_t *);

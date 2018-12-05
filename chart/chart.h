#include <stm32f30x.h>

/* functions */
#define QUOTE_REGEX "%*[^\"]%*c%[^\"]"
#define NUM_REGEX 	"%*[^=]%*2c%hd"

/* song data */
typedef struct song_t {
	char name[64];
	char artist[64];
	uint16_t offset;
	uint16_t resolution;
	char genre[64];
	char music_stream[128];
} song_t;

/* sync data
 * mod is one of:
 * -> B  - bitrate
 * -> TS - ?
 * -> A  - ?
 */
typedef struct sync_track_t {
	uint32_t time;
	char mod[3];			// mod string (B/TS/A)
	uint32_t val;
	struct sync_track_t * next;	// pointer to the next struct
} sync_track_t;

/* event data */
typedef struct event_t {
	uint32_t time;
	char message[1024];
	struct event_t * next;			// pointer to the next struct
} event_t;

/* level data
 * arg0 could be one of:
 * -> N followed by 2 numbers (0-4, representing string)
 * -> E followed by either T or *
 */
typedef struct single_t {
	uint32_t time;
	char arg0;
	char arg1;
	uint16_t num0;
	uint16_t num1;
	struct single_t * next;	// pointer to the next struct
} single_t;

void init_song(song_t *);
void init_sync_track(sync_track_t *);
void init_event(event_t *);
void init_single(single_t *);

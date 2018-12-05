#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "chart.h"

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

FILE *fp;

int main(void)
{
	song_t 			s_song = {0};
	sync_track_t 	s_sync = {0};
	event_t 		s_event = {0};
	single_t 		s_sing = {0};

	fp = fopen("./charts/Undertale/1. Once Upon A Time/Undertale OST- 001 - Once Upon A Time.chart", "r");

	init_song(&s_song);
	init_sync_track(&s_sync);
	init_event(&s_event);
	init_single(&s_sing);

	printf("\n__________________Song struct__________________\n");
	printf("%s\n", s_song.name);
	printf("%s\n", s_song.artist);
	printf("%u\n", s_song.offset);
	printf("%u\n", s_song.resolution);
	printf("%s\n", s_song.genre);
	printf("%s\n", s_song.music_stream);
	/*
	printf("\n__________________Sync track struct____________\n");
	sync_track_t * sptr = &s_sync;
	while (sptr->next) {
		printf("time: %u\n", sptr->time);
		printf("mod: %s\n", sptr->mod);
		printf("val: %u\n", sptr->val);
		sptr = sptr->next;
	}
	
	printf("\n__________________Events struct________________\n");
	event_t * eptr = &s_event;
	while (eptr->next) {
		printf("time: %u\n", eptr->time);
		printf("message: %s\n", eptr->message);
		eptr = eptr->next;	        
	}
	*/
	printf("\n__________________Single struct________________\n");
	single_t * siptr = &s_sing;
	while (siptr->next) {
		printf("time: %-10u", siptr->time);
		printf("arg0: '%c'\t", siptr->arg0);
		printf("arg1: '%c'\t", siptr->arg1);
		printf("num0: %-10d", siptr->num0);
		printf("num1: %-10d\n", siptr->num1);
		siptr = siptr->next;
	}

	return 0;
}

void init_song(song_t * song)
{
	char buffer[128];
	char temp_str[128];
	int count = 0;

	while(fgets(buffer, 128, fp) && strcmp(temp_str, "[SyncTrack]")) {
		sscanf(buffer, "%s", temp_str);
		if (!strcmp(temp_str, "Name"))
			sscanf(buffer, QUOTE_REGEX, song->name);
		if (!strcmp(temp_str, "Artist"))
			sscanf(buffer, QUOTE_REGEX, song->artist);
		if (!strcmp(temp_str, "Offset"))
			sscanf(buffer, NUM_REGEX, &song->offset);
		if (!strcmp(temp_str, "Resolution"))
			sscanf(buffer, NUM_REGEX, &song->resolution);
		if (!strcmp(temp_str, "Genre"))
			sscanf(buffer, QUOTE_REGEX, song->genre);
		if (!strcmp(temp_str, "MusicStream"))
			sscanf(buffer, QUOTE_REGEX, song->music_stream);
		count++;
	}
	printf("Number of song init lines :%d\n", count);
}

void init_sync_track(sync_track_t * sync_track)
{
	char buffer[128];
	char temp_str[128];
	sync_track_t * st = sync_track;

	int count = 0;

	while(fgets(buffer, 128, fp) && buffer[0] != '}') {
		sscanf(buffer, "%d", &st->time);
		sscanf(buffer, "%*[^=]%*c%s", st->mod);
		sscanf(buffer, "%*[^=]%*c%*s%d", &st->val);

		st->next = (sync_track_t *) malloc(sizeof(sync_track_t));
		st = st->next;

		sscanf(buffer, "%s", temp_str);
		count++;
	}
	fgets(buffer, 128, fp);	// move to the next area
	fgets(buffer, 128, fp);	// move to the next area
	printf("Number of sync track init lines :%d\n", count);
}

void init_event(event_t * event)
{
	char buffer[1024];
	char temp_str[1024];
	event_t * e = event;

	int count = 0;

	while(fgets(buffer, 1024, fp) && buffer[0] != '}') {
		sscanf(buffer, "%d", &e->time);
		sscanf(buffer, QUOTE_REGEX, e->message);

		e->next = (event_t *) malloc(sizeof(event_t));
		e = e->next;

		sscanf(buffer, "%s", temp_str);
		count++;
	}
	printf("Number of event init lines :%d\n", count);
}

void init_single(single_t * single)
{
	char buffer[128];
	char temp_str[128];
	single_t * s = single;

	int count = 0;

	while(fgets(buffer, 128, fp) && buffer[0] != '}') {
		sscanf(buffer, "%d", &s->time);
		sscanf(buffer, "%*[^=]%*2c%c", &s->arg0);
		if (s->arg0 == 'N') {
			sscanf(buffer, "%*[^=]%*4c%hd", &s->num0);
			sscanf(buffer, "%*[^=]%*4c%*hd%hd", &s->num1);
		} else {
			sscanf(buffer, "%*[^=]%*4c%c", &s->arg1);
		}

		s->next = (single_t *) malloc(sizeof(single_t));
		s = s->next;

		sscanf(buffer, "%s", temp_str);
		count++;
	}
	printf("Number of single init lines :%d\n", count);
}

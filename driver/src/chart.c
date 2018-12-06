#include <stm32f30x.h>
#include <f3d_lcd_sd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <diskio.h>
#include <ff.h>
#include <chart.h>

/* manually copied from ff.c because it wouldn't include... */
TCHAR* f_gets (
	TCHAR* buff,	/* Pointer to the string buffer to read */
	int len,		/* Size of string buffer (characters) */
	FIL* fp			/* Pointer to the file object */
)
{
	int n = 0;
	TCHAR c, *p = buff;
	BYTE s[2];
	UINT rc;


	while (n < len - 1) {			/* Read bytes until buffer gets filled */
		f_read(fp, s, 1, &rc);
		if (rc != 1) break;			/* Break on EOF or error */
		c = s[0];
#if _LFN_UNICODE					/* Read a character in UTF-8 encoding */
		if (c >= 0x80) {
			if (c < 0xC0) continue;	/* Skip stray trailer */
			if (c < 0xE0) {			/* Two-byte sequence */
				f_read(fp, s, 1, &rc);
				if (rc != 1) break;
				c = ((c & 0x1F) << 6) | (s[0] & 0x3F);
				if (c < 0x80) c = '?';
			} else {
				if (c < 0xF0) {		/* Three-byte sequence */
					f_read(fp, s, 2, &rc);
					if (rc != 2) break;
					c = (c << 12) | ((s[0] & 0x3F) << 6) | (s[1] & 0x3F);
					if (c < 0x800) c = '?';
				} else {			/* Reject four-byte sequence */
					c = '?';
				}
			}
		}
#endif
#if _USE_STRFUNC >= 2
		if (c == '\r') continue;	/* Strip '\r' */
#endif
		*p++ = c;
		n++;
		if (c == '\n') break;		/* Break on EOL */
	}
	*p = 0;
	return n ? buff : 0;			/* When no data read (eof or error), return with error. */
}

void init_song(FIL * fp, song_t * song)
{

	TCHAR buffer[128];
	TCHAR temp_str[128];
	int count = 0;


	while(f_gets(buffer, 128, fp) && strcmp(temp_str, "[SyncTrack]")) {
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
	printf("song lines :%d\n", count);
}

void init_sync_track(FIL * fp, sync_track_t * sync_track)
{
	TCHAR buffer[128];
	TCHAR temp_str[128];
	sync_track_t * st = sync_track;

	int count = 0;

	while (f_gets(buffer, 128, fp) && buffer[0] != '}') {
		
		sscanf(buffer, "%d", &st->time);
		sscanf(buffer, "%*[^=]%*c%s", st->mod);
		sscanf(buffer, "%*[^=]%*c%*s%d", &st->val);

		st->next = calloc(1, sizeof(sync_track_t));
		st = st->next;

		sscanf(buffer, "%s", temp_str);
		count++;
	}
	// skip 2 lines
	f_gets(buffer, 128, fp);
	f_gets(buffer, 128, fp);
	printf("sync lines :%d\n", count);
}

void init_event(FIL * fp, event_t * event)
{
	TCHAR buffer[256];
	TCHAR temp_str[256];
	event_t * e = event;

	int count = 0;

	while(f_gets(buffer, 256, fp) && buffer[0] != '}') {

		sscanf(buffer, "%d", &e->time);
		sscanf(buffer, QUOTE_REGEX, e->message);

		e->next = calloc(1, sizeof(event_t));
		e = e->next;

		sscanf(buffer, "%s", temp_str);
		count++;
	}
	printf("event lines :%d\n", count);
	f_gets(buffer, 128, fp);
	f_gets(buffer, 128, fp);
}

void init_single(FIL * fp, single_t * single)
{
	TCHAR buffer[128];
	TCHAR temp_str[128];
	single_t * s = single;

	int count = 0;

	while(f_gets(buffer, 128, fp) && buffer[0] != '}') {

		// read time and TCHAR
		sscanf(buffer, "%d", &s->time);
		sscanf(buffer, "%*[^=]%*2c%c", &s->arg0);

		if (s->arg0 == 'N' || s->arg0 == 'S') {
			sscanf(buffer, "%*[^=]%*4c%hd", &s->num0);
			sscanf(buffer, "%*[^=]%*4c%*hd%hd", &s->num1);
		} else {
			sscanf(buffer, "%*[^=]%*4c%c", &s->arg1);
		}

		s->next = calloc(1, sizeof(single_t));
		s = s->next;

		sscanf(buffer, "%s", temp_str);
		count++;
	}
	printf("single lines :%d\n", count);
}

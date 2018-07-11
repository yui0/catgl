/* public domain Simple, Minimalistic, Sound library
 *	©2018 Yuichiro Nakada
 *
 * Basic usage:
 *	BERRY_SOUND a;
 *	b_open_sound_device(&a);
 *	...
 *	b_sound_play_file(&snd, "audio.mp3", 0);
 *	...
 *	b_sound_close(&a);
 *
 * */

#define BERRY_SOUND_ALSA

#ifdef BERRY_SOUND_ALSA
// Use the newer ALSA API
#define ALSA_PCM_NEW_HW_PARAMS_API
#define __timespec_defined
#include <alloca.h>
#include <alsa/asoundlib.h>
#else
#include <fcntl.h>
#include <unistd.h>
#endif

#include <pthread.h>

#define BERRY_SOUND_MAXTRACK	10
#define BERRY_SOUND_BUFFER_SIZE	4096 // PCM data samples (i.e. 16bit, Mono: 8Kb)

typedef struct {
#if defined(SUPPORT_FILEFORMAT_OGG)
	stb_vorbis *ctxOgg;		// OGG audio context
#endif
#if defined(SUPPORT_FILEFORMAT_FLAC)
	drflac *ctxFlac;		// FLAC audio context
#endif
#if defined(SUPPORT_FILEFORMAT_XM)
	jar_xm_context_t *ctxXm;	// XM chiptune context
#endif
#if defined(SUPPORT_FILEFORMAT_MOD)
	jar_mod_context_t ctxMod;	// MOD chiptune context
#endif

	int loop;			// Loops count (times music repeats), -1 means infinite loop
//	unsigned int totalSamples;	// Total number of samples
//	unsigned int samplesLeft;	// Number of samples left to end
	short *pcm;
	int pos;
	int size;
	char *name;
} BERRY_SOUND_TRACK;

typedef struct {
#ifdef BERRY_SOUND_ALSA
	snd_pcm_t *handle;
	snd_pcm_uframes_t frames;
#endif
	char *pcm;
	int size;
	char *name;
	pthread_t thread[BERRY_SOUND_MAXTRACK+1];

//	int playing;
	BERRY_SOUND_TRACK track[BERRY_SOUND_MAXTRACK];
} BERRY_SOUND;

// ALSA
#ifdef BERRY_SOUND_ALSA
int b_sound_init_ALSA(BERRY_SOUND *thiz, char *dev, unsigned int val, int ch, int frames, int flag)
{
	// Open PCM device.
	int rc = snd_pcm_open(&thiz->handle, dev, flag ? SND_PCM_STREAM_PLAYBACK : SND_PCM_STREAM_CAPTURE, 0);
	if (rc < 0) {
		fprintf(stderr, "unable to open pcm device '%s' (%s)\n", dev, snd_strerror(rc));
		return 1;
	}

	// Allocate a hardware parameters object.
	snd_pcm_hw_params_t *params;
	snd_pcm_hw_params_alloca(&params);

	// Fill it in with default values.
	snd_pcm_hw_params_any(thiz->handle, params);

	// Set the desired hardware parameters.
	// Interleaved mode
	rc = snd_pcm_hw_params_set_access(thiz->handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
	if (rc < 0) {
		fprintf(stderr, "cannot set access type (%s)\n", snd_strerror(rc));
		snd_pcm_drain(thiz->handle);
		snd_pcm_close(thiz->handle);
		return 1;
	}

	// Signed 16-bit little-endian format
	rc = snd_pcm_hw_params_set_format(thiz->handle, params, SND_PCM_FORMAT_S16_LE);
	if (rc < 0) {
		fprintf(stderr, "cannot set sample format (%s)\n", snd_strerror(rc));
		snd_pcm_drain(thiz->handle);
		snd_pcm_close(thiz->handle);
		return 1;
	}

	// Channels
	rc = snd_pcm_hw_params_set_channels(thiz->handle, params, ch);
	if (rc < 0) {
		fprintf(stderr, "cannot set channel count (%s)\n", snd_strerror(rc));
		snd_pcm_drain(thiz->handle);
		snd_pcm_close(thiz->handle);
		return 1;
	}

	// 44100 bits/second sampling rate (CD quality)
	int dir;
	rc = snd_pcm_hw_params_set_rate_near(thiz->handle, params, &val, &dir);
	if (rc < 0) {
		fprintf(stderr, "cannot set sample rate (%s)\n", snd_strerror(rc));
		snd_pcm_drain(thiz->handle);
		snd_pcm_close(thiz->handle);
		return 1;
	}

	// Set period size to 32 frames.
	thiz->frames = frames;
	snd_pcm_hw_params_set_period_size_near(thiz->handle, params, &thiz->frames, &dir);

	// Write the parameters to the driver
	rc = snd_pcm_hw_params(thiz->handle, params);
	if (rc < 0) {
		fprintf(stderr, "unable to set parameters (%s)\n", snd_strerror(rc));
		snd_pcm_drain(thiz->handle);
		snd_pcm_close(thiz->handle);
		return 1;
	}

	// Use a buffer large enough to hold one period
	snd_pcm_hw_params_get_period_size(params, &thiz->frames, &dir);
	thiz->size = thiz->frames * 2 * ch; /* 2 bytes/sample, 2 channels */
	thiz->pcm = (char*)malloc(thiz->size);

	snd_pcm_hw_params_get_period_time(params, &val, &dir);
	return 0;
}

int b_sound_record_ALSA(BERRY_SOUND *thiz)
{
	int rc = snd_pcm_readi(thiz->handle, thiz->pcm, thiz->frames);
	if (rc == -EPIPE) {
		// EPIPE means overrun
		fprintf(stderr, "overrun occurred\n");
		snd_pcm_prepare(thiz->handle);
	} else if (rc < 0) {
		fprintf(stderr, "read failed (%s)\n", snd_strerror(rc));
	} else if (rc != (int)thiz->frames) {
		fprintf(stderr, "short read, read %d frames\n", rc);
	}
	return rc;
}

int b_sound_play_ALSA(BERRY_SOUND *thiz, char *data, int frames)
{
	int rc = snd_pcm_writei(thiz->handle, data, frames);
	if (rc == -EPIPE) {
		// EPIPE means overrun
		fprintf(stderr, "overrun occurred\n");
		snd_pcm_recover(thiz->handle, rc, 0);
		//snd_pcm_prepare(thiz->handle);
	} else if (rc < 0) {
		fprintf(stderr, "write failed (%s)\n", snd_strerror(rc));
	} else if (rc != frames) {
		fprintf(stderr, "short write, write %d/%d frames\n", rc, (int)thiz->frames);
	}
	return rc;
}

int b_sound_play0(BERRY_SOUND *thiz)
{
	return b_sound_play_ALSA(thiz, thiz->pcm, thiz->frames);
}

void b_sound_wait_ALSA(BERRY_SOUND *thiz, int msec)
{
	snd_pcm_wait(thiz->handle, msec);
}

void b_sound_close_ALSA(BERRY_SOUND *thiz)
{
	snd_pcm_drain(thiz->handle);
	snd_pcm_close(thiz->handle);
	free(thiz->pcm);
}
#else
int b_sound_init_ALSA(BERRY_SOUND *thiz, char *dev, unsigned int val, int ch, int frames, int flag) {}
int b_sound_record_ALSA(BERRY_SOUND *thiz) {}
int b_sound_play_ALSA(BERRY_SOUND *thiz, char *data, int frames) {}
void b_sound_wait_ALSA(BERRY_SOUND *thiz, int msec) {}
void b_sound_close_ALSA(BERRY_SOUND *thib_sound_frame_ALSAz) {}
#endif

// API
int b_sound_play(BERRY_SOUND *thiz, char *data, int frames)
{
	return b_sound_play_ALSA(thiz, data, frames);
}
void b_sound_wait(BERRY_SOUND *thiz, int msec)
{
	b_sound_wait_ALSA(thiz, msec);
}
short b_sound_pcm[BERRY_SOUND_BUFFER_SIZE];
void *b_sound_play_thread(void *args)
{
	BERRY_SOUND *a = (BERRY_SOUND*)args;

	while (/*a->playing*/1) {
		memset(b_sound_pcm, 0, sizeof(b_sound_pcm));
		for (int i=0; i<BERRY_SOUND_MAXTRACK; i++) {
			if (!a->track[i].loop) continue;

//			float *p = &a->track[i].pcm[a->track[i].pos];
			short *p = &a->track[i].pcm[a->track[i].pos];
			for (int n=0; n<BERRY_SOUND_BUFFER_SIZE; n++) {
//				b_sound_pcm[n] += *p++ *65535;
				b_sound_pcm[n] += *p++;
				a->track[i].pos++;
				if (a->track[i].pos > a->track[i].size) {
					a->track[i].pos = 0;
					if (a->track[i].loop>0) a->track[i].loop--;
				}
			}
		}

		b_sound_play(a, (char*)b_sound_pcm, BERRY_SOUND_BUFFER_SIZE/2);
		b_sound_wait(a, 100);
//		pthread_yield();
	}
//	pthread_exit(NULL);
}
int b_open_sound_device(BERRY_SOUND *a)
{
//	return b_sound_init_ALSA(a, "default", 48000, 2, 32, 1);
//	return b_sound_init_ALSA(a, "default", 44100, 2, 32, 1);

	int r = b_sound_init_ALSA(a, "default", 44100, 2, 32, 1);

//	a->playing = 1;
/*	int ret = pthread_create(&a->thread[BERRY_SOUND_MAXTRACK], NULL, b_sound_play_thread, (void*)a);
	if (ret != 0) {
		printf("pthread_create() failed.\n");
	}*/

	return r;
}
void b_close_soound_device(BERRY_SOUND *a)
{
	for (int i=0; i<BERRY_SOUND_MAXTRACK+1; i++) pthread_cancel(a->thread[i]);
	b_sound_close_ALSA(a);
}

#include <sys/mman.h>
#include "berry_minimp3.h"
void *preload(char *name, int *len)
{
	int fd = open(name, O_RDONLY);
	if (fd < 0) {
		printf("Error: cannot open `%s`\n", name);
		return 0;
	}
	*len = lseek(fd, 0, SEEK_END);
	void *p = mmap(0, *len, PROT_READ, MAP_PRIVATE, fd, 0);
	close(fd);
	return p;
}
//short *b_mp3_load(char *name, int *plen)
void b_mp3_load(char *name, short **data, int *plen)
{
	int len;
	void *file_data = preload(name, &len);
	unsigned char *stream_pos = (unsigned char *)file_data;
	int bytes_left = len - 100;

	int c = 0;
	int size = len*10;
	short *pcm = malloc(size);
	short *p = pcm;
	mp3_info_t info;
	mp3_decoder_t mp3 = mp3_create();
	int frame_size = mp3_decode(mp3, stream_pos, bytes_left, p, &info);
	*data = pcm;
	while ((bytes_left >= 0) && (frame_size > 0)) {
		stream_pos += frame_size;
		bytes_left -= frame_size;
		p += info.audio_bytes/2;
		c += info.audio_bytes/2;
		if (c > size-info.audio_bytes/2) {
			size += 1024*1024;
			pcm = realloc(pcm, size);
			p = pcm+c;
		}

		frame_size = mp3_decode(mp3, stream_pos, bytes_left, p, NULL);
	}
	*plen = c;

	mp3_free(mp3);
	munmap(file_data, len);
//	return pcm;
}
/*void *b_sound_play_mp3(void *args)
{
	BERRY_SOUND_TRACK *t = (BERRY_SOUND_TRACK*)args;
	b_mp3_load(t->name, &t->pcm, &t->size);
}*/
void *b_sound_play_mp3(void *args)
{
	BERRY_SOUND *a = (BERRY_SOUND*)args;
	short sample_buf[MP3_MAX_SAMPLES_PER_FRAME];
	int frame_size;

	int len;
	void *file_data = preload(a->name, &len);
	unsigned char *stream_pos = (unsigned char *)file_data;
	int bytes_left = len - 100;

	mp3_info_t info;
	mp3_decoder_t mp3 = mp3_create();
	frame_size = mp3_decode(mp3, stream_pos, bytes_left, sample_buf, &info);
	if (!frame_size) {
		printf("Error: not a valid MP3 audio file!\n");
		return (void*)1;
	}
//	printf("%dHz %dch\n", info.sample_rate, info.channels);

	int c = 0;
	printf("\e[?25l");
	while ((bytes_left >= 0) && (frame_size > 0)) {
//		printf("\r%d", c);

		stream_pos += frame_size;
		bytes_left -= frame_size;
		b_sound_play(a, (char*)sample_buf, info.audio_bytes/2/info.channels);
		b_sound_wait(a, 100);

		c += frame_size;
		frame_size = mp3_decode(mp3, stream_pos, bytes_left, sample_buf, NULL);
	}
	printf("\e[?25h");

	mp3_free(mp3);
	munmap(file_data, len);
	return (void*)0;
}

void b_sound_stop(BERRY_SOUND *a, int n)
{
	if (a->thread[n]) pthread_cancel(a->thread[n]);
	a->thread[n] = 0;
}
int b_sound_play_file(BERRY_SOUND *a, char *name, int n)
{
#if 0
	a->track[n].loop = 0;
//	if (a->track[n].pcm) free(a->track[n].pcm);
	a->track[n].name = name;
//	a->track[n].pcm = b_mp3_load(name, &a->track[n].size);
	a->track[n].pos = 0;
//	a->track[n].loop = -1;

	b_sound_stop(a, n);
	if (a->track[n].pcm) {
		free(a->track[n].pcm);
		a->track[n].pcm = 0;
	}
	int ret = pthread_create(&a->thread[n], NULL, b_sound_play_mp3, (void*)&a->track[n]);
	if (ret != 0) {
		printf("pthread_create() failed.\n");
	}
//	sleep(1);
//	a->track[n].loop = -1;
#endif
	b_sound_stop(a, n);

	a->name = name;
	int ret = pthread_create(&a->thread[n], NULL, b_sound_play_mp3, (void*)a);
	if (ret != 0) {
		printf("pthread_create() failed.\n");
	}
	return 0;
}

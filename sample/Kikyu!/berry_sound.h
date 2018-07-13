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

#include "berry_minimp3.h"
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
	berry_mp3 mp3;

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
	pthread_t thread;

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
	int max_frame = 0;

	while (/*a->playing*/1) {
		memset(b_sound_pcm, 0, sizeof(b_sound_pcm));
		for (int i=0; i<BERRY_SOUND_MAXTRACK; i++) {
			if (!a->track[i].loop) continue;

			int frame = berry_mp3_decode_frame(&a->track[i].mp3);
			if (!frame) {
				if (a->track[i].loop>0) a->track[i].loop--;
				/*if (!a->track[i].loop) {
					berry_mp3_destroy(&a->track[i].mp3);
				}*/
				continue;
			}
			if (max_frame<frame) max_frame = frame;

			for (int n=0; n<frame*2; n++) {
				b_sound_pcm[n] += a->track[i].mp3.p[n];
			}
		}

//		b_sound_play(a, (char*)b_sound_pcm, BERRY_SOUND_BUFFER_SIZE/2);
		b_sound_play(a, (char*)b_sound_pcm, max_frame);
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
	int ret = pthread_create(&a->thread, NULL, b_sound_play_thread, (void*)a);
	if (ret != 0) {
		printf("pthread_create() failed.\n");
	}

	return r;
}
void b_close_soound_device(BERRY_SOUND *a)
{
	pthread_cancel(a->thread);
	b_sound_close_ALSA(a);
}

/*void b_sound_stop(BERRY_SOUND *a, int n)
{
	if (a->thread[n]) pthread_cancel(a->thread[n]);
	a->thread[n] = 0;
}*/
int b_sound_play_file(BERRY_SOUND *a, char *name, int n, int loop)
{
	a->track[n].loop = 0;
//	if (strcmp(a->track[n].name, name)) {
	if (a->track[n].mp3.data) {
		berry_mp3_destroy(&a->track[n].mp3);
	}

//	b_sound_stop(a, n);
//	a->name = name;
/*	if (!a->track[n].mp3.data)*/ berry_mp3_init(&a->track[n].mp3, name);
	a->track[n].loop = loop;
	return 0;
}

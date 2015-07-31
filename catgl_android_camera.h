#include <hardware/hardware.h>
#include <hardware/camera.h>
#include <system/window.h>		// for ANativeWindowBuffer_t not ANativeWindow_Buffer

#include "catgl_android_hardware.h"

const hw_module_t *module;
camera_device_t *camera_device;


// from https://android.googlesource.com/platform/frameworks/av/+/jb-mr1.1-dev/services/camera/libcameraservice/CameraHardwareInterface.h
static void __notify_cb(int32_t msg_type, int32_t ext1, int32_t ext2, void *user)
{
	LOGD("%s", __FUNCTION__);
//	CameraHardwareInterface *__this = static_cast<CameraHardwareInterface *>(user);
//	__this->mNotifyCb(msg_type, ext1, ext2, __this->mCbUser);
}
static void __data_cb(int32_t msg_type, const camera_memory_t *data, unsigned int index, camera_frame_metadata_t *metadata, void *user)
{
	LOGD("%s", __FUNCTION__);
/*	CameraHardwareInterface *__this = static_cast<CameraHardwareInterface *>(user);
	sp<CameraHeapMemory> mem(static_cast<CameraHeapMemory *>(data->handle));
	if (index >= mem->mNumBufs) {
		ALOGE("%s: invalid buffer index %d, max allowed is %d", __FUNCTION__, index, mem->mNumBufs);
		return;
	}
	__this->mDataCb(msg_type, mem->mBuffers[index], metadata, __this->mCbUser);*/
}
static void __data_cb_timestamp(/*nsecs_t*/int64_t timestamp, int32_t msg_type, const camera_memory_t *data, unsigned index, void *user)
{
	LOGD("%s", __FUNCTION__);
/*	CameraHardwareInterface *__this = static_cast<CameraHardwareInterface *>(user);
	// Start refcounting the heap object from here on.  When the clients
	// drop all references, it will be destroyed (as well as the enclosed
	// MemoryHeapBase.
	sp<CameraHeapMemory> mem(static_cast<CameraHeapMemory *>(data->handle));
	if (index >= mem->mNumBufs) {
		ALOGE("%s: invalid buffer index %d, max allowed is %d", __FUNCTION__, index, mem->mNumBufs);
		return;
	}
	__this->mDataCbTimestamp(timestamp, msg_type, mem->mBuffers[index], __this->mCbUser);*/
}

static void __release_memory(struct camera_memory *mem)
{
	if (mem->data) free(mem->data);
	free(mem);
}
static camera_memory_t* __get_memory(int fd, size_t buf_size, uint_t num_bufs, void *user)
{
	LOGD("%s", __FUNCTION__);
	void *m = malloc(buf_size * num_bufs);
	if (!m) return NULL;

	camera_memory_t *camera_memory;
	camera_memory = (camera_memory_t*)malloc(sizeof(camera_memory_t));
	camera_memory->size = buf_size * num_bufs;
	camera_memory->data = m;
	camera_memory->handle = camera_memory;
	camera_memory->release = __release_memory;

	return camera_memory;
}


// gralloc_priv.h
struct private_handle_t {
	struct native_handle nativeHandle;

	// file-descriptors
	int fd;
	// ints
	int magic;
	int flags;
	int size;
	int offset;

	// FIXME: the attributes below should be out-of-line
	uint64_t base __attribute__((aligned(8)));
	int pid;
};

typedef struct _AQueue {
	struct ANativeWindowBuffer buf[32];
	int q[32];
	int count;
} AQueue;
int __setSwapInterval(struct ANativeWindow* window, int interval)
{
	LOGD("%s", __FUNCTION__);
	return 0;
}
int __dequeueBuffer_DEPRECATED(struct ANativeWindow* a, struct ANativeWindowBuffer** buffer)
{
	LOGD("%s", __FUNCTION__);
	AQueue *aq = (AQueue*)a->oem[0];
	int i;
	for (i=0; i<aq->count; i++) {
		if (!aq->q[i]) break;
	}
	if (aq->q[i]) return -1;
	aq->q[i] = 1;
	*buffer = &aq->buf[i];

	if (!aq->buf[i].handle) {
		aq->buf[i].handle = malloc(sizeof(struct private_handle_t));

		struct private_handle_t *ph = (struct private_handle_t*)aq->buf[i].handle;
		ph->fd = -1;
		ph->magic = 0x3141592;
		ph->size = 640*480*2;
		ph->offset = 0;
		ph->base = malloc(640*480*2);
	}

	return 0;
}
int __lockBuffer_DEPRECATED(struct ANativeWindow* window, struct ANativeWindowBuffer* buffer)
{
	LOGD("%s", __FUNCTION__);
	return 0;
}
int __queueBuffer_DEPRECATED(struct ANativeWindow* a, struct ANativeWindowBuffer* buffer)
{
	LOGD("%s", __FUNCTION__);
	AQueue *aq = (AQueue*)a->oem[0];
	int i;
	for (i=0; i<aq->count; i++) {
		if (buffer == (&aq->buf[i])) break;
	}
	if (buffer != (&aq->buf[i])) return -1;
	aq->q[i] = 0;
	return 0;
}
int __query(const struct ANativeWindow* window, int what, int* value)
{
	LOGD("%s", __FUNCTION__);
	return 0;
}
int __perform(struct ANativeWindow* window, int operation, ... )
{
	LOGD("%s [%x]", __FUNCTION__, operation);
	return 0;
}
int __cancelBuffer_DEPRECATED(struct ANativeWindow* a, struct ANativeWindowBuffer* buffer)
{
	LOGD("%s", __FUNCTION__);
	return 0;
}
int __dequeueBuffer(struct ANativeWindow* a, struct ANativeWindowBuffer** buffer, int* fenceFd)
{
	LOGD("%s", __FUNCTION__);
	__dequeueBuffer_DEPRECATED(a, buffer);
	return 0;
}
int __queueBuffer(struct ANativeWindow* a, struct ANativeWindowBuffer* buffer, int fenceFd)
{
	LOGD("%s", __FUNCTION__);
	__queueBuffer_DEPRECATED(a, buffer);
	return 0;
}
int __cancelBuffer(struct ANativeWindow* a, struct ANativeWindowBuffer* buffer, int fenceFd)
{
	LOGD("%s", __FUNCTION__);
	__cancelBuffer_DEPRECATED(a, buffer);
	return 0;
}
void initANativeWindow(ANativeWindow *a)
{
	a->common.magic = ANDROID_NATIVE_WINDOW_MAGIC;
	a->common.version = sizeof(ANativeWindow);
	memset(a->common.reserved, 0, sizeof(a->common.reserved));

//	a->flags = /*AWINDOW_FLAG_FULLSCREEN*/0;
//	a->minSwapInterval = a->maxSwapInterval = 0;
//	a->xdpi = a->ydpi = 72;
	AQueue aq;
	memset(&aq, 0, sizeof(AQueue));
	a->oem[0] = (int)(&aq);

	a->setSwapInterval = __setSwapInterval;
	a->dequeueBuffer_DEPRECATED = __dequeueBuffer_DEPRECATED;
	a->lockBuffer_DEPRECATED = __lockBuffer_DEPRECATED;
	a->queueBuffer_DEPRECATED = __queueBuffer_DEPRECATED;
	a->query = __query;
	a->perform = __perform;
	a->cancelBuffer_DEPRECATED = __cancelBuffer_DEPRECATED;
	a->dequeueBuffer = __dequeueBuffer;
	a->queueBuffer = __queueBuffer;
	a->cancelBuffer = __cancelBuffer;
}


struct camera_preview_window {
	struct preview_stream_ops win;
	void *user;
};
struct camera_preview_window mHalPreviewWindow;

#define anw(n) (ANativeWindow*)(((struct camera_preview_window *)n)->user)

static int __dequeue_buffer(struct preview_stream_ops* w, buffer_handle_t** buffer, int *stride)
{
	int rc;
	ANativeWindow *a = anw(w);
	ANativeWindowBuffer_t* anb;
	rc = a->dequeueBuffer_DEPRECATED(a, &anb);
	if (!rc) {
		*buffer = &anb->handle;
		*stride = anb->stride;
	}
	LOGD("  __dequeue_buffer %d", rc);
	return rc;
}
#ifndef container_of
#define container_of(ptr, type, member) ({ \
	const typeof(((type *) 0)->member) *__mptr = (ptr); \
	(type *) ((char *) __mptr - (char *)(&((type *)0)->member)); })
#endif
static int __lock_buffer(struct preview_stream_ops* w, buffer_handle_t* buffer)
{
	LOGD("  __lock_buffer");
	ANativeWindow *a = anw(w);
	return 0;
}
static int __enqueue_buffer(struct preview_stream_ops* w, buffer_handle_t* buffer)
{
	LOGD("  __enqueue_buffer");
	ANativeWindow *a = anw(w);
	return a->queueBuffer(a, container_of(buffer, ANativeWindowBuffer_t, handle), -1);
}
static int __cancel_buffer(struct preview_stream_ops* w, buffer_handle_t* buffer)
{
	LOGD("  __cancel_buffer");
	ANativeWindow *a = anw(w);
	return a->cancelBuffer(a, container_of(buffer, ANativeWindowBuffer_t, handle), -1);
}
static int __set_buffer_count(struct preview_stream_ops* w, int count)
{
//	LOGD("  __set_buffer_count: %d", count);
//	return 0;
	ANativeWindow *a = anw(w);
	return native_window_set_buffer_count(a, count);
}
static int __set_buffers_geometry(struct preview_stream_ops* w, int width, int height, int format)
{
	LOGD("  __set_buffers_geometry: %dx%d %d", width, height, format);
	ANativeWindow *a = anw(w);
	return native_window_set_buffers_geometry(a, width, height, format);
}
static int __set_crop(struct preview_stream_ops *w, int left, int top, int right, int bottom)
{
	LOGD("  __set_crop");
	ANativeWindow *a = anw(w);
	android_native_rect_t crop;
	crop.left = left;
	crop.top = top;
	crop.right = right;
	crop.bottom = bottom;
	return native_window_set_crop(a, &crop);
}
static int __set_timestamp(struct preview_stream_ops *w, int64_t timestamp) {
	ANativeWindow *a = anw(w);
	return native_window_set_buffers_timestamp(a, timestamp);
}
static int __set_usage(struct preview_stream_ops* w, int usage)
{
	ANativeWindow *a = anw(w);
	return native_window_set_usage(a, usage);
}
static int __set_swap_interval(struct preview_stream_ops *w, int interval)
{
	ANativeWindow *a = anw(w);
	return a->setSwapInterval(a, interval);
}
static int __get_min_undequeued_buffer_count(const struct preview_stream_ops *w, int *count)
{
	ANativeWindow *a = anw(w);
	return a->query(a, NATIVE_WINDOW_MIN_UNDEQUEUED_BUFFERS, count);
}
void initHalPreviewWindow(ANativeWindow *a)
{
	mHalPreviewWindow.win.cancel_buffer = __cancel_buffer;
	mHalPreviewWindow.win.lock_buffer = __lock_buffer;
	mHalPreviewWindow.win.dequeue_buffer = __dequeue_buffer;
	mHalPreviewWindow.win.enqueue_buffer = __enqueue_buffer;
	mHalPreviewWindow.win.set_buffer_count = __set_buffer_count;
	mHalPreviewWindow.win.set_buffers_geometry = __set_buffers_geometry;
	mHalPreviewWindow.win.set_crop = __set_crop;
	mHalPreviewWindow.win.set_timestamp = __set_timestamp;
	mHalPreviewWindow.win.set_usage = __set_usage;
	mHalPreviewWindow.win.set_swap_interval = __set_swap_interval;

	mHalPreviewWindow.win.get_min_undequeued_buffer_count = __get_min_undequeued_buffer_count;

	mHalPreviewWindow.user = a;
}


void caCameraInit(struct android_app* app)
{
	if (hw_get_module(CAMERA_HARDWARE_MODULE_ID, &module) < 0) {
		LOGE("Could not load camera HAL module: %s.", CAMERA_HARDWARE_MODULE_ID);
		//mNumberOfCameras = 0;
	}
	LOGD("  Camera module name: %s", module->name);
	LOGD("  Camera module author: %s", module->author);
	LOGD("  Camera module API version: 0x%x", module->module_api_version);
	LOGD("  Camera module HAL API version: 0x%x", module->hal_api_version);

	LOGD("  Number of cameras: %d", ((camera_module_t*)module)->get_number_of_cameras());

	//snprintf(camId, 10, "%d", id);
	module->methods->open(module, CAMERA_HARDWARE_MODULE_ID/*camId*/, (hw_device_t**)&camera_device);
	LOGD("  Camera device: 0x%x", camera_device);

//	camera_device->ops->take_picture(camera_device);
//	camera_device->ops->set_preview_window(camera_device, app->window);

	camera_device->ops->set_callbacks(camera_device, __notify_cb, __data_cb, __data_cb_timestamp, __get_memory, 0);

//	native_window_set_buffers_transform(app->window, mOrientation);

	ANativeWindow a;
	initANativeWindow(&a);
	initHalPreviewWindow(&a);
//	initHalPreviewWindow(app->window);
//	LOGD("  set_preview_window: 0x%x", camera_device->ops->set_preview_window(camera_device, &mHalPreviewWindow.win));
//	LOGD("  set_preview_window: 0x%x", camera_device->ops->set_preview_window(camera_device, 0));

	struct preview_stream_ops *win = &mHalPreviewWindow.win;
	int min_bufs;
	win->get_min_undequeued_buffer_count(win, &min_bufs);
	LOGD("%s: bufs:%i", __FUNCTION__, min_bufs);

	//win->set_usage(win, GRALLOC_USAGE_PMEM_PRIVATE_ADSP | GRALLOC_USAGE_SW_READ_OFTEN);
	win->set_buffers_geometry(win, 640, 480, HAL_PIXEL_FORMAT_YCrCb_420_SP);

	LOGD("  preview_enabled: 0x%x", camera_device->ops->preview_enabled(camera_device));
	LOGD("  start_preview: 0x%x", camera_device->ops->start_preview(camera_device));

	return;
}

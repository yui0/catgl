void caCameraInit(struct android_app* app)
{
	JNIEnv* env = app->activity->env;
	JavaVM* vm = app->activity->vm;
	(*vm)->AttachCurrentThread(vm, &env, NULL);

	jclass activityClass = (*env)->GetObjectClass(env, app->activity->clazz);//android/app/NativeActivity
	jmethodID method = (*env)->GetMethodID(env, activityClass, "cameraOpen", "()V;");
/*	(*env)->CallVoidMethod(env, app->activity->clazz, method);*/
//	(*env)->CallObjectMethod(env, app->activity->clazz, method);

//	jclass jcls = (*env)->FindClass(env, "berry/catgl/CameraView");
//	jclass jcls = (*env)->FindClass(env, "berry/catgl/Catgl");
//	jmethodID mj = (*env)->GetMethodID(env, jcls, "FlashOn", "()V");
//	jmethodID mj = (*env)->GetMethodID(env, jcls, "cameraOpen", "()V");
//	jobject jobj = (*env)->NewObject(env, jcls, mj);
//	(*env)->CallObjectMethod(env, jobj, mj);
//	(*env)->CallVoidMethod(env, app->activity->clazz, method);

	(*vm)->DetachCurrentThread(vm);
}

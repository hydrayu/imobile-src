#include "com_imobile_iq8readserialnumber_iMobileReadSerialNumber.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <android/log.h>
// file : myboolean.h
#ifndef MYBOOLEAN_H
#define MYBOOLEAN_H

#define false 0
#define true 1
typedef int bool; // or #define bool int

#endif

#define LOG_TAG "Po_JNI"
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)

#ifdef __cplusplus
extern "C"
{
#endif

JNIEXPORT jstring JNICALL Java_com_imobile_iq8readserialnumber_iMobileReadSerialNumber_ReadSN(
		JNIEnv *env, jclass arg) {

#define CHUNK 1024 // read 1024 bytes at a time
	char buf[CHUNK];
	FILE *file;
	size_t nread;

	file = fopen("/data/Po_prop.txt", "r");
	if (file) {
		while ((nread = fread(buf, 1, sizeof buf, file)) > 0)
			fwrite(buf, 1, nread, stdout);

		if (ferror(file)) {
			// deal with error
		}
		fclose(file);
	}

	//LOGI("[Po add] buf=%s", buf);
	char *test = strtok(buf, "\n");
	LOGI("[Po add] test=%s", test);

	jstring str = (*env)->NewStringUTF(env, "init");

	if((test != "en-US") && (test != NULL) && (test != "")){
		str = (*env)->NewStringUTF(env, test);
	}else{
		str = (*env)->NewStringUTF(env, "Not SN!");
	}


	return str;
}

#ifdef __cplusplus
}
#endif

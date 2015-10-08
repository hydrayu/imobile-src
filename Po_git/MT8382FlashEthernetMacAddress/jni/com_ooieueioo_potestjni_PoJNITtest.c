#include "com_ooieueioo_potestjni_PoJNITtest.h"

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

/*
 *jstring 2 char*
 */
char* Jstring2CStr(JNIEnv* env, jstring jstr) {
	char* rtn = NULL;
	jclass clsstring = (*env)->FindClass(env, "java/lang/String");
	jstring strencode = (*env)->NewStringUTF(env, "utf-8");
	jmethodID mid = (*env)->GetMethodID(env, clsstring, "getBytes",
			"(Ljava/lang/String;)[B");
	jbyteArray barr = (jbyteArray)(*env)->CallObjectMethod(env, jstr, mid,
			strencode);
	jsize alen = (*env)->GetArrayLength(env, barr);
	jbyte* ba = (*env)->GetByteArrayElements(env, barr, JNI_FALSE);
	if (alen > 0) {
		rtn = (char*) malloc(alen + 1);
		memcpy(rtn, ba, alen);
		rtn[alen] = 0;
	}
	(*env)->ReleaseByteArrayElements(env, barr, ba, 0);
	return rtn;
}

JNIEXPORT jstring JNICALL Java_com_ooieueioo_potestjni_PoJNITtest_WriteProc(JNIEnv *env, jclass arg, jstring Write_Path_from_JNI, jstring Write_value_from_JNI)
{
	char* Write_Path = Jstring2CStr(env,Write_Path_from_JNI);
	char* Write_value = Jstring2CStr(env,Write_value_from_JNI);
	char* status = "";

	//LOGI("[Po add] WriteProc() Write_Path=%s", Write_Path);
	//LOGI("[Po add] WriteProc() Write_value=%s", Write_value);
	//LOGI("[Po add] WriteProc() status=%s", status);

	FILE *f = fopen(Write_Path, "w");
	if (f == NULL)
	{
		LOGI("[Po add] WriteProc() error!");
		status = "xx";
	    exit(1);
	}

	/* print some text */
	const char *text = Write_value;
	fprintf(f, "%s\n", text);

	fclose(f);
	status = "oo";

	//LOGI("[Po add] TransportStringToC() %s", p);
	//Po log start
	LOGI("[Po add] WriteProc() %s", status);
	//Po log end

	return (*env)->NewStringUTF(env, status);
}

/***** Po area
 *
*/
JNIEXPORT jstring JNICALL Java_com_ooieueioo_potestjni_PoJNITtest_ReadMacAddress(JNIEnv *env, jclass arg)
{

	char Po_str[1024];
	LOGI("[Po add] ReadMacAddress()");

	//** write**/
	//** echo ShowMac > /proc/eth0 **/
	/*
	FILE *ff = fopen("/proc/eth0", "w");
	if (ff == NULL)
	{
		LOGI("[Po add] WriteProc() error!");
	}

	/// print some text
	// echo S > /proc/eth0
	// 會造成 OS 當機 start
	const char *text = "ShowMac";
	fprintf(ff, "%s\n", text);
	// 會造成 OS 當機 End
	LOGI("[Po add] WriteProc() success!");
	fclose(ff);
	*/

	/** read **/
	/** cat /proc/eth0 **/
#define CHUNK 1024 /* read 1024 bytes at a time */

	char buf[CHUNK];
	FILE *file;
	size_t nread;

	file = fopen("/proc/eth0", "r");
	if (file) {
		while ((nread = fread(buf, 1, sizeof buf, file)) > 0)
			fwrite(buf, 1, nread, stdout);

		if (ferror(file)) {
			// deal with error
		}
		fclose(file);
	}else{
		strcpy(Po_str, "Not Read Mac");
		return (*env)->NewStringUTF(env, Po_str);;
	}

	LOGI("[Po add] ReadProc() %s", buf);

	/** read end **/

	// check format
	if(strstr(buf, "not") || strstr(buf, "thing")){
		LOGI("[Po add] return()");
		strcpy(Po_str, "Not Read Mac");
		return (*env)->NewStringUTF(env, Po_str);;
	}

	/** strsep start **/
	char* const delim = " ";
	char *token, *cur = buf;

	// 00 60 6e 10 00 f1 51 55 46 0a.....
	// get 5 number (result -> 00 60 6e 10 00 f1)
	int number = 0;
	while (token = strsep(&cur, delim)) {
		if(number++ < 6){
			LOGI("[Po add] %s\n", token);
			strcat(Po_str, token);
		}
	}
	/** strsep End **/


	LOGI("[Po add] Po_str = %s", Po_str);



	return (*env)->NewStringUTF(env, Po_str);;
}

#ifdef __cplusplus
}
#endif

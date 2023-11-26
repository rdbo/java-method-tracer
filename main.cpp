#include "jni.h"
#include "jvmti.h"
#include <iostream>
#include <libmem/libmem.h>

#define JNI_VERSION JNI_VERSION_1_8

void JNICALL MethodEntryCallback(jvmtiEnv *jvmti, JNIEnv* jni, jthread thread, jmethodID method)
{
	char *mname;
	char *msig;
	
	jvmti->GetMethodName(method, &mname, &msig, NULL);
	std::cout << "Method called: " << mname << " -> " << msig << std::endl;
}

/*
void __attribute__((constructor))
dl_entry()
{
	JavaVM *jvm;
	JNIEnv *env;
	JavaVMAttachArgs attach_args;
	jvmtiEnv *jvmti;
	jvmtiEventCallbacks callbacks = { 0 };

	std::cout << "Library loaded!" << std::endl;

	std::cout << "JNI_GetCreatedJavaVMs: " << JNI_GetCreatedJavaVMs(&jvm, 1, NULL) << std::endl;
	std::cout << "JavaVM pointer: " << (void *)jvm << std::endl;

	attach_args.version = JNI_VERSION;
	attach_args.name = nullptr;
	attach_args.group = nullptr;

	jvm->AttachCurrentThread((void **)&env, (void *)&attach_args);

	std::cout << "JNIEnv pointer: " << (void *)env << std::endl;

	jclass main = env->FindClass("main/Main");
	printf("Main class pointer: %p\n", (void *)main);

	jfieldID myNumberID = env->GetStaticFieldID(main, "myNumber", "I");
	printf("myNumber ID address: %p\n", (void *)myNumberID);

	jint myNumber = env->GetStaticIntField(main, myNumberID);
	printf("myNumber value: %d\n", myNumber);

	env->SetStaticIntField(main, myNumberID, 1337);
	printf("changed number value\n");

	jmethodID hookMeID = env->GetStaticMethodID(main, "hookMe", "()V");
	env->CallStaticVoidMethod(main, hookMeID);

	jvm->GetEnv((void **)&jvmti, JVMTI_VERSION_1_0);
	std::cout << "JVMTI: " << (void *)jvmti << std::endl;

	jvmtiCapabilities capabilities = { 0 };
	capabilities.can_generate_method_entry_events = 1;
	jvmti->AddCapabilities(&capabilities);
	jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_METHOD_ENTRY, NULL);
	
	callbacks.MethodEntry = MethodEntryCallback;
	std::cout << "SetEventCallbacks: " << jvmti->SetEventCallbacks(&callbacks, sizeof(callbacks)) << std::endl;

	env->CallStaticVoidMethod(main, hookMeID);
}
*/

JNIEXPORT jint JNICALL Agent_OnLoad(JavaVM *jvm, char *options, void *reserved)
{
	JNIEnv *env;
	jvmtiEnv *jvmti;

	std::cout << "Agent_OnLoad called" << std::endl;
	std::cout << "JVM: " << (void *)jvm << std::endl;

	jvm->GetEnv((void **)&env, JNI_VERSION);
	std::cout << "JNIEnv: " << (void *)env << std::endl;
	
	jvm->GetEnv((void **)&jvmti, JVMTI_VERSION_1_0);
	std::cout << "JVMTI: " << (void *)jvmti << std::endl;

	jvmtiCapabilities capabilities = { 0 };
	capabilities.can_generate_method_entry_events = 1;
	jvmti->AddCapabilities(&capabilities);
	jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_METHOD_ENTRY, NULL);

	jvmtiEventCallbacks callbacks = { 0 };
	callbacks.MethodEntry = MethodEntryCallback;
	std::cout << "SetEventCallbacks: " << jvmti->SetEventCallbacks(&callbacks, sizeof(callbacks)) << std::endl;

	return JNI_OK;
}

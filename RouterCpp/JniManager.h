#include "stdafx.h"
#include <msclr/marshal.h>
#include <jni.h>
#include <map>
#include <cassert>
#include "TypeConverter.h"

using namespace System;
using namespace System::Collections::Generic;

class JniManager
{
private:
	JavaVM* jvm;
	JNIEnv* env;
	jmethodID getAssemblyName;
	jmethodID getAssemblyPath;
	jmethodID getMethodName;
	jmethodID getLog;
	jmethodID getNotifyEvents;
	jmethodID getFullTrust;
	jmethodID getIsSingleton;
	jmethodID getMethodArguments;
	jmethodID getInvocationProperties;
	jmethodID getSessionProperties;
	jmethodID getOutboundProperties;
	jmethodID getInboundProperties;
	jmethodID responseCtor;
	jmethodID setPayloadMethod;
	jmethodID size;
	jmethodID keySet;
	jmethodID getMapValue;
	jmethodID toArray;
	jclass responseClazz;
	jclass mapClazz;
	jclass setClazz;
	TypeConverter* typeConverter;

public:

	JniManager(JavaVM*);
	void init();
	/*static JniManager& get();*/
	void setJVM(JavaVM*);
	JavaVM* getJVM();
	JNIEnv* getEnv();
	array<int>^ toIntArray(jintArray);
	RouterSDK::ProcessRequest^ toProcessRequest(jobject);
	jobject toResponseObject(String^);
	Dictionary<String^, Object^>^ toDictionary(jobject);
	void cleanup();
};

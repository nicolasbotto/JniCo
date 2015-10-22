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
	TypeConverter* typeConverter;
	JavaVM* jvm;
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
	jmethodID logMethod;
	jmethodID instrumentMethod;
	jmethodID setInvocationProperties;
	jmethodID setSessionProperties;
	jmethodID setOutboundProperties;
	jclass responseClazz;
	jobject routerInstance;
	jclass exceptionClazz;

public:

	JniManager(JavaVM*);
	void init();
	void setJVM(JavaVM*);
	JavaVM* getJVM();
	JNIEnv* getEnv();
	Org::Mule::Api::Routing::ProcessRequest^ toProcessRequest(jobject);
	jobject toResponseObject(Org::Mule::Api::Routing::ProcessRequest^);
	void cleanup();
	void log(String^);
	void instrument(String^, Int32^);
	void setRouter(jobject);
	void throwException(String^);
	void checkJniException();
};

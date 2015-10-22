#include "stdafx.h"
#include "JniManager.h"

//JniManager& JniManager::get()
//{
//	static JniManager jni;
//	return jni;
//}

JniManager::JniManager(JavaVM* vm)
{
	assert(vm);
	//env = nullptr;
	jvm = vm;
	init();
}

void JniManager::init()
{
	JNIEnv* env = getEnv();

	assert(env);

	typeConverter = new TypeConverter();
	typeConverter->init(env);

	// Exception class
	jclass exceptionClazzTemp = env->FindClass("java/lang/Exception");

	if (exceptionClazzTemp == NULL)
	{
		Console::WriteLine("Cannot find class java/lang/Exception");
		
		checkJniException();
	}

	exceptionClazz = (jclass)env->NewGlobalRef(exceptionClazzTemp);

	env->DeleteLocalRef(exceptionClazzTemp);

	// jni/Router
	jclass routerClazzTmp = env->FindClass("jni/Router");

	checkJniException();

	logMethod = env->GetMethodID(routerClazzTmp, "log", "(Ljava/lang/String;)V");

	checkJniException();

	env->DeleteLocalRef(routerClazzTmp);

	// jni/Response
	jclass responseClazzTmp = env->FindClass("jni/Response");

	checkJniException();

	responseClazz = (jclass)env->NewGlobalRef(responseClazzTmp);
	env->DeleteLocalRef(responseClazzTmp);

	responseCtor = env->GetMethodID(responseClazz, "<init>", "()V");

	checkJniException();

	setPayloadMethod = env->GetMethodID(responseClazz, "setPayload", "(Ljava/lang/String;)V");

	checkJniException();

	// jni/Request

	jclass processRequestClazz = env->FindClass("jni/Request");

	checkJniException();

	getAssemblyName = env->GetMethodID(processRequestClazz, "getAssemblyName", "()Ljava/lang/String;");

	checkJniException();

	getAssemblyPath = env->GetMethodID(processRequestClazz, "getAssemblyPath", "()Ljava/lang/String;");

	checkJniException();

	getMethodName = env->GetMethodID(processRequestClazz, "getMethodName", "()Ljava/lang/String;");

	checkJniException();

	getLog = env->GetMethodID(processRequestClazz, "getLog", "()Z");

	checkJniException();

	getNotifyEvents = env->GetMethodID(processRequestClazz, "getNotifyEvents", "()Z");

	checkJniException();

	getFullTrust = env->GetMethodID(processRequestClazz, "getFullTrust", "()Z");

	checkJniException();

	getIsSingleton = env->GetMethodID(processRequestClazz, "getIsSingleton", "()Z");

	checkJniException();

	getInboundProperties = env->GetMethodID(processRequestClazz, "getInboundProperties", "()Ljava/util/Map;");

	checkJniException();

	getMethodArguments = env->GetMethodID(processRequestClazz, "getMethodArguments", "()Ljava/util/Map;");

	checkJniException();

	getInvocationProperties = env->GetMethodID(processRequestClazz, "getInvocationProperties", "()Ljava/util/Map;");

	checkJniException();

	getSessionProperties = env->GetMethodID(processRequestClazz, "getSessionProperties", "()Ljava/util/Map;");

	checkJniException();

	getOutboundProperties = env->GetMethodID(processRequestClazz, "getOutboundProperties", "()Ljava/util/Map;");

	checkJniException();
}

void JniManager::setRouter(jobject obj)
{
	JNIEnv* env = getEnv();

	assert(env);

	routerInstance = env->NewGlobalRef(obj);
}

void JniManager::log(String^ message)
{
	JNIEnv* env = getEnv();

	assert(env);

	msclr::interop::marshal_context ctx;
	const char* c_message = ctx.marshal_as<const char*>(message);

	jstring j_message = env->NewStringUTF(c_message);

	env->CallVoidMethod(routerInstance, logMethod, j_message);

	env->DeleteLocalRef(j_message);
}

void JniManager::setJVM(JavaVM* java)
{
	assert(java);

	jvm = java;
	init();
}

JavaVM* JniManager::getJVM()
{
	return jvm;
}

JNIEnv* JniManager::getEnv()
{
	JNIEnv *env;
	assert(jvm);
	int envStatus = jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

	if (envStatus == JNI_OK)
	{
		jvm->AttachCurrentThread((void**)&env, NULL);
		return env;
	}

	return NULL;
}

void JniManager::cleanup()
{
	JNIEnv* env = getEnv();

	assert(env);

	try
	{
		env->DeleteGlobalRef(responseClazz);
		env->DeleteGlobalRef(routerInstance);

		typeConverter->cleanup(env);

		/*if (env->ExceptionCheck())
		{
			env->ExceptionOccurred();
		}*/
	}
	catch (Exception^ ex)
	{
		throwException(ex->Message);
	}
	
}

Org::Mule::Api::Routing::ProcessRequest^ JniManager::toProcessRequest(jobject obj)
{
	Org::Mule::Api::Routing::ProcessRequest^ request = gcnew Org::Mule::Api::Routing::ProcessRequest();

	JNIEnv* env = getEnv();

	request->AssemblyName = typeConverter->convertToC<String^>(env, env->CallObjectMethod(obj, getAssemblyName, "()Ljava/lang/String;"));
	request->AssemblyPath = typeConverter->convertToC<String^>(env, env->CallObjectMethod(obj, getAssemblyPath, "()Ljava/lang/String;"));
	request->MethodName = typeConverter->convertToC<String^>(env, env->CallObjectMethod(obj, getMethodName, "()Ljava/lang/String;"));
	request->FullTrust = env->CallBooleanMethod(obj, getFullTrust);
	request->IsSingleton = env->CallBooleanMethod(obj, getIsSingleton);
	request->Log = env->CallBooleanMethod(obj, getLog);
	request->NotifyEvents = env->CallBooleanMethod(obj, getNotifyEvents);
	request->MethodArguments = typeConverter->convertToC<Dictionary<String^, Object^>^>(env, env->CallObjectMethod(obj, getMethodArguments));
	request->InboundProperties = typeConverter->convertToC<Dictionary<String^, Object^>^>(env, env->CallObjectMethod(obj, getInboundProperties));
	request->InvocationProperties = typeConverter->convertToC<Dictionary<String^, Object^>^>(env, env->CallObjectMethod(obj, getInvocationProperties));
	request->OutboundProperties = typeConverter->convertToC<Dictionary<String^, Object^>^>(env, env->CallObjectMethod(obj, getOutboundProperties));
	request->SessionProperties = typeConverter->convertToC<Dictionary<String^, Object^>^>(env, env->CallObjectMethod(obj, getSessionProperties));

	return request;
}

jobject JniManager::toResponseObject(String^ payload)
{
	JNIEnv* env = getEnv();

	assert(env);

	msclr::interop::marshal_context ctx;
	const char* convertedPayload = ctx.marshal_as<const char*>(payload);

	jobject response = env->NewObject(responseClazz, responseCtor);
	
	jstring jPayload = env->NewStringUTF(convertedPayload);

	env->CallVoidMethod(response, setPayloadMethod, jPayload);

	env->DeleteLocalRef(jPayload);

	return response;
}

void JniManager::throwException(String^ message)
{
	JNIEnv* env = getEnv();
	assert(env);

	msclr::interop::marshal_context ctx;
	const char* convertedMessage = ctx.marshal_as<const char*>(message);

	env->ThrowNew(exceptionClazz, convertedMessage);
}

void JniManager::checkJniException()
{
	JNIEnv* env = getEnv();
	assert(env);

	jthrowable error = env->ExceptionOccurred();

	if (error)
	{
		env->ExceptionDescribe();
		env->ExceptionClear();
	}
}
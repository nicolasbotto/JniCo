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

	// jni/Router
	jclass routerClazzTmp = env->FindClass("jni/Router");

	if (routerClazzTmp == NULL)
	{
		Console::WriteLine("Cannot find class jni/Router");
	}

	logMethod = env->GetMethodID(routerClazzTmp, "log", "(Ljava/lang/String;)V");

	if (logMethod == NULL)
	{
		Console::WriteLine("Cannot find class jni/Router method log");
	}

	env->DeleteLocalRef(routerClazzTmp);

	// jni/Response
	jclass responseClazzTmp = env->FindClass("jni/Response");

	if (responseClazzTmp == NULL)
	{
		Console::WriteLine("Cannot find class jni/Response");
	}

	responseClazz = (jclass)env->NewGlobalRef(responseClazzTmp);
	env->DeleteLocalRef(responseClazzTmp);

	responseCtor = env->GetMethodID(responseClazz, "<init>", "()V");

	if (responseCtor == NULL)
	{
		Console::WriteLine("Cannot find class jni/Response ctor");
	}

	setPayloadMethod = env->GetMethodID(responseClazz, "setPayload", "(Ljava/lang/String;)V");

	if (setPayloadMethod == NULL)
	{
		Console::WriteLine("Cannot find class jni/Response setPayload method");
	}

	// jni/Request

	jclass processRequestClazz = env->FindClass("jni/Request");

	if (processRequestClazz == NULL)
	{
		Console::WriteLine("Cannot find class jni/Request");
	}

	getAssemblyName = env->GetMethodID(processRequestClazz, "getAssemblyName", "()Ljava/lang/String;");

	if (getAssemblyName == NULL)
	{
		Console::WriteLine("Cannot find class jni/Request getAssemblyName method");
	}

	getAssemblyPath = env->GetMethodID(processRequestClazz, "getAssemblyPath", "()Ljava/lang/String;");

	if (getAssemblyPath == NULL)
	{
		Console::WriteLine("Cannot find class jni/Request getAssemblyPath method");
	}

	getMethodName = env->GetMethodID(processRequestClazz, "getMethodName", "()Ljava/lang/String;");

	if (getMethodName == NULL)
	{
		Console::WriteLine("Cannot find class jni/Request getMethodName method");
	}

	getLog = env->GetMethodID(processRequestClazz, "getLog", "()Z");

	if (getLog == NULL)
	{
		Console::WriteLine("Cannot find class jni/Request getLog method");
	}

	getNotifyEvents = env->GetMethodID(processRequestClazz, "getNotifyEvents", "()Z");

	if (getNotifyEvents == NULL)
	{
		Console::WriteLine("Cannot find class jni/Request getNotifyEvents method");
	}

	getFullTrust = env->GetMethodID(processRequestClazz, "getFullTrust", "()Z");

	if (getFullTrust == NULL)
	{
		Console::WriteLine("Cannot find class jni/Request getFullTrust method");
	}

	getIsSingleton = env->GetMethodID(processRequestClazz, "getIsSingleton", "()Z");

	if (getIsSingleton == NULL)
	{
		Console::WriteLine("Cannot find class jni/Request getIsSingleton method");
	}

	getInboundProperties = env->GetMethodID(processRequestClazz, "getInboundProperties", "()Ljava/util/Map;");

	if (getInboundProperties == NULL)
	{
		Console::WriteLine("Cannot find class jni/Request getInboundProperties method");
	}

	getMethodArguments = env->GetMethodID(processRequestClazz, "getMethodArguments", "()Ljava/util/Map;");

	if (getMethodArguments == NULL)
	{
		Console::WriteLine("Cannot find class jni/Request getMethodArguments method");
	}

	getInvocationProperties = env->GetMethodID(processRequestClazz, "getInvocationProperties", "()Ljava/util/Map;");

	if (getInvocationProperties == NULL)
	{
		Console::WriteLine("Cannot find class jni/Request getInvocationProperties method");
	}

	getSessionProperties = env->GetMethodID(processRequestClazz, "getSessionProperties", "()Ljava/util/Map;");

	if (getSessionProperties == NULL)
	{
		Console::WriteLine("Cannot find class jni/Request getSessionProperties method");
	}

	getOutboundProperties = env->GetMethodID(processRequestClazz, "getOutboundProperties", "()Ljava/util/Map;");

	if (getOutboundProperties == NULL)
	{
		Console::WriteLine("Cannot find class jni/Request getOutboundProperties method");
	}
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
	String^ name = Threading::Thread::CurrentThread->Name;
	JNIEnv *env;
	/*if (env == nullptr)
	{*/
		assert(jvm);
		int envStatus = jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
		if (envStatus == JNI_OK)
		{
			int attached = jvm->AttachCurrentThread((void**)&env, NULL);

			return env;
		}

		/*if (envStatus == JNI_EDETACHED) 
		{
			jvm->AttachCurrentThread((void**)env, NULL);
			return env;
		}*/
	//}
	/*assert(env);
	return env;*/

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
		Console::WriteLine(ex->Message);
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

	msclr::interop::marshal_context ctx;
	const char* convertedPayload = ctx.marshal_as<const char*>(payload);

	jobject response = env->NewObject(responseClazz, responseCtor);
	
	jstring jPayload = env->NewStringUTF(convertedPayload);

	env->CallVoidMethod(response, setPayloadMethod, jPayload);

	env->DeleteLocalRef(jPayload);

	return response;
}

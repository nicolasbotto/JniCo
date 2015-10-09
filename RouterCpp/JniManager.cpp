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
	env = nullptr;
	jvm = vm;
	init();
}

void JniManager::init()
{
	JNIEnv* env = getEnv();

	typeConverter = new TypeConverter(env);

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

	getLog = env->GetMethodID(processRequestClazz, "getLog", "()Ljava/lang/Boolean;");

	if (getLog == NULL)
	{
		Console::WriteLine("Cannot find class jni/Request getLog method");
	}

	getNotifyEvents = env->GetMethodID(processRequestClazz, "getNotifyEvents", "()Ljava/lang/Boolean;");

	if (getNotifyEvents == NULL)
	{
		Console::WriteLine("Cannot find class jni/Request getNotifyEvents method");
	}

	getFullTrust = env->GetMethodID(processRequestClazz, "getFullTrust", "()Ljava/lang/Boolean;");

	if (getFullTrust == NULL)
	{
		Console::WriteLine("Cannot find class jni/Request getFullTrust method");
	}

	getIsSingleton = env->GetMethodID(processRequestClazz, "getIsSingleton", "()Ljava/lang/Boolean;");

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

void JniManager::setJVM(JavaVM* java)
{
	jvm = java;
	init();
}

JavaVM* JniManager::getJVM()
{
	return jvm;
}

JNIEnv* JniManager::getEnv()
{
	if (env == nullptr)
	{
		assert(jvm);
		if (jvm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK)
		{
			return nullptr;
		}

		//jvm->AttachCurrentThread((void**)env, nullptr);
	}
	assert(env);
	return env;
}

void JniManager::cleanup()
{
	if (!env)
	{
		env = getEnv();
	}

	try
	{
		env->DeleteGlobalRef(responseClazz);
		
		typeConverter->cleanup();

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

	request->AssemblyName = typeConverter->convertToC<String^>(env->CallObjectMethod(obj, getAssemblyName, "()Ljava/lang/String;"));
	request->AssemblyPath = typeConverter->convertToC<String^>(env->CallObjectMethod(obj, getAssemblyPath, "()Ljava/lang/String;"));
	request->MethodName = typeConverter->convertToC<String^>(env->CallObjectMethod(obj, getMethodName, "()Ljava/lang/String;"));
	request->FullTrust = (jboolean)env->CallBooleanMethod(obj, getFullTrust);
	request->IsSingleton = (jboolean)env->CallBooleanMethod(obj, getIsSingleton);
	request->Log = (jboolean)env->CallBooleanMethod(obj, getLog);
	request->NotifyEvents = (jboolean)env->CallBooleanMethod(obj, getNotifyEvents);
	request->MethodArguments = typeConverter->convertToC<Dictionary<String^, Object^>^>(env->CallObjectMethod(obj, getMethodArguments));
	request->InboundProperties = typeConverter->convertToC<Dictionary<String^, Object^>^>(env->CallObjectMethod(obj, getInboundProperties));
	request->InvocationProperties = typeConverter->convertToC<Dictionary<String^, Object^>^>(env->CallObjectMethod(obj, getInvocationProperties));
	request->OutboundProperties = typeConverter->convertToC<Dictionary<String^, Object^>^>(env->CallObjectMethod(obj, getOutboundProperties));
	request->SessionProperties = typeConverter->convertToC<Dictionary<String^, Object^>^>(env->CallObjectMethod(obj, getSessionProperties));

	return request;
}

array<int>^ JniManager::toIntArray(jintArray input)
{
	const int intsSize = env->GetArrayLength(input);

	array<int>^ intArray = gcnew array<int>(intsSize);

	jint *body = env->GetIntArrayElements(input, false);

	env->ReleaseIntArrayElements(input, body, 0);

	for (int i = 0; i < intsSize; i++)
	{
		intArray[i] = body[i];
	}

	return intArray;
}

jobject JniManager::toResponseObject(String^ payload)
{
	msclr::interop::marshal_context ctx;
	const char* convertedPayload = ctx.marshal_as<const char*>(payload);

	jobject response = env->NewObject(responseClazz, responseCtor);
	
	jstring jPayload = env->NewStringUTF(convertedPayload);

	env->CallVoidMethod(response, setPayloadMethod, jPayload);

	env->DeleteLocalRef(jPayload);

	return response;
}

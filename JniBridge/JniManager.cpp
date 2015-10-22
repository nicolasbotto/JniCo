#include "stdafx.h"
#include "JniManager.h"

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

	// org/mule/api/jniRouter
	jclass routerClazzTmp = env->FindClass("org/mule/api/jni/Bridge");

	checkJniException();

	logMethod = env->GetMethodID(routerClazzTmp, "log", "(Ljava/lang/String;)V");

	checkJniException();

	instrumentMethod = env->GetMethodID(routerClazzTmp, "instrument", "(Ljava/lang/String;I)V");

	checkJniException();

	env->DeleteLocalRef(routerClazzTmp);

	// org/mule/api/jni/Response
	jclass responseClazzTmp = env->FindClass("org/mule/api/jni/Response");

	checkJniException();

	responseClazz = (jclass)env->NewGlobalRef(responseClazzTmp);
	env->DeleteLocalRef(responseClazzTmp);

	responseCtor = env->GetMethodID(responseClazz, "<init>", "()V");

	checkJniException();

	setPayloadMethod = env->GetMethodID(responseClazz, "setPayload", "(Ljava/lang/Object;)V");

	checkJniException();

	setInvocationProperties = env->GetMethodID(responseClazz, "setInvocationProperties", "(Ljava/util/Map;)V");

	checkJniException();

	setSessionProperties = env->GetMethodID(responseClazz, "setSessionProperties", "(Ljava/util/Map;)V");

	checkJniException();

	setOutboundProperties = env->GetMethodID(responseClazz, "setOutboundProperties", "(Ljava/util/Map;)V");

	checkJniException();

	// org/mule/api/jni/Request

	jclass processRequestClazz = env->FindClass("org/mule/api/jni/Request");

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

void JniManager::instrument(String^ message, Int32^ id)
{
	JNIEnv* env = getEnv();

	assert(env);

	msclr::interop::marshal_context ctx;
	const char* c_message = ctx.marshal_as<const char*>(message);

	jstring jMessage = env->NewStringUTF(c_message);
	jint jId = (int)id;

	env->CallVoidMethod(routerInstance, instrumentMethod, jMessage, jId);

	env->DeleteLocalRef(jMessage);
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

		checkJniException();
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

jobject JniManager::toResponseObject(Org::Mule::Api::Routing::ProcessRequest^ request)
{
	JNIEnv* env = getEnv();

	assert(env);

	jobject response = env->NewObject(responseClazz, responseCtor);
	Object^ payload = request->Result;
	jobject jPayload;

	if (request->Result != nullptr)
	{
		// If it's a string or byte[]
		if (payload->GetType()->ToString() == "System.String")
		{
			msclr::interop::marshal_context ctx;
			const char* convertedPayload = ctx.marshal_as<const char*>((String^)payload);
			jPayload = env->NewStringUTF(convertedPayload);
		}
		else
		{
			jPayload = typeConverter->convertToJavaArray<jbyteArray>(env, payload);
		}

		env->CallVoidMethod(response, setPayloadMethod, jPayload);
	}

	// set Mule Message properties
	jobject jInvocationProperties = typeConverter->convertToJavaMap(env, request->InvocationProperties);
	jobject jSessionProperties = typeConverter->convertToJavaMap(env, request->SessionProperties);
	jobject jOutboundProperties = typeConverter->convertToJavaMap(env, request->OutboundProperties);
	
	env->CallVoidMethod(response, setInvocationProperties, jInvocationProperties);
	env->CallVoidMethod(response, setOutboundProperties, jOutboundProperties);
	env->CallVoidMethod(response, setSessionProperties, jSessionProperties);

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
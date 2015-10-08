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

	// java/util/Map
	jclass mapClazzTmp = env->FindClass("Ljava/util/Map;");

	if (mapClazzTmp == NULL)
	{
		throw gcnew Exception("Cannot find class java/util/Map");
	}

	mapClazz = (jclass)env->NewGlobalRef(mapClazzTmp);
	env->DeleteLocalRef(mapClazzTmp);

	size = env->GetMethodID(mapClazz, "size", "()I");

	if (size == NULL)
	{
		throw gcnew Exception("Cannot find class java/util/Map size method");
	}

	getMapValue = env->GetMethodID(mapClazz, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");

	if (getMapValue == NULL)
	{
		throw gcnew Exception("Cannot find class java/util/Map get method");
	}

	keySet = env->GetMethodID(mapClazz, "keySet", "()Ljava/util/Set;");

	if (keySet == NULL)
	{
		throw gcnew Exception("Cannot find class java/util/Map keySet method");
	}

	// java/util/Set
	jclass setClazzTmp = env->FindClass("Ljava/util/Set;");

	if (setClazzTmp == NULL)
	{
		throw gcnew Exception("Cannot find class java/util/Set");
	}

	setClazz = (jclass)env->NewGlobalRef(setClazzTmp);
	env->DeleteLocalRef(setClazzTmp);

	toArray = env->GetMethodID(setClazz, "toArray", "()[Ljava/lang/Object;");

	/*if (env->ExceptionCheck())
	{*/
		//env->Throw(env->ExceptionOccurred());
	//}

	// jni/Response
	jclass responseClazzTmp = env->FindClass("jni/Response");

	if (responseClazzTmp == NULL)
	{
		throw gcnew Exception("Cannot find class jni/Response");
	}

	responseClazz = (jclass)env->NewGlobalRef(responseClazzTmp);
	env->DeleteLocalRef(responseClazzTmp);

	responseCtor = env->GetMethodID(responseClazz, "<init>", "()V");

	if (responseCtor == NULL)
	{
		throw gcnew Exception("Cannot find class jni/Response ctor");
	}

	setPayloadMethod = env->GetMethodID(responseClazz, "setPayload", "(Ljava/lang/String;)V");

	if (setPayloadMethod == NULL)
	{
		throw gcnew Exception("Cannot find class jni/Response setPayload method");
	}

	// jni/Request

	jclass processRequestClazz = env->FindClass("jni/Request");

	if (processRequestClazz == NULL)
	{
		throw gcnew Exception("Cannot find class jni/Request");
	}

	getAssemblyName = env->GetMethodID(processRequestClazz, "getAssemblyName", "()Ljava/lang/String;");

	if (getAssemblyName == NULL)
	{
		throw gcnew Exception("Cannot find class jni/Request getAssemblyName method");
	}

	getAssemblyPath = env->GetMethodID(processRequestClazz, "getAssemblyPath", "()Ljava/lang/String;");

	if (getAssemblyPath == NULL)
	{
		throw gcnew Exception("Cannot find class jni/Request getAssemblyPath method");
	}

	getMethodName = env->GetMethodID(processRequestClazz, "getMethodName", "()Ljava/lang/String;");

	if (getMethodName == NULL)
	{
		throw gcnew Exception("Cannot find class jni/Request getMethodName method");
	}

	getLog = env->GetMethodID(processRequestClazz, "getLog", "()Ljava/lang/Boolean;");

	if (getLog == NULL)
	{
		throw gcnew Exception("Cannot find class jni/Request getLog method");
	}

	getNotifyEvents = env->GetMethodID(processRequestClazz, "getNotifyEvents", "()Ljava/lang/Boolean;");

	if (getNotifyEvents == NULL)
	{
		throw gcnew Exception("Cannot find class jni/Request getNotifyEvents method");
	}

	getFullTrust = env->GetMethodID(processRequestClazz, "getFullTrust", "()Ljava/lang/Boolean;");

	if (getFullTrust == NULL)
	{
		throw gcnew Exception("Cannot find class jni/Request getFullTrust method");
	}

	getIsSingleton = env->GetMethodID(processRequestClazz, "getIsSingleton", "()Ljava/lang/Boolean;");

	if (getIsSingleton == NULL)
	{
		throw gcnew Exception("Cannot find class jni/Request getIsSingleton method");
	}

	getInboundProperties = env->GetMethodID(processRequestClazz, "getInboundProperties", "()Ljava/util/Map;");

	if (getInboundProperties == NULL)
	{
		throw gcnew Exception("Cannot find class jni/Request getInboundProperties method");
	}

	getMethodArguments = env->GetMethodID(processRequestClazz, "getMethodArguments", "()Ljava/util/Map;");

	if (getMethodArguments == NULL)
	{
		throw gcnew Exception("Cannot find class jni/Request getMethodArguments method");
	}

	getInvocationProperties = env->GetMethodID(processRequestClazz, "getInvocationProperties", "()Ljava/util/Map;");

	if (getInvocationProperties == NULL)
	{
		throw gcnew Exception("Cannot find class jni/Request getInvocationProperties method");
	}

	getSessionProperties = env->GetMethodID(processRequestClazz, "getSessionProperties", "()Ljava/util/Map;");

	if (getSessionProperties == NULL)
	{
		throw gcnew Exception("Cannot find class jni/Request getSessionProperties method");
	}

	getOutboundProperties = env->GetMethodID(processRequestClazz, "getOutboundProperties", "()Ljava/util/Map;");

	if (getOutboundProperties == NULL)
	{
		throw gcnew Exception("Cannot find class jni/Request getOutboundProperties method");
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
		env->DeleteGlobalRef(mapClazz);
		env->DeleteGlobalRef(setClazz);

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

// Helpers
//const char * JniManager::toCString(jstring input)
//{
//	if (input == NULL)
//	{
//		return nullptr;
//	}
//
//	const char * c_input = env->GetStringUTFChars(input, false);
//	env->ReleaseStringUTFChars(input, c_input);
//
//	return c_input;
//}

RouterSDK::ProcessRequest^ JniManager::toProcessRequest(jobject obj)
{
	RouterSDK::ProcessRequest^ request = gcnew RouterSDK::ProcessRequest();

	request->AssemblyName = typeConverter->convertToC<String^>(env->CallObjectMethod(obj, getAssemblyName, "()Ljava/lang/String;"));
	request->AssemblyPath = typeConverter->convertToC<String^>(env->CallObjectMethod(obj, getAssemblyPath, "()Ljava/lang/String;"));
	request->MethodName = typeConverter->convertToC<String^>(env->CallObjectMethod(obj, getMethodName, "()Ljava/lang/String;"));
	request->FullTrust = (jboolean)env->CallBooleanMethod(obj, getFullTrust);
	request->IsSingleton = (jboolean)env->CallBooleanMethod(obj, getIsSingleton);
	request->Log = (jboolean)env->CallBooleanMethod(obj, getLog);
	request->NotifyEvents = (jboolean)env->CallBooleanMethod(obj, getNotifyEvents);
	request->MethodArguments = toDictionary(env->CallObjectMethod(obj, getMethodArguments));

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

Dictionary<String^, Object^>^ JniManager::toDictionary(jobject map)
{
	Dictionary<String^, Object^>^ result = gcnew Dictionary<String^, Object^>();
	
	if (map == NULL)
	{
		return result;
	}
	
	int mapSize = env->CallIntMethod(map, size);

	jobject keys = env->CallObjectMethod(map, keySet);

	jobjectArray arrayOfKeys = (jobjectArray)env->CallObjectMethod(keys, toArray);

	for (int i = 0; i < mapSize; i++)
	{
		jstring keyName = (jstring)env->GetObjectArrayElement(arrayOfKeys, i);
		jobject mapValue = env->CallObjectMethod(map, getMapValue, keyName);

		result->Add(typeConverter->convertToC<String^>(keyName), typeConverter->toManagedObject(mapValue));
	}

	return result;
}


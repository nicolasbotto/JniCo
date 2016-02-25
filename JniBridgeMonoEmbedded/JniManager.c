#include "JniManager.h"
#include <iostream>

JniManager::JniManager(JavaVM* vm)
{
	assert(vm);
	jvm = vm;
	init();
}

MonoDomain* JniManager::getMonoDomain()
{
    MonoDomain* monoDomain = mono_get_root_domain();
    mono_thread_attach(monoDomain);
    return monoDomain;
}

void JniManager::setMono(MonoImage* monoImage)
{
    processRequestClass = mono_class_from_name(monoImage, "Org.Mule.Api.Routing", "ProcessRequest");
    setAssemblyNameField = mono_class_get_method_from_name(processRequestClass, "set_AssemblyName", 1);
    setMethodNameField = mono_class_get_method_from_name(processRequestClass, "set_MethodName", 1);
    setAssemblyPathField = mono_class_get_method_from_name(processRequestClass, "set_AssemblyPath", 1);
    setFullTrustField = mono_class_get_method_from_name(processRequestClass, "set_FullTrust", 1);
    setIsSingletonField = mono_class_get_method_from_name(processRequestClass, "set_IsSingleton", 1);
    setLogField = mono_class_get_method_from_name(processRequestClass, "set_Log", 1);
    getResult = mono_class_get_method_from_name(processRequestClass, "get_Result", 0);
    setNotifyEventsField = mono_class_get_method_from_name(processRequestClass, "set_NotifyEvents", 1);
    addMethodArgumentsProperty = mono_class_get_method_from_name(processRequestClass, "AddMethodArgumentProperty", 2);
    addInboundProperty = mono_class_get_method_from_name(processRequestClass, "AddInboundProperty", 2);
    addInvocationProperty = mono_class_get_method_from_name(processRequestClass, "AddInvocationProperty", 2);
    addSessionProperty = mono_class_get_method_from_name(processRequestClass, "AddSessionProperty", 2);
    addOutboundProperty = mono_class_get_method_from_name(processRequestClass, "AddOutboundProperty", 2);
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
            checkJniException();
    }

    exceptionClazz = (jclass)env->NewGlobalRef(exceptionClazzTemp);

    env->DeleteLocalRef(exceptionClazzTemp);
    /*
    // org/mule/api/jniRouter
    jclass routerClazzTmp = env->FindClass("org/mule/api/jni/Bridge");

    checkJniException();

    logMethod = env->GetMethodID(routerClazzTmp, "log", "(Ljava/lang/String;)V");

    checkJniException();

    instrumentMethod = env->GetMethodID(routerClazzTmp, "instrument", "(Ljava/lang/String;I)V");

    checkJniException();

    env->DeleteLocalRef(routerClazzTmp);
    */
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


    env->DeleteGlobalRef(responseClazz);
    env->DeleteGlobalRef(routerInstance);

    //typeConverter->cleanup(env);

    checkJniException();
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

void JniManager::throwException(const char* message)
{
    JNIEnv* env = getEnv();
    assert(env);
    env->ThrowNew(exceptionClazz, message);
}

void JniManager::toProcessRequest(jobject obj, MonoObject* processRequest)
{
    JNIEnv* env = getEnv();

    MonoDomain* monoDomain = getMonoDomain();

    string c_assemblyName = typeConverter->convertToC<string>(env, env->CallObjectMethod(obj, getAssemblyName, "()Ljava/lang/String;"));
    string c_assemblyPath = typeConverter->convertToC<string>(env, env->CallObjectMethod(obj, getAssemblyPath, "()Ljava/lang/String;"));
    string c_methodName = typeConverter->convertToC<string>(env, env->CallObjectMethod(obj, getMethodName, "()Ljava/lang/String;"));

    MonoString* assemblyName = mono_string_new(monoDomain, c_assemblyName.c_str());
    MonoString* assemblyPath = mono_string_new(monoDomain, c_assemblyPath.c_str());
    MonoString* methodName = mono_string_new(monoDomain, c_methodName.c_str());
    bool fullTrust = env->CallBooleanMethod(obj, getFullTrust);
    bool isSingleton = env->CallBooleanMethod(obj, getIsSingleton);
    bool log = env->CallBooleanMethod(obj, getLog);
    bool notifyEvents = env->CallBooleanMethod(obj, getNotifyEvents);

    MonoObject* exception = NULL;

    void* args[1];
    args[0] = assemblyName;
    mono_runtime_invoke(setAssemblyNameField, processRequest, args, &exception);

    args[0] = assemblyPath;
    mono_runtime_invoke(setAssemblyPathField, processRequest, args, &exception);

    args[0] = methodName;
    mono_runtime_invoke(setMethodNameField, processRequest, args, &exception);

    args[0] = &fullTrust;
    mono_runtime_invoke(setFullTrustField, processRequest, args, &exception);

    args[0] = &log;
    mono_runtime_invoke(setLogField, processRequest, args, &exception);

    args[0] = &isSingleton;
    mono_runtime_invoke(setIsSingletonField, processRequest, args, &exception);

    args[0] = &notifyEvents;
    mono_runtime_invoke(setNotifyEventsField, processRequest, args, &exception);

    if (exception)
    {
        const char* message = mono_string_to_utf8(mono_object_to_string(exception, NULL));
        throwException(message);
    }

    jobject javaMethodArguments = env->CallObjectMethod(obj, getMethodArguments);

    setProperties(env, javaMethodArguments, addMethodArgumentsProperty, processRequest);
    //request->MethodArguments = typeConverter->convertToC<Dictionary<String^, Object^>^>(env, env->CallObjectMethod(obj, getMethodArguments));
    //request->InboundProperties = typeConverter->convertToC<Dictionary<String^, Object^>^>(env, env->CallObjectMethod(obj, getInboundProperties));
    //request->InvocationProperties = typeConverter->convertToC<Dictionary<String^, Object^>^>(env, env->CallObjectMethod(obj, getInvocationProperties));
    //request->OutboundProperties = typeConverter->convertToC<Dictionary<String^, Object^>^>(env, env->CallObjectMethod(obj, getOutboundProperties));
    //request->SessionProperties = typeConverter->convertToC<Dictionary<String^, Object^>^>(env, env->CallObjectMethod(obj, getSessionProperties));
}

void JniManager::setProperties(JNIEnv* env, jobject map, MonoMethod* method, MonoObject* instance)
{
    assert(env);

    MonoDomain* monoDomain = getMonoDomain();

    if (map == NULL)
    {
        return;
    }

    int mapSize = env->CallIntMethod(map, typeConverter->size);

    jobject keys = env->CallObjectMethod(map, typeConverter->keySet);

    jobjectArray arrayOfKeys = (jobjectArray)env->CallObjectMethod(keys, typeConverter->toArray);

    for (int i = 0; i < mapSize; i++)
    {
        jstring javaKeyName = (jstring)env->GetObjectArrayElement(arrayOfKeys, i);
        string keyName = typeConverter->convertToC<string>(env, javaKeyName);
        jobject mapValue = env->CallObjectMethod(map, typeConverter->getMapValue, javaKeyName);

        //if (env->IsInstanceOf(mapValue, typeConverter->mapClazz))
        //{

        //}

        MonoObject* exc = NULL;
        void* args[2];
        args[0] = mono_string_new(monoDomain, keyName.c_str());
        args[1] = toMonoObject(env, mapValue);

        mono_runtime_invoke(method, instance, args, &exc);

        if (exc)
        {
            const char* message = mono_string_to_utf8(mono_object_to_string(exc, NULL));
            throwException(message);
            return;
        }


        env->DeleteLocalRef(javaKeyName);
        env->DeleteLocalRef(mapValue);
    }

    env->DeleteLocalRef(keys);
    env->DeleteLocalRef(arrayOfKeys);
    env->DeleteLocalRef(map);
}

jobject JniManager::toResponse(MonoObject* monoObject)
{
    assert(monoObject);

    JNIEnv* env = getEnv();

    assert(env);

    jobject response = env->NewObject(responseClazz, responseCtor);

    MonoObject* exc = NULL;

    MonoObject* result = mono_runtime_invoke(getResult, monoObject, NULL, &exc);

    if (exc)
    {
        const char* message = mono_string_to_utf8(mono_object_to_string(exc, NULL));
        throwException(message);
        return NULL;
    }

    jobject jPayload = NULL;

    if (result != NULL)
    {
        MonoClass* resultClass = mono_object_get_class(result);

        MonoType* monoType = mono_class_get_type(resultClass);

        string typeName = string(mono_type_get_name(monoType));

        // If it's a string or byte[]
        if (typeName == "System.String")
        {
            const char* convertedPayload = mono_string_to_utf8(mono_object_to_string(result, NULL));
            jPayload = env->NewStringUTF(convertedPayload);
        }
        else
        {
            jPayload = typeConverter->convertToJavaArray<jbyteArray>(env, result);
        }

        env->CallVoidMethod(response, setPayloadMethod, jPayload);
        env->DeleteLocalRef(jPayload);
    }

    // set Mule Message properties
    //jobject jInvocationProperties = typeConverter->convertToJavaMap(env, request->InvocationProperties);
    //jobject jSessionProperties = typeConverter->convertToJavaMap(env, request->SessionProperties);
    //jobject jOutboundProperties = typeConverter->convertToJavaMap(env, request->OutboundProperties);

    /*env->CallVoidMethod(response, setInvocationProperties, jInvocationProperties);
    env->CallVoidMethod(response, setOutboundProperties, jOutboundProperties);
    env->CallVoidMethod(response, setSessionProperties, jSessionProperties);

    env->DeleteLocalRef(jInvocationProperties);
    env->DeleteLocalRef(jOutboundProperties);
    env->DeleteLocalRef(jSessionProperties);*/

    return response;
}

MonoObject* JniManager::toMonoObject(JNIEnv* env, jobject obj)
{
    assert(env);

    MonoDomain* monoDomain = getMonoDomain();

    MonoObject* result = NULL;

    jclass clazz = env->GetObjectClass(obj);
    jstring clazzName = (jstring)env->CallObjectMethod(clazz, typeConverter->getClassName);

    string className = string(typeConverter->convertToC<string>(env, clazzName));

    if (className == "java.lang.Integer")
    {
        int value = typeConverter->convertToC<int>(env, obj);
        result = mono_value_box(monoDomain, mono_get_int32_class(), &value);
    }

    if (className == "java.lang.String")
    {
        string value = typeConverter->convertToC<string>(env, obj);
        result = (MonoObject*)mono_string_new(monoDomain, value.c_str());
    }

    if (className == "java.lang.Boolean")
    {
        bool value = typeConverter->convertToC<bool>(env, obj);
        result = (MonoObject*)mono_value_box(monoDomain, mono_get_boolean_class(), &value);
    }

    if (className == "java.lang.Character")
    {
        char value = typeConverter->convertToC<char>(env, obj);
        result = (MonoObject*)mono_value_box(monoDomain, mono_get_char_class(), &value);
    }

    if (className == "java.lang.Long")
    {
        long value = typeConverter->convertToC<long>(env, obj);
        result = (MonoObject*)mono_value_box(monoDomain, mono_get_int64_class(), &value);
    }

    if (className == "java.lang.Short")
    {
        short value = typeConverter->convertToC<short>(env, obj);
        result = (MonoObject*)mono_value_box(monoDomain, mono_get_int16_class(), &value);
    }

    if (className =="java.lang.Byte")
    {
        byte value = typeConverter->convertToC<byte>(env, obj);
        result = (MonoObject*)mono_value_box(monoDomain, mono_get_byte_class(), &value);
    }

    if (className == "java.lang.Double" )
    {
        double value = typeConverter->convertToC<double>(env, obj);
        result = (MonoObject*)mono_value_box(monoDomain, mono_get_double_class(), &value);
    }
    
    if (className == "java.lang.Float" )
    {
        float value = typeConverter->convertToC<float>(env, obj);
        result = (MonoObject*)mono_value_box(monoDomain, mono_get_single_class(), &value);
    }
    
    if (className == "[B")
    {
        vector<byte> value = typeConverter->convertToC< vector<byte> >(env, obj);
        int vectorSize = value.size();
        MonoArray *data = mono_array_new (monoDomain, mono_get_byte_class (), vectorSize);
    
        for (int i=0; i<vectorSize; i++)
        {
            mono_array_set(data, uint8_t, i, value[i]);
        }
        
        result = (MonoObject*)data;
    }

    env->DeleteLocalRef(obj);
    return result;
}

//template<typename T>
//MonoObject* toMonoObject(T type)
//{
//	string name = string(typeid(type).name());
//	MonoClass* clazz = NULL;
//
//	if (name == "unsigned char"){
//		clazz = mono_get_byte_class();
//	}
//
//	if (name == "float"){
//		clazz = mono_get_single_class();
//	}
//
//	if (name == "short"){
//		clazz = mono_get_int16_class();
//	}
//
//	if (name == "sbyte"){
//		clazz = mono_get_sbyte_class();
//	}
//
//	if (name == "char"){
//		clazz = mono_get_char_class();
//	}
//
//	if (name == "int"){
//		clazz = mono_get_int32_class();
//	}
//
//	if (name == "__int64"){
//		clazz = mono_get_int64_class();
//	}
//
//	if (name == "double"){
//		clazz = mono_get_double_class();
//	}
//
//	if (name == "bool"){
//		clazz = mono_get_boolean_class();
//	}
//
//	if (clazz)
//	{
//		return mono_value_box(monoDomain, clazz, &type);
//	}
//	else
//	{
//		return NULL;
//	}
//}


#include "TypeConverter.h"

using namespace std;

void TypeConverter::init(JNIEnv* env)
{
    assert(env);

    jclass intClazzTemp = env->FindClass("java/lang/Integer");
    intValue = env->GetMethodID(intClazzTemp, "intValue", "()I");
    intCtor = env->GetMethodID(intClazzTemp, "<init>", "(I)V");

    intClazz = (jclass)env->NewGlobalRef(intClazzTemp);
    env->DeleteLocalRef(intClazzTemp);

    jclass doubleClazzTemp = env->FindClass("java/lang/Double");
    doubleValue = env->GetMethodID(doubleClazzTemp, "doubleValue", "()D");
    doubleCtor = env->GetMethodID(doubleClazzTemp, "<init>", "(D)V");

    doubleClazz = (jclass)env->NewGlobalRef(doubleClazzTemp);
    env->DeleteLocalRef(doubleClazzTemp);

    jclass charClazzTemp = env->FindClass("java/lang/Character");
    charValue = env->GetMethodID(charClazzTemp, "charValue", "()C");
    charCtor = env->GetMethodID(charClazzTemp, "<init>", "(C)V");

    characterClazz = (jclass)env->NewGlobalRef(charClazzTemp);
    env->DeleteLocalRef(charClazzTemp);

    jclass longClazzTemp = env->FindClass("java/lang/Long");
    longValue = env->GetMethodID(longClazzTemp, "longValue", "()J");
    longCtor = env->GetMethodID(longClazzTemp, "<init>", "(J)V");

    longClazz = (jclass)env->NewGlobalRef(longClazzTemp);
    env->DeleteLocalRef(longClazzTemp);

    jclass booleanClazzTemp = env->FindClass("java/lang/Boolean");
    booleanValue = env->GetMethodID(booleanClazzTemp, "booleanValue", "()Z");
    booleanCtor = env->GetMethodID(booleanClazzTemp, "<init>", "(Z)V");

    booleanClazz = (jclass)env->NewGlobalRef(booleanClazzTemp);
    env->DeleteLocalRef(booleanClazzTemp);

    jclass byteClazzTemp = env->FindClass("java/lang/Byte");
    byteValue = env->GetMethodID(byteClazzTemp, "byteValue", "()B");
    byteCtor = env->GetMethodID(byteClazzTemp, "<init>", "(B)V");

    byteClazz = (jclass)env->NewGlobalRef(byteClazzTemp);
    env->DeleteLocalRef(byteClazzTemp);

    jclass shortClazzTemp = env->FindClass("java/lang/Short");
    shortValue = env->GetMethodID(shortClazzTemp, "shortValue", "()S");
    shortCtor = env->GetMethodID(shortClazzTemp, "<init>", "(S)V");

    shortClazz = (jclass)env->NewGlobalRef(shortClazzTemp);
    env->DeleteLocalRef(shortClazzTemp);

    jclass floatClazzTemp = env->FindClass("java/lang/Float");
    floatValue = env->GetMethodID(floatClazzTemp, "floatValue", "()F");
    floatCtor = env->GetMethodID(floatClazzTemp, "<init>", "(F)V");

    floatClazz = (jclass)env->NewGlobalRef(floatClazzTemp);
    env->DeleteLocalRef(floatClazzTemp);

    jclass clazz = env->FindClass("java/lang/Class");
    getClassName = env->GetMethodID(clazz, "getName", "()Ljava/lang/String;");
    env->DeleteLocalRef(clazz);

    // java/util/Map
    jclass mapClazzTmp = env->FindClass("java/util/Map");

    if (mapClazzTmp == NULL)
    {
            printf("Cannot find class java/util/Map");
            return;
    }

    mapClazz = (jclass)env->NewGlobalRef(mapClazzTmp);
    env->DeleteLocalRef(mapClazzTmp);

    size = env->GetMethodID(mapClazz, "size", "()I");

    if (size == NULL)
    {
            printf("Cannot find class java/util/Map size method");
            return;
    }

    getMapValue = env->GetMethodID(mapClazz, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");

    if (getMapValue == NULL)
    {
            printf("Cannot find class java/util/Map get method");
    }

    keySet = env->GetMethodID(mapClazz, "keySet", "()Ljava/util/Set;");

    if (keySet == NULL)
    {
            printf("Cannot find class java/util/Map keySet method");
    }

    // java/util/LinkedHashMap
    jclass setLinkedHashMapTmp = env->FindClass("java/util/HashMap");

    if (setLinkedHashMapTmp == NULL)
    {
            printf("Cannot find class java/util/LinkedHashMap");
    }

    hashMapClazz = (jclass)env->NewGlobalRef(setLinkedHashMapTmp);
    env->DeleteLocalRef(setLinkedHashMapTmp);

    mapCtor = env->GetMethodID(hashMapClazz, "<init>", "()V");

    if (mapCtor == NULL)
    {
            printf("Cannot find class java/util/LinkedHashMap ctor");
    }

    putMapValue = env->GetMethodID(hashMapClazz, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

    if (putMapValue == NULL)
    {
            printf("Cannot find class java/util/LinkedHashMap put method");
    }

    // java/util/Set
    jclass setClazzTmp = env->FindClass("java/util/Set");

    if (setClazzTmp == NULL)
    {
            printf("Cannot find class java/util/Set");
    }

    setClazz = (jclass)env->NewGlobalRef(setClazzTmp);
    env->DeleteLocalRef(setClazzTmp);

    toArray = env->GetMethodID(setClazz, "toArray", "()[Ljava/lang/Object;");
}

template<>
string TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    jstring input = (jstring)obj;
    if (input == NULL)
    {
        return NULL;
    }

    const char * tmp = env->GetStringUTFChars(input, JNI_FALSE);
    //copy the string
    string result(tmp);
    env->ReleaseStringUTFChars(input, tmp);

    return result;
}

template<>
const char* TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    jstring input = (jstring)obj;
    if (input == NULL)
    {
        return NULL;
    }

    const char * tmp = env->GetStringUTFChars(input, JNI_FALSE);
    //copy the string
    string result(tmp);
    env->ReleaseStringUTFChars(input, tmp);

    return result.c_str();
}
/*
template<>
string TypeConverter::convertToC(JNIEnv* env, jobject obj)mono_domain_a
{
	assert(env);
mono_domain_a
	jstring input = (jstring)obj;
	if (input == NULL)
	{
		return NULL;
	}

	jboolean* isCopy = true;

	const char * c_input = env->GetStringUTFChars(input, isCopy);
	env->ReleaseStringUTFChars(input, c_input);

	return string(c_input);
}*/

template<>
int TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    return env->CallIntMethod(obj, intValue);
}

template<>
double TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    return env->CallDoubleMethod(obj, doubleValue);
}

template<>
char TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    return env->CallCharMethod(obj, charValue);
}

template<>
byte TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    return env->CallByteMethod(obj, byteValue);
}

template<>
short TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    return env->CallShortMethod(obj, shortValue);
}

template<>
long TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    return env->CallLongMethod(obj, longValue);
}

template<>
float TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    return env->CallFloatMethod(obj, floatValue);
}

template<>
bool TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    return env->CallBooleanMethod(obj, booleanValue);
}

template<>
vector<int> TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    jintArray input = (jintArray)obj;

    const int intsSize = env->GetArrayLength(input);
    vector<int> intArray(intsSize);

    jint *body = env->GetIntArrayElements(input, JNI_FALSE);

    for (int i = 0; i < intsSize; i++)
    {
            intArray[i] = body[i];
    }

    env->ReleaseIntArrayElements(input, body, 0);
    env->DeleteLocalRef(input);

    return intArray;
}

template<>
vector<double> TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    jdoubleArray input = (jdoubleArray)obj;

    const int intsSize = env->GetArrayLength(input);

    vector<double> doubleArray(intsSize);

    jdouble *body = env->GetDoubleArrayElements(input, JNI_FALSE);

    for (int i = 0; i < intsSize; i++)
    {
            doubleArray[i] = body[i];
    }

    env->ReleaseDoubleArrayElements(input, body, 0);
    env->DeleteLocalRef(input);

    return doubleArray;
}

template<>
vector<char> TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    jcharArray input = (jcharArray)obj;

    const int intsSize = env->GetArrayLength(input);

    vector<char> charArray(intsSize);

    jchar *body = env->GetCharArrayElements(input, JNI_FALSE);

    for (int i = 0; i < intsSize; i++)
    {
            charArray[i] = body[i];
    }

    env->ReleaseCharArrayElements(input, body, 0);
    env->DeleteLocalRef(input);

    return charArray;
}

template<>
vector<long> TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    jlongArray input = (jlongArray)obj;

    const int intsSize = env->GetArrayLength(input);

    vector<long> longArray(intsSize);

    jlong *body = env->GetLongArrayElements(input, JNI_FALSE);

    for (int i = 0; i < intsSize; i++)
    {
            longArray[i] = body[i];
    }

    env->ReleaseLongArrayElements(input, body, 0);
    env->DeleteLocalRef(input);

    return longArray;
}

template<>
vector<bool> TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    jbooleanArray input = (jbooleanArray)obj;

    const int intsSize = env->GetArrayLength(input);

    vector<bool> booleanArray(intsSize);

    jboolean *body = env->GetBooleanArrayElements(input, JNI_FALSE);

    for (int i = 0; i < intsSize; i++)
    {
            booleanArray[i] = body[i];
    }

    env->ReleaseBooleanArrayElements(input, body, 0);
    env->DeleteLocalRef(input);

    return booleanArray;
}

template<>
vector<float> TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    jfloatArray input = (jfloatArray)obj;

    const int intsSize = env->GetArrayLength(input);

    vector<float> floatArray(intsSize);

    jfloat *body = env->GetFloatArrayElements(input, JNI_FALSE);

    for (int i = 0; i < intsSize; i++)
    {
            floatArray[i] = body[i];
    }

    env->ReleaseFloatArrayElements(input, body, 0);
    env->DeleteLocalRef(input);

    return floatArray;
}

template<>
vector<short> TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    jshortArray input = (jshortArray)obj;

    const int intsSize = env->GetArrayLength(input);

    vector<short> shortArray(intsSize);

    jshort *body = env->GetShortArrayElements(input, JNI_FALSE);

    for (int i = 0; i < intsSize; i++)
    {
            shortArray[i] = body[i];
    }

    env->ReleaseShortArrayElements(input, body, 0);
    env->DeleteLocalRef(input);

    return shortArray;
}

template<>
vector<byte> TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    jbyteArray input = (jbyteArray)obj;

    const int intsSize = env->GetArrayLength(input);

    vector<byte> byteArray(intsSize);

    jbyte *body = env->GetByteArrayElements(input, JNI_FALSE);

    for (int i = 0; i < intsSize; i++)
    {
            byteArray[i] = body[i];
    }

    env->ReleaseByteArrayElements(input, body, 0);
    env->DeleteLocalRef(input);

    return byteArray;
}

template<>
vector<string> TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    jobjectArray input = (jobjectArray)obj;

    const int intsSize = env->GetArrayLength(input);

    vector<string> stringArray(intsSize);

    for (int i = 0; i < intsSize; i++)
    {
            jobject item = env->GetObjectArrayElement(input, i);

            stringArray[i] = convertToC<const char*>(env, item);

            env->DeleteLocalRef(item);
    }

    env->DeleteLocalRef(input);

    return stringArray;
}

template<>
jbyteArray TypeConverter::convertToJavaArray(JNIEnv* env, MonoObject* obj)
{
	assert(env);
        assert(obj);
        
        MonoArray *data = (MonoArray*)obj;
            
        uintptr_t intsSize = mono_array_length(data);

	jbyteArray byteArray = env->NewByteArray(intsSize);

	jbyte* buffer = env->GetByteArrayElements(byteArray, JNI_FALSE);

	for (int i = 0; i < intsSize; i++)
	{
		buffer[i] = mono_array_get(data, uint8_t, i);
	}

	env->ReleaseByteArrayElements(byteArray, buffer, 0);

	return byteArray;
}
#include "stdafx.h"
#include "TypeConverter.h"

using namespace System;

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
		Console::WriteLine("Cannot find class java/util/Map");
		return;
	}

	mapClazz = (jclass)env->NewGlobalRef(mapClazzTmp);
	env->DeleteLocalRef(mapClazzTmp);

	size = env->GetMethodID(mapClazz, "size", "()I");

	if (size == NULL)
	{
		Console::WriteLine("Cannot find class java/util/Map size method");
		return;
	}

	getMapValue = env->GetMethodID(mapClazz, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");

	if (getMapValue == NULL)
	{
		Console::WriteLine("Cannot find class java/util/Map get method");
	}

	keySet = env->GetMethodID(mapClazz, "keySet", "()Ljava/util/Set;");

	if (keySet == NULL)
	{
		Console::WriteLine("Cannot find class java/util/Map keySet method");
	}

	// java/util/LinkedHashMap
	jclass setLinkedHashMapTmp = env->FindClass("java/util/HashMap");

	if (setLinkedHashMapTmp == NULL)
	{
		Console::WriteLine("Cannot find class java/util/LinkedHashMap");
	}

	hashMapClazz = (jclass)env->NewGlobalRef(setLinkedHashMapTmp);
	env->DeleteLocalRef(setLinkedHashMapTmp);

	mapCtor = env->GetMethodID(hashMapClazz, "<init>", "()V");

	if (mapCtor == NULL)
	{
		Console::WriteLine("Cannot find class java/util/LinkedHashMap ctor");
	}

	putMapValue = env->GetMethodID(hashMapClazz, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

	if (putMapValue == NULL)
	{
		Console::WriteLine("Cannot find class java/util/LinkedHashMap put method");
	}

	// java/util/Set
	jclass setClazzTmp = env->FindClass("java/util/Set");

	if (setClazzTmp == NULL)
	{
		Console::WriteLine("Cannot find class java/util/Set");
	}

	setClazz = (jclass)env->NewGlobalRef(setClazzTmp);
	env->DeleteLocalRef(setClazzTmp);

	toArray = env->GetMethodID(setClazz, "toArray", "()[Ljava/lang/Object;");
}

template<>
const char* TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
	assert(env);

	jstring input = (jstring)obj;
	if (input == NULL)
	{
		return nullptr;
	}

	const char * c_input = env->GetStringUTFChars(input, 0);
	env->ReleaseStringUTFChars(input, c_input);

	return c_input;
}

template<>
String^ TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
	assert(env);

	jstring input = (jstring)obj;
	if (input == NULL)
	{
		return nullptr;
	}

	const char * c_input = env->GetStringUTFChars(input, 0);

	String^ result = gcnew String(c_input);

	env->ReleaseStringUTFChars(input, c_input);

	return result;
}

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
Dictionary<String^, Object^>^ TypeConverter::convertToC(JNIEnv* env, jobject map)
{
	assert(env);

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

		if (env->IsInstanceOf(mapValue, mapClazz))
		{
			result->Add(convertToC<String^>(env, keyName), convertToC<Dictionary<String^, Object^>^>(env, mapValue));
		}
		else
		{
			result->Add(convertToC<String^>(env, keyName), toManagedObject(env, mapValue));
		}

		env->DeleteLocalRef(keyName);
		env->DeleteLocalRef(mapValue);
	}

	env->DeleteLocalRef(map);

	return result;
}

template<>
array<int>^ TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
	assert(env);

	jintArray input = (jintArray)obj;

	const int intsSize = env->GetArrayLength(input);

	array<int>^ intArray = gcnew array<int>(intsSize);

	jint *body = env->GetIntArrayElements(input, false);

	env->ReleaseIntArrayElements(input, body, 0);

	for (int i = 0; i < intsSize; i++)
	{
		intArray[i] = body[i];
	}

	env->DeleteLocalRef((jobject)body);
	env->DeleteLocalRef(input);

	return intArray;
}

template<>
array<double>^ TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
	assert(env);

	jdoubleArray input = (jdoubleArray)obj;

	const int intsSize = env->GetArrayLength(input);

	array<double>^ doubleArray = gcnew array<double>(intsSize);

	jdouble *body = env->GetDoubleArrayElements(input, false);

	env->ReleaseDoubleArrayElements(input, body, 0);

	for (int i = 0; i < intsSize; i++)
	{
		doubleArray[i] = body[i];
	}

	env->DeleteLocalRef((jobject)body);
	env->DeleteLocalRef(input);

	return doubleArray;
}

template<>
array<Char>^ TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
	assert(env);

	jcharArray input = (jcharArray)obj;

	const int intsSize = env->GetArrayLength(input);

	array<Char>^ charArray = gcnew array<Char>(intsSize);

	jchar *body = env->GetCharArrayElements(input, false);

	env->ReleaseCharArrayElements(input, body, 0);

	for (int i = 0; i < intsSize; i++)
	{
		charArray[i] = body[i];
	}

	env->DeleteLocalRef((jobject)body);
	env->DeleteLocalRef(input);

	return charArray;
}

template<>
array<long>^ TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
	assert(env);

	jlongArray input = (jlongArray)obj;

	const int intsSize = env->GetArrayLength(input);

	array<long>^ charArray = gcnew array<long>(intsSize);

	jlong *body = env->GetLongArrayElements(input, false);

	env->ReleaseLongArrayElements(input, body, 0);

	for (int i = 0; i < intsSize; i++)
	{
		charArray[i] = body[i];
	}

	env->DeleteLocalRef((jobject)body);
	env->DeleteLocalRef(input);

	return charArray;
}

template<>
array<Int64>^ TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
	assert(env);

	jlongArray input = (jlongArray)obj;

	const int intsSize = env->GetArrayLength(input);

	array<Int64>^ charArray = gcnew array<Int64>(intsSize);

	jlong *body = env->GetLongArrayElements(input, false);

	env->ReleaseLongArrayElements(input, body, 0);

	for (int i = 0; i < intsSize; i++)
	{
		charArray[i] = body[i];
	}

	env->DeleteLocalRef((jobject)body);
	env->DeleteLocalRef(input);

	return charArray;
}


template<>
array<bool>^ TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
	assert(env);

	jbooleanArray input = (jbooleanArray)obj;

	const int intsSize = env->GetArrayLength(input);

	array<bool>^ booleanArray = gcnew array<bool>(intsSize);

	jboolean *body = env->GetBooleanArrayElements(input, false);

	env->ReleaseBooleanArrayElements(input, body, 0);

	for (int i = 0; i < intsSize; i++)
	{
		booleanArray[i] = body[i];
	}

	env->DeleteLocalRef((jobject)body);
	env->DeleteLocalRef(input);

	return booleanArray;
}

template<>
array<float>^ TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
	assert(env);

	jfloatArray input = (jfloatArray)obj;

	const int intsSize = env->GetArrayLength(input);

	array<float>^ floatArray = gcnew array<float>(intsSize);

	jfloat *body = env->GetFloatArrayElements(input, false);

	env->ReleaseFloatArrayElements(input, body, 0);

	for (int i = 0; i < intsSize; i++)
	{
		floatArray[i] = body[i];
	}

	env->DeleteLocalRef((jobject)body);
	env->DeleteLocalRef(input);

	return floatArray;
}

template<>
array<short>^ TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
	assert(env);

	jshortArray input = (jshortArray)obj;

	const int intsSize = env->GetArrayLength(input);

	array<short>^ shortArray = gcnew array<short>(intsSize);

	jshort *body = env->GetShortArrayElements(input, false);

	env->ReleaseShortArrayElements(input, body, 0);

	for (int i = 0; i < intsSize; i++)
	{
		shortArray[i] = body[i];
	}

	env->DeleteLocalRef((jobject)body);
	env->DeleteLocalRef(input);

	return shortArray;
}

template<>
array<byte>^ TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
	assert(env);

	jbyteArray input = (jbyteArray)obj;

	const int intsSize = env->GetArrayLength(input);

	array<byte>^ byteArray = gcnew array<byte>(intsSize);

	jbyte *body = env->GetByteArrayElements(input, false);

	env->ReleaseByteArrayElements(input, body, 0);

	for (int i = 0; i < intsSize; i++)
	{
		byteArray[i] = body[i];
	}

	env->DeleteLocalRef((jobject)body);
	env->DeleteLocalRef(input);

	return byteArray;
}

template<>
array<String^>^ TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
	assert(env);

	jobjectArray input = (jobjectArray)obj;

	const int intsSize = env->GetArrayLength(input);

	array<String^>^ stringArray = gcnew array<String^>(intsSize);

	for (int i = 0; i < intsSize; i++)
	{
		jobject item = env->GetObjectArrayElement(input, i);

		stringArray[i] = convertToC<String^>(env, item);

		env->DeleteLocalRef(item);
	}

	env->DeleteLocalRef(input);

	return stringArray;
}

// 2 dimensional arrays
template<>
array<array<byte>^>^ TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
	assert(env);

	jobjectArray input = (jobjectArray)obj;
	const int intsSize = env->GetArrayLength(input);

	array<array<byte>^>^ byteArray = gcnew array<array<byte>^>(intsSize);

	for (int i = 0; i < intsSize; i++)
	{
		jobject body = env->GetObjectArrayElement(input, i);
		byteArray[i] = convertToC<array<byte>^>(env, body);

		env->DeleteLocalRef((jobject)body);
	}

	env->DeleteLocalRef(input);

	return byteArray;
}


template<>
array<array<int>^>^ TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
	assert(env);

	jobjectArray input = (jobjectArray)obj;
	const int intsSize = env->GetArrayLength(input);

	array<array<int>^>^ intArray = gcnew array<array<int>^>(intsSize);

	for (int i = 0; i < intsSize; i++)
	{
		jobject body = env->GetObjectArrayElement(input, i);
		intArray[i] = convertToC<array<int>^>(env, body);

		env->DeleteLocalRef((jobject)body);
	}

	env->DeleteLocalRef(input);

	return intArray;
}

template<>
array<array<Char>^>^ TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
	assert(env);

	jobjectArray input = (jobjectArray)obj;
	const int intsSize = env->GetArrayLength(input);

	array<array<Char>^>^ charArray = gcnew array<array<Char>^>(intsSize);

	for (int i = 0; i < intsSize; i++)
	{
		jobject body = env->GetObjectArrayElement(input, i);
		charArray[i] = convertToC<array<Char>^>(env, body);

		env->DeleteLocalRef((jobject)body);
	}

	env->DeleteLocalRef(input);

	return charArray;
}

template<>
array<array<double>^>^ TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
	assert(env);

	jobjectArray input = (jobjectArray)obj;
	const int intsSize = env->GetArrayLength(input);

	array<array<double>^>^ doubleArray = gcnew array<array<double>^>(intsSize);

	for (int i = 0; i < intsSize; i++)
	{
		jobject body = env->GetObjectArrayElement(input, i);
		doubleArray[i] = convertToC<array<double>^>(env, body);

		env->DeleteLocalRef((jobject)body);
	}

	env->DeleteLocalRef(input);

	return doubleArray;
}

template<>
array<array<float>^>^ TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
	assert(env);

	jobjectArray input = (jobjectArray)obj;
	const int intsSize = env->GetArrayLength(input);

	array<array<float>^>^ floatArray = gcnew array<array<float>^>(intsSize);

	for (int i = 0; i < intsSize; i++)
	{
		jobject body = env->GetObjectArrayElement(input, i);
		floatArray[i] = convertToC<array<float>^>(env, body);

		env->DeleteLocalRef((jobject)body);
	}

	env->DeleteLocalRef(input);

	return floatArray;
}

template<>
array<array<Int64>^>^ TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
	assert(env);

	jobjectArray input = (jobjectArray)obj;
	const int intsSize = env->GetArrayLength(input);

	array<array<Int64>^>^ longArray = gcnew array<array<Int64>^>(intsSize);

	for (int i = 0; i < intsSize; i++)
	{
		jobject body = env->GetObjectArrayElement(input, i);
		longArray[i] = convertToC<array<Int64>^>(env, body);

		env->DeleteLocalRef((jobject)body);
	}

	env->DeleteLocalRef(input);

	return longArray;
}

template<>
array<array<bool>^>^ TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
	assert(env);

	jobjectArray input = (jobjectArray)obj;
	const int intsSize = env->GetArrayLength(input);

	array<array<bool>^>^ boolArray = gcnew array<array<bool>^>(intsSize);

	for (int i = 0; i < intsSize; i++)
	{
		jobject body = env->GetObjectArrayElement(input, i);
		boolArray[i] = convertToC<array<bool>^>(env, body);

		env->DeleteLocalRef((jobject)body);
	}

	env->DeleteLocalRef(input);

	return boolArray;
}

template<>
array<array<short>^>^ TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
	assert(env);

	jobjectArray input = (jobjectArray)obj;
	const int intsSize = env->GetArrayLength(input);

	array<array<short>^>^ shortArray = gcnew array<array<short>^>(intsSize);

	for (int i = 0; i < intsSize; i++)
	{
		jobject body = env->GetObjectArrayElement(input, i);
		shortArray[i] = convertToC<array<short>^>(env, body);

		env->DeleteLocalRef((jobject)body);
	}

	env->DeleteLocalRef(input);

	return shortArray;
}

template<>
array<array<String^>^>^ TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
	assert(env);

	jobjectArray input = (jobjectArray)obj;
	const int intsSize = env->GetArrayLength(input);

	array<array<String^>^>^ stringArray = gcnew array<array<String^>^>(intsSize);

	for (int i = 0; i < intsSize; i++)
	{
		jobject body = env->GetObjectArrayElement(input, i);
		stringArray[i] = convertToC<array<String^>^>(env, body);

		env->DeleteLocalRef((jobject)body);
	}

	env->DeleteLocalRef(input);

	return stringArray;
}

Object^ TypeConverter::toManagedObject(JNIEnv* env, jobject obj)
{
	assert(env);

	jclass clazz = env->GetObjectClass(obj);
	jstring clazzName = (jstring)env->CallObjectMethod(clazz, getClassName);

	String^ className = convertToC<String^>(env, clazzName);
	
	Object^ result = nullptr;

	if (className->Equals("java.lang.Integer", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<int>(env, obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("java.lang.Boolean", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<bool>(env, obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("java.lang.Character", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<char>(env, obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("java.lang.Long", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<long>(env, obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("java.lang.Short", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<short>(env, obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("java.lang.Byte", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<byte>(env, obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("java.lang.String", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<String^>(env, obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("java.lang.Double", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<double>(env, obj);
		env->DeleteLocalRef(obj);
	}

	// one dimension arrays
	if (className->Equals("[I", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<array<int>^>(env, obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("[D", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<array<double>^>(env, obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("[Z", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<array<bool>^>(env, obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("[B", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<array<byte>^>(env, obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("[C", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<array<Char>^>(env, obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("[S", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<array<short>^>(env, obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("[J", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<array<long>^>(env, obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("[F", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<array<float>^>(env, obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("[Ljava.lang.String;", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<array<String^>^>(env, obj);
		env->DeleteLocalRef(obj);
	}

	//TODO: [[I (two dimensions) & ByteBuffer
	if (className->Equals("[[B", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<array<array<byte>^>^>(env, obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("[[C", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<array<array<Char>^>^>(env, obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("[[S", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<array<array<short>^>^>(env, obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("[[J", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<array<array<Int64>^>^>(env, obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("[[F", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<array<array<float>^>^>(env, obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("[[Ljava.lang.String;", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<array<array<String^>^>^>(env, obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("[[I", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<array<array<int>^>^>(env, obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("[[D", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<array<array<double>^>^>(env, obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("[[Z", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<array<array<bool>^>^>(env, obj);
		env->DeleteLocalRef(obj);
	}

	return result;
}

void TypeConverter::cleanup(JNIEnv *env)
{
	assert(env);

	try
	{
		env->DeleteGlobalRef(mapClazz);
		env->DeleteGlobalRef(setClazz);
		env->DeleteGlobalRef(hashMapClazz);
		env->DeleteGlobalRef(intClazz);
		env->DeleteGlobalRef(doubleClazz);
		env->DeleteGlobalRef(booleanClazz);
		env->DeleteGlobalRef(shortClazz);
		env->DeleteGlobalRef(longClazz);
		env->DeleteGlobalRef(byteClazz);
		env->DeleteGlobalRef(characterClazz);
		env->DeleteGlobalRef(floatClazz);
	}
	catch (Exception^ ex)
	{
		Console::WriteLine(ex->Message);
	}
}

template<>
jbyteArray TypeConverter::convertToJavaArray(JNIEnv* env, Object^ obj)
{
	assert(env);

	array<byte>^ input = (array<byte>^)obj;

	const int intsSize = input->Length;

	jbyteArray byteArray = env->NewByteArray(intsSize);

	jbyte* buffer = env->GetByteArrayElements(byteArray, NULL);

	for (int i = 0; i < intsSize; i++)
	{
		buffer[i] = input[i];
	}

	env->ReleaseByteArrayElements(byteArray, buffer, 0);

	return byteArray;
}

template<>
jintArray TypeConverter::convertToJavaArray(JNIEnv* env, Object^ obj)
{
	assert(env);

	array<int>^ input = (array<int>^)obj;

	const int intsSize = input->Length;

	jintArray intArray = env->NewIntArray(intsSize);

	jint* buffer = env->GetIntArrayElements(intArray, NULL);

	for (int i = 0; i < intsSize; i++)
	{
		buffer[i] = input[i];
	}

	env->ReleaseIntArrayElements(intArray, buffer, 0);

	return intArray;
}

template<>
jbooleanArray TypeConverter::convertToJavaArray(JNIEnv* env, Object^ obj)
{
	assert(env);

	array<bool>^ input = (array<bool>^)obj;

	const int intsSize = input->Length;

	jbooleanArray jArray = env->NewBooleanArray(intsSize);

	jboolean* buffer = env->GetBooleanArrayElements(jArray, NULL);

	for (int i = 0; i < intsSize; i++)
	{
		buffer[i] = input[i];
	}

	env->ReleaseBooleanArrayElements(jArray, buffer, 0);

	return jArray;
}

template<>
jshortArray TypeConverter::convertToJavaArray(JNIEnv* env, Object^ obj)
{
	assert(env);

	array<Int16>^ input = (array<Int16>^)obj;

	const int intsSize = input->Length;

	jshortArray jArray = env->NewShortArray(intsSize);

	jshort* buffer = env->GetShortArrayElements(jArray, NULL);

	for (int i = 0; i < intsSize; i++)
	{
		buffer[i] = input[i];
	}

	env->ReleaseShortArrayElements(jArray, buffer, 0);

	return jArray;
}

template<>
jlongArray TypeConverter::convertToJavaArray(JNIEnv* env, Object^ obj)
{
	assert(env);

	array<Int64>^ input = (array<Int64>^)obj;

	const int intsSize = input->Length;

	jlongArray jArray = env->NewLongArray(intsSize);

	jlong* buffer = env->GetLongArrayElements(jArray, NULL);

	for (int i = 0; i < intsSize; i++)
	{
		buffer[i] = input[i];
	}

	env->ReleaseLongArrayElements(jArray, buffer, 0);

	return jArray;
}

template<>
jcharArray TypeConverter::convertToJavaArray(JNIEnv* env, Object^ obj)
{
	assert(env);

	array<Char>^ input = (array<Char>^)obj;

	const int intsSize = input->Length;

	jcharArray jArray = env->NewCharArray(intsSize);

	jchar* buffer = env->GetCharArrayElements(jArray, NULL);

	for (int i = 0; i < intsSize; i++)
	{
		buffer[i] = input[i];
	}

	env->ReleaseCharArrayElements(jArray, buffer, 0);

	return jArray;
}

template<>
jdoubleArray TypeConverter::convertToJavaArray(JNIEnv* env, Object^ obj)
{
	assert(env);

	array<double>^ input = (array<double>^)obj;

	const int intsSize = input->Length;

	jdoubleArray jArray = env->NewDoubleArray(intsSize);

	jdouble* buffer = env->GetDoubleArrayElements(jArray, NULL);

	for (int i = 0; i < intsSize; i++)
	{
		buffer[i] = input[i];
	}

	env->ReleaseDoubleArrayElements(jArray, buffer, 0);

	return jArray;
}

template<>
jfloatArray TypeConverter::convertToJavaArray(JNIEnv* env, Object^ obj)
{
	assert(env);

	array<float>^ input = (array<float>^)obj;

	const int intsSize = input->Length;

	jfloatArray jArray = env->NewFloatArray(intsSize);

	jfloat* buffer = env->GetFloatArrayElements(jArray, NULL);

	for (int i = 0; i < intsSize; i++)
	{
		buffer[i] = input[i];
	}

	env->ReleaseFloatArrayElements(jArray, buffer, 0);

	return jArray;
}

jobject TypeConverter::convertToJavaMap(JNIEnv* env, Object^ obj)
{
	assert(env);

	Dictionary<String^, Object^>^ dictionary = (Dictionary<String^, Object^>^)obj;

	jobject map = env->NewObject(hashMapClazz, mapCtor);

	msclr::interop::marshal_context ctx;

	for each (KeyValuePair<String^, Object^> pair in dictionary)
	{
		jobject key = toJavaObject(env, pair.Key);
		jobject value = toJavaObject(env, pair.Value);
	
		env->CallObjectMethod(map, putMapValue, key, value);
	}

	return map;
}

jobject TypeConverter::toJavaObject(JNIEnv* env, Object^ systemObject)
{
	Type^ type = systemObject->GetType();

	if (type->ToString() == "System.Int32")
	{
		int value = (int)systemObject;
		return env->NewObject(intClazz, intCtor, value);
	}

	if (type->ToString() == "System.Double")
	{
		double value = (double)systemObject;
		return env->NewObject(doubleClazz, doubleCtor, value);
	}

	if (type->ToString() == "System.Boolean")
	{
		bool value = (bool)systemObject;
		return env->NewObject(booleanClazz, booleanCtor, value);
	}

	if (type->ToString() == "System.Char")
	{
		char value = System::Char::Parse(systemObject->ToString());
		return env->NewObject(characterClazz, charCtor, value);
	}

	if (type->ToString() == "System.Int16")
	{
		short value = (short)systemObject;
		return env->NewObject(shortClazz, shortCtor, value);
	}

	if (type->ToString() == "System.Single")
	{
		float value = (float)systemObject;
		return env->NewObject(floatClazz, floatCtor, value);
	}

	if (type->ToString() == "System.Int64")
	{
		long long value = (long long)systemObject;
		return env->NewObject(longClazz, longCtor, value);
	}

	if (type->ToString() == "System.Byte")
	{
		byte value = (byte)systemObject;
		return env->NewObject(byteClazz, byteCtor, value);
	}
	
	if (type->ToString() == "System.String")
	{
		msclr::interop::marshal_context ctx;
		const char * cString = ctx.marshal_as<const char *>(systemObject->ToString());
		return env->NewStringUTF(cString);
	}

	if (type->IsArray)
	{
		Type^ arrayType = type->GetElementType();

		if (arrayType->ToString() == "System.Byte")
		{
			return convertToJavaArray<jbyteArray>(env, systemObject);
		}

		if (arrayType->ToString() == "System.Int32")
		{
			return convertToJavaArray<jintArray>(env, systemObject);
		}

		if (arrayType->ToString() == "System.Int16")
		{
			return convertToJavaArray<jshortArray>(env, systemObject);
		}

		if (arrayType->ToString() == "System.Int64")
		{
			return convertToJavaArray<jlongArray>(env, systemObject);
		}

		if (arrayType->ToString() == "System.Char")
		{
			return convertToJavaArray<jcharArray>(env, systemObject);
		}

		if (arrayType->ToString() == "System.Single")
		{
			return convertToJavaArray<jfloatArray>(env, systemObject);
		}

		if (arrayType->ToString() == "System.Boolean")
		{
			return convertToJavaArray<jbooleanArray>(env, systemObject);
		}

		if (arrayType->ToString() == "System.Double")
		{
			return convertToJavaArray<jdoubleArray>(env, systemObject);
		}
	}

	throw gcnew Exception("Unsupported type");
}
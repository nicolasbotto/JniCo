#include "stdafx.h"
#include "TypeConverter.h"

using namespace System;

TypeConverter::TypeConverter(JNIEnv* jniEnv)
{
	assert(jniEnv);
	env = jniEnv;

	jclass intClazz = env->FindClass("Ljava/lang/Integer;");
	intValue = env->GetMethodID(intClazz, "intValue", "()I");
	env->DeleteLocalRef(intClazz);

	jclass doubleClazz = env->FindClass("Ljava/lang/Double;");
	doubleValue = env->GetMethodID(doubleClazz, "doubleValue", "()D");
	env->DeleteLocalRef(doubleClazz);

	jclass charClazz = env->FindClass("Ljava/lang/Character;");
	charValue = env->GetMethodID(charClazz, "charValue", "()C");
	env->DeleteLocalRef(charClazz);

	jclass longClazz = env->FindClass("Ljava/lang/Long;");
	longValue = env->GetMethodID(longClazz, "longValue", "()J");
	env->DeleteLocalRef(longClazz);

	jclass booleanClazz = env->FindClass("Ljava/lang/Boolean;");
	booleanValue = env->GetMethodID(booleanClazz, "booleanValue", "()Z");
	env->DeleteLocalRef(booleanClazz);

	jclass byteClazz = env->FindClass("Ljava/lang/Byte;");
	byteValue = env->GetMethodID(byteClazz, "byteValue", "()B");
	env->DeleteLocalRef(byteClazz);

	jclass shortClazz = env->FindClass("Ljava/lang/Short;");
	shortValue = env->GetMethodID(shortClazz, "shortValue", "()S");
	env->DeleteLocalRef(shortClazz);

	jclass floatClazz = env->FindClass("Ljava/lang/Float;");
	floatValue = env->GetMethodID(floatClazz, "floatValue", "()F");
	env->DeleteLocalRef(floatClazz);

	jclass clazz = env->FindClass("Ljava/lang/Class;");
	getClassName = env->GetMethodID(clazz, "getName", "()Ljava/lang/String;");
	env->DeleteLocalRef(clazz);

	// java/util/Map
	jclass mapClazzTmp = env->FindClass("Ljava/util/Map;");

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

	// java/util/Set
	jclass setClazzTmp = env->FindClass("Ljava/util/Set;");

	if (setClazzTmp == NULL)
	{
		Console::WriteLine("Cannot find class java/util/Set");
	}

	setClazz = (jclass)env->NewGlobalRef(setClazzTmp);
	env->DeleteLocalRef(setClazzTmp);

	toArray = env->GetMethodID(setClazz, "toArray", "()[Ljava/lang/Object;");
}

template<>
const char* TypeConverter::convertToC(jobject obj)
{
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
String^ TypeConverter::convertToC(jobject obj)
{
	//return gcnew String(convertToC<const char*>(obj));
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
int TypeConverter::convertToC(jobject obj)
{
	return env->CallIntMethod(obj, intValue);
}

template<>
double TypeConverter::convertToC(jobject obj)
{
	return env->CallDoubleMethod(obj, doubleValue);
}

template<>
char TypeConverter::convertToC(jobject obj)
{
	return env->CallCharMethod(obj, charValue);
}

template<>
byte TypeConverter::convertToC(jobject obj)
{
	return env->CallByteMethod(obj, byteValue);
}

template<>
short TypeConverter::convertToC(jobject obj)
{
	return env->CallShortMethod(obj, shortValue);
}

template<>
long TypeConverter::convertToC(jobject obj)
{
	return env->CallLongMethod(obj, longValue);
}

template<>
float TypeConverter::convertToC(jobject obj)
{
	return env->CallFloatMethod(obj, floatValue);
}

template<>
bool TypeConverter::convertToC(jobject obj)
{
	return env->CallBooleanMethod(obj, booleanValue);
}

template<>
Dictionary<String^, Object^>^ TypeConverter::convertToC(jobject map)
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

		if (env->IsInstanceOf(mapValue, mapClazz))
		{
			result->Add(convertToC<String^>(keyName), convertToC<Dictionary<String^, Object^>^>(mapValue));
		}
		else
		{
			result->Add(convertToC<String^>(keyName), toManagedObject(mapValue));
		}

		env->DeleteLocalRef(keyName);
		env->DeleteLocalRef(mapValue);
	}

	env->DeleteLocalRef(map);

	return result;
}

template<>
array<int>^ TypeConverter::convertToC(jobject obj)
{
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
array<double>^ TypeConverter::convertToC(jobject obj)
{
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
array<char>^ TypeConverter::convertToC(jobject obj)
{
	jcharArray input = (jcharArray)obj;

	const int intsSize = env->GetArrayLength(input);

	array<char>^ charArray = gcnew array<char>(intsSize);

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
array<long>^ TypeConverter::convertToC(jobject obj)
{
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
array<bool>^ TypeConverter::convertToC(jobject obj)
{
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
array<float>^ TypeConverter::convertToC(jobject obj)
{
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
array<short>^ TypeConverter::convertToC(jobject obj)
{
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
array<byte>^ TypeConverter::convertToC(jobject obj)
{
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
array<array<byte>^>^ TypeConverter::convertToC(jobject obj)
{
	jobjectArray input = (jobjectArray)obj;
	const int intsSize = env->GetArrayLength(input);

	array<array<byte>^>^ byteArray = gcnew array<array<byte>^>(intsSize);

	for (int i = 0; i < intsSize; i++)
	{
		jobject body = env->GetObjectArrayElement(input, i);
		byteArray[i] = convertToC<array<byte>^>(body);

		env->DeleteLocalRef((jobject)body);
	}
	
	env->DeleteLocalRef(input);

	return byteArray;
}



template<>
array<String^>^ TypeConverter::convertToC(jobject obj)
{
	jobjectArray input = (jobjectArray)obj;

	const int intsSize = env->GetArrayLength(input);

	array<String^>^ stringArray = gcnew array<String^>(intsSize);

	for (int i = 0; i < intsSize; i++)
	{
		jobject item = env->GetObjectArrayElement(input, i);

		stringArray[i] = convertToC<String^>(item);

		env->DeleteLocalRef(item);
	}

	env->DeleteLocalRef(input);

	return stringArray;
}

Object^ TypeConverter::toManagedObject(jobject obj)
{
	jclass clazz = env->GetObjectClass(obj);
	jstring clazzName = (jstring)env->CallObjectMethod(clazz, getClassName);

	String^ className = convertToC<String^>(clazzName);
	
	Object^ result = nullptr;

	if (className->Equals("java.lang.Integer", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<int>(obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("java.lang.Boolean", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<bool>(obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("java.lang.Character", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<char>(obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("java.lang.Long", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<long>(obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("java.lang.Short", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<short>(obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("java.lang.Byte", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<byte>(obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("java.lang.String", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<String^>(obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("java.lang.Double", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<double>(obj);
		env->DeleteLocalRef(obj);
	}

	// arrays
	if (className->Equals("[I", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<array<int>^>(obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("[D", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<array<double>^>(obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("[Z", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<array<bool>^>(obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("[B", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<array<byte>^>(obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("[C", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<array<char>^>(obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("[S", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<array<short>^>(obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("[J", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<array<long>^>(obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("[F", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<array<float>^>(obj);
		env->DeleteLocalRef(obj);
	}

	if (className->Equals("[Ljava.lang.String;", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<array<String^>^>(obj);
		env->DeleteLocalRef(obj);
	}

	//TODO: [[I (two dimensions) & ByteBuffer
	if (className->Equals("[[B", StringComparison::InvariantCultureIgnoreCase))
	{
		result = convertToC<array<array<byte>^>^>(obj);
		env->DeleteLocalRef(obj);
	}

	return result;
}

void TypeConverter::cleanup()
{
	try
	{
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
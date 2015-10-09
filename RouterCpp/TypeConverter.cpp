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
	}

	return result;
}

Object^ TypeConverter::toManagedObject(jobject obj)
{
	jclass clazz = env->GetObjectClass(obj);
	jstring clazzName = (jstring)env->CallObjectMethod(clazz, getClassName);

	String^ className = convertToC<String^>(clazzName);

	if (className->Equals("java.lang.Integer", StringComparison::InvariantCultureIgnoreCase))
	{
		return convertToC<int>(obj);
	}

	if (className->Equals("java.lang.Boolean", StringComparison::InvariantCultureIgnoreCase))
	{
		return convertToC<bool>(obj);
	}

	if (className->Equals("java.lang.Character", StringComparison::InvariantCultureIgnoreCase))
	{
		return convertToC<char>(obj);
	}

	if (className->Equals("java.lang.Long", StringComparison::InvariantCultureIgnoreCase))
	{
		return convertToC<long>(obj);
	}

	if (className->Equals("java.lang.Short", StringComparison::InvariantCultureIgnoreCase))
	{
		return convertToC<short>(obj);
	}

	if (className->Equals("java.lang.Byte", StringComparison::InvariantCultureIgnoreCase))
	{
		return convertToC<byte>(obj);
	}

	if (className->Equals("java.lang.String", StringComparison::InvariantCultureIgnoreCase))
	{
		return convertToC<String^>(obj);
	}

	if (className->Equals("java.lang.Double", StringComparison::InvariantCultureIgnoreCase))
	{
		return convertToC<double>(obj);
	}

	return nullptr;
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
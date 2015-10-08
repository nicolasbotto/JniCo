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
}

template<>
const char* TypeConverter::convertToC(jobject obj)
{
	jstring input = (jstring)obj;
	if (input == NULL)
	{
		return nullptr;
	}

	const char * c_input = env->GetStringUTFChars(input, false);
	env->ReleaseStringUTFChars(input, c_input);

	return c_input;
}

template<>
String^ TypeConverter::convertToC(jobject obj)
{
	return gcnew String(convertToC<const char*>(obj));
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
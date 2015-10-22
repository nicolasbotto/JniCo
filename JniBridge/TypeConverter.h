#include "stdafx.h"
#include <msclr/marshal.h>
#include <jni.h>
#include <cassert>

using namespace System;
using namespace System::Collections::Generic;

class TypeConverter
{
private:
	jmethodID doubleValue;
	jmethodID doubleCtor;
	jmethodID intValue;
	jmethodID intCtor;
	jmethodID charValue;
	jmethodID charCtor;
	jmethodID floatValue;
	jmethodID floatCtor;
	jmethodID byteValue;
	jmethodID byteCtor;
	jmethodID shortValue;
	jmethodID shortCtor;
	jmethodID longValue;
	jmethodID longCtor;
	jmethodID booleanValue;
	jmethodID booleanCtor;
	jmethodID getClassName;
	jmethodID size;
	jmethodID keySet;
	jmethodID getMapValue;
	jmethodID mapCtor;
	jmethodID putMapValue;
	jmethodID toArray;
	jclass mapClazz;
	jclass hashMapClazz;
	jclass setClazz;
	jclass intClazz;
	jclass doubleClazz;
	jclass booleanClazz;
	jclass shortClazz;
	jclass longClazz;
	jclass byteClazz;
	jclass characterClazz;
	jclass floatClazz;

public:
	void init(JNIEnv*);
	// conversion functions
	template<typename TOut>
	TOut convertToC(JNIEnv*, jobject);
	Object^ toManagedObject(JNIEnv*, jobject);
	void cleanup(JNIEnv*);
	template<typename TOut>
	TOut convertToJavaArray(JNIEnv*, Object^);
	jobject toJavaObject(JNIEnv*, Object^);
	jobject convertToJavaMap(JNIEnv*, Object^);
};
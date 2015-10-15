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
	jmethodID intValue;
	jmethodID charValue;
	jmethodID floatValue;
	jmethodID byteValue;
	jmethodID shortValue;
	jmethodID longValue;
	jmethodID booleanValue;
	jmethodID getClassName;
	jmethodID size;
	jmethodID keySet;
	jmethodID getMapValue;
	jmethodID toArray;
	jclass mapClazz;
	jclass setClazz;

public:
	void init(JNIEnv*);
	
	// conversion functions
	template<typename TOut>
	TOut convertToC(JNIEnv*, jobject);
	Object^ toManagedObject(JNIEnv*, jobject);
	void cleanup(JNIEnv*);
};

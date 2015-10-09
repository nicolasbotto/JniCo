#include "stdafx.h"
#include <msclr/marshal.h>
#include <jni.h>
#include <cassert>

using namespace System;
using namespace System::Collections::Generic;

class TypeConverter
{
private:
	JNIEnv *env;
	jmethodID doubleValue;
	jmethodID intValue;
	jmethodID charValue;
	jmethodID floatValue;
	jmethodID byteValue;
	jmethodID shortValue;
	jmethodID longValue;
	jmethodID booleanValue;
	jmethodID getClassName;

public:
	TypeConverter(JNIEnv*);
	
	// conversion functions
	template<typename TOut>
	TOut convertToC(jobject);

	Object^ toManagedObject(jobject);
};
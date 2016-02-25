#include <mono/metadata/mono-config.h>
#include <mono/metadata/threads.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-gc.h>
#include <mono/metadata/environment.h>
#include <mono/jit/jit.h>
#include <jni.h>
#include <cassert>
#include <vector>
#include <string>
#include <exception>

typedef unsigned char byte;

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
	jmethodID mapCtor;
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
	jmethodID getClassName;
	jmethodID getMapValue;
	jclass mapClazz;
	jmethodID keySet;
	jmethodID putMapValue;
	jmethodID toArray;

	jmethodID size;
	void init(JNIEnv*);
	// conversion functions
	template<typename TOut>
	TOut convertToC(JNIEnv*, jobject);
	void cleanup(JNIEnv*);
	template<typename TOut, typename TIn>
	TOut convertToJavaArray(JNIEnv*, TIn);
	template<typename T>
	MonoObject* toMonoObject(T);
        template<typename TOut>
        TOut convertToJavaArray(JNIEnv* env, MonoObject* obj);
	//jobject toJavaObject(JNIEnv*, Object^);
	//jobject convertToJavaMap(JNIEnv*, Object^);
};
#include <jni.h>
#include <map>
#include "TypeConverter.h"

using namespace std;

class JniManager
{
private:
	TypeConverter* typeConverter;
	JavaVM* jvm;
	jmethodID getAssemblyName;
	jmethodID getAssemblyPath;
	jmethodID getMethodName;
	jmethodID getLog;
	jmethodID getNotifyEvents;
	jmethodID getFullTrust;
	jmethodID getIsSingleton;
	jmethodID getMethodArguments;
	jmethodID getInvocationProperties;
	jmethodID getSessionProperties;
	jmethodID getOutboundProperties;
	jmethodID getInboundProperties;
	jmethodID responseCtor;
	jmethodID setPayloadMethod;
	jmethodID logMethod;
	jmethodID instrumentMethod;
	jmethodID setInvocationProperties;
	jmethodID setSessionProperties;
	jmethodID setOutboundProperties;
	jclass responseClazz;
	jobject routerInstance;
	jclass exceptionClazz;

	// Mono Methods
	MonoMethod* processMethod;
	MonoMethod* setAssemblyPathField;
	MonoMethod* setMethodNameField;
	MonoMethod* setAssemblyNameField;
	MonoMethod* setFullTrustField;
	MonoMethod* setIsSingletonField;
	MonoMethod* setLogField;
	MonoMethod* setNotifyEventsField;
	MonoMethod* addMethodArgumentsProperty;
	MonoMethod* addOutboundProperty;
	MonoMethod* addInboundProperty;
	MonoMethod* addInvocationProperty;
	MonoMethod* addSessionProperty;
	MonoMethod* getResult;

	MonoClass* processRequestClass;

public:

        MonoDomain* getMonoDomain();
	JniManager(JavaVM*);
	void setMono(MonoImage*);
	void init();
	JNIEnv* getEnv();
	jobject toResponseObject(jobject);
	void cleanup();
	void setRouter(jobject);
	void checkJniException();
	void toProcessRequest(jobject, MonoObject*);
	void throwException(const char*);
	void setProperties(JNIEnv*, jobject, MonoMethod*, MonoObject*);
	MonoObject* toMonoObject(JNIEnv*, jobject);
	jobject toResponse(MonoObject*);
};


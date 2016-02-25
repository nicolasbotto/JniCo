#include "org_mule_api_jni_Bridge.h"
#include "JniManager.h"

using namespace std;

static JniManager* jniManager;
static jobject javaRouter;
static MonoImage* monoImage;
static MonoMethod* processMethod;
static uint32_t routerInstance_handle;
static MonoClass* processRequestClass;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
	jniManager = new JniManager(vm);
	JNIEnv* env = jniManager->getEnv();

	if (!env)
	{
		return JNI_ERR;
	}

	return JNI_VERSION_1_6;
}

// The VM calls JNI_OnUnload when the class loader containing the native library is garbage collected.
JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved)
{
	//remove global references
//	jniManager->cleanup();
}

JNIEXPORT void JNICALL Java_org_mule_api_jni_Bridge_init
(JNIEnv *env, jobject obj)
{
    // initialize the root domain which will hold corlib and will always be alive
    try
    {
    // load the default Mono configuration file in 'etc/mono/config'
    mono_config_parse(NULL);

    MonoDomain* monoDomain = mono_jit_init_version("jni Root Domain", "v4.0.30319");

    //MonoThread* mainThread = mono_thread_attach(monoDomain);
   // mono_thread_attach(monoDomain);

    MonoAssembly* assembly = mono_domain_assembly_open(monoDomain, "/home/test/Documents/MonoEmbedded/Org.Mule.Api.Routing.dll");
    monoImage = mono_assembly_get_image(assembly);

    MonoClass* routerClass = mono_class_from_name(monoImage, "Org.Mule.Api.Routing", "Router");

    MonoObject* router = mono_object_new(monoDomain, routerClass);
    mono_runtime_object_init(router);

    routerInstance_handle = mono_gchandle_new(router, 1);

    processMethod = mono_class_get_method_from_name(routerClass,"Process", 1);

    jniManager->setMono(monoImage);

    processRequestClass = mono_class_from_name(monoImage, "Org.Mule.Api.Routing", "ProcessRequest");

    }
    catch (char* errorMessage)
    {
        printf("%s\n", errorMessage);
        jniManager->throwException(errorMessage);
    }
}

JNIEXPORT jobject JNICALL Java_org_mule_api_jni_Bridge_invokeNetMethod
(JNIEnv *env, jobject obj, jobject request)
{
    MonoObject* exception = NULL;

    MonoDomain* monoDomain = mono_get_root_domain();

    mono_thread_attach(monoDomain);

    MonoObject* processRequest = mono_object_new(monoDomain, processRequestClass);
    mono_runtime_object_init(processRequest);

    jniManager->toProcessRequest(request, processRequest);
    MonoObject *routerInstance = mono_gchandle_get_target (routerInstance_handle);

    void* args[1];
    args[0] = processRequest;

    MonoObject* response = mono_runtime_invoke(processMethod, routerInstance, args, &exception);

    // check for any thrown exception
    if (exception)
    {
        printf("%s", "exception!!\n");
        const char* message = mono_string_to_utf8(mono_object_to_string(exception, NULL));
        printf("%s\n", message);
        jniManager->throwException(message);
        return NULL;
    }

    jobject result = jniManager->toResponse(response);
    return result;
}


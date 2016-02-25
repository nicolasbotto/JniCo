#include <mono/metadata/mono-config.h>
#include <mono/metadata/threads.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-gc.h>
#include <mono/metadata/environment.h>
#include <mono/jit/jit.h>


int main(int a, char** args)
{
	printf("%s", "Hi\n");
	MonoDomain* monoDomain = mono_jit_init_version("jni Root Domain", "v4.0.30319");

	// open our Example.dll assembly
	MonoAssembly* assembly = mono_domain_assembly_open(monoDomain, "/home/test/Documents/MonoEmbedded/Org.Mule.Api.Routing.dll");
	MonoImage* monoImage = mono_assembly_get_image(assembly);

	// find the Entity class in the image
	MonoClass* routerClass = mono_class_from_name(monoImage, "Org.Mule.Api.Routing", "Router");

	// allocate memory for one Entity instance
	MonoObject* routerInstance = mono_object_new(monoDomain, routerClass);
	mono_runtime_object_init(routerInstance);

	// find the Process method that takes zero parameters
	MonoMethod* processMethod = mono_class_get_method_from_name(routerClass,"Process", 1);
	
	printf("%s", "End\n");
	return 1;
}



// This is the main DLL file.

#include "stdafx.h"
#include <msclr/marshal.h>
#include "RouterCpp.h"
#include "org_mule_api_jni_Bridge.h"
#include <string>
#include "JniManager.h"

using namespace System;
using namespace System::Reflection;

static void OnOnLog(System::Object ^sender, Org::Mule::Api::Routing::MessageEventArgs ^e);
static void OnOnInstrument(System::Object ^sender, Org::Mule::Api::Routing::InstrumentationEventArgs ^e);

ref class AssemblyResolver
{
	public:
		static String^ WorkingFolder = System::IO::Path::GetDirectoryName(Assembly::GetExecutingAssembly()->Location);

		static Assembly^ MyResolveEventHandler(Object^ sender, ResolveEventArgs^ args)
		{
			AssemblyName^ assemblyName = gcnew AssemblyName(args->Name);
			String^ pathToLoadFrom = System::IO::Path::Combine(WorkingFolder, assemblyName->Name + ".dll");

			if (System::IO::File::Exists(pathToLoadFrom))
			{
				//Console::WriteLine("Loading assembly from: " + pathToLoadFrom);
				return Assembly::LoadFrom(pathToLoadFrom);
			}
			else
			{
				//Console::WriteLine("Cannot load  assembly from: " + pathToLoadFrom);
				return nullptr;
			}
		}
};

ref class GlobalObjects {
public:
	static Org::Mule::Api::Routing::Router^ router;

	static void init()
	{
		router = gcnew Org::Mule::Api::Routing::Router();
		router->OnLog += gcnew System::EventHandler<Org::Mule::Api::Routing::MessageEventArgs ^>(&OnOnLog);
		router->OnInstrument += gcnew System::EventHandler<Org::Mule::Api::Routing::InstrumentationEventArgs ^>(&OnOnInstrument);
	}
};

static JniManager* jniManager;
static jobject javaRouter;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
	try
	{
		jniManager = new JniManager(vm);
		JNIEnv* env = jniManager->getEnv();

		if (!env)
		{
			return JNI_ERR;
		}

		// attach to AssemblyResolve event
		String^ path = System::IO::Path::GetDirectoryName(Assembly::GetExecutingAssembly()->Location);

		AppDomain^ currentDomain = AppDomain::CurrentDomain;
		currentDomain->AssemblyResolve += gcnew ResolveEventHandler(AssemblyResolver::MyResolveEventHandler);

		GlobalObjects::init();
	}
	catch (Exception^ ex)
	{
		Console::WriteLine(ex->Message);
		return JNI_ERR;
	}

	return JNI_VERSION_1_6;
}

// The VM calls JNI_OnUnload when the class loader containing the native library is garbage collected.
JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved)
{
	Console::WriteLine("JNI_OnUnload called.");

	//remove global references
	jniManager->cleanup();
}

JNIEXPORT void JNICALL Java_org_mule_api_jni_Bridge_init
(JNIEnv *env, jobject obj)
{
	jniManager->setRouter(obj);
}

JNIEXPORT jobject JNICALL Java_org_mule_api_jni_Bridge_invokeNetMethod
(JNIEnv *env, jobject obj, jobject request)
{
	try
	{
		Org::Mule::Api::Routing::ProcessRequest^ processRequest = jniManager->toProcessRequest(request);

		Org::Mule::Api::Routing::ProcessRequest^ result = (Org::Mule::Api::Routing::ProcessRequest^)GlobalObjects::router->Process(processRequest);

		jobject responseInstance = jniManager->toResponseObject(result);

		return responseInstance;
	}
	catch (Exception^ ex)
	{
		//System::Console::WriteLine(ex->Message);
		jniManager->throwException(ex->Message);
	}
}

// Event callback for logging
void OnOnLog(System::Object ^sender, Org::Mule::Api::Routing::MessageEventArgs ^e)
{
	jniManager->log(e->Message);
}

// Event callback for instrumentation
void OnOnInstrument(System::Object ^sender, Org::Mule::Api::Routing::InstrumentationEventArgs ^e)
{
	jniManager->instrument(e->Message, e->Id);
}

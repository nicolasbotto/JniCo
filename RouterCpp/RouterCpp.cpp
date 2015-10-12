// This is the main DLL file.

#include "stdafx.h"
#include <msclr/marshal.h>
#include "RouterCpp.h"
#include "jni_Router.h"
#include <string>
#include "JniManager.h"

using namespace System;
using namespace System::Reflection;

void OnOnLog(System::Object ^sender, Org::Mule::Api::Routing::MessageEventArgs ^e);
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

delegate void MyCallback(String^ str);

ref class GlobalObjects {
public:
	//static Org::Mule::Api::Routing::Router^ router = gcnew Org::Mule::Api::Routing::Router();

	static void MyLoggerEvent(Object^ sender, Org::Mule::Api::Routing::MessageEventArgs^ args)
	{
		Console::WriteLine(args->Message);
	}
	static Org::Mule::Api::Routing::Router^ router;

	static void init()
	{
		router = gcnew Org::Mule::Api::Routing::Router();
		router->OnLog += gcnew System::EventHandler<Org::Mule::Api::Routing::MessageEventArgs ^>(&GlobalObjects::OnOnLog);
	}
	static void OnOnLog(System::Object ^sender, Org::Mule::Api::Routing::MessageEventArgs ^e);
};

static JniManager* jniManager;

JNIEXPORT jobject JNICALL Java_jni_Router_InvokeNetMethod
(JNIEnv *env, jobject obj, jobject request)
{
	try
	{
		Org::Mule::Api::Routing::ProcessRequest^ processRequest = jniManager->toProcessRequest(request);
		
		try
		{
			Org::Mule::Api::Routing::ProcessRequest^ result = (Org::Mule::Api::Routing::ProcessRequest^)GlobalObjects::router->Process(processRequest);
			
			jobject responseInstance = jniManager->toResponseObject(result->Result->ToString());

			return responseInstance;
		}
		catch (Exception^ ex)
		{
			System::Console::WriteLine(ex->Message);
		}

		jobject responseInstance = jniManager->toResponseObject(processRequest->AssemblyName);
		
		return responseInstance;
	
	}
	catch (Exception^ ex)
	{
		System::Console::WriteLine(ex->Message);
	}
}

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
		//Org::Mule::Api::Routing::Router^ r = GlobalObjects::router;

		//GlobalObjects::router->OnLog += gcnew System::EventHandler<Org::Mule::Api::Routing::MessageEventArgs ^>(GlobalObjects::MyLoggerEvent);
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

	//TODO REMOVE javaRouter globalreference
	//remove global references
	jniManager->cleanup();
}

static jobject javaRouter;
JNIEXPORT void JNICALL Java_jni_Router_Init
(JNIEnv *env, jobject obj)
{
	jniManager->setRouter(obj);
}

void GlobalObjects::OnOnLog(System::Object ^sender, Org::Mule::Api::Routing::MessageEventArgs ^e)
{
	jniManager->log(e->Message);
}


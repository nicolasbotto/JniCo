// This is the main DLL file.

#include "stdafx.h"
#include <msclr/marshal.h>
#include "RouterCpp.h"
#include "jni_Router.h"
#include <string>
#include "JniManager.h"

using namespace System;
using namespace System::Reflection;

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
	static Org::Mule::Api::Routing::Router^ router = gcnew Org::Mule::Api::Routing::Router();
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

//JNIEXPORT jobject JNICALL Java_jni_Router_ExecuteMethod
//(JNIEnv * env, jobject obj, jstring assemblyPath, jstring assemblyType, jstring methodName)
//{
//	String^ path = jniManager->toString(assemblyPath);
//	String^ type = jniManager->toString(assemblyType);
//	String^ method = jniManager->toString(methodName);
//
//	//System::Console::WriteLine("Parameters:");
//	//System::Console::WriteLine(path);
//	//System::Console::WriteLine(type);
//	//System::Console::WriteLine(method);
//
//	try
//	{
//		Assembly^ myAss = Assembly::LoadFrom("C:\\Users\\nico\\Documents\\visual studio 2013\\Projects\\Router\\RouterSharp\\bin\\Debug\\RouterSharp.dll");
//		Type^ myType = myAss->GetType("RouterSharp.Router");
//
//		MethodInfo^ myMethod = myType->GetMethod("ExecuteMethod");
//		Object^ instance = Activator::CreateInstance(myType);
//		array< Object^ >^ local = gcnew array< Object^ >(3);
//		local[0] = path;
//		local[1] = type;
//		local[2] = method;
//		Object^ result = myMethod->Invoke(instance, local);
//		//System::Console::WriteLine(result->ToString());
//
//		jobject responseInstance = jniManager->toResponseObject(result->ToString());
//
//		return responseInstance;
//	}
//	catch (Exception^ ex)
//	{
//		System::Console::WriteLine(ex->Message);
//	}
//
//	return env->NewStringUTF("something failed");
//}

//JNIEXPORT jint JNICALL Java_jni_Router_GetInt
//(JNIEnv * env, jobject obj)
//{
//	Assembly^ myAss = Assembly::LoadFrom("C:\\Users\\nico\\Documents\\visual studio 2013\\Projects\\Router\\RouterSharp\\bin\\Debug\\RouterSharp.dll");
//	Type^ myType = myAss->GetType("RouterSharp.Router");
//
//	MethodInfo^ myMethod = myType->GetMethod("ReturnInt");
//	Object^ instance = Activator::CreateInstance(myType);
//
//	Object^ result = myMethod->Invoke(instance, nullptr);
//
//	return (int)result;
//}
//
//JNIEXPORT jdouble JNICALL Java_jni_Router_GetDouble
//(JNIEnv * env, jobject obj)
//{
//	Assembly^ myAss = Assembly::LoadFrom("C:\\Users\\nico\\Documents\\visual studio 2013\\Projects\\Router\\RouterSharp\\bin\\Debug\\RouterSharp.dll");
//	Type^ myType = myAss->GetType("RouterSharp.Router");
//
//	MethodInfo^ myMethod = myType->GetMethod("ReturnDouble");
//	Object^ instance = Activator::CreateInstance(myType);
//
//	Object^ result = myMethod->Invoke(instance, nullptr);
//
//	return (double)result;
//}
//
//JNIEXPORT jboolean JNICALL Java_jni_Router_GetBoolean
//(JNIEnv * env, jobject obj)
//{
//	Assembly^ myAss = Assembly::LoadFrom("C:\\Users\\nico\\Documents\\visual studio 2013\\Projects\\Router\\RouterSharp\\bin\\Debug\\RouterSharp.dll");
//	Type^ myType = myAss->GetType("RouterSharp.Router");
//
//	MethodInfo^ myMethod = myType->GetMethod("ReturnBoolean");
//	Object^ instance = Activator::CreateInstance(myType);
//
//	Object^ result = myMethod->Invoke(instance, nullptr);
//
//	return (bool)result;
//}
//
//JNIEXPORT jintArray JNICALL Java_jni_Router_PassArray
//(JNIEnv * env, jobject obj, jintArray ints, jbyteArray bytes, jobjectArray strings)
//{
//	try
//	{
//		array<int>^ intArray = jniManager->toIntArray(ints);
//
//		Assembly^ myAss = Assembly::LoadFrom("C:\\Users\\nico\\Documents\\visual studio 2013\\Projects\\Router\\RouterSharp\\bin\\Debug\\RouterSharp.dll");
//		Type^ myType = myAss->GetType("RouterSharp.Router");
//
//		MethodInfo^ myMethod = myType->GetMethod("ExecuteIntArray");
//		Object^ instance = Activator::CreateInstance(myType);
//
//		array<Object^>^ local = gcnew array<Object^>(1);
//		local[0] = intArray;
//
//		Object^ result = myMethod->Invoke(instance, local);
//
//		System::Array^ arr = (System::Array^)result;
//
//		int intsSize = intArray->Length;
//
//		jint *tempArray = new jint[intsSize];
//
//		jintArray jArray = env->NewIntArray(intsSize);
//
//		for (int i = 0; i < arr->Length; i++)
//		{
//			int toC = (int)arr->GetValue(i);
//			tempArray[i] = toC;
//		}
//
//		//copy
//		env->SetIntArrayRegion(jArray, 0, intsSize, tempArray);
//
//		return jArray;
//	}
//	catch (Exception^ ex)
//	{
//		System::Console::WriteLine(ex->Message);
//	}
//}
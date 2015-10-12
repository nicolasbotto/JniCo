//using net.sf.jni4net;
//using net.sf.jni4net.jni;
//using System;
//using System.Collections.Generic;
//using System.Linq;
//using System.Text;

//namespace Org.Mule.Api.Routing.Jni
//{
//    public static class JniHelper
//    {
//        public static java.lang.Object ToJavaObject(this object systemObject)
//        {
//            if (systemObject is int)
//            {
//                return new java.lang.Integer((int)systemObject);
//            }
//            else if (systemObject is double)
//            {
//                return new java.lang.Double((double)systemObject);
//            }
//            else if (systemObject is char)
//            {
//                return new java.lang.Character((char)systemObject);
//            }
//            else if (systemObject is short || systemObject is ushort)
//            {
//                return new java.lang.Short((short)systemObject);
//            }
//            else if (systemObject is float)
//            {
//                return new java.lang.Float((float)systemObject);
//            }
//            else if (systemObject is long || systemObject is ulong)
//            {
//                return new java.lang.Long((long)systemObject);
//            }
//            else if (systemObject is byte || systemObject is sbyte)
//            {
//                return new java.lang.Byte((byte)systemObject);
//            }
//            else if (systemObject is bool)
//            {
//                return new java.lang.Boolean((bool)systemObject);
//            }
//            else if (systemObject is string)
//            {
//                return new java.lang.String((string)systemObject);
//            }
//            else if (systemObject.GetType().IsArray)
//            {
//                //return java.nio.ByteBuffer.wrap((byte[])systemObject);
//                java.lang.Object result = net.sf.jni4net.jni.JNIEnv.ThreadEnv.GetArray(systemObject);
//                return result;
//            }

//            return Bridge.WrapCLR(systemObject);
//        }
//        public static object ToSystemObject(this java.lang.Object jObject)
//        {
//            object item = null;
//            if (jObject == null || jObject.getClass().FullName.Equals("org.mule.transport.NullPayload", StringComparison.InvariantCultureIgnoreCase))
//            {
//                return null;
//            }

//            if (jObject is java.util.Date)
//            {
//                item = DateTime.Parse(((java.util.Date)jObject).toGMTString());
//            }
//            else if (jObject is java.lang.Number)
//            {
//                java.lang.Number jNumber = (java.lang.Number)jObject;
//                java.lang.Class jClass = jNumber.getClass();

//                if (jClass == java.lang.Integer._class)
//                {
//                    item = jNumber.intValue();
//                }
//                else if (jClass == java.lang.Short._class)
//                {
//                    item = jNumber.shortValue();
//                }
//                else if (jClass == java.lang.Byte._class)
//                {
//                    item = jNumber.byteValue();
//                }
//                else if (jClass == java.lang.Float._class)
//                {
//                    item = jNumber.floatValue();
//                }
//                else if (jClass == java.lang.Double._class)
//                {
//                    item = jNumber.doubleValue();
//                }
//                else if (jClass == java.lang.Long._class)
//                {
//                    item = jNumber.longValue();
//                }
//            }
//            else if (jObject is java.lang.Boolean)
//            {
//                item = ((java.lang.Boolean)jObject).booleanValue();
//            }
//            else if (jObject is java.lang.Character)
//            {
//                item = ((java.lang.Character)jObject).charValue();
//            }
//            else if (jObject is java.lang.String)
//            {
//                item = jObject.ToString();
//            }
//            else
//            {
//                // Handle array of primitive types and string array
//                var className = NormalizeClassName(jObject.getClass().ToString());

//                switch (className)
//                {
//                    case "[Ljava.lang.Object;":
//                        var jObjectArray = net.sf.jni4net.utils.Convertor.ArrayFullJ2CGlobal<java.lang.Object[], java.lang.Object>(JNIEnv.ThreadEnv, ((IJvmProxy)jObject).JvmHandle);
//                        item = jObjectArray.Select(x => x.ToSystemObject()).ToArray();
//                        break;
//                    case "[Ljava.lang.String;":
//                        item = net.sf.jni4net.utils.Convertor.ArrayStrongJ2CStringGlobal(JNIEnv.ThreadEnv, ((IJvmProxy)jObject).JvmHandle);
//                        break;
//                    case "[[Ljava.lang.String;":
//                        item = net.sf.jni4net.utils.Convertor.ArrayStrongJ2CStringGlobalTwoDimension(JNIEnv.ThreadEnv, ((IJvmProxy)jObject).JvmHandle);
//                        break;
//                    case "[Z":
//                        var boolArray = net.sf.jni4net.utils.Convertor.ArrayPrimJ2CbooleanGlobal(JNIEnv.ThreadEnv, ((IJvmProxy)jObject).JvmHandle);
//                        item = boolArray;
//                        break;
//                    case "[[Z":
//                        item = net.sf.jni4net.utils.Convertor.ArrayPrimJ2CTwoDimensionBoolArray(JNIEnv.ThreadEnv, ((IJvmProxy)jObject).JvmHandle);
//                        break;
//                    case "[B":
//                        item = net.sf.jni4net.utils.Convertor.ArrayPrimJ2CbyteGlobal(JNIEnv.ThreadEnv, ((IJvmProxy)jObject).JvmHandle);
//                        break;
//                    case "[[B":
//                        item = net.sf.jni4net.utils.Convertor.ArrayPrimJ2CTwoDimensionArray(JNIEnv.ThreadEnv, ((IJvmProxy)jObject).JvmHandle);
//                        break;
//                    case "[C":
//                        item = net.sf.jni4net.utils.Convertor.ArrayPrimJ2CcharGlobal(JNIEnv.ThreadEnv, ((IJvmProxy)jObject).JvmHandle);
//                        break;
//                    case "[[C":
//                        item = net.sf.jni4net.utils.Convertor.ArrayPrimJ2CTwoDimensionCharArray(JNIEnv.ThreadEnv, ((IJvmProxy)jObject).JvmHandle);
//                        break;
//                    case "[D":
//                        item = net.sf.jni4net.utils.Convertor.ArrayPrimJ2CdoubleGlobal(JNIEnv.ThreadEnv, ((IJvmProxy)jObject).JvmHandle);
//                        break;
//                    case "[[D":
//                        item = net.sf.jni4net.utils.Convertor.ArrayPrimJ2CTwoDimensionDoubleArray(JNIEnv.ThreadEnv, ((IJvmProxy)jObject).JvmHandle);
//                        break;
//                    case "[F":
//                        item = net.sf.jni4net.utils.Convertor.ArrayPrimJ2CfloatGlobal(JNIEnv.ThreadEnv, ((IJvmProxy)jObject).JvmHandle);
//                        break;
//                    case "[[F":
//                        item = net.sf.jni4net.utils.Convertor.ArrayPrimJ2CTwoDimensionFloatArray(JNIEnv.ThreadEnv, ((IJvmProxy)jObject).JvmHandle);
//                        break;
//                    case "[I":
//                        item = net.sf.jni4net.utils.Convertor.ArrayPrimJ2CintGlobal(JNIEnv.ThreadEnv, ((IJvmProxy)jObject).JvmHandle);
//                        break;
//                    case "[[I":
//                        item = net.sf.jni4net.utils.Convertor.ArrayPrimJ2CTwoDimensionIntArray(JNIEnv.ThreadEnv, ((IJvmProxy)jObject).JvmHandle);
//                        break;
//                    case "[J":
//                        item = net.sf.jni4net.utils.Convertor.ArrayPrimJ2ClongGlobal(JNIEnv.ThreadEnv, ((IJvmProxy)jObject).JvmHandle);
//                        break;
//                    case "[[J":
//                        item = net.sf.jni4net.utils.Convertor.ArrayPrimJ2CTwoDimensionLongArray(JNIEnv.ThreadEnv, ((IJvmProxy)jObject).JvmHandle);
//                        break;
//                    case "[S":
//                        item = net.sf.jni4net.utils.Convertor.ArrayPrimJ2CshortGlobal(JNIEnv.ThreadEnv, ((IJvmProxy)jObject).JvmHandle);
//                        break;
//                    case "[[S":
//                        item = net.sf.jni4net.utils.Convertor.ArrayPrimJ2CTwoDimensionShortArray(JNIEnv.ThreadEnv, ((IJvmProxy)jObject).JvmHandle);
//                        break;
//                    case "java.util.UUID":
//                        item = Guid.Parse(jObject.toString());
//                        break;
//                    default: // cannot cast then return null
//                        break;
//                }
//            }

//            return item;
//        }

//        public static object[] ToObjectArray(this java.util.List data)
//        {
//            var size = data.size();
//            var result = new object[size];

//            for (int i = 0; i < size; i++)
//            {
//                var item = data.get(i).ToSystemObject();

//                if(item != null)
//                {
//                    result[i] =  item;
//                }
//            }

//            return result;
//        }

//        public static java.util.Hashtable ToJavaMap(this Dictionary<string, object> data)
//        {
//            var result = new java.util.Hashtable();

//            foreach (var item in data)
//            {
//                result.put(item.Key.ToJavaObject(), item.Value.ToJavaObject());
//            }

//            return result;
//        }

//        public static Dictionary<string, object> ToSystemObjectDictionary(this java.util.Map data)
//        {
//            var size = data.size();
//            var result = new Dictionary<string, object>();
//            var keys = data.keySet().toArray().OfType<java.lang.String>();

//            foreach (var item in keys)
//            {
//                var obj = data.get(item).ToSystemObject();

//                if(obj != null)
//                { 
//                    result.Add(item, obj);
//                }
//            }

//            return result;
//        }

//        public static Dictionary<string, string> ToSystemStringDictionary(this java.util.Map data)
//        {
//            var size = data.size();
//            var result = new Dictionary<string, string>();
//            var keys = data.keySet().toArray().OfType<java.lang.String>();

//            foreach (var item in keys)
//            {
//                var obj = data.get(item).ToString();
//                result.Add(item, obj);
//            }

//            return result;
//        }

//        private static string NormalizeClassName(string className)
//        {
//            return className.Replace("class ", "");
//        }
//    }
//}

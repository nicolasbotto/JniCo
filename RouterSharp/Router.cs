using RouterSDK;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RouterSharp
{
    public class Router
    {
        public object ExecuteMethod(ProcessRequest request)
        {
            StringBuilder sb = new StringBuilder();

            foreach (var item in request.MethodArguments)
            {
                sb.AppendFormat("{0} : {1}{2}", item.Key, item.Value, Environment.NewLine);
            }

            sb.AppendFormat("{0} : {1}{2}", "AssemblyName", request.AssemblyName, Environment.NewLine);
            sb.AppendFormat("{0} : {1}{2}", "AssemblyPath", request.AssemblyPath, Environment.NewLine);
            sb.AppendFormat("{0} : {1}{2}", "FullTrust", request.FullTrust, Environment.NewLine);
            sb.AppendFormat("{0} : {1}{2}", "IsSingleton", request.IsSingleton, Environment.NewLine);
            sb.AppendFormat("{0} : {1}{2}", "Log", request.Log, Environment.NewLine);
            sb.AppendFormat("{0} : {1}{2}", "MethodName", request.MethodName, Environment.NewLine);
            sb.AppendFormat("{0} : {1}{2}", "NotifyEvents", request.NotifyEvents, Environment.NewLine);
            
            return sb.ToString();
        }

        public int[] ExecuteIntArray(int[] data)
        {
            for (int i = 0; i < data.Length; i++)
            {
                data[i] = data[i] * 2;
            }

            return data;
        }

        public object ReturnInt()
        {
            return 5;
        }

        public double ReturnDouble()
        {
            return 2.4;
        }

        public bool ReturnBoolean()
        {
            return true;
        }
    }
}

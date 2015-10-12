using System.Collections.Generic;
namespace Org.Mule.Api.Routing
{
    public class ProcessRequest
    {
        public string AssemblyName { get; set; }
        public string AssemblyPath { get; set; }
        public string MethodName { get; set; }
        public bool Log { get; set; }
        public bool FullTrust { get; set; }
        public bool IsSingleton { get; set; }
        public Dictionary<string, object> MethodArguments { get; set; }
        public Dictionary<string, object> InboundProperties { get; set; }
        public Dictionary<string, object> SessionProperties { get; set; }
        public Dictionary<string, object> OutboundProperties { get; set; }
        public Dictionary<string, object> InvocationProperties { get; set; }
        public object Result { get; set; }
        public bool NotifyEvents { get; set; }
    }
}

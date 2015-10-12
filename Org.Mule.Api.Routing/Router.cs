using Org.Mule.Api.Component;
using System;
using System.Collections.Concurrent;
using System.Collections.Specialized;
using System.IO;
using System.Reflection;
using System.Threading;
using System.Web;

namespace Org.Mule.Api.Routing
{
    public class MessageEventArgs : EventArgs
    {
        public string Message { get; set; }
    }

    public class Router
    {
        private ComponentManager componentManager;
        private string componentPath;
        private ConcurrentDictionary<string, ComponentManager> appDomains;
        private MuleLogger logger;
        public event EventHandler<MessageEventArgs> OnLog;

        public Router()
        {
            appDomains = new ConcurrentDictionary<string, ComponentManager>();
            componentPath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            componentManager = new ComponentManager();
            logger = new MuleLogger(Log);
        }

        private void DoOnLog(string message)
        {
            EventHandler<MessageEventArgs> handler = OnLog;
            if (handler != null)
            {
                var args = new MessageEventArgs() { Message = message };
                handler(this, args);
            }
        }

        public void Log(string message)
        {
            DoOnLog(message);
        }

        public object Process(ProcessRequest request)
        {
            var assembly = string.Empty;
            var typeName = string.Empty;
            var methodName = string.Empty;
            var appDomainKey = string.Empty;

            MuleMessage result;
            MuleMessage muleMessage = new MuleMessage();
            IMuleInstrumentation instrumentation = new MuleInstrumentation(new Notification(), request.NotifyEvents);

            if (string.IsNullOrWhiteSpace(request.AssemblyName))
            {
                throw new ArgumentNullException("assemblyName");
            }

            var split = request.AssemblyName.Split(new[] { ',' }, StringSplitOptions.RemoveEmptyEntries);

            if (split.Length == 0)
            {
                throw new ArgumentException("Invalid .NET Assembly Type. The .NET Assembly Type should be: partially qualified \"Namespace.Type, AssemblyName\" or fully qualified name \"Namespace.Type, AssemblyName, [Version], [Culture], [PublicKey]\"");
            }

            typeName = split[0].Trim();

            if (split.Length == 2)
            {
                assembly = split[1].Trim();
                appDomainKey = string.Format("{0}_{1}", request.AssemblyPath, assembly);
            }
            else // remove type from assembly name
            {
                assembly = request.AssemblyName.Substring(request.AssemblyName.IndexOf(",") + 1).Trim();
                appDomainKey = assembly;
            }

            if (string.IsNullOrWhiteSpace(request.MethodName))
            {
                throw new ArgumentNullException("The method name cannot be empty.");
            }

            // add trust to key
            appDomainKey = string.Concat(appDomainKey, request.FullTrust);

            // add timestamp to assembly not in GAC
            if (request.AssemblyPath.EndsWith(".dll"))
            {
                appDomainKey = string.Concat(appDomainKey, "_", File.GetLastWriteTime(request.AssemblyPath).Ticks.ToString());
            }

            methodName = request.MethodName;

            muleMessage.Arguments = request.MethodArguments;
            muleMessage.InboundProperties = new ReadOnlyDictionary<string, object>(request.InboundProperties);
            muleMessage.SessionProperties = request.SessionProperties;
            muleMessage.InvocationProperties = request.InvocationProperties;
            muleMessage.OutboundProperties = request.OutboundProperties;
            var requestPath = muleMessage.InboundProperties.ContainsKey(MuleMessage.RequestKey) ? muleMessage.InboundProperties[MuleMessage.RequestKey] : string.Empty;
            muleMessage.Payload = muleMessage.Arguments.ContainsKey(MuleMessage.DataKey) ? muleMessage.Arguments[MuleMessage.DataKey] : requestPath;

            var queryString = muleMessage.InboundProperties.ContainsKey("http.query.string") ?
                muleMessage.InboundProperties["http.query.string"].ToString().Replace("{", "").Replace("}", "").Replace(" ", "").Replace(",", "&") :
                string.Empty;

            muleMessage.QueryStrings = string.IsNullOrWhiteSpace(queryString) ? new NameValueCollection() : HttpUtility.ParseQueryString(queryString);

            if (string.IsNullOrWhiteSpace(request.AssemblyPath))
            {
                request.AssemblyPath = componentPath;
            }

            componentManager = appDomains.GetOrAdd(appDomainKey,
                         (key) =>
                         {
                             var cm = new ComponentManager();
                             cm.AppDomainKey = appDomainKey;

                             //Bypass assemblies installed in the GAC
                             if (request.AssemblyPath.EndsWith(".dll"))
                             {
                                 // Add FileSystemWatcher to watch any updates on the requested assembly, if changes occur, unload AppDoamin and create a new one
                                 var watcher = new FileSystemWatcher();
                                 watcher.Path = Path.GetDirectoryName(request.AssemblyPath);
                                 watcher.NotifyFilter = NotifyFilters.LastWrite;
                                 watcher.Filter = "*.dll";
                                 watcher.Changed += (o, e) =>
                                 {
                                     var componentWatcher = cm;

                                     watcher.EnableRaisingEvents = false;
                                     var retry = 0;
                                     var maxRetries = 10;
                                     // wait until AppDomain is idle to unload it
                                     while (componentWatcher.State != ComponentState.Idle && retry < maxRetries)
                                     {
                                         Thread.Sleep(15000);
                                         retry++;
                                     }

                                     AppDomain.Unload(componentWatcher.LoaderDomain);
                                     appDomains.TryRemove(appDomainKey, out componentWatcher);

                                     if (retry >= maxRetries)
                                     {
                                         throw new Exception(string.Format("The AppDomain couldn't be unloaded normally after {0} attempts.", retry));
                                     }
                                 };
                                 watcher.EnableRaisingEvents = true;
                             }

                             cm.Init(componentPath, request.AssemblyPath, request.FullTrust, logger);

                             return cm;
                         });

            try
            {
                // Execute method
                string assemblyName;
                // Check if it's an assembly in the GAC or one specified by the user
                if (request.AssemblyPath.EndsWith(".dll"))
                {
                    assemblyName = AssemblyName.GetAssemblyName(request.AssemblyPath).FullName;
                }
                else
                {
                    assemblyName = assembly;
                }

                if(request.Log)
                { 
                    Log(string.Format("Start execution of component: {0}.{1}", typeName, methodName));
                }

                //logger.Write(string.Format("Start execution of component: {0}.{1}", typeName, methodName), LoggerLevel.Trace);

                result = (MuleMessage)componentManager.Execute(assemblyName, typeName, methodName, muleMessage, request.IsSingleton, instrumentation);

                //logger.Write("End execution {0}.{1}", LoggerLevel.Trace, typeName, methodName);
                if(request.Log)
                { 
                    Log(string.Format("End execution {0}.{1}", typeName, methodName));
                }
            }
            catch (Exception ex)
            {
                Log(string.Format("Error executing component {0}.{1} : {2}", typeName, methodName, ex.Message));
                throw;
            }

            return ToProcessRequest(result);
        }

        private ProcessRequest ToProcessRequest(MuleMessage message)
        {
            object payload = message.Payload;

            //if (message.Payload != null)
            //{
            //    if (message.Payload.GetType() == typeof(byte[]))
            //    {
            //        payload = java.nio.ByteBuffer.wrap((byte[])message.Payload);
            //    }
            //    else
            //    {
            //        payload = Bridge.WrapCLR(message.Payload);
            //    }
            //}

            return new ProcessRequest()
            {
                Result = payload,
                SessionProperties = message.SessionProperties,
                InvocationProperties = message.InvocationProperties,
                OutboundProperties = message.OutboundProperties
            };
        }
    }
}
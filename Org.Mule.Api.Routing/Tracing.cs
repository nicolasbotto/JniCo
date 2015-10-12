using System;
using System.Configuration;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Reflection;

namespace Org.Mule.Api.Routing
{
    public static class Tracing
    {
        private const string ConfigurationFileName = "Org.Mule.Api.Routing.dll.config";
        private const string LogFileName = "Org.Mule.Api.Routing.log";
        private static TraceSource _ts = new TraceSource("Org.Mule.Api.Routing");

        static Tracing()
        {
            var executingPath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            var logPath = Path.Combine(executingPath, LogFileName);

            var configPath = Path.Combine(executingPath, ConfigurationFileName);

            if (File.Exists(configPath))
            {
                var fileMap = new ExeConfigurationFileMap();
                fileMap.ExeConfigFilename = configPath;

                var configuration = ConfigurationManager.OpenMappedExeConfiguration(fileMap, ConfigurationUserLevel.None);

                if (configuration.AppSettings != null &&
                    configuration.AppSettings.Settings.AllKeys.Contains("logPath") && 
                    !string.IsNullOrWhiteSpace(configuration.AppSettings.Settings["logPath"].Value))
                {
                    logPath = configuration.AppSettings.Settings["logPath"].Value;
                }
            }

            Trace.AutoFlush = true;
            _ts.Listeners.Clear();
            _ts.Listeners.Add(new TextWriterTraceListener(logPath) { TraceOutputOptions = TraceOptions.DateTime });
            _ts.Switch = new SourceSwitch("DotNetModule") { Level = SourceLevels.All };
        }

        [DebuggerStepThrough]
        public static void Start(string message)
        {
            TraceEvent(TraceEventType.Start, message);
        }

        [DebuggerStepThrough]
        public static void Stop(string message)
        {
            TraceEvent(TraceEventType.Stop, message);
        }

        [DebuggerStepThrough]
        public static void Information(string message)
        {
            TraceEvent(TraceEventType.Information, message);
        }

        [DebuggerStepThrough]
        public static void InformationFormat(string message, params object[] objects)
        {
            TraceEventFormat(TraceEventType.Information, message, objects);
        }

        [DebuggerStepThrough]
        public static void Warning(string message)
        {
            TraceEvent(TraceEventType.Warning, message);
        }

        [DebuggerStepThrough]
        public static void WarningFormat(string message, params object[] objects)
        {
            TraceEventFormat(TraceEventType.Warning, message, objects);
        }

        [DebuggerStepThrough]
        public static void Error(string message)
        {
            TraceEvent(TraceEventType.Error, message);
        }

        //[DebuggerStepThrough]
        //public static void ErrorVerbose(string message, [CallerMemberName] string memberName = "", [CallerFilePath] string filePath = "", [CallerLineNumber] int lineNumber = 0)
        //{
        //    TraceEventFormat(TraceEventType.Error, "{0}\n\nMethod: {1}\nFilename: {2}\nLine number: {3}", message, memberName, filePath, lineNumber);
        //}

        [DebuggerStepThrough]
        public static void ErrorFormat(string message, params object[] objects)
        {
            TraceEventFormat(TraceEventType.Error, message, objects);
        }

        [DebuggerStepThrough]
        public static void Verbose(string message)
        {
            TraceEvent(TraceEventType.Verbose, message);
        }

        [DebuggerStepThrough]
        public static void VerboseFormat(string message, params object[] objects)
        {
            TraceEventFormat(TraceEventType.Verbose, message, objects);
        }

        [DebuggerStepThrough]
        public static void Transfer(string message, Guid activity)
        {
            _ts.TraceTransfer(0, message, activity);
        }

        [DebuggerStepThrough]
        public static void TraceEventFormat(TraceEventType type, string message, params object[] objects)
        {
            var format = string.Format(message, objects);
            TraceEvent(type, format);
        }

        [DebuggerStepThrough]
        public static void TraceEvent(TraceEventType type, string message)
        {
            if (Trace.CorrelationManager.ActivityId == Guid.Empty)
            {
                Trace.CorrelationManager.ActivityId = Guid.NewGuid();
            }

            _ts.TraceEvent(type, 0, message);
        }
    }
}
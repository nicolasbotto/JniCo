using System;
using System.Globalization;

namespace Org.Mule.Api.Routing
{
    public class MuleLogger : MarshalByRefObject, IMuleLogger
    {
        private Action<string> log;

        public MuleLogger(Action<string> log)
        {
            this.log = log;
        }

        public void Write(string message)
        {
            //this.internalLogger.debug(message);
            this.log(message);
        }

        public void Write(string format, params object[] args)
        {
            //this.internalLogger.debug(string.Format(CultureInfo.InvariantCulture, format, args));
            this.log(string.Format(CultureInfo.InvariantCulture, format, args));
        }

        public void Write(string message, LoggerLevel level)
        {
            this.log(message);
            //switch (level)
            //{
            //    case LoggerLevel.Debug:
            //        this.internalLogger.debug(message);
            //        break;
            //    case LoggerLevel.Info:
            //        this.internalLogger.info(message);
            //        break;
            //    case LoggerLevel.Trace:
            //        this.internalLogger.trace(message);
            //        break;
            //    default:
            //        this.internalLogger.error(message);
            //        break;
            //}
        }


        public void Write(string format, LoggerLevel level, params object[] args)
        {
            this.log(string.Format(CultureInfo.InvariantCulture, format, args));
            //switch (level)
            //{
            //    case LoggerLevel.Debug:
            //        this.internalLogger.debug(string.Format(CultureInfo.InvariantCulture, format, args));
            //        break;
            //    case LoggerLevel.Info:
            //        this.internalLogger.info(string.Format(CultureInfo.InvariantCulture, format, args));
            //        break;
            //    case LoggerLevel.Trace:
            //        this.internalLogger.trace(string.Format(CultureInfo.InvariantCulture, format, args));
            //        break;
            //    default:
            //        this.internalLogger.error(string.Format(CultureInfo.InvariantCulture, format, args));
            //        break;
            //}
        }

        public override object InitializeLifetimeService()
        {
            // ensure the remote object never dies (lease never expires) therefore it won't GC'd avoiding a disconnect remoting exception.
            return null;
        }
    }
}
using System;
using System.Globalization;

namespace Org.Mule.Api.Routing
{
    public class MuleInstrumentation : MarshalByRefObject, IMuleInstrumentation
    {
        private Notification internalNotification;
        private bool shouldInstrument;
        public MuleInstrumentation(Notification internalNotification, bool shouldInstrument)
        {
            this.internalNotification = internalNotification;
            this.shouldInstrument = shouldInstrument;
        }

        public void FireEvent(MuleNotificationEvent muleNotification, string message)
        {
            if (shouldInstrument)
            {
                this.internalNotification.fireEvent(message, (int)muleNotification);
            }
        }

        public void FireEvent(MuleNotificationEvent muleNotification, string format, params object[] args)
        {
            if (shouldInstrument)
            {
                this.internalNotification.fireEvent(string.Format(CultureInfo.InvariantCulture, format, args), (int)muleNotification);
            }
        }

        public override object InitializeLifetimeService()
        {
            // ensure the remote object never dies (lease never expires) therefore it won't GC'd avoiding a disconnect remoting exception.
            return null;
        }
    }

    // remove
    public class Notification
    {
        public void fireEvent(string message, int id)
        {
        }
    }
}

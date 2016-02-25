using System;
using System.Globalization;

namespace Org.Mule.Api.Routing
{
    public class MuleInstrumentation : MarshalByRefObject, IMuleInstrumentation
    {
        private Action<string, int> instrument;
        private bool shouldInstrument;
        public MuleInstrumentation(Action<string, int> instrument, bool shouldInstrument)
        {
            this.instrument = instrument;
            this.shouldInstrument = shouldInstrument;
        }

        public void FireEvent(MuleNotificationEvent muleNotification, string message)
        {
            if (shouldInstrument)
            {
                this.instrument(message, (int)muleNotification);
            }
        }

        public void FireEvent(MuleNotificationEvent muleNotification, string format, params object[] args)
        {
            if (shouldInstrument)
            {
                this.instrument(string.Format(CultureInfo.InvariantCulture, format, args), (int)muleNotification);
            }
        }

        public override object InitializeLifetimeService()
        {
            // ensure the remote object never dies (lease never expires) therefore it won't GC'd avoiding a disconnect remoting exception.
            return null;
        }
    }
}

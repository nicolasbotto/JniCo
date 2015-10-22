package org.mule.api.jni;

import java.io.File;
import java.io.FilenameFilter;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

public class Bridge
{
	private JniLogger logger;
	private Object instrumentationManager;
	private Method fireEventMethod = null;

	// Native methods
	private native void init();
	public native Response invokeNetMethod(Request request);

	public void initJni(String path)
	{
		// Do version check to load 32 or 64 bit version
		System.load(path);
		init();
	}

	public void setInstrumentationManager(Object instrumentationManager) throws IOException, NoSuchMethodException
	{
		if(instrumentationManager != null) {
			this.instrumentationManager = instrumentationManager;

			if(fireEventMethod == null) {
				fireEventMethod = this.instrumentationManager.getClass().getMethod("fireEvent", new Class[] { String.class, Integer.class });
			}
		}
	}

	public void setLogger(JniLogger logger)
	{
	    this.logger = logger;
	}
	public void log(String message)
	{
		if(this.logger != null) {
			this.logger.debug(message);
		}
	}

	public void instrument(String message, int id) throws InterruptedException, SecurityException, IllegalAccessException, IllegalArgumentException, InvocationTargetException {

		if(fireEventMethod != null)
		{
			fireEventMethod.invoke(this.instrumentationManager, new Object[] { message, id});
		}
	}

	private String getRuntime()
	{
		String sysRoot = System.getenv("SystemRoot");
		if (sysRoot == null || sysRoot.equals("")) {
			sysRoot = "c:/Windows";
		}

		File d = new File(sysRoot, "Microsoft.NET/Framework/");
		final String[] vers = d.list(new FilenameFilter()
		{
			public boolean accept(File dir, String name) {
				return name.startsWith("v4.0.");
			}
		});

		if (vers != null && vers.length > 0)
		{
			return "v40";
		} else
		{
			return "v20";
		}
	}
}
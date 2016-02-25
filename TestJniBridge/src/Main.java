import org.mule.api.jni.*;

import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.Map;

/**
 * Created by nico on 16/10/2015.
 */
public class Main {

    private static final Bridge BRIDGE;

    static {
        BRIDGE = new Bridge();

        //String path = "C:/Users/nico/AnypointStudio/workspace/.mule/tmp/testjni/plugins/dotnet-connector-2.3.0-SNAPSHOT/lib/JniBridge.dll";
        //String path = "D:\\Work\\MuleFlow\\Sdk\\JniBridge\\x64\\Debug\\JniBridge.dll";
        //String path = "D:\\Work\\MuleFlow\\src\\main\\resources\\JniBridge.dll";
        String path = "D:\\MyStuff\\JniCo\\Sdk\\JniBridge\\x64\\Debug\\JniBridge64.dll";
        BRIDGE.initJni(path);
        try {
            BRIDGE.setInstrumentationManager(new Notification());
        } catch (IOException e) {
            e.printStackTrace();
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        }
    }

    public static void main(String args[]) {

        Request request = new Request();

        request.setAssemblyName("Test.SampleComponent.Sample, Test.SampleComponent");
        request.setAssemblyPath("D:\\Work\\MuleFlow\\Samples\\CustomComponent\\CS\\Test.SampleComponent\\bin\\Debug\\Test.SampleComponent.dll");

        request.setFullTrust(true);
        request.setIsSingleton(false);
        request.setLog(false);
        request.setNotifyEvents(false);

        Map<String, Object> arguments2 = new HashMap<String, Object>();

        arguments2.put("intArray", new int[]{1, 2, 3, 4});
        arguments2.put("doubleArray", new double[]{1.1, 2.2, 3.3, 4.4});
        arguments2.put("booleanArray", new boolean[]{true, false});
        arguments2.put("byteArray", new byte[]{1, 2});
        arguments2.put("shortArray", new short[]{(short) 1, (short) 2});
        arguments2.put("charArray", new char[]{'a', 'b'});
        arguments2.put("floatArray", new float[]{1.5f, 2.5f, 3.5f, 4.5f});
        arguments2.put("stringArray", new String[]{"nico", "las", "from Java!!!"});
        arguments2.put("longArray", new long[]{(long) 1000, (long) 1001, (long) 1002});


        request.setMethodName("TestArrays(System.Int32[] intArray, System.Byte[] byteArray, System.Double[] doubleArray, System.Char[] charArray, System.Single[] floatArray, System.Boolean[] booleanArray, System.Int64[] longArray, System.String[] stringArray) -&gt; System.String");
        request.setMethodArguments(arguments2);

        Response response = BRIDGE.invokeNetMethod(request);

        Object payload = response.getPayload();
        if (payload != null) {
            if (payload instanceof String) {
                System.out.println(payload.toString());
            } else {
                byte[] bytes = (byte[]) payload;

                String s = new String(bytes);

                System.out.println(s);
            }
        }



        // request.setMethodName("GetCount() -&gt; System.Void");

        // String data = "Bytes from Java...";

        //arguments2.put("data", data.getBytes());
        // request.setMethodName("TestMuleMessageProperties()");
        //request.setMethodArguments(arguments2);


        /*for (int i = 0; i < 200; i++) {
            new Thread("T"+i+"") {
                @Override
                public void run() {
                    Request request = new Request();

                    request.setAssemblyName("Org.Mule.Api.Metadata.MetadataHelper, Org.Mule.Api.Metadata");
                    request.setAssemblyPath("D:\\Work\\MuleFlow\\src\\main\\resources\\Org.Mule.Api.Metadata.dll");
                    request.setFullTrust(true);
                    request.setIsSingleton(false);
                    request.setLog(false);
                    request.setNotifyEvents(false);

                    request.setMethodName("GetTypeMetadata(System.String type, System.Boolean onlyDeclared, System.Boolean includeAutoGenerated)");

                    Map<String, Object> arguments2 = new HashMap<String, Object>();
                    arguments2.put("type", "D:\\Work\\MuleFlow\\Samples\\CustomComponent\\CS\\Test.SampleComponent\\bin\\Debug\\Test.SampleComponent.dll");
                    arguments2.put("onlyDeclared", true);
                    arguments2.put("includeAutoGenerated", true);
                    request.setMethodArguments(arguments2);

                    Response response = BRIDGE.invokeNetMethod(request);

                    Object payload = response.getPayload();
                    if (payload != null) {
                        if (payload instanceof String) {
                            System.out.println(getName() + " : " + payload.toString());
                        } else {
                            byte[] bytes = (byte[]) payload;

                            String s = new String(bytes);

                            System.out.println(s);
                        }
                    }
                }


            }.start();
        }*/
    }


        /*
        Map<String, Object> invocation = response.getInvocationProperties();
        System.out.println("Invocation:");
        for (Map.Entry<String, Object> entry : invocation.entrySet()) {
            if(entry.getValue() instanceof byte[])
            {
                System.out.println(entry.getKey() + " : " + new String((byte[])entry.getValue()).toString());
            }

            if(entry.getValue() instanceof int[])
            {
                System.out.println(entry.getKey());
                int[] array = (int[])entry.getValue();
                for (int i = 0; i < array.length; i++) {
                    System.out.println(array[i]);
                }
            }

            if(entry.getValue() instanceof long[])
            {
                System.out.println(entry.getKey());
                long[] array = (long[])entry.getValue();
                for (int i = 0; i < array.length; i++) {
                    System.out.println(array[i]);
                }
            }

            if(entry.getValue() instanceof short[])
            {
                System.out.println(entry.getKey());
                short[] array = (short[])entry.getValue();
                for (int i = 0; i < array.length; i++) {
                    System.out.println(array[i]);
                }
            }

            if(entry.getValue() instanceof char[])
            {
                System.out.println(entry.getKey());
                char[] array = (char[])entry.getValue();
                for (int i = 0; i < array.length; i++) {
                    System.out.println(array[i]);
                }
            }

            if(entry.getValue() instanceof boolean[])
            {
                System.out.println(entry.getKey());
                boolean[] array = (boolean[])entry.getValue();
                for (int i = 0; i < array.length; i++) {
                    System.out.println(array[i]);
                }
            }

            if(entry.getValue() instanceof double[])
            {
                System.out.println(entry.getKey());
                double[] array = (double[])entry.getValue();
                for (int i = 0; i < array.length; i++) {
                    System.out.println(array[i]);
                }
            }

            if(entry.getValue() instanceof float[])
            {
                System.out.println(entry.getKey());
                float[] array = (float[])entry.getValue();
                for (int i = 0; i < array.length; i++) {
                    System.out.println(array[i]);
                }
            }

        }

        Map<String, Object> session = response.getSessionProperties();
        System.out.println("Session:");
        for (Map.Entry<String, Object> entry : session.entrySet()) {
            System.out.println(entry.getKey() + " : " + entry.getValue().toString());
        }

        Map<String, Object> outbound = response.getOutboundProperties();
        System.out.println("Outbound:");
        for (Map.Entry<String, Object> entry : outbound.entrySet()) {
            System.out.println(entry.getKey() + " : " + entry.getValue().toString());
        }
*/
            //}


}

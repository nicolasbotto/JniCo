CFILES = TypeConverter.c JniManager.c RouterCpp.c

all: g++ 

clean:
	rm -f *.o *.obj *.so* *.dll *.exe *.pdb *.exp *.lib sample


g++:
	g++ `pkg-config --cflags --libs mono-2` -fPIC -c \
		-I /usr/lib/jvm/java-1.7.0-openjdk-amd64/include \
		$(CFILES) -g
	g++ `pkg-config --cflags --libs mono-2` --shared -I /usr/lib/jvm/java-1.7.0-openjdk-amd64/include \
		-L /usr/lib/jvm/java-7-openjdk-amd64/jre/lib/amd64/server -ljsig -ljvm \
		TypeConverter.o JniManager.o RouterCpp.o -o JniBridge.so -lmono-2.0 -g


# adapt.o: adapt.c 
# 	gcc adapt.c -c adapt.o
adapt: adapt.c lws2_32 liphlpapi
	gcc adapt.c -o adapt.exe -lws2_32 -liphlpapi
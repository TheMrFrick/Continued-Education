all: WTF WTFserver

WTF: WTFclient.c
	gcc WTFclient.c -o WTF -lpthread -lcrypto -lnsl -lz
	
WTFserver: WTFserver.c
	gcc -o WTFserver WTFserver.c -lcrypto -lpthread -lnsl -lz
	
clean:
	$(RM) WTF WTFserver	.configure

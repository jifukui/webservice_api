CC	=powerpc-linux-gcc
CCOPT 	=-O2
INCLS 	=
CFLAGS =	$(CCOPT)  $(INCLS)
LDFLAGS =
LIBS 	=-ljansson
ALL 	=upload
uplaod:upload.c
	$(CC) $(CFLAGS) $(LDFLAGS) upload.c -o upload.cgi  $(LIBS)

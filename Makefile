# Generated automatically from Makefile.in by configure.
# Makefile.in for thttpd
#
# Copyright � 1995,1998 by Jef Poskanzer <jef@mail.acme.com>.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.

# Various configurable paths (remember to edit Makefile.in, not Makefile)

# Top level hierarchy.
prefix = /usr/local
exec_prefix = ${prefix}
# Pathname of directory to install the binary.
BINDIR = ${exec_prefix}/sbin
# Pathname of directory to install the man page.
MANDIR = ${prefix}/man
# Pathname of directory to install the CGI programs.
WEBDIR = $(prefix)/www

# CONFIGURE: The group that the web directory belongs to.  This is so that
# the makeweb program can be installed set-group-id to that group, and make
# subdirectories.  If you're not going to use makeweb, ignore this.
WEBGROUP =	www

# CONFIGURE: Directory for CGI executables.
CGIBINDIR =	$(WEBDIR)/cgi-bin

# You shouldn't need to edit anything below here.

#CC =arm-poky-linux-gnueabi-gcc -march=armv7ve -mfpu=neon -mfloat-abi=hard -mcpu=cortex-a7 --sysroot=/opt/fsl-imx-x11/4.1.15-2.1.0/sysroots/cortexa7hf-neon-poky-linux-gnueabi -fdiagnostics-color=auto
CC= powerpc-linux-gcc
CCOPT =		-O2
DEFS =		 -DHAVE__PROGNAME=1 -DHAVE_FCNTL_H=1 -DHAVE_GRP_H=1 -DHAVE_MEMORY_H=1 -DHAVE_PATHS_H=1 -DHAVE_POLL_H=1 -DHAVE_SYS_POLL_H=1 -DTIME_WITH_SYS_TIME=1 -DHAVE_DIRENT_H=1 -DHAVE_LIBCRYPT=1 -DHAVE_STRERROR=1 -DHAVE_WAITPID=1 -DHAVE_VSNPRINTF=1 -DHAVE_DAEMON=1 -DHAVE_SETSID=1 -DHAVE_GETADDRINFO=1 -DHAVE_GETNAMEINFO=1 -DHAVE_GAI_STRERROR=1 -DHAVE_SIGSET=1 -DHAVE_ATOLL=1 -DHAVE_UNISTD_H=1 -DHAVE_GETPAGESIZE=1 -DHAVE_MMAP=1 -DHAVE_SELECT=1 -DHAVE_POLL=1 -DHAVE_TM_GMTOFF=1 -DHAVE_INT64T=1 -DHAVE_SOCKLENT=1
#INCLS =		-I. 
INCLS= -I.
CFLAGS =	$(CCOPT) $(DEFS) $(INCLS)
LDFLAGS =	
#LDFLAGS =	-L/opt/fsl-imx-x11/4.1.15-2.1.0/sysroots/cortexa7hf-neon-poky-linux-gnueabi/usr/lib
LIBS =		-lcrypt -ljansson -llig_matrix -lssl -lcrypto
NETLIBS =	
INSTALL =	/usr/bin/install -c



.c.o:
	@rm -f $@
	$(CC) $(CFLAGS) -c $*.c

SRC =		thttpd.c libhttpd.c fdwatch.c mmc.c timers.c match.c tdate_parse.c LiguoWeb.c Session.c

OBJ =		$(SRC:.c=.o) 

ALL =		thttpd

GENHDR =	mime_encodings.h mime_types.h

CLEANFILES =	$(ALL) $(OBJ) $(GENSRC) $(GENHDR)

#SUBDIRS =	cgi-src extras
SUBDIRS =
all:		this subdirs
this:		$(ALL)

thttpd: $(OBJ)
	@rm -f $@
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJ) $(LIBS) $(NETLIBS)

mime_encodings.h:	mime_encodings.txt
	rm -f mime_encodings.h
	sed < mime_encodings.txt > mime_encodings.h \
	  -e 's/#.*//' -e 's/[ 	]*$$//' -e '/^$$/d' \
	  -e 's/[ 	][ 	]*/", 0, "/' -e 's/^/{ "/' -e 's/$$/", 0 },/'

mime_types.h:	mime_types.txt
	rm -f mime_types.h
	sed < mime_types.txt > mime_types.h \
	  -e 's/#.*//' -e 's/[ 	]*$$//' -e '/^$$/d' \
	  -e 's/[ 	][ 	]*/", 0, "/' -e 's/^/{ "/' -e 's/$$/", 0 },/'


subdirs:
	for i in $(SUBDIRS) ; do ( \
	    cd $$i ; \
	    pwd ; \
	    $(MAKE) $(MFLAGS) \
		WEBDIR=$(WEBDIR) \
		CGIBINDIR=$(CGIBINDIR) \
		MANDIR=$(MANDIR) \
		WEBGROUP=$(WEBGROUP) \
	) ; done


install:	installthis install-man installsubdirs

installthis:
	-mkdir -p $(DESTDIR)$(BINDIR)
	$(INSTALL) -m 555 -o bin -g bin thttpd $(DESTDIR)$(BINDIR)

install-man:
	-mkdir -p $(DESTDIR)$(MANDIR)/man8
	$(INSTALL) -m 444 -o bin -g bin thttpd.8 $(DESTDIR)$(MANDIR)/man8

installsubdirs:
	for i in $(SUBDIRS) ; do ( \
	    cd $$i ; \
	    pwd ; \
	    $(MAKE) $(MFLAGS) \
		WEBDIR=$(WEBDIR) \
		CGIBINDIR=$(CGIBINDIR) \
		MANDIR=$(MANDIR) \
		WEBGROUP=$(WEBGROUP) \
		install \
	) ; done


clean:		cleansubdirs
	rm -f $(CLEANFILES)

distclean:	distcleansubdirs
	rm -f $(CLEANFILES) Makefile config.cache config.log config.status tags

cleansubdirs:
	for i in $(SUBDIRS) ; do ( \
	    cd $$i ; \
	    pwd ; \
	    $(MAKE) $(MFLAGS) clean \
	) ; done

distcleansubdirs:
	for i in $(SUBDIRS) ; do ( \
	    cd $$i ; \
	    pwd ; \
	    $(MAKE) $(MFLAGS) distclean \
	) ; done

tags:
	ctags -wtd *.c *.h

tar:
	@name=`sed -n -e '/SERVER_SOFTWARE/!d' -e 's,.*thttpd/,thttpd-,' -e 's, .*,,p' version.h` ; \
	  rm -rf $$name ; \
	  mkdir $$name ; \
	  tar cf - `cat FILES` | ( cd $$name ; tar xfBp - ) ; \
	  chmod 644 $$name/Makefile.in $$name/config.h $$name/mime_encodings.txt $$name/mime_types.txt ; \
	  chmod 755 $$name/cgi-bin $$name/cgi-src $$name/contrib $$name/contrib/redhat-rpm $$name/extras $$name/scripts ; \
	  tar cf $$name.tar $$name ; \
	  rm -rf $$name ; \
	  gzip $$name.tar

thttpd.o:	config.h version.h libhttpd.h fdwatch.h mmc.h timers.h match.h lig_pid_file.h
libhttpd.o:	config.h version.h libhttpd.h mime_encodings.h mime_types.h \
		mmc.h timers.h match.h tdate_parse.h LiguoWeb.h
fdwatch.o:	fdwatch.h
mmc.o:		mmc.h libhttpd.h
timers.o:	timers.h
match.o:	match.h
tdate_parse.o:	tdate_parse.h
LiguoWeb.o:  	LiguoWeb.h

#===============================================================================
#
#         File: Makefile
#  Description: 
#
#        Usage: make              (generate executable(s)                    )
#               make clean        (remove objects, executables, prerequisits )
#               make tarball      (generate compressed archive               )
#               make zip          (generate compressed archive               )
#                                  
#       Author: Iven Day(Xu Lijian)
#        Email: ivenvd@gmail.com
#      Created: 
#
#===============================================================================

export PREFIX	= /usr
LIBCHATSYS_DIR  = ./libchatsys

CC              = gcc
CFLAGS          = -c -g -Wall -I$(LIBCHATSYS_DIR)
GTKCFLAGS       = `pkg-config gtk+-2.0 --cflags`
LFLAGS          = -g
LIBS            = -L$(LIBCHATSYS_DIR) -lm -lpthread -lchatsys
GTKLIBS         = `pkg-config gtk+-2.0 --libs`
TARBALL_EXCLUDE = "*.{o,gz,zip}"
ZIP_EXCLUDE     = *.o *.gz *.zip

TARGETS	= chat_server chat_client_cli chat_client_gui

#---------- targets --------------------------------------
all:	$(TARGETS)

%.o:	%.c
			$(CC) $(CFLAGS) $(GTKCFLAGS) $*.c

#---------- target 1 -------------------------------------
chat_client_cli:	client_cli.o libchatsys.so
			$(CC) $(LFLAGS) -o $@ $<  $(LIBS)

#---------- target 2 -------------------------------------
chat_client_gui:	client_gui.o libchatsys.so
			$(CC) $(LFLAGS) -o $@ $<  $(LIBS) $(GTKLIBS)

#---------- target 3 -------------------------------------
chat_server:		server.o libchatsys.so
			$(CC) $(LFLAGS) -o $@ $<  $(LIBS)

#---------- target 4 -------------------------------------
libchatsys.so:
			$(MAKE) -C $(LIBCHATSYS_DIR)



#---------- tarball --------------------------------------
tarball:
			lokaldir=`pwd`; lokaldir=$${lokaldir##*/}; \
			rm --force $$lokaldir.tar.gz;              \
			tar --exclude=$(TARBALL_EXCLUDE)           \
				--create                               \
				--gzip                                 \
				--verbose                              \
				--file  $$lokaldir.tar.gz *

#---------- zip ------------------------------------------
zip:
			lokaldir=`pwd`; lokaldir=$${lokaldir##*/}; \
			zip -r  $$lokaldir.zip * -x $(ZIP_EXCLUDE)

#---------- clear up -------------------------------------
clean:
			rm  --force  $(TARGETS) $(ZIP_EXCLUDE)
			$(MAKE) -C $(LIBCHATSYS_DIR) clean
 
#---------- install -------------------------------------
install:	all
			install -m 755 $(TARGETS) $(PREFIX)/bin/
			$(MAKE) -C $(LIBCHATSYS_DIR) install


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

PREFIX			= /usr
LIBCHATSYS_DIR  = ./libchatsys

CC              = gcc
CFLAGS          = -c -g -Wall
INCLUDES		= -I$(LIBCHATSYS_DIR)
LFLAGS          = -g
LIBS        	= -L$(LIBCHATSYS_DIR) -lm -lpthread -lchatsys
TARBALL_EXCLUDE = "*.{o,gz,zip}"
ZIP_EXCLUDE     = *.o *.gz *.zip

TARGETS	= tcp_client tcp_server

#---------- targets --------------------------------------
all:	$(TARGETS)

%.o:	%.c
			$(CC) $(CFLAGS) $(INCLUDES) $*.c

#---------- target 1 -------------------------------------
tcp_client:	tcp_client.o libchatsys.so
			$(CC) $(LFLAGS) -o $@ $<  $(LIBS)

#---------- target 2 -------------------------------------
tcp_server:	tcp_server.o libchatsys.so
			$(CC) $(LFLAGS) -o $@ $<  $(LIBS)

#---------- target 3 -------------------------------------
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


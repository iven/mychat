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


CC              = gcc
CFLAGS          = -c -g -Wall
LFLAGS          = -g
SYS_LIBS        = -lm
TARBALL_EXCLUDE = "*.{o,gz,zip}"
ZIP_EXCLUDE     = *.o *.gz *.zip

TARGETS	= tcp_client tcp_server

#---------- targets --------------------------------------
all:	$(TARGETS)

%.o:	%.c
			$(CC) $(CFLAGS) $*.c

#---------- target 1 -------------------------------------
tcp_client:	tcp_client.o sock_wrapper.o
			$(CC) $(LFLAGS) -o $@ $^  $(SYS_LIBS)

#---------- target 2 -------------------------------------
tcp_server:	tcp_server.o sock_wrapper.o
			$(CC) $(LFLAGS) -o $@ $^  $(SYS_LIBS)

#---------- target 3 -------------------------------------



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
 

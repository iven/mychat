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
CFLAGS          = -c -g -Wall -fPIC
LFLAGS          = -g -shared
SYS_LIBS        = -lpthread
TARBALL_EXCLUDE = "*.{o,gz,zip}"
ZIP_EXCLUDE     = *.o *.gz *.zip

TARGETS	= libchatsys.so libchatsys.so.1.0.0

#---------- targets --------------------------------------
all:	$(TARGETS)

%.o:	%.c
			$(CC) $(CFLAGS) $*.c

#---------- target 1 -------------------------------------
libchatsys.so.1.0.0 :	parser.o sock_wrapper.o event.o
	$(CC) $(LFLAGS) -o $@ $^  $(SYS_LIBS)

#---------- target 2 -------------------------------------
libchatsys.so		:	libchatsys.so.1.0.0
			ln -sf $^ $@

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
 

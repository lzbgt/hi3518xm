############################################################################
#
# (C) Copyright 2018 to 2018 Hangzhou Xiongmai Information Technology Stock CO., LTD.
#
#  2018-08-17		File created
#
############################################################################

include $(TOPDIR)/config

CC	= @echo " $(CROSS_COMPILE)gcc	$@"; $(CROSS_COMPILE)gcc
CPP	= @echo " $(CROSS_COMPILE)g++	$@"; $(CROSS_COMPILE)g++
LD	= @echo " $(CROSS_COMPILE)ld	$@"; $(CROSS_COMPILE)ld
AR	= @echo " $(CROSS_COMPILE)ar	$@"; $(CROSS_COMPILE)ar
RM	= @echo " $(CROSS_COMPILE)rm	$@"; rm -f
STRIP 	= @echo " $(CROSS_COMPILE)strip $@"; $(CROSS_COMPILE)strip

ifndef DEBUG_ON
	CFLAGS	+= -Wall -Os
	DEBUG_SUFFIX =
else
	CFLAGS	+= -Wall -g
	DEBUG_SUFFIX = -d
endif

ifeq ($(debug), yes)
CFLAGS	+= -DDEBUG_PRINTF_OPEN
endif

ifeq ($(static), yes)
LDFLAGS	+= -static
endif

CFLAGS	+= -I$(TOPDIR)include
CFLAGS	+= -I$(TOPDIR)ext_inc
CFLAGS	+= -D_REENTRANT
CFLAGS	+= -fomit-frame-pointer
CFLAGS	+= -Wstrict-prototypes
CFLAGS	+= -fPIC
CFLAGS 	+= -ffunction-sections -fdata-sections
LDFLAGS	+=	-Wl,-gc-sections

LDFLAGS	+= -L$(TOPDIR)/lib/share

LIBS += $(TOPDIR)/lib/libXmMaQue.a
LIBS += $(TOPDIR)/lib/libXmSns_*.a
LIBS += $(TOPDIR)/lib/libXmRtsp.a

LDFLAGS +=  -lhive_AEC  \
			-lhive_AGC  \
			-lhive_ANR  \
			-lhive_common  \
			-lhive_EQ  \
			-lhive_HPF  \
			-lhive_RES  \
			-lhive_record	\
			-lsecurec

LDFLAGS	+= -pthread -lm -ldl

TARGET = $(EXEC) 

all: $(TARGET) 

$(EXEC): $(OBJS) $(EXEC_OBJS) $(LIBS)
	mkdir -p $(TOPDIR)$(BINDIR)
	$(RM) $@;
	$(CC) -o $@ $^ $(LDFLAGS)
ifndef DEBUG_ON
	$(STRIP) $(EXEC)
endif

.c.o:
	$(CC)  $(CFLAGS) -c -o $@ $<
	@echo $@
.cpp.o:
	$(CPP) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJS)  $(EXEC_OBJS) $(OBJS2);
	find ./ -type f \( -name '*.elf*' -o -name '*.gdb' \) | xargs rm -f

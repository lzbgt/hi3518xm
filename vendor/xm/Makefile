############################################################################
#
#  (c) Copyright 2018-2018, XM Information Technology Stock CO.LTD.
#
#  2018-07-17		File created	
#
############################################################################
ifndef TOPDIR
	TOPDIR = ./
endif

LIBDIR = lib/
BINDIR = bin/

OBJSRC=$(wildcard *.c ./src/*.c)
OBJS=$(addprefix $(TOPDIR)/, $(addsuffix .o, $(basename $(OBJSRC))))

EXEC = $(TOPDIR)$(BINDIR)XmMaQueSdkDemo$(DEBUG_ON)

include $(TOPDIR)rules.mak

## Select one of these
#DEVICE=stm8s003f3
DEVICE=stm8s103f3

## A directory for common include files
COMMONDIR = ..

## Get program name from enclosing directory name
PROGRAM = robot

SOURCES=robot.c robotUtils.c
OBJECTS=$(SOURCES:.c=.rel)
HEADERS=$(wildcard *.h $(COMMONDIR)/*.h)

CC = sdcc
PROGRAMMER = stlinkv2
FLASHEXE = ../stm8flash/stm8flash


DEFINES=
## Set MCU-type DEFINE
ifeq ($(DEVICE),stm8s003f3)
    DEFINES += -DSTM8S003
endif
ifeq ($(DEVICE),stm8s103f3)
    DEFINES += -DSTM8S103
endif

CPPFLAGS = -I$(COMMONDIR)
CFLAGS = --Werror --std-sdcc99 -mstm8 $(DEFINES)
LDFLAGS = -lstm8 -mstm8 --out-fmt-ihx

.PHONY: all clean flash

$(PROGRAM).ihx: $(OBJECTS)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

%.rel : %.c $(HEADERS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

CCOMPILEDFILES=$(SOURCES:.c=.asm) $(SOURCES:.c=.lst) $(SOURCES:.c=.rel) \
               $(SOURCES:.c=.rst) $(SOURCES:.c=.sym)
clean:
	rm -f $(PROGRAM).ihx $(PROGRAM).cdb $(PROGRAM).lk $(PROGRAM).map $(CCOMPILEDFILES)

test: 
	gcc robotUtils.c robotUtilsTest.c  -o utilsTest.out
	./utilsTest.out

simu: 
	gcc robotUtils.c robotSimuAutoAnim.c -lSDL2 -lm -o simu.out
	./simu.out


flash: $(PROGRAM).ihx
	$(FLASHEXE) -c $(PROGRAMMER) -p $(DEVICE) -w $(PROGRAM).ihx

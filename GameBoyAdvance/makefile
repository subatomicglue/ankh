
#USE_DEPEND?=YES

.PHONY : all
all: data.h ankh.gba

data.h: assets.dat makeassets.pl
	makeassets.pl

SRCFILES = ankh.cpp data.cpp Sprite.cpp InGameState.cpp \
			ActorDrawManager.cpp spewgba.cpp Inventory.cpp \
			ShotManager.cpp MapRoom.cpp

SPRITES = art/badguy.raw.c  \
      art/benign.raw.c \
      art/bonus.raw.c \
      art/box.raw.c \
      art/chalice.raw.c \
      art/crown.raw.c \
      art/flit.raw.c \
      art/horzdoor.raw.c \
      art/illumina.raw.c \
      art/item.raw.c \
      art/key.raw.c \
      art/lock.raw.c \
      art/lockface.raw.c \
      art/musicbox.raw.c \
      art/other.raw.c \
      art/suck.raw.c \
      art/tele.raw.c \
      art/trap.raw.c \
      art/treasure.raw.c \
      art/vertdoor.raw.c \
      art/monster.raw.c\
	  art/locklisten.raw.c \
	  art/bullet.raw.c \
	  art/touchvert.raw.c \
	  art/touchhorz.raw.c \
		art/sprite.pal.c \
		art/quake3.raw.c \
		art/walls.pal.c \
		art/walls.raw.c \
		art/hud.raw.c \
		art/title1.raw.c \
		art/title2.raw.c \
		art/title3.raw.c \
		art/title4.raw.c \
		art/title5.raw.c \
		art/title6.raw.c \
		art/title7.raw.c \
		art/title8.raw.c \
		art/title9.raw.c \
		art/title.pal.c \
		art/walls.map.c \
		art/hud.map.c \
		art/nobitmap.raw.c

SRCS=$(SPRITES) $(SRCFILES)

# devkit ARM uses -specs, dkAdvance doesn't...
#LINKFLAGS = -specs=gba_mb.specs -mthumb-interwork
LINKFLAGS = -mthumb-interwork
OPTIONAL_COMPILE_FLAGS ?= -O2
C_COMPILE_FLAGS = -mthumb-interwork $(OPTIONAL_COMPILE_FLAGS)
CPP_COMPILE_FLAGS = -mthumb-interwork $(OPTIONAL_COMPILE_FLAGS)

# no multiboot (normal rom)
#CPPFLAGS = -specs=gba.specs -mthumb-interwork

CC=g++
C_COMPILE=gcc
CXX_COMPILE=g++
OBJDIR=.objs
DEPDIR=.deps
OBJ_EXT=.o
OBJ_NAME_FLAG=-o $@
OBJ_BUILD_FLAG=-c





include mk/dzr.obj.mk


# debugging? -gstabs with no -O
.PHONY : ankh.elf
ankh.elf:  $(DEPEND_FILES) $(OBJS)
	arm-agb-elf-as -mthumb-interwork single_ints.s -o single_ints.o
	#arm-agb-elf-as -mthumb swi.s -o swi.o
	arm-agb-elf-gcc $(C_COMPILE_FLAGS) -mthumb -c interrupt.c -o interrupt.o
	arm-agb-elf-g++ $(CPP_COMPILE_FLAGS) $(LINKFLAGS) -o ankh.elf single_ints.o $(OBJS) \
	interrupt.o -lm


.PHONY : ankh.gba
ankh.gba: $(DEPEND_FILES) $(SRCS)
	make ankh.elf
	@echo "making debug version"
	arm-agb-elf-objcopy -O binary ankh.elf ankh.gba
	gbafix ankh.gba -p -t[ankh] -csuba -mkm -r6969
	@echo "making release version (stripping debug symbols)"
	arm-agb-elf-objcopy -ggdb -O binary ankh.elf ankh-release.gba
	gbafix ankh-release.gba -p -t[ankh] -csuba -mkm -r6969

.PHONY : test
test:
	g++ SortedListTest.cpp -oSortedListTest.exe

.PHONY : clean
clean:
	-rm -r .objs/
	-rm *.gba *.elf *.o


cleandepend:
	@echo "removing dep files..."
	$(MAKE) USE_DEPEND_DEFINED=1 USE_DEPEND=NO _cleandepend

_cleandepend:
	-rm -r .deps/

.PHONY : clobber
clobber: clean cleandepend

# debug symbols + trace
dbg:
	$(MAKE) OPTIONAL_COMPILE_FLAGS="-ggdb  -D_DEBUG -DUSE_GBA_TRACE"

# opt symbols + trace + assert
trace:
	$(MAKE) OPTIONAL_COMPILE_FLAGS="-O1 -ggdb -D_DEBUG -DDEBUG -DUSE_GBA_TRACE"

.PHONY : run
run: ankh.gba
	run.bat

.PHONY : run
run-dbg: ankh.gba
	run-dbg.bat

-include mk/dzr.dep.mk

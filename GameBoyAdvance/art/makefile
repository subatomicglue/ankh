all: spritepalette mappalette titlescreens

SPRITES = quake3.bmp \
		badguy.bmp  \
      benign.bmp \
      bonus.bmp \
      box.bmp \
      chalice.bmp \
      crown.bmp \
      flit.bmp \
      horzdoor.bmp \
      illumina.bmp \
      item.bmp \
      key.bmp \
      lock.bmp \
      lockface.bmp \
      musicbox.bmp \
      other.bmp \
      suck.bmp \
      tele.bmp \
      trap.bmp \
      treasure.bmp \
      vertdoor.bmp \
      monster.bmp\
		locklisten.bmp \
		bullet.bmp \
		touchvert.bmp \
		touchhorz.bmp \
		hud.bmp \
		nobitmap.bmp

# If no object directory is being used, set it to the current directory.
OBJDIR?= obj
OBJ_EXT=.o
CIMG_EXT=.raw.c

#/////////////////////////////////////////////////////////
# Define the list of supported source file suffixes.
_image_suffix_list=  bmp tga

# Loop over the suffixes, translate them to $(OBJ_EXT) and store the
# result in $(OBJS).
makeimgs=   $(filter %$(CIMG_EXT), $(foreach _suffix, $(_image_suffix_list), $(1:.$(_suffix)=$(CIMG_EXT))))

# Construct $(OBJS) from $(SRCS) using the makeobjs function.
SPRITE_CFILES= $(call makeimgs, $(SPRITES))
TILEMAP_CFILES= $(call makeimgs, $(TILEMAPS))
############################################################3

#
# Set a list of files you want to compile 
#
SRCS += $(SPRITE_CFILES) \
        $(TILEMAP_CFILES)
#SRCS += $(TILEMAP_DIR).pal.c

#/////////////////////////////////////////////////////////
# Define the list of supported source file suffixes.
_suffix_list=  S s c++ cpp cxx c C CC cc

# Loop over the suffixes, translate them to $(OBJ_EXT) and store the
# result in $(OBJS).
makeobjs=   $(filter %$(OBJ_EXT), $(foreach _suffix, $(_suffix_list), $(1:.$(_suffix)=$(OBJ_EXT))))

# Construct $(OBJS) from $(SRCS) using the makeobjs function.
BASIC_OBJS= $(call makeobjs, $(SRCS))
OBJS += $(addprefix $(OBJDIR)/, $(BASIC_OBJS))


#/////////////////////////////////////////////////////////


spritepalette: makespritesource #compilespritesource
	
makespritesource:
	gfx2gba -D -fsrc -psprite.pal -t8 $(SPRITES)

$(OBJDIR)/%.o: %.c
	arm-agb-elf-g++ -O2 -c $< -o $@

compilespritesource: $(OBJS)

mappalette:
	gfx2gba -D -t8 -fsrc -pwalls.pal walls.bmp hud.bmp
titlescreens:
	gfx2gba -fsrc -ptitle.pal title1.bmp title2.bmp title3.bmp title4.bmp title5.bmp title6.bmp title7.bmp title8.bmp title9.bmp

convert: walls.map.c

walls.map.c: ankhmap-96x96/walls.TXT walltiles.convert.cpp
	g++ walltiles.convert.cpp -o convert.exe
	convert.exe
	rm convert.exe

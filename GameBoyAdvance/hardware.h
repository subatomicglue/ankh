#ifndef GBA_HARDWARE_H
#define GBA_HARDWARE_H

#include "common.h"

//namespace gba
//{

   // compiler/environment setup...
   /*
   Use these to dictate where functions go in memory. 

   examples:
      (must initialize data): 
      char myarray[3] MEM_IN_EWRAM = {0,0,0};

      void myfunction(void) IN_IWRAM;
   --------------------------
   */
   #define MEM_IN_EWRAM __attribute__ ((section (".ewram"))) = {0}
   #define MEM_IN_IWRAM __attribute__ ((section (".iwram"))) = {0}
   #define FUNC_IN_IWRAM __attribute__ ((section (".iwram"), long_call))

   // use this if you want to the game to run in ewram (multiboot over the serial cable)
   // including this places game in ewram, omitting this places game in cart rom.
   #define MULTIBOOT volatile const int __gba_multiboot = 1;  


   // memory locations
   const u32 MEM_SYSROM = 0x00000000;  // System ROM, for BIOS Calls
   const u32 MEM_EXRAM = 0x02000000;   // External WRAM, slow, also used for Multiboot uploads
   const u32 MEM_RAM = 0x03000000;     // Fast CPU internal RAM
   const u32 MEM_IO = 0x04000000;      // Register Base, all HW Registers are in here.
   const u32 MEM_PAL = 0x05000000;     // Palette Base
   const u32 MEM_PAL_BG = 0x05000000;	// Palette for BG
   const u32 MEM_PAL_OBJ = 0x05000200;	// Palette for OBJ
   const u32 MEM_VRAM = 0x06000000; 	// GBA Video RAM
   const u32 MEM_BG = 0x06000000;	   // GBA Video RAM (in BG mode)
   const u32 MEM_OBJ =  0x06010000;    // OBJ memoryspace (32 kBytes)
   const u32 MEM_OAM = 0x07000000;     // Object control space for sprites
   const u32 MEM_ROM0 = 0x08000000;    // Rom Space 0 (fastest, 0 wait)
   const u32 MEM_ROM1 = 0x0A000000;    // Rom Space 1 (1 wait)
   const u32 MEM_ROM2 = 0x0C000000;    // Rom Space 2 (slowest, 2 wait)
   const u32 MEM_SRAM = 0x0E000000;    // Gamepak SRAM, if any.

   // interrupt
   #define REG_INTERUPT (*(volatile u32*)0x3007FFC)
   
   // display register
   #define REG_DISPCNT (*(volatile u32*)0x04000000)	      // Address to display controller register
   #define REG_DISPSTAT (*(volatile u16*)0x4000004)
   #define REG_VCOUNT (*(volatile u16*)0x4000006) // vsync reg

   #define REG_BASE    MEM_IO
   #define REG_INTMAIN      *(fp*)(REG_BASE-0x0004)	// -> 0300:7ffc

   // display params for REG_DISPCNT
   #define BGMODE0    0x0           //Enable screen mode 0  // Tile modes, Mode 0 supports all 4 map layers, but it does not support rotation and scaling. 
   #define BGMODE1    0x1           //Enable screen mode 1  // Tile modes, Mode 1 supports layers 0,1, and 2, and layer 2 can rotate/scale. 
   #define BGMODE2    0x2           //Enable screen mode 2  // Mode 2 supports layers 2 and 3, and both can rotate/scale. 
   #define BGMODE3    0x3           //Enable screen mode 3  // single buffered, 16bit color.  240x160 screen
   #define BGMODE4    0x4           //Enable screen mode 4  // double buffered, 8bit color w/ palette, 240x160 screen
   #define BGMODE5    0x5           //Enable screen mode 5  // double buffered, 16bit color. 160x128 (small) screen
   #define BACKBUFFER     0x10          //Determine backbuffer address
   #define HBLANKOAM      0x20          //Update OAM during HBlank?
   #define OBJCHARMEMMAP2D       0x0           //2D object (sprite) mapping
   #define OBJCHARMEMMAP1D       0x40          //1D object(sprite) mapping
   #define FORCEBLANK     0x80          //Force a blank
   #define BG0ENABLE      0x100         //Enable background 0
   #define BG1ENABLE      0x200         //Enable background 1
   #define BG2ENABLE      0x400         //Enable background 2
   #define BG3ENABLE      0x800         //Enable background 3
   #define OBJENABLE      0x1000        //Enable sprites
   #define WIN1ENABLE     0x2000        //Enable window 1
   #define WIN2ENABLE     0x4000        //Enable window 2
   #define WINOBJENABLE   0x8000        //Enable object window

   inline void SetMode( u32 mode ) { REG_DISPCNT = mode; }



   const u16 SCREENX = 240;
   const u16 SCREENY = 160;

   // background memory addresses
   // video memory:   (tile bitmap(s) is in ch0-1, tile indicies start at one of the screenbase 0-31...)
   // ch = 16,384 b
   // sb =  2,048 b
   //
   // ch 0:    0 screenbase
   //          1 screenbase
   //          2 screenbase
   //          3 screenbase
   //          4 screenbase
   //          5 screenbase
   //          6 screenbase
   //          7 screenbase
   // ch 1:    8 screenbase
   //          9 screenbase
   //          10 screenbase
   //          11 screenbase
   //          12 screenbase
   //          13 screenbase
   //          14 screenbase
   //          15 screenbase
   // ch 2:    16 screenbase
   //          17 screenbase
   //          18 screenbase
   //          19 screenbase
   //          20 screenbase
   //          21 screenbase
   //          22 screenbase
   //          23 screenbase
   // ch 3:    24 screenbase
   //          25 screenbase
   //          26 screenbase
   //          27 screenbase
   //          28 screenbase
   //          29 screenbase
   //          30 screenbase
   //          31 screenbase
   u16*const GBA_BG_DATA = ((u16*)0x06000000);    // background tiles bitmap data
   #define GBA_BG_DATA_S(S) ((u16*)(0x6000000 + S * 0x4000))     // 0-3   Lookup by Character Base Block - background tiles bitmap data
   #define GBA_BG_DATA_M(M) ((u16*)(0x06000000 + M * 0x800 ))    // 0-31  Lookup by Screen Base Block - background tiles bitmap data
   u16*const GBA_BG_PAL = ((u16*)0x05000000);     // background palette
   #define  GBA_BG_MAP GBA_BG_DATA_M(31)     // the map indicies that reference the background tiles

   
   // background registers
   #define REG_BG0CNT (*(volatile u16*)0x4000008)	      // Background 0 control register address
   #define REG_BG1CNT (*(volatile u16*)0x400000A)	      // Background 1 control register address
   #define REG_BG2CNT (*(volatile u16*)0x400000C)	      // Background 2 control register address
   #define REG_BG3CNT (*(volatile u16*)0x400000E)	      // Background 3 control register address
   #define REG_BG0HOFS (*(volatile u16*)0x4000010)
   #define REG_BG0VOFS (*(volatile u16*)0x4000012)
   #define REG_BG1HOFS (*(volatile u16*)0x4000014)
   #define REG_BG1VOFS (*(volatile u16*)0x4000016)
   #define REG_BG2HOFS (*(volatile u16*)0x4000018)
   #define REG_BG2VOFS (*(volatile u16*)0x400001A)
   #define REG_BG3HOFS (*(volatile u16*)0x400001C)
   #define REG_BG3VOFS (*(volatile u16*)0x400001E)

   // scroll the background
   #define REG_BG0SCRLX (*(volatile u16*)(MEM_IO + 0x10))
   #define REG_BG0SCRLY (*(volatile u16*)(MEM_IO + 0x12))
   #define REG_BG1SCRLX (*(volatile u16*)(MEM_IO + 0x14))
   #define REG_BG1SCRLY (*(volatile u16*)(MEM_IO + 0x16))
   #define REG_BG2SCRLX (*(volatile u16*)(MEM_IO + 0x18))
   #define REG_BG2SCRLY (*(volatile u16*)(MEM_IO + 0x1A))
   #define REG_BG3SCRLX (*(volatile u16*)(MEM_IO + 0x1C))
   #define REG_BG3SCRLY (*(volatile u16*)(MEM_IO + 0x1E))

//REG_TMxCNT (replace x with the Timer number 0 - 3)
//Bits 	Function
//0-1 	Timer frequency (0 = 1 cycle, 1 = 64 cycles, 2 = 256 cycles, 3 = 1024 cycles)
//2 	Increment Timer x on Timer x-1 overflow (0 = No, 1 = Yes)
//3-5 	Unused
//6 	Timer will generate an interrupt on overflow (0 = No, 1 = Yes)
//7 	Enable Timer (0 = No (disable), 1 = Yes (enable))
//8-15 	Unused
#define REG_TM0CNT     (*(u16*)0x4000102)
#define REG_TM1CNT     (*(u16*)0x4000106)
#define REG_TM2CNT     (*(u16*)0x400010A)
#define REG_TM3CNT     (*(u16*)0x400010E)

//REG_TMxD (replace x with the Timer number 0 - 3)
//Bits 	Function
//0-15 	Timer start value
#define REG_TM0D       (*(u16*)0x4000100)
#define REG_TM1D       (*(u16*)0x4000104)
#define REG_TM2D       (*(u16*)0x4000108)
#define REG_TM3D       (*(u16*)0x400010C)

#define REG_SCD0       (*(u16*)0x4000120)
#define REG_SCD1       (*(u16*)0x4000122)
#define REG_SCD2       (*(u16*)0x4000124)
#define REG_SCD3       (*(u16*)0x4000126)
#define REG_SCCNT      (*(u32*)0x4000128)
#define REG_SCCNT_L    (*(u16*)0x4000128)
#define REG_SCCNT_H    (*(u16*)0x400012A)
#define REG_P1         (*(u16*)0x4000130)
#define REG_P1CNT      (*(u16*)0x4000132)
#define REG_R          (*(u16*)0x4000134)
#define REG_HS_CTRL    (*(u16*)0x4000140)
#define REG_JOYRE      (*(u32*)0x4000150)
#define REG_JOYRE_L    (*(u16*)0x4000150)
#define REG_JOYRE_H    (*(u16*)0x4000152)
#define REG_JOYTR      (*(u32*)0x4000154)
#define REG_JOYTR_L    (*(u16*)0x4000154)
#define REG_JOYTR_H    (*(u16*)0x4000156)
#define REG_JSTAT      (*(u32*)0x4000158)
#define REG_JSTAT_L    (*(u16*)0x4000158)
#define REG_JSTAT_H    (*(u16*)0x400015A)
#define REG_IE         (*(vu16*)0x4000200)
#define REG_IF         (*(vu16*)0x4000202)
#define REG_WSCNT      (*(vu16*)0x4000204)
#define REG_IME        (*(vu16*)0x4000208)
#define REG_PAUSE      (*(u16*)0x4000300)

   // background register parameters
   const u16 BGPRIORITY0 = 0;	                  // Priority of background layers (1st-4th)
   const u16 BGPRIORITY1 = 1;
   const u16 BGPRIORITY2 = 2;
   const u16 BGPRIORITY3 = 3;

                                                // In which base block (0-3) character data 
	                                             // (tile map) for background can be found
   inline u16 BGCHARBASEBLOCK( u16 n ) { return n << 2; }

   const u16 BGMOSAIC = 1 << 6;	               // Mosaic effect on background

   const u16 BGCOLORMODE16X16 = 0 << 7;	      // 16x16 color palettes
   const u16 BGCOLORMODE1X256 = 1 << 7;	      // 1x256 color palette

                                                // In which base block (0-31) screen data 
	                                             // (tile set) for background can be found
   inline u16 BGSCREENBASEBLOCK( u16 n ) { return n << 8; }	

   const u16 BGOVERFLOWWRAP = 1 << 13;		      // Makes the screen wrap at offset overflow

	                                             // Size of text background character data
   const u16 BGTEXTSCREENSIZE256x256 = 0 << 14;	// 2kb
   const u16 BGTEXTSCREENSIZE512x256 = 1 << 14;	// 4kb	
   const u16 BGTEXTSCREENSIZE256x512 = 2 << 14;	// 4kb
   const u16 BGTEXTSCREENSIZE512x512 = 3 << 14;	// 8kb

	                                                   // Size of rotscale background character data
   const u16 BGROTSCALESCREENSIZE128x128	 = 0 << 14;	// 256 bytes
   const u16 BGROTSCALESCREENSIZE256x256	 = 1 << 14;	// 1kb
   const u16 BGROTSCALESCREENSIZE512x512	 = 2 << 14;	// 4kb
   const u16 BGROTSCALESCREENSIZE1024x1024 = 3 << 14;	// 16kb

   // sprite memory addresses
   u16*const GBA_SPRITE_PAL = (u16*)0x05000200; // sprite palette
   u32*const GBA_SPRITE_OAM = (u32*)0x07000000; // object attribute memory, used to control sprites.
   u16*const GBA_SPRITE_DATA = (u16*)0x6010000; // sprite tiles bitmap data  (4000h size)
   u16*const GBA_SPRITE_DATA2 = (u16*)0x6014000; // sprite tiles bitmap data (high block) (4000h size)

   // controller registers
   #define REG_CTRLINPUT (*(volatile u16 *)(MEM_IO + 0x130))

   // controller masks
   const u16 CTRLINPUT_MSK = 0xFC00;
   
   // controller data (poll these for button presses) returns true or false
   #define CTRLINPUT_A_PRESSED        (~REG_CTRLINPUT & 0x0001)
   #define CTRLINPUT_B_PRESSED       ((~REG_CTRLINPUT & 0x0002) >> 1)
   #define CTRLINPUT_SELECT_PRESSED  ((~REG_CTRLINPUT & 0x0004) >> 2)
   #define CTRLINPUT_START_PRESSED   ((~REG_CTRLINPUT & 0x0008) >> 3)
   #define CTRLINPUT_RIGHT_PRESSED   ((~REG_CTRLINPUT & 0x0010) >> 4)
   #define CTRLINPUT_LEFT_PRESSED    ((~REG_CTRLINPUT & 0x0020) >> 5)
   #define CTRLINPUT_UP_PRESSED      ((~REG_CTRLINPUT & 0x0040) >> 6)
   #define CTRLINPUT_DOWN_PRESSED    ((~REG_CTRLINPUT & 0x0080) >> 7)
   #define CTRLINPUT_R_PRESSED       ((~REG_CTRLINPUT & 0x0100) >> 8)
   #define CTRLINPUT_L_PRESSED       ((~REG_CTRLINPUT & 0x0200) >> 9)


   #define OAM_SPRITE_OFF (1<<9) // assign bit 9 to attribute[0] in the OAM

      /// Object Attribute Memory.
      /// sprite structure definitions
      struct OamEntry
      {

         /*
         Bytes 1 and 2 (Attribute 0)

         | F E | D | C | B A | 9 8 | 7 6 5 4  3 2 1 0 |
         |  S  | A | M |  T  | D R |        J         |

         0-7 (J) = Y co-ordinate of the sprite (pixels). Note that for regular sprites,
                  this is the y coordinate of the upper left corner. For rotate/scale
                  sprites, this is the y coordinate of the sprite's center. center .
                  Note on Coordinates: The values actually wrap around: to achieve a -1
                  y coordinate, use y = 255.

         8   (R) = Rotation/Scaling on/off 
         9   (D) = 0 - sprite is single sized;
                   1 - sprite is virtually double sized; allowing sheared sprite pixels to
                     overflow sprite the size (specified by bits 14 - 15 of
                     OAM attribute 1). A 16x16 sized sprite is treated internaly as a
                     32x32 sprite. This specification comes in evidence when rotating a
                     sprite at 45°, since the H/V size of the sprite becomes 
                     SQRT(16² + 16²) = SQRT(512) =~ 22.62 pixels. This will cause the
                     sprite to appear clipped if this bit is set to 0. 
                     (Thanks to Kay for the description)


         A-B (T) = 00 - normal
                   01 - semi-transparent
                   10 - obj window
                   11 - illegal code
                   
                  Note that semi-transparent sprites appear as transparent even if
                  REG_BLDMOD has the sprites bit turned off.  Also note that
                  sprites cannot be blended against one another.  For more details, see
                  REG_BLDMOD.

         C   (M) = enables mosaic for this sprite.
         D   (A) = 256 color if on, 16 color if off 
         E-F (S) = See below 
         */
         u16 attribute0;

         /*
         Bytes 3 and 4 (Attribute 1)

         F E D C  B A 9 8  7 6 5 4  3 2 1 0
         S S V H  X X X I  I I I I  I I I I	(standard sprites)
         S S F F  F F F I  I I I I  I I I I  (rotation/scaling on)

         0-8 (I) = X coordinate of the sprite (pixels). For regular sprites,
                  this is the x coordinate of the upper left corner. For rotate/scale
                  sprites, this is the x coordinate of the sprite's center. 
                  Note on coordinates: The values actually wrap around. To achieve a 
                  -1 x, use x = 511.

         C   (H) = The flip horizinal bit
         D   (V) = The flip vertical bit

         9-D (F) = For rotation scaling sprites, the index into the rotation data to
                  be used for that sprite. This index can be from 0 - 31. The
                  rotation/scaling data is located in OAM attribute 3 (bytes 7 and 8).
                  However, instead of the rotation and scaling data going with the
                  corresponding sprite, it is separated accross four sequential sprites.
                  This index can be thought of as referencing into an array of
                  four-sprite blocks, 32 bytes each. 

         E-F (S) = Size of the sprite. The top two bits of the size value are found in
                  attribute 0 and the bottom two bits are in attribute 1. This forms a 
                  4-bit value which sets the size of the sprite in the following way:

                  0000: 8  x 8         1000: 8  x 16
                  0001: 16 x 16        1001: 8  x 32
                  0010: 32 x 32        1010: 16 x 32
                  0011: 64 x 64        1011: 32 x 64
                  0100: 16 x 8         1100: Not used
                  0101: 32 x 8         1101: Not used
                  0110: 32 x 16        1110: Not used
                  0111: 64 x 32        1111: Not used
         */
	      u16 attribute1;

         /*
         Bytes 5 and 6 (Attribute 2)

         F E D C  B A 9 8  7 6 5 4  3 2 1 0
         L L L L  P P T T  T T T T  T T T T

         0-9 (T) = Tile number. This value indexes selects the bitmap of the tile to be
                  displayed by indexing into the tile data area. Each index refernces
                  32 bytes, so the memory address of a tile is roughly 0x6010000 + T*32.
                  (see Sprite Tile Data for details) 

         A-B (P) = Priority. This controls the priority of the sprite. Note thate sprites take
                  precedence over backgrounds of the same priority.  See the description of
                  priority under REG_BG0 - REG_BG3 for a more detailed explanation.

         C-F (L) = Palette number. If you use 16 color palettes, this tells you which pallette
                  number to use. 
         */
	      u16 attribute2;

         /*
         Bytes 7 and 8 (Attribute 3)

         F E D C  B A 9 8  7 6 5 4  3 2 1 0
         S I I I  I I I I  F F F F  F F F F

         0-7 (F) = Fraction. 
         8-E (I) = Integer
         F = (S) = Sign bit

         These bytes control sprite rotation and scaling. Instead of the rotation and scaling data going with the corresponding sprite, it is separated accross four sequential sprites. This is indexed by bits 9 - 13 in attribute 1 (bytes 3 and 4). Note that these are all relative to the center of the sprite (background rotation/scaling is relative to the upper left). Starting with sprite 0 and repeating every 4 sprites, they appear in the following order:

         o  Sprite 0, Attribute 3 - PA (DX):
            Scales the sprite in the x direction by an amount equal to 1/(register value). Thus, a value of 1.0 results in the original image size, while a value of 2 is half as large, and a value of .5 is twice as large.

         o  Sprite 1, Attribute 3 - PB (DMX):
            Shears the x coordinates of the sprite over y. A value of 0 will result in no shearing, a value of 1.00 will make the image appear to be sheared left going down the screen, and a value of -1 will make the image appear sheared right going down the screen.

         o  Sprite 2, Attribute 3 - PC (DY):
            Shears the y coordinates of the sprite over x. A value of 0 will result in no shearing, a value of 1.00 will make the image appear to be sheared upwards to the right, and a value of -1 will make the image appear sheared downwards and to the right.

         o  Sprite 3, Attribute 3 - PD (DMY):
            Scales the image in the y direction by an amount equal to 1/(register value). Thus, a value of 1.0 results in the original image size, while a value of 2 is half as large, and a value of .5 is twice as large.
         */
	      u16 attribute3;
         inline void clear() { attribute0 = 0; attribute1 = 0; attribute2 = 0;attribute3 = 0; }
         inline void reset() { attribute0 = OAM_SPRITE_OFF; }

         /// set the x position of the sprite
         inline void setX( u16 x )
         {
            // clear the position part
            attribute1 &= 0xe000;
            
            // write the position part
            attribute1 |= (0x01ff & x);
         }

         /// set the y position of the sprite
         inline void setY( u16 y )
         {
            // clear the position part
            attribute0 &= 0xff00; 
            
            // write the position part
            attribute0 |= (0x00ff & y);
         }

         inline void setSize( u16 size )
         {
            attribute0 |= 0xc000 & (size << 12); // top two bits go in bit 15 and 16
            attribute1 |= 0xc000 & (size << 14); // bottom two bits go in bit 15 and 16
         }
      };

      //turn all the sprites off
      inline void InitOAM(void)
      {
         OamEntry* oamArray = (OamEntry*)GBA_SPRITE_OAM;
         for(int i = 0; i < 128; i++)
            oamArray[i].attribute0 = OAM_SPRITE_OFF;  
      }

      /// this overlays 4 OamEntrys to define an affine matrix for rotation/scale of the sprites.
      /// there are 32 total of these in gba memory (32 == 128 OamEntries / 4)
      struct AffObject
      {
         u16 fill0[3];
         s16 pa;
         u16 fill1[3];
         s16 pb;
         u16 fill2[3];
         s16 pc;
         u16 fill3[3];
         s16 pd;
      };

/*
// ==== deprecated, see interrupt.h/cpp =====
#ifndef INTERUPT_C
//void EnableInterupts(u16 interupts);
//void DissableInterupts(u16 interupts);
//extern fp IntrTable[];
#endif

#define INT_VBLANK    		0x0001
#define INT_HBLANK		0x0002	
#define INT_VCOUNT		0x0004	
#define INT_TIMMER0		0x0008
#define INT_TIMMER1		0x0010
#define INT_TIMMER2		0x0020	
#define INT_TIMMER3		0x0040
#define INT_COMUNICATION	0x0080
#define INT_DMA0		0x0100
#define INT_DMA1		0x0200
#define INT_DMA2		0x0400
#define INT_DMA3		0x0800
#define INT_KEYBOARD		0x1000
#define INT_CART		0x2000
#define INT_ALL			0x4000
*/


// --- REG_DISPSTAT (0400:0004) ---------------------------

// blank period status bits
#define VID_STAT_VB		0x0001	// (r)
#define VID_STAT_HB		0x0002	// (r)
#define VID_STAT_VC		0x0004	// (r)
// video interrupt requests
#define VID_IRQ_VB		0x0008
#define VID_IRQ_HB		0x0010
#define VID_IRQ_VC		0x0020
// VCount irq condition mask
#define VID_CND_MASK    0xff00


// DMA!
// === DMA REGISTERS ===
#define REG_DMA0SAD       *(vu32*)(REG_BASE+0x00B0) // source
#define REG_DMA0SAD_L     *(vu16*)(REG_BASE+0x00B0)
#define REG_DMA0SAD_H     *(vu16*)(REG_BASE+0x00B2)
#define REG_DMA0DAD       *(vu32*)(REG_BASE+0x00B4) // destination
#define REG_DMA0DAD_L     *(vu16*)(REG_BASE+0x00B4)
#define REG_DMA0DAD_H     *(vu16*)(REG_BASE+0x00B6)
#define REG_DMA0CNT       *(vu32*)(REG_BASE+0x00B8) // control
#define REG_DMA0CNT_L     *(vu16*)(REG_BASE+0x00B8) // count
#define REG_DMA0CNT_H     *(vu16*)(REG_BASE+0x00BA) // flags
#define REG_DMA1SAD       *(vu32*)(REG_BASE+0x00BC)
#define REG_DMA1SAD_L     *(vu16*)(REG_BASE+0x00BC)
#define REG_DMA1SAD_H     *(vu16*)(REG_BASE+0x00BE)
#define REG_DMA1DAD       *(vu32*)(REG_BASE+0x00C0)
#define REG_DMA1DAD_L     *(vu16*)(REG_BASE+0x00C0)
#define REG_DMA1DAD_H     *(vu16*)(REG_BASE+0x00C2)
#define REG_DMA1CNT       *(vu32*)(REG_BASE+0x00C4)
#define REG_DMA1CNT_L     *(vu16*)(REG_BASE+0x00C4)
#define REG_DMA1CNT_H     *(vu16*)(REG_BASE+0x00C6)
#define REG_DMA2SAD       *(vu32*)(REG_BASE+0x00C8)
#define REG_DMA2SAD_L     *(vu16*)(REG_BASE+0x00C8)
#define REG_DMA2SAD_H     *(vu16*)(REG_BASE+0x00CA)
#define REG_DMA2DAD       *(vu32*)(REG_BASE+0x00CC)
#define REG_DMA2DAD_L     *(vu16*)(REG_BASE+0x00CC)
#define REG_DMA2DAD_H     *(vu16*)(REG_BASE+0x00CE)
#define REG_DMA2CNT       *(vu32*)(REG_BASE+0x00D0)
#define REG_DMA2CNT_L     *(vu16*)(REG_BASE+0x00D0)
#define REG_DMA2CNT_H     *(vu16*)(REG_BASE+0x00D2)
#define REG_DMA3SAD       *(vu32*)(REG_BASE+0x00D4)
#define REG_DMA3SAD_L     *(vu16*)(REG_BASE+0x00D4)
#define REG_DMA3SAD_H     *(vu16*)(REG_BASE+0x00D6)
#define REG_DMA3DAD       *(vu32*)(REG_BASE+0x00D8)
#define REG_DMA3DAD_L     *(vu16*)(REG_BASE+0x00D8)
#define REG_DMA3DAD_H     *(vu16*)(REG_BASE+0x00DA)
#define REG_DMA3CNT       *(vu32*)(REG_BASE+0x00DC)
#define REG_DMA3CNT_L     *(vu16*)(REG_BASE+0x00DC)
#define REG_DMA3CNT_H     *(vu16*)(REG_BASE+0x00DE)

      // DMA transfer functionality

#define DMA_ON              0x80000000
#define DMA_IRQ             0x40000000
#define DMA_AT_NOW          0x00000000
#define DMA_AT_VBLANK       0x10000000
#define DMA_AT_HBLANK       0x20000000
#define DMA_AT_FIFO         0x30000000		// for sound ( DMA 1 & 2)
#define DMA_AT_REFRESH      0x30000000		// for video ( DMA 3)
#define DMA_16              0x00000000
#define DMA_32              0x04000000
#define DMA_REPEAT          0x02000000
#define DMA_SRC_INC         0x00000000
#define DMA_SRC_DEC         0x00800000
#define DMA_SRC_FIX         0x01000000
#define DMA_SRC_RESET       0x00600000
#define DMA_DST_INC         0x00000000
#define DMA_DST_DEC         0x00200000
#define DMA_DST_FIX         0x00400000
#define DMA_DST_RESET       0x00600000

#define DMA_32NOW           DMA_ON | DMA_AT_NOW |DMA_32
#define DMA_16NOW           DMA_ON | DMA_AT_NOW |DMA_16

typedef struct tagDMAINFO
{
  const void *src;
  void *dst;
  u32 cnt;
} DMAINFO;

#define dma_mem ((volatile DMAINFO*)0x040000b0)

// DMA transfer macro; looks like a function, but none of the 
//   penalties I luvs meh preprocessor :)
// NOTE: (2004-05-13)
//   I have switched the arguments to make it act more like memcpy
//   Only ch and mode are different. (Well, duh, memcpy doesn't have 
//   those)
// NOTE: apparently the real transfer doesn't start till 2 cycles 
//   after you set the DMAxCNT, so using this twice in a row may 
//   result in badness. (Haven't had any problems myself yet, though)
#define DMA_TRANSFER(dst, src, count, ch, mode)	\
{												\
	REG_DMA##ch##SAD = (u32)(src);				\
	REG_DMA##ch##DAD = (u32)(dst);				\
	REG_DMA##ch##CNT = (count) | (mode);		\
}

// separate enable and disable macros (redundant, but what the hell)
#define DMA_ENABLE(ch)	REG_DMA##ch##CNT |= DMA_ON
#define DMA_DISABLE(ch)	REG_DMA##ch##CNT &= ~DMA_ON


// --- dma prototypes

inline void DMA_Copy(u8 channel, const void* source, void* dest, u32 WordCount, u32 mode)
{
   switch (channel)
   {
      case 0:
         REG_DMA0SAD = (u32)source;
         REG_DMA0DAD = (u32)dest;
         REG_DMA0CNT = WordCount | mode;
         break;
      case 1:
         REG_DMA1SAD = (u32)source;
         REG_DMA1DAD = (u32)dest;
         REG_DMA1CNT = WordCount | mode;
         break;
      case 2:
         REG_DMA2SAD = (u32)source;
         REG_DMA2DAD = (u32)dest;
         REG_DMA2CNT = WordCount | mode;
         break;

      case 3:
         REG_DMA3SAD = (u32)source;
         REG_DMA3DAD = (u32)dest;
         REG_DMA3CNT = WordCount | mode;
         break;
   }
}

inline void DMA_CopyChannel0(const void* source, void* dest, u32 WordCount, u32 mode)
{
   REG_DMA0SAD = (u32)source;
   REG_DMA0DAD = (u32)dest;
   REG_DMA0CNT = WordCount | mode;     
}

inline void DMA_CopyChannel1(const void* source, void* dest, u32 WordCount, u32 mode)
{
   REG_DMA1SAD = (u32)source;
   REG_DMA1DAD = (u32)dest;
   REG_DMA1CNT = WordCount | mode;
}

inline void DMA_CopyChannel2(const void* source, void* dest, u32 WordCount, u32 mode)
{
   REG_DMA2SAD = (u32)source;
   REG_DMA2DAD = (u32)dest;
   REG_DMA2CNT = WordCount | mode;
}

inline void DMA_CopyChannel3(const void* source, void* dest, u32 WordCount, u32 mode)
{
   REG_DMA3SAD = (u32)source;
   REG_DMA3DAD = (u32)dest;
   REG_DMA3CNT = WordCount | mode;
}


//wait for the screen to stop drawing
inline void gbaVsync()
{
   while(! (REG_DISPSTAT & 0x1));
   while(REG_DISPSTAT & 0x1);
//   while (REG_VCOUNT != 160) {}
}

inline void gbaCopySpritePalette( const unsigned short* data, int size )
{
   // fast way
   DMA_TRANSFER( GBA_SPRITE_PAL, data, size, 3, DMA_16NOW );
         
   // slow way
   /*
   for (int i = 0; i < size; ++i)
      GBA_SPRITE_PAL[i] = data[i];
   */
}

inline void gbaCopyBgPalette( const unsigned short* data, int size )
{
   // fast way
   DMA_TRANSFER( GBA_BG_PAL, data, size, 2, DMA_16NOW );
         
   // slow way
   /*
   for (int i = 0; i < size; ++i)
      GBA_BG_PAL[i] = data[i];
   */
}

// copy map data to tile memory
// tile memory is finite (can range from 32x32 to 64x64), but map data can be much larger.
inline void gbaCopyBgMap( u16* dest, 
                         const u16* data, const u32 src_width, const u32 src_height, 
                           u32 copy_x, u32 copy_y, u32 copy_width, u32 copy_height,
                           u32 tilemem_width = 32 )
{
   // clamp values to avoid memory issues.
   copy_x = ::clamp( copy_x, u32(0), src_width );
   copy_y = ::clamp( copy_y, u32(0), src_height );
   copy_width = ::clamp( copy_width, u32(0), (src_width - copy_x) );
   copy_height = ::clamp( copy_height, u32(0), (src_height - copy_y) );
   copy_width = ::clamp( copy_width, u32(0), tilemem_width );

   u32 x, y;
	for(y = 0; y < copy_height; ++y)
	for(x = 0; x < copy_width; ++x)
		dest[x + y * tilemem_width] = data[x + copy_x + (y + copy_y) * src_width];
}

/*inline int worldMapIndex( s32 world_tile_x, s32 world_tile_y, u32 blank )
{
   if (26 <= world_tile_x && world_tile_x <= (26+16) && 51 <= world_tile_y && world_tile_y <= (51+16))
      return 0;
   return blank;
}
*/

// using this function, you can condense a world map down to a small grid.
// scanning across the x/y rooms of the map, will return the upper left corner tile index.
// if "output_tiles" is more than 1, then will sample more tiles for each room (i.e. 2x2 grid in upper left corner)
// roomx/y         - which room to sample, reads from upper left corner of each room.
// output_tilesx/y - how many tiles to sample per room
// map_data        - the map data to read
// map_width/height- width and height of the map data (in tiles)
// map_roomsx/y    - width and height of the map data (in rooms)

inline int worldMapIndex( const u32 roomx, const u32 roomy,
                          const u32 output_tilesx, const u32 output_tilesy,
                          const u16* map_data,
                          const u32 map_width, const u32 map_height,
                          const u32 map_roomsx, const u32 map_roomsy )
{
   u32 xsnap = (roomx / output_tilesx) * output_tilesx;
   u32 ysnap = (roomy / output_tilesy) * output_tilesy;
   u32 xdiff = roomx - xsnap;
   u32 ydiff = roomy - ysnap;
   u32 xmap = xdiff + xsnap * map_width / map_roomsx;
   u32 ymap = ydiff + ysnap * map_height / map_roomsy;
   return map_data[xmap + ymap*map_width];
}

// copy map data to tile memory
// copy a 64bit 8x8 block of binary data into the map at dest_x/y
// the 8x8 bitfield represents 1 tile from each of the 8x8 (64) rooms.
// therefore what we write to the dest is an index of the tiles from 
// the upper left corner of each room, if the bitfield is 0x1 for that location.
// bitfield is indexed using data>>(x + y*8)...
inline void gbaCopyBgMap8x8( u16* dest,
                             u64 data,
                             s32 dest_x, s32 dest_y,
                             const u16* map_data, const u32 map_width, const u32 map_height, 
                             u32 tilemem_width = 32 )
{
   s32 x, y;
	for(y = 0; y < 16; ++y)
   {
   	for(x = 0; x < 16; ++x)
      {
         // index the 64bit 'data' as 8x8
         u32 xsnap = (x / 2);
         u32 ysnap = (y / 2);
         u32 bit = 0x1 & (data>>(xsnap + ysnap * 8));

         if (bit)
         {
            dest[x + dest_x + (y + dest_y) * tilemem_width] = worldMapIndex(x, y, 2,2, map_data, map_width, map_height, 16, 16 );
         }
      }
   }
}

#define RGB16(r,g,b) (r+(g<<5)+(b<<10))    //Macro to build a color from its parts

////////////////////////////////////////////////////////////////////////////////////


#define Mode4VideoBuffer (u16*)0x6000000;
#define Mode4BackBuffer (u16*)0x600A000;
#define Mode4Palette (u16*)0x5000000;
inline void InitMode4()
{
   SetMode( BGMODE3 | OBJENABLE );
}
inline u16* FlipMode4() // this might not work?... 
{
  if (REG_DISPCNT & BACKBUFFER)
  {
    REG_DISPCNT &= ~BACKBUFFER;
    return Mode4VideoBuffer;
  }
  else
  {
    REG_DISPCNT |= BACKBUFFER;
    return Mode4BackBuffer;
  }
}


//////////////////////////////////////////////////////////
// SOUND

// enable sound: REG_SOUNDCNT_X = SND_ENABLED;
#define REG_SOUNDCNT_X *(u32*)0x04000084
//bits:
//0-1  	Output sound ratio for channels 1 - 4 (00 = 25%, 01 = 50%, 10 = 100%)
//2 	Direct Sound A output ratio (0 = 50%, 1 = 100%)
//3 	Direct Sound B output ratio (0 = 50%, 1 = 100%)
//4-7 	Unused
//8 	Enable Direct Sound A output to right speaker (0 = No, 1 = Yes)
//9 	Enable Direct Sound A output to left speaker (0 = No, 1 = Yes)
//A 	Direct Sound A sampling rate Timer (0 = Timer 0, 1 = Timer 1)
//B 	Direct Sound A FIFO reset (write a 1 here to reset the FIFO)
//C 	Enable Direct Sound B output to right speaker (0 = No, 1 = Yes)
//D 	Enable Direct Sound B output to left speaker (0 = No, 1 = Yes)
//E 	Direct Sound B sampling rate Timer (0 = Timer 0, 1 = Timer 1)
//F 	Direct Sound B FIFO reset (write a 1 here to reset the FIFO)

// register definitions
#define REG_SOUNDCNT_L        *(u16*)0x04000080
#define REG_SOUNDCNT_H        *(u16*)0x04000082

// flags
#define SND_ENABLED           0x00000080
#define SND_OUTPUT_RATIO_25   0x0000
#define SND_OUTPUT_RATIO_50   0x0001
#define SND_OUTPUT_RATIO_100  0x0002
#define DSA_OUTPUT_RATIO_50   0x0000
#define DSA_OUTPUT_RATIO_100  0x0004
#define DSA_OUTPUT_TO_RIGHT   0x0100
#define DSA_OUTPUT_TO_LEFT    0x0200
#define DSA_OUTPUT_TO_BOTH    0x0300
#define DSA_TIMER0            0x0000
#define DSA_TIMER1            0x0400
#define DSA_FIFO_RESET        0x0800
#define DSB_OUTPUT_RATIO_50   0x0000
#define DSB_OUTPUT_RATIO_100  0x0008
#define DSB_OUTPUT_TO_RIGHT   0x1000
#define DSB_OUTPUT_TO_LEFT    0x2000
#define DSB_OUTPUT_TO_BOTH    0x3000
#define DSB_TIMER0            0x0000
#define DSB_TIMER1            0x4000
#define DSB_FIFO_RESET        0x8000

// we don’t want to mess with sound channels 1-4
//REG_SOUNDCNT_L = 0;

// enable and reset Direct Sound channel A, at full volume, using
// Timer 0 to determine frequency
//REG_SOUNDCNT_H = SND_OUTPUT_RATIO_100 |
//                 DSA_OUTPUT_RATIO_100 |
//                 DSA_OUTPUT_TO_BOTH |
//                 DSA_TIMER0 |
//                 DSA_FIFO_RESET;

// REG_DMAxSAD (replace x with the channel number 0 - 3)
//Bits 	Function
//0-31 	Source address of the data being sent

// REG_DMAxDAD (replace x with the channel number 0 - 3)
//Bits 	Function
//0-31 	Destination address of the data being sent

// REG_DMAxCNT (replace x with the channel number 0 - 3)
//Bits 	Function
//0-15 	The word count of the data being sent
//16-20 	Unused
//21-22 	Destination register update
//       00 = increment the destination counter after each word transfer
//       01 = decrement the destination counter after each word transfer
//       10 = destination is always the Direct Sound FIFO
//       11 = increment the destination register after each word transfer and reset on DMA completion (used to create so-called Mode 7 effects)
//23-24 	Source register update
//       00 = increment the source counter after each word transfer
//       01 = decrement the source counter after each word transfer
//       10 = fixed (the source counter is the same for each word transfer)
//       11 = invalid setting
//25 	DMA repeat (causes the DMA to be repeated on the interval specified by the timing bits below)
//       0 = No
//       1 = Yes
//26 	Transfer word size (0 = 2 byte words, 1 = 4 byte words)
//27 	Unused
//28-29 	Timing control (determines when the DMA begins)
//       00 = DMA starts immediately
//       01 = DMA starts at the next vblank
//       10 = DMA starts at the next hblank
//       11 = DMA starts when the FIFO needs re-filling
//30 	Causes an interrupt upon completion of the data transfer (0 = No, 1 = Yes)
//31 	Enable the transfer (0 = transfer disabled, 1 = transfer enabled)


// DMA channel 1 register definitions
/*
#define REG_DMA1SAD         *(u32*)0x40000BC  // source address
#define REG_DMA1DAD         *(u32*)0x40000C0  // destination address
#define REG_DMA1CNT         *(u32*)0x40000C4  // control register

// DMA flags
#define WORD_DMA            0x04000000
#define HALF_WORD_DMA       0x00000000
#define ENABLE_DMA          0x80000000
#define START_ON_FIFO_EMPTY 0x30000000
#define DMA_REPEAT          0x02000000
#define DEST_REG_SAME       0x00400000

// Timer 0 register definitions
#define REG_TM0D            *(u16*)0x4000100
#define REG_TM0CNT          *(u16*)0x4000102

// Timer flags
#define TIMER_ENABLED       0x0080

// FIFO address defines
#define REG_FIFO_A          0x040000A0
#define REG_FIFO_B          0x040000A4

// our Timer interval that we calculated earlier (note that this
// value depends on our playback frequency and is therefore not set in
// stone)
#define TIMER_INTERVAL      (0xFFFF - 761)
*/
// set the timer to overflow at the appropriate frequency and start it
//REG_TM0D   = TIMER_INTERVAL;
//REG_TM0CNT = TIMER_ENABLED;

// start the DMA transfer (assume that pSample is a (signed char*)
// pointer to the buffer containing our sound data)
//REG_DMA1SAD = (u32)(pSample);
//REG_DMA1DAD = (u32)REG_FIFO_A;
//REG_DMA1CNT = ENABLE_DMA | START_ON_FIFO_EMPTY | WORD_DMA | DMA_REPEAT;




//
#endif

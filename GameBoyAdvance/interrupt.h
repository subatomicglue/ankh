// interrupt.h: 
// interrupt header file
//
// (Created: 2003-11, Modified: 2004-05-21, Cearn)

#ifndef GBA_INTERRUPT
#define GBA_INTERRUPT

//#include "types.h"

// === CONSTANTS ======================================================

/// interrupt index (to index IntrTable)
enum eIntrIndex
{
	II_VBLANK=0, II_HBLANK, II_VCOUNT, II_TM0, 
	II_TM1,      II_TM2,    II_TM3,    II_COM, 
	II_DMA0,     II_DMA1,   II_DMA2,   II_DMA3, 
	II_KEYS,     II_CART
};

/// interrupt flags for REG_IE, REG_IF, REG_IFBIOS
#define INT_VBLANK      0x0001
#define INT_HBLANK      0x0002
#define INT_VCOUNT      0x0004
#define INT_TM0         0x0008
#define INT_TM1         0x0010
#define INT_TM2         0x0020
#define INT_TM3         0x0040
#define INT_COM         0x0080
#define INT_DMA0        0x0100
#define INT_DMA1        0x0200
#define INT_DMA2        0x0400
#define INT_DMA3        0x0800
#define INT_KEYS        0x1000
#define INT_CART        0x2000

#define INT_ALL         0x4000
// === GLOBALS ========================================================

// interrupt table
// example:
//      // set function pointer to be called on vblank
//      int_init();
//      int_enable(INT_VBLANK);
//      REG_DISPSTAT |= VID_IRQ_VB;
//      IntrTable[II_VBLANK]= vblFunc;
//      REG_IME= 1;
//
extern fp IntrTable[];

// === PROTOTYPES =====================================================

// default interrupts

void IntrMain();	// see single_ints.s for definition

void int_init();	// Must be called for interrupts to work
void int_dummy();	// default interrupt function (does nothing)

// simple interrupt en/disable
inline void int_enable(u16 flags);
inline void int_disable(u16 flags);

// advanced interrupt en/disable
// NOTE: uses the II_xx enums, not the INT_xx flags
void int_enable_ex(enum eIntrIndex eii, fp isr);
void int_disable_ex(enum eIntrIndex eii);

// === MACROS =========================================================

// === INLINES =========================================================

inline void int_enable(u16 flags)
{	REG_IE |= flags;		}

inline void int_disable(u16 flags)
{	REG_IE &= ~flags;	}

#endif // GBA_INTERRUPT


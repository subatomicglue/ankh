// interrupt.c: 
// interrupt implementation file
//
// (Created: 2003-11, Modified: 2004-05-21, Cearn)

//#include "types.h"
//#include "regs.h"

// types
#ifndef __cplusplus
typedef     unsigned char           bool;
#endif
typedef void (*fp)(void);

typedef     unsigned char           u8;
typedef     unsigned short int      u16;
typedef     unsigned int            u32;
typedef     unsigned long long int  u64;
typedef     signed char             s8;
typedef     signed short int        s16;
typedef     signed int              s32;
typedef     signed long long int    s64;
typedef     volatile unsigned char           vu8;
typedef     volatile unsigned short int      vu16;
typedef     volatile unsigned int            vu32;
typedef     volatile unsigned long long int  vu64;
typedef     volatile signed char             vs8;
typedef     volatile signed short int        vs16;
typedef     volatile signed int              vs32;
typedef     volatile signed long long int    vs64;
typedef     s16     sfp16;  //1:7:8 fixed point
typedef     s32     sfp32;  //1:19:8 fixed point
typedef     u16     ufp16;  //8:8 fixed point
typedef     u32     ufp32;  //24:8 fixed point
#define REG_BASE    MEM_IO
#define REG_INTMAIN      *(fp*)(REG_BASE-0x0004)	// -> 0300:7ffc
#define REG_IE         (*(vu16*)0x4000200)
#define REG_IF         (*(vu16*)0x4000202)
#define MEM_IO         0x04000000
#define REG_IME        (*(vu16*)0x4000208)
#define REG_VCOUNT        *(vu16*)(REG_BASE+0x0006)	// vertical count

#include "interrupt.h"

// === CLASSES ========================================================

// used for int_enable_ex and int_disable_ex
typedef struct tagINT_SENDER
{
	u16 reg_ofs, flag;
}	INT_SENDER;

// === GLOBALS ========================================================

fp IntrTable[] = 
{
	int_dummy,		// INT_VBLANK
	int_dummy,		// INT_HBLANK
	int_dummy,		// INT_VCOUNT
	int_dummy,		// INT_TM0
	int_dummy,		// INT_TM1
	int_dummy,		// INT_TM2
	int_dummy,		// INT_TM3
	int_dummy,		// INT_COM
	int_dummy,		// INT_DMA0
	int_dummy,		// INT_DMA1
	int_dummy,		// INT_DMA2
	int_dummy,		// INT_DMA3
	int_dummy,		// INT_KEYS
	int_dummy,		// INT_CART
};

// yeah, yeah, I really should use the registers and defines
// I have else where.
// NOTE: haven't really tested this very much; if inaccurate, 
// plz tell me
static const INT_SENDER _int_senders[] = 
{
	{ 0x0004, 0x0008 },		// REG_DISPSTAT, VID_IRQ_VB
	{ 0x0004, 0x0010 },		// REG_DISPSTAT, VID_IRQ_VH
	{ 0x0004, 0x0020 },		// REG_DISPSTAT, VID_IRQ_VC
	{ 0x0102, 0x0040 },		// REG_TM0CNT, TM_IRQ
	{ 0x0106, 0x0040 },		// REG_TM1CNT, TM_IRQ
	{ 0x0108, 0x0040 },		// REG_TM2CNT, TM_IRQ
	{ 0x0102, 0x0040 },		// REG_TM3CNT, TM_IRQ
	{ 0x0128, 0x4000 },		// REG_SCCNT_L{14}			// unsure
	{ 0x00ba, 0x4000 },		// REG_DMA0CNT_H, DMA_IRQ>>16
	{ 0x00c6, 0x4000 },		// REG_DMA1CNT_H, DMA_IRQ>>16
	{ 0x00d2, 0x4000 },		// REG_DMA2CNT_H, DMA_IRQ>>16
	{ 0x00de, 0x4000 },		// REG_DMA3CNT_H, DMA_IRQ>>16
	{ 0x0132, 0x4000 },		// REG_P1CNT, KEY_CNT_IRQ
	{ 0x0000, 0x0000 },		// none
};
// === PROTOTYPES =====================================================

// === FUNCTIONS ======================================================

// IntrMain is an asm function in single_ints.s
void int_init()
{
	REG_INTMAIN = IntrMain;
}

void int_enable_ex(enum eIntrIndex eii, fp isr)
{
	REG_IME=0;

	const INT_SENDER *is= &_int_senders[eii];
	*(u16*)(REG_BASE+is->reg_ofs) |= is->flag;
	if(isr != 0) // isr != NULL
		IntrTable[eii]= isr;

	REG_IE |= 1 << eii;
	REG_IME= 1;
}

void int_disable_ex(enum eIntrIndex eii)
{
	u16 tmp= REG_IME;
	REG_IME= 0;

	const INT_SENDER *is= &_int_senders[eii];
	*(u16*)(REG_BASE+is->reg_ofs) &= ~is->flag;

	REG_IE &= ~(1 << eii);
	REG_IME= tmp;
}
 

// --- dummy isrs ---
// (no need to set REG_IF or REG_IFBIOS bits, 
//  single_ints.s' switchboard does that for us)
void int_dummy() {}



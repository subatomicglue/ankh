
#ifndef SPEW_GBA_H
#define SPEW_GBA_H


//#include <stdio.h>
#include <stdarg.h> // varargs
//#include <assert.h>
#include <cstdio> // vsnprinf
#include <string.h>

// mini spew : trace output (to VisualBoyAdvance), for GBA apps.
namespace spew
{
   enum TraceFilter
   {
      FILTERDEFAULT = 0xffffffff, //< FILTERALL
      FILTERALL = 0xffffffff,  //< always output
      FILTERNONE = 0x00000000, //< never output
      GFX      = 0x00000001,
      SCRIPT   = 0x00000002,
      SOUND    = 0x00000004,
      PHYSICS  = 0x00000008,
      IO       = 0x00000010,
      ANIM     = 0x00000020,
      LUA      = 0x00000040,
      ERROR    = 0x00000080,
      ACTOR    = 0x00000100,
      APP      = 0x00000200,
   };
   inline void doNothing() {}
#ifdef USE_GBA_TRACE
   extern unsigned char g_nTraceLevel;
   extern unsigned short g_nTraceFilter;
   inline void SetLevel( int lvl ) { g_nTraceLevel = lvl; }
   inline void SetFilter( int filter ) { g_nTraceFilter = filter; }
#else
   inline void SetLevel( int lvl ) {}
   inline void SetFilter( int filter ) {}
#endif


#ifdef USE_GBA_TRACE
   void GBAPrint( const char* const s );
   void GBAPrint( const char* const s, const char* const s1 );
   void GBAPrint( const char* const s, int d);
   void GBAPrint( const char* const s, int d1, int d2);
   void GBAPrint( const char* const s, int d1, int d2, int d3);
   void GBAPrint( const char* const s, int d1, int d2, int d3, int d4);
#else
   #define GBAPrint(...) doNothing()
#endif

#if 0 //def USE_GBA_TRACE
   /// Debug Trace... (printf that goes to VisualBoyAdvance debug window)
   void Trace(char *fmtstr, ...);

   /// Debug Trace... (printf that goes to VisualBoyAdvance debug window)
   void Trace( spew::TraceFilter filter, char *fmtstr, ... );
   
   /// Debug Trace... (printf that goes to VisualBoyAdvance debug window)
   void Trace( spew::TraceFilter filter, unsigned char lvl, char *fmtstr, ... );
#else
   // no ..., seems to be buggy on gba...
   #define Trace(...) doNothing()
#endif



class AssertBase
{
public:
	/// return true if break
	bool callAssert( const char* str, int line, const char* file )
	{
      char buf[256];
      sprintf( buf, "assertion:    '%s'\n   in '%s' line %d\n", str, file, line );
      //spew::Trace( "assertion:    '%s'\n   in '%s' line %d\n", str, file, line );
      spew::GBAPrint( buf );
      return true;
	}
	static AssertBase& instance() { static AssertBase b; return b; }
};
inline void need_namespace_before_Assert() {};

/// trigger a compiler breakpoint
#if defined( WIN32 ) && defined(_MSC_VER) && (_MSC_VER >= 1020)
	#define TRIGGERBREAK() __asm int 3
#else //< fallback if no compiler specific break instruction available.
   #define TRIGGERBREAK() while (1) {};
#endif

#ifdef _DEBUG
	#define Assert( test,message ) need_namespace_before_Assert(); \
	   if (!(test)) \
	   { \
		   /* see if we should break (true) or continue (false) */ \
         if (spew::AssertBase::instance().callAssert( message, __LINE__, __FILE__ )) \
		   { \
			   TRIGGERBREAK(); \
		   } \
	   }
#else
	#define Assert( test, message ) need_namespace_before_Assert()
#endif

/// TODO: upgrade to boost's static assert for message output...
template <bool> struct STATIC_ASSERTION_FAILURE;
template <> struct STATIC_ASSERTION_FAILURE<true>{};
#define STATICASSERT( test, str )  sizeof( STATIC_ASSERTION_FAILURE<test> )




} // mini spew


#endif


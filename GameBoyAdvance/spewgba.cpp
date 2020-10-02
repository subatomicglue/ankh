
#include "spewgba.h"

// mini spew : trace output (to VisualBoyAdvance), for GBA apps.
namespace spew
{
#ifdef USE_GBA_TRACE
   unsigned char g_nTraceLevel = 0;
   unsigned short g_nTraceFilter = spew::APP;

   // THUMB code
/*  void GBAPrint(char *s)
   {
     asm volatile("mov r0, %0;"
                  "swi 0xff;"
                  : // no ouput
                  : "r" (s)
                  : "r0");
   }
*/
   // arm code
   void GBAPrint( const char* const s )
   {
     asm volatile("mov r0, %0;"
                  "swi 0xff0000;"
                  : // no ouput
                  : "r" (s)
                  : "r0");
   }

   void GBAPrint( const char* const s, int d1 )
   {
      char buf[256];
      sprintf( buf, s, d1 );
      Assert( strlen( buf ) < 256, "out of bounds" );
      GBAPrint( buf );
   }
   void GBAPrint( const char* const s, const char* const s1 )
   {
      char buf[256];
      sprintf( buf, s, s1 );
      Assert( strlen( buf ) < 256, "out of bounds" );
      GBAPrint( buf );
   }
   void GBAPrint(const char* const s, int d1, int d2)
   {
      char buf[256];
      sprintf( buf, s, d1, d2 );
      Assert( strlen( buf ) < 256, "out of bounds" );
      GBAPrint( buf );
   }
   void GBAPrint(const char* const s, int d1, int d2, int d3)
   {
      char buf[256];
      sprintf( buf, s, d1, d2, d3 );
      Assert( strlen( buf ) < 256, "out of bounds" );
      GBAPrint( buf );
   }
   void GBAPrint(const char* const s, int d1, int d2, int d3, int d4)
   {
      char buf[256];
      sprintf( buf, s, d1, d2, d3, d4 );
      Assert( strlen( buf ) < 256, "out of bounds" );
      GBAPrint( buf );
   }
#endif

#if 0 //def USE_GBA_TRACE

   /// Debug Trace... (printf that goes to VisualBoyAdvance debug window)
   inline void vTrace(char *fmtstr, va_list arg_ptr)
   {
      const static int MAX_BUF_SIZE = 256;
      char buf[MAX_BUF_SIZE] = "\0";
      vsprintf( buf, fmtstr, arg_ptr );
   
      if (MAX_BUF_SIZE <= strlen( buf ))
      {
         GBAPrint( "overflow in gba_printf()\n" );
         assert( strlen( buf ) < MAX_BUF_SIZE && "overflow" );
      }

      // ensure nul termination
      buf[MAX_BUF_SIZE-1] = '\0';

      GBAPrint( buf );
   }

   /// Debug Trace... (printf that goes to VisualBoyAdvance debug window)
   void Trace(char *fmtstr, ...)
   {
      va_list arg_ptr;
      va_start( arg_ptr, fmtstr );
         vTrace(fmtstr, arg_ptr);
      va_end( arg_ptr );
   }
   
   /// Debug Trace... (printf that goes to VisualBoyAdvance debug window)
   void Trace( spew::TraceFilter filter, char *fmtstr, ... )
   {
      if (g_nTraceFilter & filter)
      {
         va_list arg_ptr;
         va_start( arg_ptr, fmtstr );
            vTrace(fmtstr, arg_ptr);
         va_end( arg_ptr );
      }
   }
   
   /// Debug Trace... (printf that goes to VisualBoyAdvance debug window)
   void Trace( spew::TraceFilter filter, unsigned char lvl, char *fmtstr, ... )
   {
      if (g_nTraceFilter & filter && lvl <= g_nTraceLevel)
      {
         va_list arg_ptr;
         va_start( arg_ptr, fmtstr );
            vTrace(fmtstr, arg_ptr);
         va_end( arg_ptr );
      }
   }
#endif

} // mini spew

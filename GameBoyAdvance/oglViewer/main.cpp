// simple glut application: look for the !!!TODO!!!s, and fill in your code 
//                          there as needed
// kevin meinert - kevn@vrac.iastate.edu
// Public domain
//

typedef     unsigned char           u8;
typedef     unsigned short int      u16;
typedef     unsigned int            u32;
typedef     unsigned long long int  u64;
typedef     signed char             s8;
typedef     signed short int        s16;
typedef     signed int              s32;
typedef     signed long long int    s64;

#ifdef WIN32
   #include <windows.h>  // make the app win32 friendly. :)
#endif
//#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
#include <GL/glew.h>
#include <GLUT/glut.h>

#define EDITOR


#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <vector>

#include "Vec2.h"
#include "AABox2.h"
#include "StopWatch.h"
#include "BitmapAsset.h"
#include "MapAsset.h"
#include "old-data.h"
#include "fstream"

typedef unsigned short u16;
typedef unsigned char u8;



// pre: supply a 16bit palette with 256 entries, and an indexed bitmap with width*height size
void convert( std::vector<char>& image, int width, int height, int ch, const u8 bitmap[], const u16 palette[], bool flipVert = 0 )
{
   image.resize( width * height * ch );
   for (unsigned int y = 0; y < height; ++y)
   {
      for (unsigned int x = 0; x < width; ++x)
      {
         unsigned int tsize = 8;
         unsigned int tilex = x / tsize;
         unsigned int tiley = y / tsize;
         unsigned int col = x % tsize, row = y % tsize;
         unsigned int bmpindex = (tilex + tiley*(width/tsize))*tsize*tsize + col + row * tsize;
         unsigned int index = bitmap[bmpindex];
         int yy = y;
         if (flipVert)
         {
            yy = (height - 1)- yy;
         }
         image[(x*ch + yy*ch * width) + 0] = (palette[index] & 0x001f ) >> 0;  // R
         image[(x*ch + yy*ch * width) + 1] = (palette[index] & 0x03e0 ) >> 5;  // g
         image[(x*ch + yy*ch * width) + 2] = (palette[index] & 0x7c00 ) >> 10; // B
         if (ch == 4)
            image[(x*ch + yy*ch * width) + 3] = 255; // ignore bit 15, no alpha. 
         if (index == 0)
            image[(x*ch + yy*ch * width) + 3] = 0; // alpha key
         
         // shift it into something we can see....
         image[(x*ch + yy*ch * width) + 0] <<= 3;
         image[(x*ch + yy*ch * width) + 1] <<= 3;
         image[(x*ch + yy*ch * width) + 2] <<= 3;
      }
   }
}



#include <map>
#include <unordered_map>
#include "BuiltInText.h"

// a place to store application data...
class App
{
public:
   App() : width( 0 ), height( 0 ), mainWin_contextID( -1 ), camx( 0 ), camy( 0 ), rightbutton( false ), leftbutton( false ), downbutton( false ), upbutton( false )
   {
      oStopWatch.pulse();
   }
   void init()
   {
      zoomFactor = 1.0f;

      PixmapFontBuiltin::init();
      pickedActor = NULL;
      pickrequest = false;
      tileselectx = 0;
      tileselecty = 0;
      tilemapselected = 0;

      cloneMapAsset( oMap, wallsMap );
   }

   void initGraphics();

   int width, height;
   int mainWin_contextID;
   float camx, camy;

   std::map< const gba::BitmapAsset*, std::vector<char> > images;

   std::map< const gba::BitmapAsset*, GLuint > texobjs;
   bool rightbutton, leftbutton, downbutton, upbutton;

   aki::StopWatch oStopWatch;

   bool zoomin, zoomout;
   float zoomFactor;

   Actor* pickedActor;
   bool pickrequest;
   gba::Vec<float> pickoffset;
   
   float mousex, mousey;
   int tileselectx, tileselecty;

   gba::MapAssetTemp oMap;
   int tilemapselected;
};
App app;


void saveOut()
{
   const float tilesizex = wallsBitmap.tpx, tilesizey = wallsBitmap.tpy;
   const float tilesperroomX = tilesizex * 12.0f, tilesperroomY = tilesizey * 12.0f;

   std::ofstream output( "actors.out.txt" );
   for (int x = 0; x < gActorCount; ++x)
   {
      float roomx = gAllActors[x]->startRoom()[0];
      float roomy = gAllActors[x]->startRoom()[1];
      float localroomx = (float(gAllActors[x]->m_oPos[0] / 2) - ((roomx * tilesperroomX) - (1.0f * tilesizex))) / tilesizex;
      float localroomy = (float(gAllActors[x]->m_oPos[1] / 2) - ((roomy * tilesperroomY) - (1.0f * tilesizey))) / tilesizey;
      output << gAllActors[x]->name() << " " << localroomx << ", " << localroomy << "\n";
   }
   output.close();

   output.open( "map.out.txt" );
   output << "const u16 walls_Map[" << app.oMap.tx << "*" << app.oMap.ty << "] = {\n";
   for (int y = 0; y < app.oMap.ty; ++y)
   {  
      output << "\t";
      for (int x = 0; x < app.oMap.tx; ++x)
      {
         output << app.oMap.getTile( x, y ) << ",";
      }
      output << "\n";
   }
   output << "};";
   output.close();
}

int maximum( int a, int b )
{
   return a > b ? a : b;
}

enum Filtering
{
   NEAREST, LINEAR, MIPMAP_NEAREST, MIPMAP_LINEAR
};

int loadTex( char* data, 
            int width, int height, Filtering f = NEAREST, 
            int mipmapLevelOfDetail = 0,
            int format = GL_RGBA, 
            int type = GL_UNSIGNED_BYTE, 
            int bpc = 32,
            int channels = 4, 
            int bordersize = 0 )
{
   //make sure the alignment matches the pixel size in bytes
   ::glPixelStorei( GL_UNPACK_ALIGNMENT, bpc );

   // fatal error. Nothing to render
   if ( data == NULL )
      return false; 

   // find out what dimension texture we're using
   int texDimension;
   if( height == 1 || width == 1 )
   {
      // handle one dimensional images...
      int length = maximum( width, height );
      if (f != NEAREST ||
            f != LINEAR)
      {
         ::gluBuild1DMipmaps( GL_TEXTURE_1D, 
                              channels, length, 
                              format, type, data );
      } 
      else 
      {
         ::glTexImage1D( GL_TEXTURE_1D, mipmapLevelOfDetail, 
                        channels, length, 
                        bordersize, format, type, data );
      }

      texDimension = GL_TEXTURE_1D;	
   } 

   else 
   {
      // other wise, handle 2D images.
      if (f == MIPMAP_NEAREST ||
            f == MIPMAP_LINEAR)
      {
         ::gluBuild2DMipmaps( GL_TEXTURE_2D, 
                              channels, width, 
                              height, format, type, 
                              data );
      }
      else 
      {
         ::glTexImage2D( GL_TEXTURE_2D, mipmapLevelOfDetail, 
                        channels, width, 
                        height, bordersize, format, 
                        type, data );
      }

      texDimension = GL_TEXTURE_2D;
   }

   // figure out what filtering to use.
   // set the filtering for the texture...
   switch (f)
   {
   case NEAREST:
      ::glTexParameteri( texDimension, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
      ::glTexParameteri( texDimension, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
   break;

   case LINEAR:
      ::glTexParameteri( texDimension, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
      ::glTexParameteri( texDimension, GL_TEXTURE_MIN_FILTER, GL_LINEAR );	
   break;

   case MIPMAP_NEAREST:
      ::glTexParameteri( texDimension, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
      ::glTexParameteri( texDimension, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST );
   break;

   case MIPMAP_LINEAR:
      ::glTexParameteri( texDimension, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
      ::glTexParameteri( texDimension, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
   break;
   default:
      assert(false);
   }

   return true;
}

void App::initGraphics()
{
   assert( &player.m_rfoActorInit == &playerInit );
   assert( player.bitmap() == &other );
   for (int x = 0; x < gBitmapCount; ++x)
   {
      convert( images[gAllBitmaps[x]], gAllBitmaps[x]->px(), gAllBitmaps[x]->py(), 4, gAllBitmaps[x]->d, gAllBitmaps[x]->palette );
   }
   convert( images[&quake3], quake3.px(), quake3.py(), 4, quake3.d, quake3.palette, 1 );

   for (int x = 0; x < gBitmapCount; ++x)
   {
      glGenTextures( 1, &texobjs[gAllBitmaps[x]] );
      glBindTexture( GL_TEXTURE_2D, texobjs[gAllBitmaps[x]] );
      loadTex( &(images[gAllBitmaps[x]])[0], gAllBitmaps[x]->px(), gAllBitmaps[x]->py(), MIPMAP_LINEAR );
   }
}

void drawTileSet()
{
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, app.texobjs[&wallsBitmap] );
    //loadTex( &(app.images[&wallsBitmap])[0], wallsBitmap.px(), wallsBitmap.py(), MIPMAP_LINEAR );
}

void drawQuad( float xstart, float ystart, float xend, float yend,
               float xstartTex, float ystartTex, float xendTex, float yendTex, float Z = 200 )
{
   glBegin( GL_TRIANGLE_STRIP );
      
         glTexCoord2f( xstartTex, ystartTex );
         glColor3f( 1.0f, 1.0f, 1.0f );
         glNormal3f( 0,0,1 );
         glVertex3f( xstart, ystart, -Z );
         
         glTexCoord2f( xstartTex, yendTex );
         glColor3f( 1.0f, 1.0f, 1.0f );
         glNormal3f( 0,0,1 );
         glVertex3f( xstart, yend, -Z );
         
         glTexCoord2f( xendTex, ystartTex );
         glColor3f( 1.0f, 1.0f, 1.0f );
         glNormal3f( 0,0,1 );
         glVertex3f( xend, ystart, -Z );

         glTexCoord2f( xendTex, yendTex );
         glColor3f( 1.0f, 1.0f, 1.0f );
         glNormal3f( 0,0,1 );
         glVertex3f( xend, yend, -Z );
      
   glEnd();
}

void drawMapTileSelection( int width, int height, const u16 map[] )
{
	// draw a single quad with the selected tile so you can see what you've got selected...
   int tilesize = wallsBitmap.tpx;
   int mapsize = wallsBitmap.px();
   glPushMatrix();
   glLoadIdentity();      
   int x = app.tileselectx, y = app.tileselecty;
   float xtex = x * float( wallsBitmap.tpx ) / float( wallsBitmap.px() );
   float ytex = y * float( wallsBitmap.tpy ) / float( wallsBitmap.py() );
   drawQuad( -app.width/2, -app.height/2, -app.width/2 + wallsBitmap.tx, -app.height/2 + wallsBitmap.ty, 
			xtex, ytex, xtex + 1.0f/float(wallsBitmap.tx) - 1.0f/float( wallsBitmap.px() ), 
			ytex + 1.0f/float( wallsBitmap.ty ) - 1.0f/float( wallsBitmap.py() ), 100 );

   // some status text about the selected tile.
   glBindTexture( GL_TEXTURE_2D, app.texobjs[&quake3] );
   //PixmapFontBuiltin::pushTex();
   const int charheight = 2;
   char buf[256];
   app.tilemapselected = x + y*(mapsize/tilesize);
   sprintf( buf, "(%d, %d) tile# %d", x, y, app.tilemapselected );
   gBuiltInText.render( buf, -app.width/2, -app.height/2 + 32 + charheight, -100, 12 );
   glPopMatrix();

   // draw a little red [] around whatever tile is selected.
   glDisable( GL_TEXTURE_2D );
 
   glBegin( GL_LINE_STRIP );
      glColor3f( 1,0,0 );
      glVertex3f( -(mapsize - x*tilesize), -(mapsize - y*tilesize), -100 );
      glVertex3f( -(mapsize - x*tilesize), -(mapsize - y*tilesize) + tilesize, -100 );
      glVertex3f( -(mapsize - x*tilesize) + tilesize, -(mapsize - y*tilesize) + tilesize, -100 );
      glVertex3f( -(mapsize - x*tilesize) + tilesize, -(mapsize - y*tilesize), -100 );
      glVertex3f( -(mapsize - x*tilesize), -(mapsize - y*tilesize), -100 );
   glEnd();
   glEnable( GL_TEXTURE_2D );
}

void drawSprites()
{
   glEnable( GL_TEXTURE_2D );
   for (int x = 0; x < gActorCount; ++x)
   {
      const gba::BitmapAsset* const index = gAllActors[x]->bitmap();
      glBindTexture( GL_TEXTURE_2D, app.texobjs[gAllActors[x]->bitmap()] );
      //loadTex( &(app.images[index])[0], gAllActors[x]->bitmap().px(), gAllActors[x]->bitmap().py(), MIPMAP_LINEAR );
      drawQuad( gAllActors[x]->m_oPos[0], 
                gAllActors[x]->m_oPos[1], 
            (gAllActors[x]->m_oPos[0] + gAllActors[x]->bitmap()->px()), 
            (gAllActors[x]->m_oPos[1] + gAllActors[x]->bitmap()->py()),
               0.0f,0.0f,1.0f,1.0f, 199 );
   }

   glBindTexture( GL_TEXTURE_2D, app.texobjs[&quake3] );
   //PixmapFontBuiltin::pushTex();
   for (int x = 0; x < gActorCount; ++x)
   {
      const int charheight = 2;
      gBuiltInText.render( gAllActors[x]->name(), (float)gAllActors[x]->m_oPos[0], (float)gAllActors[x]->m_oPos[1] - charheight, -197.0f, 4 );
   }
}

void drawMap( int width, int height, const u16 map[])
{
   for (int x = 0; x < width; ++x)
   for (int y = 0; y < height; ++y)
   {
      int xpos = x * wallsBitmap.tpx, ypos = y * wallsBitmap.tpy;
      
      int index = map[x + y*height];
      float xtile = index % wallsBitmap.tx;
      float ytile = index / wallsBitmap.tx;
      float xtex = xtile / float(wallsBitmap.tx);
      float ytex = ytile / float(wallsBitmap.ty);
      drawQuad( xpos, ypos, xpos+wallsBitmap.tpx, (ypos+wallsBitmap.tpy), xtex,ytex, 
				xtex + 1.0f/float(wallsBitmap.tx) - 1.0f/float( wallsBitmap.px() ), 
				ytex + 1.0f/float(wallsBitmap.ty) - 1.0f/float( wallsBitmap.py() ) );
   }
}

//////////////////////////////////////////////////
// This is called on a glutPostRedisplay
//////////////////////////////////////////////////
static void OnRedisplay()
{ 
   // context init.
   static bool bFirstTime = true;
   if (bFirstTime)
   {
      app.initGraphics();
      bFirstTime = false;
   }

   glClearColor( 0.0f, 0.0f, 1.0f, 1.0f );
   glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
   glEnable( GL_DEPTH_TEST );
   glEnable( GL_BLEND );
   glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ); 
   
   
   // set up the projection matrix
   glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        glOrtho( -app.width/2.0f, app.width/2.0f, app.height/2.0f, -app.height/2.0f, 0.01f, 1000.0f );
        //gluPerspective( 80.0f, app.width / app.width, 0.01f, 1000.0f );
                           
   // initialize your matrix stack used for transforming your models
    glMatrixMode( GL_MODELVIEW );
      glLoadIdentity();      

      glScalef( app.zoomFactor, app.zoomFactor, app.zoomFactor );

   // !!!TODO!!!: replace the following with your own opengl commands!
   glTranslatef( app.camx, app.camy, 0 );
   drawTileSet();
   drawQuad( -wallsBitmap.px(), -wallsBitmap.py(), 0, 0, 0,0, 1,1 );
   drawMap( app.oMap.tx, 96*2, app.oMap.d );
   drawMapTileSelection( 96*2, 96*2, app.oMap.d );
   drawSprites();

   // !!!TODO!!!: ////////////////////////////////////////
   
   // swaps the front and back frame buffers.
   // hint: you've been drawing on the back, offscreen, buffer.  
   // This command then brings that framebuffer onscreen.
   glutSwapBuffers();
}

static void OnMouseEvent()
{
   // manipulation of actors with mouse
   if (!app.pickrequest)
   {
      app.pickedActor = NULL;
   }
   if (app.pickrequest && app.pickedActor == NULL)
   {
      gba::Vec<float> pickpoint( app.mousex / app.zoomFactor - app.camx,
                                 app.mousey / app.zoomFactor - app.camy );
      
      for (int x = 0; x < gActorCount && app.pickedActor == NULL; ++x)
      {
         gba::AABox2<float> box( gba::Vec<float>( gAllActors[x]->m_oPos[0], gAllActors[x]->m_oPos[1] ),
                                 gba::Vec<float>(  gAllActors[x]->m_oPos[0] + gAllActors[x]->bitmap()->px(), 
                                                   gAllActors[x]->m_oPos[1] + gAllActors[x]->bitmap()->py() ) );
         if (gba::intersect( box, pickpoint ))
         {
            app.pickedActor = gAllActors[x];
            app.pickoffset = gba::Vec<float>( pickpoint[0] - gAllActors[x]->m_oPos[0],
                                              pickpoint[1] - gAllActors[x]->m_oPos[1] );
         }
      }
   }
   if (app.pickedActor)
   {
      app.pickedActor->m_oPos[0] = app.mousex / app.zoomFactor - app.camx - app.pickoffset[0];
      app.pickedActor->m_oPos[1] = app.mousey / app.zoomFactor - app.camy - app.pickoffset[1];
   }

   // if pick didn't hit an actor, try the tiles...
   if (!app.pickedActor && app.pickrequest)
   {
      int worldmousex = app.mousex / app.zoomFactor - app.camx;
      int worldmousey = app.mousey / app.zoomFactor - app.camy;
      float tileSizeInPixels = float( wallsBitmap.tpx );
      
      // if intersect tilemap selection
      if (worldmousex < 0 && worldmousey < 0 && -wallsBitmap.px() < worldmousex && -wallsBitmap.py() < worldmousey)
      {
         app.tileselectx = (float( wallsBitmap.px() ) - (-worldmousex)) / float( wallsBitmap.tpx );
         app.tileselecty = (float( wallsBitmap.py() ) - (-worldmousey)) / float( wallsBitmap.tpy );
      }
      // if intersect the game map, assign the currently selected tile to the location in the game map.
      else if (0 <= worldmousex && 0 <= worldmousey && worldmousex <= app.oMap.tx*tileSizeInPixels && worldmousey <= app.oMap.ty*tileSizeInPixels)
      {
         int mapx = worldmousex / tileSizeInPixels;
         int mapy = worldmousey / tileSizeInPixels;
         app.oMap.d[mapx + mapy * app.oMap.ty] = app.tilemapselected;
      }
   }
}

//////////////////////////////////////////////////
// This is called repeatedly, as fast as possible
//////////////////////////////////////////////////
static void OnIdle()
{
   app.oStopWatch.pulse();

   // map navigation...
   const float cnMoveSpeed = 256.0f;
   if (app.rightbutton)
   {
      app.camx -= app.oStopWatch.timeDelta() * cnMoveSpeed;
   }
   if (app.upbutton)
   {
      app.camy += app.oStopWatch.timeDelta() * cnMoveSpeed;
   }
   if (app.downbutton)
   {
      app.camy -= app.oStopWatch.timeDelta() * cnMoveSpeed;
   }
   if (app.leftbutton)
   {
      app.camx += app.oStopWatch.timeDelta() * cnMoveSpeed;
   }
   if (app.zoomin)
   {
      app.zoomFactor +=  app.oStopWatch.timeDelta() * 1.0f;
   }
   if (app.zoomout)
   {
      app.zoomFactor -=  app.oStopWatch.timeDelta() * 1.0f;
   }
   app.zoomFactor = clamp( app.zoomFactor, 0.1f, 20.0f );
   
   // According to the GLUT specification, the current window is
   // undefined during an idle callback.  So we need to explicitly change
   // it if necessary
   if ( glutGetWindow() != app.mainWin_contextID )
           glutSetWindow( app.mainWin_contextID );

   // tell glut to call redisplay (which then calls OnRedisplay)
   glutPostRedisplay();
}

/////////////////////////////////////////////
// This is called on a Resize of the glut window
/////////////////////////////////////////////
static void OnReshape( int width, int height ) 
{
   // save these params in case your app needs them
   app.width = width;
   app.height = height;
   
   // set your viewport to the extents of the window
   glViewport( 0, 0, width, height );
   
   // let the app run idle, while resizing, 
   // glut does not do this for us automatically, so call OnIdle explicitly.
   OnIdle();
}

////////////////////////////////
// This is called on a Down Keypress        
////////////////////////////////
static void OnKeyboardDown( unsigned char k, int x, int y )
{ 
   switch (k)
   {
   // If user pressed 'q' or 'ESC', then exit the app.
   // this is really ungraceful, but necessary since GLUT does a while(1)
   // as it's control loop.  There is no GLUT method to exit, unfortunately.
   case 'q':
   case 27:
      exit( 0 );
      break;

   case 'a':
      // !!!TODO!!!: add handler for when UP is pressed
      break;

   case 'z':
      // !!!TODO!!!: add handler for when DOWN is pressed
      break;
   case '=':
   case '+':
      app.zoomin = true;
      break;
   case '-':
      app.zoomout = true;
      break;
      
   default:
      // do nothing if no key is pressed
      break;
   }
}
////////////////////////////////
// This is called on a Up Keypress        
////////////////////////////////
static void OnKeyboardUp( unsigned char k, int x, int y ) 
{
   switch (k)
   {
   case 'a':
      // !!!TODO!!!: add handler for when UP is released
      break;

   case 'z':
      // !!!TODO!!!: add handler for when DOWN is released
      break;
   case '=':
   case '+':
      app.zoomin = false;
      break;
   case '-':
      app.zoomout = false;
      break;
   case 's':
      saveOut();
      break;
   default:
      // do nothing if no key is pressed
      break;
   }
}

////////////////////////////////
// This is called on a Down Keypress 
// of a "special" key such as the grey arrows.
////////////////////////////////
static void OnSpecialKeyboardDown(int k, int x, int y) 
{
   switch (k)
   {
   case GLUT_KEY_UP:
      app.upbutton = true;
      break;
   case GLUT_KEY_DOWN:
      app.downbutton = true;
      break;
   case GLUT_KEY_LEFT:
      app.leftbutton = true;
      break;
   case GLUT_KEY_RIGHT:
      app.rightbutton = true;
      break;
   default:
      // do nothing if no special key pressed
      break;
   }
}

////////////////////////////////
// This is called on a Up Keypress        
////////////////////////////////
static void OnSpecialKeyboardUp( int k, int x, int y ) 
{
   switch (k)
   {
   case GLUT_KEY_UP:
      app.upbutton = false;
      break;
   case GLUT_KEY_DOWN:
      app.downbutton = false;
      break;
   case GLUT_KEY_LEFT:
      app.leftbutton = false;
      break;
   case GLUT_KEY_RIGHT:
      app.rightbutton = false;
      break;
   default:
      // do nothing if no special key pressed
      break;
   }
}

////////////////////////////////
// This is called when mouse changes position
// x and y are the screen position 
// in your 2D window's coordinate frame
////////////////////////////////
static void OnMousePos( int x, int y ) 
{
   app.mousex = x - app.width/2;
   app.mousey = y - app.height/2;
   /////////////////////////


   OnMouseEvent();
}

////////////////////////////////
// This is called when mouse clicks
////////////////////////////////
static void OnMouseClick( int button, int state, int x, int y )
{
   app.mousex = x - app.width/2;
   app.mousey = y - app.height/2;

   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
   {
      app.pickrequest = true;
   }
   else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
   {
      app.pickrequest = false;
   }
   /////////////////////////


   OnMouseEvent();
}


// Initialize the application
// initialize the state of your app here if needed...
static void OnApplicationInit()
{
   app.init();
}


void main( int argc, char* argv[] )
{
    // Initialize the application
    // initialize the state of your app here if needed...
    OnApplicationInit();
   
    // Set the window's initial size
    ::glutInitWindowSize( 640, 480 );
    ::glutInit( &argc, argv );
  
    // Set to double buffer to reduce flicker
    ::glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );
    
    // Set the window title
    app.mainWin_contextID = ::glutCreateWindow( "GLUT application" );
    
    std::cout<<"\n"<<std::flush;
    std::cout<<"YourApp - by YourName - you@your.address.com\n"<<std::flush;
    std::cout<<"       usage:  some params\n"<<std::flush;
    std::cout<<"               some more params\n"<<std::flush;
    std::cout<<"\n"<<std::flush;
    
   // display callbacks.
   ::glutReshapeFunc( OnReshape );
   ::glutIdleFunc( OnIdle );
   ::glutDisplayFunc( OnRedisplay );

   // tell glut to not call the keyboard callback repeatedly 
   // when holding down a key. (uses edge triggering, like the mouse)
   ::glutIgnoreKeyRepeat( 1 );

   // keyboard callback functions.
   ::glutKeyboardFunc( OnKeyboardDown );
   ::glutKeyboardUpFunc( OnKeyboardUp );
   ::glutSpecialFunc( OnSpecialKeyboardDown );
   ::glutSpecialUpFunc( OnSpecialKeyboardUp );

   // mouse callback functions...
   ::glutMouseFunc( OnMouseClick );
   ::glutMotionFunc( OnMousePos );
   ::glutPassiveMotionFunc( OnMousePos );

    // start the application loop, your callbacks will now be called
    // time for glut to sit and spin.
    ::glutMainLoop();
}


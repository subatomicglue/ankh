#ifndef _AABOX2_H_
#define _AABOX2_H_

#include "Vec2.h"

namespace gba
{
   /**
    * Describes an axially aligned box in 2D space. This is usually used for
    * graphics applications. It is defined by its minimum and maximum points.
    *
    * @param DATA_TYPE     the internal type used for the points
    *
    * @ingroup Types
    */
   template< class DATA_TYPE >
   class AABox2
   {
   public:
      typedef DATA_TYPE DataType;

   public:
      /**
       * Creates a new empty box.
       */
      AABox2()
         : mMin(0,0), mMax(0,0)
      {}

      /**
       * Creates a new box with the given min and max points.
       *
       * @param min     the minimum point on the box
       * @param max     the maximum point on the box
       *
       * @pre  all elements of min are less than max
       * @pre  bot min and max are not zero
       */
      AABox2(const Vec<DATA_TYPE>& min, const Vec<DATA_TYPE>& max)
         : mMin(min), mMax(max)
      {}

      /**
       * Construcst a duplicate of the given box.
       *
       * @param box     the box the make a copy of
       */
      AABox2(const AABox2<DATA_TYPE>& box)
         : mMin(box.mMin), mMax(box.mMax)
      {}

      /**
       * Gets the minimum point of the box.
       *
       * @return  the min point
       */
      const Vec<DATA_TYPE>& getMin() const
      {
         return mMin;
      }

      /**
       * Gets the maximum point of the box.
       *
       * @return  the max point
       */
      const Vec<DATA_TYPE>& getMax() const
      {
         return mMax;
      }

      /**
       * Gets the maximum point of the box.
       *
       * @return  the max point
       */
      Vec<DATA_TYPE>& oMax()
      {
         return mMax;
      }

      /**
       * Gets the minimum point of the box.
       *
       * @return  the min point
       */
      Vec<DATA_TYPE>& oMin()
      {
         return mMin;
      }

      /**
       * Sets the minimum point of the box.
       *
       * @param min     the min point
       */
      void setMin(const Vec<DATA_TYPE>& min)
      {
         mMin = min;
      }

      /**
       * Sets the maximum point of the box.
       *
       * @param max     the max point
       */
      void setMax(const Vec<DATA_TYPE>& max)
      {
         mMax = max;
      }

   public:
      /**
       * The minimum point of the box.
       */
      Vec<DATA_TYPE> mMin;

      /**
       * The maximum point on the box.
       */
      Vec<DATA_TYPE> mMax;
   };

   typedef AABox2<signed int>    AABox2s32;
   typedef AABox2<unsigned int>    AABox2u32;
   typedef AABox2<float>    AABox2f;
   typedef AABox2<double>   AABox2d;

   /// compute the intersection of 2 boxes.  return true if the boxes intersected
   template< class DATA_TYPE >
   inline bool intersection( AABox2<DATA_TYPE>& intersectionResult,
                             const AABox2<DATA_TYPE>& box1,
                             const AABox2<DATA_TYPE>& box2 )
   {
      intersectionResult.mMin[0] = Max( box1.mMin[0], box2.mMin[0] );
      intersectionResult.mMin[1] = Max( box1.mMin[1], box2.mMin[1] );
      intersectionResult.mMax[0] = Min( box1.mMax[0], box2.mMax[0] );
      intersectionResult.mMax[1] = Min( box1.mMax[1], box2.mMax[1] );
      
      return intersectionResult.mMin[0] <= intersectionResult.mMax[0] &&
             intersectionResult.mMin[1] <= intersectionResult.mMax[1];
   }

   template< class DATA_TYPE >
   inline bool intersect( const AABox2<DATA_TYPE>& box1, const AABox2<DATA_TYPE>& box2 )
   {
      // Look for a separating axis on each box for each axis
      if (box1.getMin()[0] >= box2.getMax()[0])  return false;
      if (box1.getMin()[1] >= box2.getMax()[1])  return false;
      if (box2.getMin()[0] >= box1.getMax()[0])  return false;
      if (box2.getMin()[1] >= box1.getMax()[1])  return false;
     
      // No separating axis ... they must intersect
      return true;
   }

   /**
    * Tests if the given AABox2 and point intersect with each other. On an edge IS
    * considered intersection by this algorithm.
    *
    * @param box    the box to test
    * @param point  the point to test
    *
    * @return  true if the items intersect; false otherwise
    */
   template<class DATA_TYPE, class VECTYPE>
   inline bool intersect( const AABox2<DATA_TYPE>& box, const VECTYPE& vec )
   {
      // Look for a separating axis on each box for each axis
      if (box.getMin()[0] > vec[0])  return false;
      if (box.getMin()[1] > vec[1])  return false;

      if (vec[0] > box.getMax()[0])  return false;
      if (vec[1] > box.getMax()[1])  return false;

      // they must intersect
      return true;
   }

   /// returns a number between [0,0] and lerprange
   /// divide lerpval by lerprange to get 0..1 unit space... beware of integers... :)
   template<class DATA_TYPE, class VECTYPE>
   inline bool intersect01( gba::Vec<DATA_TYPE>& lerpval, gba::Vec<DATA_TYPE>& lerprange,
                           const AABox2<DATA_TYPE>& box, const VECTYPE& vec )
   {
      lerprange = box.getMax() - box.getMin();
      lerpval = vec - box.getMin();
      return 0 <= lerpval[0] && lerpval[0] <= lerprange[0] && 0 <= lerpval[1] && lerpval[1] <= lerprange[1];
   }

   /** xform box
    */
   template< class DATA_TYPE >
   inline AABox2<DATA_TYPE> xform( const AABox2<DATA_TYPE>& box, const Vec<DATA_TYPE>& pnt )
   {
      return AABox2<DATA_TYPE>( box.getMin() + pnt, box.getMax() + pnt );
   }
}

#endif

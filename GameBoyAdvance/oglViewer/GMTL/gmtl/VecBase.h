/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date: 2004-08-05 08:17:39 -0500 (Thu, 05 Aug 2004) $
 * Version:       $Revision: 805 $
 * -----------------------------------------------------------------
 *
 *********************************************************** ggt-head end */
/*************************************************************** ggt-cpr beg
*
* GGT: The Generic Graphics Toolkit
* Copyright (C) 2001,2002 Allen Bierbaum
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*
 ************************************************************ ggt-cpr end */
#ifndef _GMTL_VECBASE_H_
#define _GMTL_VECBASE_H_

#include <gmtl/Util/Assert.h>

namespace gmtl
{

/**
 * Base type for vector-like objects including Points and Vectors. It is
 * templated on the component datatype as well as the number of components that
 * make it up.
 *
 * @param DATA_TYPE  the datatype to use for the components
 * @param SIZE       the number of components this VecBase has
 */
template<class DATA_TYPE, unsigned SIZE>
class VecBase
{
public:
   /// The datatype used for the components of this VecBase.
   typedef DATA_TYPE DataType;

   /// The number of components this VecBase has.
   enum { Size = SIZE };

public:
   /**
    * Default constructor.
    * Does nothing, leaves data alone.
    * This is for performance because this constructor is called by derived class constructors
    * Even when they just want to set the data directly
    */
   VecBase() {}

   /**
    * Makes an exact copy of the given VecBase object.
    *
    * @param rVec    the VecBase object to copy
    */
   VecBase(const VecBase<DATA_TYPE, SIZE>& rVec);

   //@{
   /**
    * Creates a new VecBase initialized to the given values.
    */
   VecBase(const DATA_TYPE& val0,const DATA_TYPE& val1);
   VecBase(const DATA_TYPE& val0,const DATA_TYPE& val1,const DATA_TYPE& val2);
   VecBase(const DATA_TYPE& val0,const DATA_TYPE& val1,const DATA_TYPE& val2,const DATA_TYPE& val3);
   //@}

   /**
    * Sets the components in this VecBase using the given array.
    *
    * @param dataPtr    the array containing the values to copy
    * @pre dataPtr has at least SIZE elements
    */
   inline void set(const DATA_TYPE* dataPtr);

   //@{
   /**
    * Sets the components in this VecBase to the given values.
    */
   inline void set(const DATA_TYPE& val0);
   inline void set(const DATA_TYPE& val0,const DATA_TYPE& val1);
   inline void set(const DATA_TYPE& val0,const DATA_TYPE& val1,const DATA_TYPE& val2);
   inline void set(const DATA_TYPE& val0,const DATA_TYPE& val1,const DATA_TYPE& val2,const DATA_TYPE& val3);
   //@}

   //@{
   /**
    * Gets the ith component in this VecBase.
    *
    * @param i    the zero-based index of the component to access.
    * @pre i < SIZE
    *
    * @return  a reference to the ith component
    */
   inline DATA_TYPE& operator [](const unsigned i)
   {
      gmtlASSERT(i < SIZE);
      return mData[i];
   }
   inline const DATA_TYPE&  operator [](const unsigned i) const
   {
      gmtlASSERT(i < SIZE);
      return mData[i];
   }
   //@}

   //@{
   /**
    * Gets the internal array of the components.
    *
    * @return  a pointer to the component array with length SIZE
    */
   DATA_TYPE* getData()
   { return mData; }
   const DATA_TYPE* getData() const
   { return mData; }
   //@}

public:
   /// The array of components.
   DATA_TYPE mData[SIZE];
};

// --- Inline members --- //
template<class DATA_TYPE, unsigned SIZE>
VecBase<DATA_TYPE,SIZE>::VecBase(const VecBase<DATA_TYPE, SIZE>& rVec)
{
   for(unsigned i=0;i<SIZE;++i)
      mData[i] = rVec.mData[i];
}

template<class DATA_TYPE, unsigned SIZE>
VecBase<DATA_TYPE,SIZE>::VecBase(const DATA_TYPE& val0,const DATA_TYPE& val1)
{
   // @todo need compile time assert
   gmtlASSERT( SIZE == 2 && "out of bounds element access in VecBase" );
   mData[0] = val0;
   mData[1] = val1;
}

template<class DATA_TYPE, unsigned SIZE>
VecBase<DATA_TYPE,SIZE>::VecBase(const DATA_TYPE& val0,const DATA_TYPE& val1,const DATA_TYPE& val2)
{
   // @todo need compile time assert
   gmtlASSERT( SIZE == 3 && "out of bounds element access in VecBase" );
   mData[0] = val0;
   mData[1] = val1;
   mData[2] = val2;
}

template<class DATA_TYPE, unsigned SIZE>
VecBase<DATA_TYPE,SIZE>::VecBase(const DATA_TYPE& val0,const DATA_TYPE& val1,const DATA_TYPE& val2,const DATA_TYPE& val3)
{
   // @todo need compile time assert
   gmtlASSERT( SIZE == 4 && "out of bounds element access in VecBase" );
   mData[0] = val0;
   mData[1] = val1;
   mData[2] = val2;
   mData[3] = val3;
}


// Setting
template<class DATA_TYPE, unsigned SIZE>
inline void VecBase<DATA_TYPE,SIZE>::set(const DATA_TYPE* dataPtr)
{
   for(unsigned i=0;i<SIZE;++i)
      mData[i] = dataPtr[i];
}
template<class DATA_TYPE, unsigned SIZE>
inline void VecBase<DATA_TYPE,SIZE>::set(const DATA_TYPE& val0)
{
   gmtlASSERT( SIZE >= 1 && "out of bounds element access in VecBase" );
   mData[0] = val0;
}
template<class DATA_TYPE, unsigned SIZE>
inline void VecBase<DATA_TYPE,SIZE>::set(const DATA_TYPE& val0,const DATA_TYPE& val1)
{
   gmtlASSERT( SIZE >= 2 && "out of bounds element access in VecBase" );
   mData[0] = val0;
   mData[1] = val1;
}
template<class DATA_TYPE, unsigned SIZE>
inline void VecBase<DATA_TYPE,SIZE>::set(const DATA_TYPE& val0,const DATA_TYPE& val1,const DATA_TYPE& val2)
{
   gmtlASSERT( SIZE >= 3 && "out of bounds element access in VecBase" );
   mData[0] = val0;
   mData[1] = val1;
   mData[2] = val2;
}
template<class DATA_TYPE, unsigned SIZE>
inline void VecBase<DATA_TYPE,SIZE>::set(const DATA_TYPE& val0,const DATA_TYPE& val1,const DATA_TYPE& val2,const DATA_TYPE& val3)
{
   gmtlASSERT( SIZE >= 4 && "out of bounds element access in VecBase" );
   mData[0] = val0;
   mData[1] = val1;
   mData[2] = val2;
   mData[3] = val3;
}

};

#endif

#ifndef PILE_O_UTILITIE_CODE_ING
#define PILE_O_UTILITIE_CODE_ING

/// StringSprintf - a convenient in-place string builder
/// typical usage: use wherever it makes debugging easier, in asserts, 
///                in profile lines, to build name strings that exist only in debug...
/// Example:
/// @code
///		mMyName = StringSprintf( "myvar[%d]", i );
/// @endcode
/// caveats:
/// - not threadsafe
/// - do not grab pointer to return value, copy result before calling another StringSprintf...
/// - this is as expensive as sprintf!!! (it calls sprintf), be careful where you use it.
/// - do not use StringSprintf recursively
/// @code
///     // don't do this!!!
///     // this may look contrived, but it's really easy to do if you pass a 
///     // StringSprintf into a function that uses StringSprintf internally.
///	mMyNames = StringSprintf( "my name is %s", StringSprintf( "fred clyde, and %s", mName ) );
/// @endcode
inline const char* StringSprintf( const char* const str, ... )
{
	static const int MAX_BUFSIZE = 256;
	static char buf[MAX_BUFSIZE];
	va_list args;
	va_start( args, str );
	vsprintf( buf, str, args );
	va_end( args );

	// it's too late to recover, but at least send up a flare...
	assert( "StringSprintf: string buffer overflow" && strlen( str ) < MAX_SIZE );
	return buf;
}

/// Fixed capacity array of any type of object.
/// for stack or heap allocation without calls to new/delete...
/// Bounds checking on array access
/// Example:
/// @code
///		FixedSizeArray<int, 6> mSixIntArray;
///		int s = mSixIntArray.capacity;	//< get the size
///		mSixIntArray[c-1] = 2;		//< set last elt to 2.
///		mSixIntArray[c] = 34;		//< Assert!
/// @endcode
template <typename T, unsigned SIZE>
class FixedSizeArray
{
public:
	enum { capacity = SIZE };
	FixedSizeArray() {}
	inline T& operator[]( unsigned int x ) 
	{ 
		assert( 0 <= x && x < capacity && "out of bounds access, fix code or increase array capacity" ); 
		return mData[x]; 
	}
	inline const T& operator[]( unsigned int x ) const 
	{ 
		assert( 0 <= x && x < capacity && "out of bounds access, fix code or increase array capacity" ); 
		return mData[x]; 
	}
protected:
	T mData[SIZE];
};

/// stl::vector-a-like container version of the FixedSizeArray
/// for stack or heap allocation without calls to new/delete...
/// Example:
/// @code
///	FixedSizeVector<int, 20> mVectorOfInts;
///	int c = mVectorOfInts.capacity;	//< get the maximum number of elts vector will hold
///	mVectorOfInts.push_back( 3 ); //< add a '3' onto the list...
///     assert( mVectorOfInts[0] == 3 && 1 == mVectorOfInts.size() ); //< we have one elt now
///     for (int x = 0; x < mVectorOfInts.size(); ++x) { std::cout << mVectorOfInts[x]; }
/// @endcode
/// @see FixedSizeArray
template <typename T, unsigned SIZE>
class FixedSizeVector : public FixedSizeArray<T, SIZE>
{
public:
	enum { capacity = FixedSizeArray<T, SIZE>::capacity };
	typedef unsigned int Iterator;
	inline FixedSizeVector() : FixedSizeArray<T, SIZE>(), mSize( 0 ) {}

	/// array access.
	inline T& operator[]( unsigned int x )
	{
		assert( 0 <= x && x < capacity && "out of bounds access, fix code or increase array capacity" );
		assert( 0 <= x && x < mSize && "out of bounds access on container size()" );
		return FixedSizeArray<T, SIZE>::operator[]( x );
	}

	/// array access.
	inline const T& operator[]( unsigned int x ) const
	{
		assert( 0 <= x && x < capacity && "out of bounds access, fix code or increase array capacity" );
		assert( 0 <= x && x < mSize && "out of bounds access on container size()" );
		return FixedSizeArray<T, SIZE>::operator[]( x );
	}

	/// resize.  doesn't change amount of memory available (remember this vector is fixed capacity), 
	/// just the container size...
	inline void resize( unsigned int s )
	{
		assert( 0 <= s && s < capacity && "resize() size out of bounds, fix code or increase array capacity" ); 
		mSize = s;
	}

	/// get the number of elements in the vector
	inline unsigned int size() const 
	{
		return mSize;
	}
	inline void push_back( const T& item )
	{
		FixedSizeArray<T, SIZE>::mData[mSize] = item;
		++mSize;
		assert( 0 <= mSize && mSize < capacity && "push_back() container full!, check size() before pushing, fix code or increase array capacity" );
	}
	inline void pop_back()
	{
		assert( 0 < mSize && "pop_back() container empty!, check size() before popping..." );
		T temp;
		FixedSizeArray<T, SIZE>::mData[mSize] = temp;
		--mSize;
	}

	/// clear out the container, make size == 0
	inline void clear()
	{
		mSize = 0;
	}

	/// get an iterator to the end
	inline Iterator end() const { return mSize; }

	/// get an iterator to the start
	inline Iterator begin() const { return 0; }

	/// get an iterator to the location of the value
	inline Iterator find( const T& value ) const
	{
		for (int x = 0; x < mSize; ++x)
		{
			if (FixedSizeArray<T, SIZE>::mData[x] == value)
			{
				return x;
			}
		}
		return end(); // invalid, not found...
	}

	/// erase the slot at the iterator position,
	/// fills the hole with the last element.
	inline void erase( Iterator it )
	{
		assert( 0 < mSize && "pop_back() container empty!, check size() before popping..." );
		assert( 0 <= it && it < mSize && "iterator not valid, check is the iterator == end()?" );
		FixedSizeArray<T, SIZE>::mData[it] = FixedSizeArray<T, SIZE>::mData[mSize-1];
		--mSize;
	}

	/// remove all instances of [value]
	inline void remove( const T& value )
	{
		Iterator it = find( value );
		while (it != end())
		{
			erase( it );
			it = find( value );
		}
	}

	// sort the contents of the container according to the sort algorithm
	template <typename SORT_FUNC>
		inline void sort()
	{
		// bubble sort:
		for (unsigned int i = mSize-1; 0 < i; --i)
		{
			bool flag = true;
			for (unsigned int j = 0; j < i; ++j)
			{
				if (SORT_FUNC::compare( FixedSizeArray<T, SIZE>::mData[j], FixedSizeArray<T, SIZE>::mData[j+1] ))
				{
					// swap
					flag = false;
					T temp = FixedSizeArray<T, SIZE>::mData[j];
					FixedSizeArray<T, SIZE>::mData[j] = FixedSizeArray<T, SIZE>::mData[j+1];
					FixedSizeArray<T, SIZE>::mData[j+1] = temp;
				}
			}
			if (flag)
			{
				// inner loop performed no swaps, so list is already sorted.
				return;
			}
		}
	}
protected:
	unsigned int mSize;
};

/// fixed sized string class.
/// for stack or heap allocation without calls to new/delete...
template <unsigned SIZE>
class FixedSizeString
{
public:
    FixedSizeString()
    {
        mEndPos = 0;
        mData[0] = '\0';
    }
    FixedSizeString& operator=( const char* const str )
    {
        size_t strSize = strlen( str );
        assert( "overflow" && (strSize + 1) < SIZE ); // 1 for the '\0' character
        strcpy( mData, str );
        mEndPos = strSize;
        return *this;
    }
    FixedSizeString& operator+=( const char* const str )
    {
        size_t strSize = strlen( str );
        assert( "overflow" && (mEndPos + strSize + 1) < SIZE ); // 1 for the '\0' character
        strcpy( &mData[mEndPos], str );
        mEndPos += strSize;
        return *this;
    }
    const char* c_str() const
    {
        return mData;
    }

private:
    size_t mEndPos;
    char mData[SIZE];
};


#endif
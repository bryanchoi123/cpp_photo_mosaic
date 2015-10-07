/**************************************************
 *                                                 *
 *  EasyBMP Cross-Platform Windows Bitmap Library  *
 *                                                 *
 *  Author: Paul Macklin                           *
 *   email: macklin01@users.sourceforge.net        *
 * support: http://easybmp.sourceforge.net         *
 * Modified by CS 225 Course Staff                 *
 *                                                 *
 *          file: EasyBMP.h                        *
 *    date added: 01-31-2005                       *
 * date modified: 07-22-2006                       *
 * branched from: v1.04                            *
 *                                                 *
 *   License: BSD (revised/modified)               *
 * Copyright: 2005-6 by the EasyBMP Project        *
 *                                                 *
 * description: Main include file                  *
 *                                                 *
 * Modified 9/06 by Bill Kinnersley for CS225      *
 * Changes:                                        *
 *   - improved "const correctness"                *
 *   - implemented BMP::operator=()                *
 *                                                 *
 * Modified 1/11 by Jack Toole for CS225           *
 * Changes:                                        *
 *   - added operator== and != for BMPs and pixels *
 *   - consolidated header files                   *
 *   - changed indentation                         *
 *   - removed PixelToPixelCopy variants           *
 *                                                 *
 * Modified Fall 2011 by Jack Toole for CS225      *
 * Changes:                                        *
 *   - changed method names to camelCase           *
 *   - changed to use single array (faster) so that*
 *     performance in MP5 can be evaluated         *
 *   - added hacky support for imagemagick convert *
 *     to read/write other file types              *
 *                                                 *
 **************************************************/

#ifndef _EASYBMP_H_
#define _EASYBMP_H_

#ifdef _MSC_VER
// MS Visual Studio gives warnings when using
// fopen. But fopen_s is not going to work well
// with most compilers, and fopen_s uses different
// syntax than fopen. (i.e., a macro won't work)
// So, we'lll use this:
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include <iostream> // this could just be in .cpp, but it also lets us rely on iostream being included when we're in *_given.h files for testing
					//   since reading output is hacky and printing to a stringstream is better and safer
#include <cmath>
#include <cstring> // For memcpy on g++ 4.3+
#include <stdint.h> // for uint8_t, et al.

#ifdef __BCPLUSPLUS__
// The Borland compiler must use this because something
// is wrong with their cstdio file.
#include <stdio.h>
#else
#include <cstdio>
#endif

#ifdef __GNUC__
// If g++ specific code is ever required, this is
// where it goes.
#endif

#ifdef __INTEL_COMPILER
// If Intel specific code is ever required, this is
// where it goes.
#endif

#ifndef _DefaultXPelsPerMeter_
#define _DefaultXPelsPerMeter_
#define DefaultXPelsPerMeter 3780
// set to a default of 96 dpi
#endif

#ifndef _DefaultYPelsPerMeter_
#define _DefaultYPelsPerMeter_
#define DefaultYPelsPerMeter 3780
// set to a default of 96 dpi
#endif

//#include "EasyBMP_DataStructures.h"
#ifndef _EasyBMP_Custom_Math_Functions_
#define _EasyBMP_Custom_Math_Functions_
inline double Square( double number )
{ return number*number; }

inline int IntSquare( int number )
{ return number*number; }
#endif

int IntPow( int base, int exponent );

typedef uint8_t   ebmpBYTE;
typedef uint16_t  ebmpWORD;
typedef uint32_t  ebmpDWORD;

inline bool IsBigEndian()
{
	uint16_t word = 0x0001;
	if((*(uint8_t *)& word) != 0x01 )
	{ return true; }
	return false;
}

inline uint16_t FlipWORD( uint16_t in )
{ return ( (in >> 8) | (in << 8) ); }

inline uint32_t FlipDWORD( uint32_t in )
{
	return ( ((in&0xFF000000)>>24) | ((in&0x000000FF)<<24) |
			((in&0x00FF0000)>>8 ) | ((in&0x0000FF00)<<8 )   );
}

// it's easier to use a struct than a class
// because we can read/write all four of the bytes
// at once (as we can count on them being continuous
// in memory

struct RGBApixel {
	uint8_t Blue;
	uint8_t Green;
	uint8_t Red;
	uint8_t Alpha;

	RGBApixel()                                : Blue(255), Green(255), Red(255), Alpha(255) { }
	RGBApixel(uint8_t r, uint8_t g, uint8_t b) : Blue(b),   Green(g),   Red(r),   Alpha(255) { }

	bool operator<(RGBApixel other) const
	{
		if (Red != other.Red) return Red < other.Red;
		if (Green != other.Green) return Green < other.Green;
		return Blue < other.Blue;
	}
};
bool operator==(RGBApixel const & left, RGBApixel const & right);
bool operator!=(RGBApixel const & left, RGBApixel const & right);
std::ostream & operator<<(std::ostream & out, RGBApixel const & pixel);

class BMFH{
	public:
	uint16_t  bfType;
	uint32_t bfSize;
	uint16_t  bfReserved1;
	uint16_t  bfReserved2;
	uint32_t bfOffBits;

	BMFH();
	void display( void );
	void SwitchEndianess( void );
};

class BMIH{
	public:
	uint32_t biSize;
	uint32_t biWidth;
	uint32_t biHeight;
	uint16_t  biPlanes;
	uint16_t  biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	uint32_t biXPelsPerMeter;
	uint32_t biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;

	BMIH();
	void display( void );
	void SwitchEndianess( void );
};


bool SafeFread(char* buffer, int size, int number, FILE* fp);
bool EasyBMPcheckDataSize(void);

class BMP
{
	private:

	int BitDepth;
	int Width;
	int Height;
	int XPelsPerMeter;
	int YPelsPerMeter;

	RGBApixel* myPixels;
	int32_t capacity;
	RGBApixel & Pixels(int x, int y);
	const RGBApixel & Pixels(int x, int y) const;

	RGBApixel* Colors;

	uint8_t* MetaData1;
	int SizeOfMetaData1;
	uint8_t* MetaData2;
	int SizeOfMetaData2;

	bool Read32bitRow(uint8_t* Buffer, int BufferSize, int Row);
	bool Read24bitRow(uint8_t* Buffer, int BufferSize, int Row);
	bool Read8bitRow(uint8_t* Buffer, int BufferSize, int Row);
	bool Read4bitRow(uint8_t* Buffer, int BufferSize, int Row);
	bool Read1bitRow(uint8_t* Buffer, int BufferSize, int Row);

	bool Write32bitRow(uint8_t* Buffer, int BufferSize, int Row) const;
	bool Write24bitRow(uint8_t* Buffer, int BufferSize, int Row) const;
	bool Write8bitRow(uint8_t* Buffer, int BufferSize, int Row) const;
	bool Write4bitRow(uint8_t* Buffer, int BufferSize, int Row) const;
	bool Write1bitRow(uint8_t* Buffer, int BufferSize, int Row) const;

	uint8_t FindClosestColor(RGBApixel const & input) const;

	void initializeDefaults();
	bool readFromBMPFile(const char * FileName);
	bool writeToBMPFile(const char* FileName) const;
	static bool isSafeFileChar(int c);
	static const char * getUserName();
	static std::string sanitize(const std::string & input);

	// Previously public functions:
	// (made private by Jack Toole because they are not used in cs225)
	int tellNumberOfColors(void) const;
	void setDPI(int HorizontalDPI, int VerticalDPI);
	int tellVerticalDPI(void) const;
	int tellHorizontalDPI(void) const;
	int tellBitDepth(void) const;

	bool createStandardColorTable(void);

	bool setBitDepth(int NewDepth);

	RGBApixel getColor(int ColorNumber) const;
	bool setColor(int ColorNumber, RGBApixel const & NewColor);

	bool CreateGrayscaleColorTable();

	public:

	BMP();
	BMP(int startWidth, int startHeight);
	BMP(const char * FileName);
	BMP(const std::string & FileName);
	BMP(const BMP & other);
	~BMP();
	BMP const & operator=(BMP const & rhs);

	RGBApixel* operator()(int i, int j);
	const RGBApixel* operator()(int i, int j) const;

	bool operator==(BMP const & rhs) const;
	bool operator!=(BMP const & rhs) const;

	bool setSize(int NewWidth, int NewHeight);
	int tellWidth() const;
	int tellHeight() const;

	bool writeToFile(std::string FileName) const;
	bool writeToFile(const char * FileName) const;
	bool readFromFile(std::string fileName);
	bool readFromFile(const char * fileName);
};

inline RGBApixel & BMP::Pixels(int x, int y)
{
	return myPixels[y*Width + x];
}

inline const RGBApixel & BMP::Pixels(int x, int y) const
{
	return myPixels[y*Width + x];
}

BMFH GetBMFH( const char* szFileNameIn );
BMIH GetBMIH( const char* szFileNameIn );
void DisplayBitmapInfo( const char* szFileNameIn );
int GetBitmapColorDepth( const char* szFileNameIn );
void PixelToPixelCopy( BMP& From, int FromX, int FromY,
					   BMP& To, int ToX, int ToY);
void PixelToPixelCopyTransparent( BMP& From, int FromX, int FromY,
								  BMP& To, int ToX, int ToY,
								  RGBApixel& Transparent );
void RangedPixelToPixelCopy( BMP& From, int FromL , int FromR, int FromB, int FromT,
							 BMP& To, int ToX, int ToY );
void RangedPixelToPixelCopyTransparent(
	 BMP& From, int FromL , int FromR, int FromB, int FromT,
	 BMP& To, int ToX, int ToY ,
	 RGBApixel& Transparent );

void SetEasyBMPwarningsOff( void );
void SetEasyBMPwarningsOn( void );
bool GetEasyBMPwarningState( void );

#endif

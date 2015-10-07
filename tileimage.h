#ifndef TILEIMAGE_H
#define TILEIMAGE_H

#include "EasyBMP.h"

class TileImage
{
	private:
	BMP image;
	RGBApixel averageColor;

	public:
	TileImage();
	explicit TileImage(const BMP & theImage);
	RGBApixel getAverageColor() const { return averageColor; }
	int getResolution() const { return image.tellWidth(); }
	void paste(BMP & canvas, int startX, int startY, int resolution) const;
	
	private:
	static BMP cropSourceImage(const BMP & source);
	RGBApixel calculateAverageColor() const;
	RGBApixel getScaledPixelDouble(double startX, double endX, double startY, double endY) const;
	RGBApixel getScaledPixelInt(int startX, int endX, int startY, int endY) const;
	static uint64_t divide(uint64_t a, uint64_t b) { return (a + b/2) / b; }
	static int divide(int a, int b) { return divide(static_cast<uint64_t>(a), static_cast<uint64_t>(b)); }
	static int fdivide(double a, double b) { return a / b + 0.5; }
	static double frac(double x) { return x - floor(x); }
};

#endif // TILEIMAGE_H

#include <algorithm>
#include "tileimage.h"

using namespace std;

TileImage::TileImage()
	: image(1, 1)
{
	averageColor = *image(0, 0);
}

TileImage::TileImage(const BMP & source)
	: image(cropSourceImage(source))
{
	averageColor = calculateAverageColor();
}

BMP TileImage::cropSourceImage(const BMP & source)
{
	int height = source.tellHeight();
	int width  = source.tellWidth();
	int resolution = min(width, height);
	
	int startX = 0;
	int startY = 0;

	if (width != height)
	{
		if (height > width)
			startY = (height - width) / 2;
		else
			startX = (width - width) / 2;
	}

	BMP cropped(resolution, resolution);

	for (int y = 0; y < resolution; y++)
		for (int x = 0; x < resolution; x++)
			*cropped(x, y) = *source(startX + x, startY + y);

	return cropped;
}

RGBApixel TileImage::calculateAverageColor() const
{
	uint64_t r = 0;
	uint64_t g = 0;
	uint64_t b = 0;

	for (int y = 0; y < image.tellHeight(); y++)
	{
		for (int x = 0; x < image.tellWidth(); x++)
		{
			r += image(x, y)->Red;
			g += image(x, y)->Green;
			b += image(x, y)->Blue;
		}
	}

	RGBApixel color;
	uint64_t numPixels = image.tellWidth() * image.tellHeight();
	color.Red   = divide(r, numPixels);
	color.Green = divide(g, numPixels);
	color.Blue  = divide(b, numPixels);
	return color;
}

void TileImage::paste(BMP & canvas, int startX, int startY, int resolution) const
{
	// If possible, avoid floating point comparisons. This helps ensure that students'
	// photomosaic's are diff-able with solutions
	if (getResolution() % resolution == 0)
	{
		int scalingRatio = getResolution() / resolution;
		
		for (int x = 0; x < resolution; x++)
		{
			for (int y = 0; y < resolution; y++)
			{
				int pixelStartX = (x)   * scalingRatio;
				int pixelEndX   = (x+1) * scalingRatio;
				int pixelStartY = (y)   * scalingRatio;
				int pixelEndY   = (y+1) * scalingRatio;
				
				*canvas(startX + x, startY + y) = getScaledPixelInt(pixelStartX, pixelEndX, pixelStartY, pixelEndY);
			}
		}
	}
	else // scaling is necessary
	{
		double scalingRatio = static_cast<double>(getResolution()) / resolution;

		for (int x = 0; x < resolution; x++)
		{
			for (int y = 0; y < resolution; y++)
			{
				double pixelStartX = (double)(x)   * scalingRatio;
				double pixelEndX   = (double)(x+1) * scalingRatio;
				double pixelStartY = (double)(y)   * scalingRatio;
				double pixelEndY   = (double)(y+1) * scalingRatio;
				
				*canvas(startX + x, startY + y) = getScaledPixelDouble(pixelStartX, pixelEndX, pixelStartY, pixelEndY);
			}
		}
	}
}

RGBApixel TileImage::getScaledPixelDouble(double startX, double endX, double startY, double endY) const
{
	double leftFrac   = 1.0 - frac(startX);
	double rightFrac  = frac(endX);
	double topFrac    = 1.0 - frac(startX);
	double bottomFrac = frac(endX);

	int startXint = static_cast<int>(startX);
	int endXint   = static_cast<int>(ceil(endX));
	int startYint = static_cast<int>(startY);
	int endYint   = static_cast<int>(ceil(endY));

	double r = 0.0;
	double g = 0.0;
	double b = 0.0;
	double totalPixels = 0.0;

	for (int x = startXint; x < endXint; x++)
	{
		for (int y = startYint; y < endYint; y++)
		{
			double weight = 1.0;
			if (x == startXint) weight *= leftFrac;
			if (x == endXint)   weight *= rightFrac;
			if (y == startYint) weight *= topFrac;
			if (y == endYint)   weight *= bottomFrac;

			r += image(x, y)->Red   * weight;
			g += image(x, y)->Green * weight;
			b += image(x, y)->Blue  * weight;
			totalPixels += weight;
		}
	}

	RGBApixel avg;
	avg.Red   = fdivide(r, totalPixels);
	avg.Green = fdivide(g, totalPixels);
	avg.Blue  = fdivide(b, totalPixels);
	return avg;
}

RGBApixel TileImage::getScaledPixelInt(int startXint, int endXint, int startYint, int endYint) const
{
	uint64_t r = 0;
	uint64_t g = 0;
	uint64_t b = 0;
	uint64_t totalPixels = 0;

	for (int x = startXint; x < endXint; x++)
	{
		for (int y = startYint; y < endYint; y++)
		{
			r += image(x, y)->Red;
			g += image(x, y)->Green;
			b += image(x, y)->Blue;
			totalPixels++;
		}
	}

	RGBApixel avg;
	avg.Red   = divide(r, totalPixels);
	avg.Green = divide(g, totalPixels);
	avg.Blue  = divide(b, totalPixels);
	return avg;
}

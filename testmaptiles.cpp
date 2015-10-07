#include "maptiles.h"

/**
 * Test mapTiles function on a list of several valid tiles
 */
int main()
{
	BMP sourceBMP("testsource.bmp");
	SourceImage source(sourceBMP, 8);

	vector<TileImage> tileList;
	BMP a(1,1);
	BMP b(1,1);
	BMP c(1,1);
	*a(0,0) = RGBApixel(255, 0, 0);
	*b(0,0) = RGBApixel(0, 255, 0);
	*c(0,0) = RGBApixel(0, 0, 255);
	tileList.push_back(TileImage(a));
	tileList.push_back(TileImage(b));
	tileList.push_back(TileImage(c));

	MosaicCanvas* canvas = mapTiles(source, tileList);
	if (canvas == NULL)
	{
		cerr << "ERROR: Returned NULL MosaicCanvas when shouldn't have" << endl;
		exit(1);
	}

	BMP actualImage = canvas->drawMosaic(10);

	actualImage.writeToFile("testmaptiles.bmp");

	delete canvas;
}


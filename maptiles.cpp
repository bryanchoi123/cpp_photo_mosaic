/**
 * maptiles.cpp
 *	 			
 * mapTiles() function
 *	 			
 * Author: Jack Toole
 *	 			
 * Developed for CS225 PhotoMosaic MP
 * Fall 2011
 */
	 			
#include "maptiles.h"
#include <iostream>
#include <map>

using namespace std;

/**
 * Map the image tiles into a mosaic canvas which closely
 * matches the input image.
 *	 			
 * @param theSource The input image to construct a photomosaic of
 * @param theTiles The tiles image to use in the mosaic
 */
MosaicCanvas * mapTiles(SourceImage const & theSource, vector<TileImage> const & theTiles)
{
	int row = theSource.getRows();
	int col = theSource.getColumns();
	int size = (int)theTiles.size();

	if(row == 0 || col == 0 || size == 0)
		return NULL;
	
	MosaicCanvas * ret = new MosaicCanvas(row, col);
	vector< Point<3> > averages;
	map<Point<3>, TileImage> avgMap;

	//create vector and map of averages
	for(int i = 0; i<size; i++)
	{
		TileImage tile = theTiles[i];
		RGBApixel avg = tile.getAverageColor();
		Point<3> temp(avg.Red, avg.Green, avg.Blue);
		averages.push_back(temp);

		avgMap.insert(pair< Point<3>, TileImage>(temp, tile));
	}
	KDTree<3> tree(averages);

	//find appropriate average from KDTree
	//and put in MosaicCanvas
	for(int r = 0; r<row; r++)
		for(int c = 0; c<col; c++)
		{
			RGBApixel target = theSource.getRegionColor(r, c);
			Point<3> avg(target.Red, target.Green, target.Blue);
			avg = tree.findNearestNeighbor(avg);

			ret->setTile(r, c, avgMap.find(avg)->second);
		}
	
	return ret; // Replace with your own code
}

// You are encouraged to add your own helper functions here


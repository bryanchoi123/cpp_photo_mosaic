/* kdtree.cpp
 * Implementation of KDTree class
 *	 			
 * Written by Cinda Heeren for CS225 MP6
 * Spring 2009
 * Revision history:
 * 3/31/2009        Created
 * Spring 2011      Modified by Jack Toole
*/

#include <time.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

template<int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim> & a, const Point<Dim> & b, int d) const
{
	if(a[d] > b[d])
		return false;
	if(a[d] == b[d])
		return a<b;
	return true;
}


template<int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim> & target, const Point<Dim> & currentBest, const Point<Dim> & potential) const
{
	int currSum = dist(target, currentBest);
	int potSum = dist(target, potential);

	if(potSum < currSum)
		return true;
	if(potSum == currSum)
		return potential < currentBest;
	return false;
}

template<int Dim>
int KDTree<Dim>::dist(const Point<Dim> & target, const Point<Dim> & point) const
{
	int sum = 0;
	for(int i = 0; i<Dim; i++)
	{
		int distance = target[i] - point[i];
		distance *= distance;
		sum += distance;
	}
	return sum;
}

template<int Dim>
KDTree<Dim>::KDTree(const vector< Point<Dim> > & newPoints)
{
	if(newPoints.empty())
		return;

	points = newPoints;
	int size = (int)newPoints.size();
	for(int i = 0; i<size; i++)
		point_index.push_back(i);
	build(0, size-1, 0);
}

template<int Dim>
void KDTree<Dim>::build(int left, int right, int dim)
{
	if(left < right)
	{
		dim = dim % Dim;
		int mid = (left+right)/2;
		select(left, right, mid, dim);
		build(left, mid-1, dim+1);
		build(mid+1, right, dim+1);
	}
}

template<int Dim>
void KDTree<Dim>::select(int left, int right, int k, int dim)
{
	while(left < right)
	{
		srand(time(NULL));
		int pivot = left + (rand() % (right-left+1));
		pivot = partition(left, right, pivot, dim);

		//printIndexes();
		
		if(pivot == k)
			return;
		if(k < pivot)
			right = pivot-1;
		else
			left = pivot+1;
	}
}

template<int Dim>
int KDTree<Dim>::partition(int left, int right, int pivot, int dim)
{
	Point<Dim> value = points[point_index[pivot]];
	swap(pivot, right);
	
	for(int i = left; i<right; i++)
		if(smallerDimVal(points[point_index[i]], value, dim))
		{
			if(left != i)
				swap(left, i);
			left++;
		}
	swap(left, right);
	return left;
}

template<int Dim>
void KDTree<Dim>::swap(int point, int other)
{
	int t = point_index[point];
	point_index[point] = point_index[other];
	point_index[other] = t;
}

template<int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim> & a) const
{
	return nearestNeighbor(a, 0, point_index.size()-1, 0);
}

template<int Dim>
Point<Dim> KDTree<Dim>::nearestNeighbor(const Point<Dim> & target, 
		int left, int right, int dim) const
{
	if(left >= right)
		return points[point_index[left]];

	dim %= Dim;

	int mid = (left+right)/2;
	Point<Dim> currBest;
	Point<Dim> midPoint = points[point_index[mid]];
	bool wentLeft = false;
	
	if(smallerDimVal(target, midPoint, dim))
		wentLeft = true;
	
	if(wentLeft)
		currBest = nearestNeighbor(target, left, mid-1, dim+1);
	else
		currBest = nearestNeighbor(target, mid+1, right, dim+1);

	if(shouldReplace(target, currBest, midPoint))
		currBest = midPoint;

	int radius = target[dim] - midPoint[dim];
	radius *= radius;
	int distance = dist(target, currBest);
	
	if(radius <= distance)
	{
		Point<Dim> other;
	
		if(wentLeft)
			other = nearestNeighbor(target, mid+1, right, dim+1);
		else
			other = nearestNeighbor(target, left, mid-1, dim+1);

		if(shouldReplace(target, currBest, other))
			currBest = other;
	}

	return currBest;
}

// define KDTree helper functions here
template <int Dim>
void KDTree<Dim>::printIndexes()
{
	cout << point_index[0];
	for(int i = 1; i < (int)point_index.size(); i++)
		cout << ", " << point_index[i];
	cout << endl;
}

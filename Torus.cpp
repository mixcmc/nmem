#include "Torus.h"


Torus::Torus(int size, int dimensions, int *dimvals):sz(size),dims(dimensions)
{
	torus_coords.reserve(sz);
	d = new int[dims]();
	for(int i=0;i<dims;i++)
		d[i] = dimvals[i];	
}


Torus::~Torus(void)
{
	delete[] d;
}
static int delta_dim(int d1, int d2, int dimsz)
{
	return d1>d2 ? std::min(dimsz - d1 + d2, d1 - d2) : std::min(dimsz - d2 + d1, d2 - d1);
}
int Torus::GetNumHops(int l1, int l2) const
{
	int dist = 0;
	if(l1 == l2)
		return dist;
	int *coords1 = new int[dims]();
	int *coords2 = new int[dims]();
	int product = 1;
	for(int i=0;i<dims;i++)
	{
		coords1[i] = (l1 % (product * d[i])) / product; 
		coords2[i] = (l2 % (product * d[i])) / product;
		product *= d[i];
		dist += delta_dim(coords1[i], coords2[i], d[i]);
	}
	return dist;
}

int Torus::GetSize(void) const
{
	return sz;
}
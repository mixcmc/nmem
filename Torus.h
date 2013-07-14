#pragma once
#include "interface.h"
class Torus :
	public Topology
{
public:
	Torus(int size, int dimensions, int *dimvals);
	~Torus(void);
	int GetSize(void) const;
	int GetNumHops(int log1, int log2) const;
private:
	int sz;
	int dims;
	int *d;
	std::vector<int*> torus_coords;
};


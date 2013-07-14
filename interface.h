#pragma once

#include <vector>

struct Latency
{
	int ram_read;
	int ram_write;
	int nic_read;
	int nic_write;
	int swap;
	int hop;
};

struct BW
{
	int ram_read;
	int ram_write;
	int nic_read;
	int nic_write;
};

class Topology
{
public:
	virtual int  GetSize() const=0;
	virtual int  GetNumHops(int log1, int log2) const =0;
	virtual ~Topology()=0;
private:
	int size;
};
inline Topology::~Topology(){};

enum OP_TYPE {READ=1, WRITE=2, SWAP=4};
class Node
{
public:
	Node(int logical, Latency &lat, BW &bw, int num_cores = 1, long long ram_size = 1024LL*1024*1024*16);
	void SetLatency(Latency &new_lat);
	void SetBW(BW &new_BW);
	/*@param size in machine words
	  @retval time to finish operation
	  time consist of:
	  1. cpu <-> nic or cpu <-> ram latency
	  2. latency of one hop * number of hops*/
	long long MemOp(int type, int logical, long long size = 1);
	void Malloc(long long size, int logical);
	void SetDistances(const Topology &top);
private:
	int       _logical;
	Latency   _lat;
	BW        _bw;
	int       _num_cores;
	long long _ram_sz;
	std::vector<int> distances;
	std::vector<int> memory_map;
};

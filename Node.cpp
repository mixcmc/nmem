#include "interface.h"
#include <iostream>

Node::Node(int logical, Latency &lat, BW &bw, int num_cores, long long ram_size)
	:_logical(logical), _lat(lat), _bw(bw), _num_cores(num_cores), _ram_sz(ram_size)
{}
static const long long page_sz = 4096;
long long Node::MemOp(int type, int logical, long long size)
{
	long long latency = distances[logical]*_lat.hop, bw = 0;
	if(type & READ)//read
	{
		//read hops latency doubles: time to send request + time to get response)
		latency *= 2;
		latency += logical != _logical ? _lat.nic_read : _lat.ram_read;
		bw      = logical != _logical ? _bw.nic_read  : _bw.ram_read;
	}
	else//write
	{
		latency += logical != _logical ? _lat.nic_write : _lat.ram_write;
		bw      = logical != _logical ? _bw.nic_write  : _bw.ram_write;
	}
	if(type & SWAP)
		latency += _lat.swap * ( (size + page_sz - 1) / page_sz);//contiguos access
	return latency + size/bw;
}

void Node::SetLatency(Latency &new_lat)
{
	this->_lat = new_lat;
}

void Node::SetBW(BW &new_bw)
{
	this->_bw = new_bw;
}

void Node::SetDistances(const Topology &top)
{
	distances.reserve(top.GetSize());
	for(int i = 0; i < top.GetSize(); i++)
		distances[i] = top.GetNumHops(_logical, i);

}
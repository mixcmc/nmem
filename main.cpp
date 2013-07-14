#include <iostream>
#include <list>

using namespace std;

#include "interface.h"
#include "Torus.h"

/*
HDD latencys:
rpm   ms
7200  4.17
10k   3
15k   2
DDR3 CAS latency = (CAS/Freq_in_Mhz)*1000 ns
dd3-1600 9-9-9-24 => (9/800)*1000 = 11.25 ns
SSD latency ~ 0.1 - 1 ms*/
#define N 1000000

void TestCase(Latency &lat, BW &bw, Topology &top, int percent,int local_node, int remote_node, long long size)
{
	int type = READ;
	unsigned long long sum = 0;
	Node n(local_node, lat, bw);
	n.SetDistances(top);
	for(unsigned long long i = 0; i < N; i++)
	{
		if(!((i+1)%percent))
			type |= SWAP;
		sum += n.MemOp(type, local_node, size);
		type = READ;
	}
	cout << sum <<" local "<< size <<" words reads with "<<percent<<"% page faults\n";
	sum = 0;
	for(unsigned long long i = 0; i < N; i++)
		sum += n.MemOp(READ, remote_node, size);
	cout << sum <<" remote "<< size <<" words reads with "<< top.GetNumHops(local_node, remote_node)<<" hops\n";
}

int main()
{
	int dims[] = {2,2};
	Topology *torus = new Torus(4,2,dims);
	Latency lat = {3, 2, 30, 20, 1000, 100};
	Latency hdd_ddr3 = {18, 11, 200, 130, 4170000 , 100};
	Latency ssd_ddr3 = {18, 11, 200, 130, 100000, 100};//super fast SSD
	BW      bw  = {10000, 15000, 10000, 15000};
	
	cout <<"ddr3-1600 9-9-9-24 with HDD 7200 rpm and 10% page faults VS remote mem with 2 hops"<<endl;
	TestCase(hdd_ddr3, bw, *torus, 10, 1, 2, 10000);
	cout <<"ddr3-1600 9-9-9-24 with superSSD(0.01 ms latency) and 10% page faults VS remote mem with 2 hops"<<endl;
	TestCase(ssd_ddr3, bw, *torus, 10, 1, 2, 10000);
	cout <<"ddr3-1600 9-9-9-24 with superSSD(0.01 ms latency) and 10% page faults VS remote mem with 1 hops"<<endl;
	TestCase(ssd_ddr3, bw, *torus, 10, 1, 0, 10000);
	cout <<"done"<<endl;
	return 0;
}
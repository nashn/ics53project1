//============================================================================
// Name        : IOSystem53.cpp
// Authors     : She Nie
//				 Yuhong Li
//				 Qixiang Zhang
// Version     : Ver 1.2
// Copyright   : Team project for ICS53, allright reserved.
// Description : First Project - Part 1
// History     : 04/19/2014: Finish the 1.0 Version
//               04/22/2014: Upgrade to 1.2 Version
//============================================================================

#ifndef IOSYSTEM53_HPP 
#define IOSYSTEM53_HPP

#include <iostream>
#include <fstream>
#include <string>

class IOSystem53
{

public:
	IOSystem53(int l, int b);

	~IOSystem53();

	void read_block(int i, char* p);

	void write_block(int i, char* p);

	void save();

	void restore();

	// for debug;
	void print_disk();
	
private:
	int MAX_BLOCK_NUM;
	int MAX_BLOCK_SIZE;
	char** ldisk;
	//char ldisk[MAX_BLOCK_NUM][MAX_BLOCK_SIZE];

};

#endif
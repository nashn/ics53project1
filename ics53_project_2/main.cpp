//============================================================================
// Name        : main.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : First Project Lab
//============================================================================

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <pthread.h>

using namespace std;

void simulation(string& filename);

int main(int argc, char* argv[])
{
	string line = "";

	getline(cin, line);


	while( 1 )
	{
		if ( line == "quit" )
			break;
		else
		{
			simulation(line);
			break;
		}
	}


	return 0;
}

void simulation(string& filename)
{
	ifstream f(filename.c_str());
	string line = "";

	int i = 0;

	if ( f.is_open() )
	{
		while ( getline (f,line) )
		{
			cout << "The " << i << " line contains: " << line << endl;
			i++;
		}
		f.close();
	}
	else cout << "Unable to open file" << endl;


}
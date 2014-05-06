//============================================================================
// Name        : IOSystem53.cpp
// Authors     : She Nie
//				 Yuhong Li
//				 Qixiang Zhang
// Version     : Ver 1.2
// Copyright   : Team project for ICS53, all rights reserved.
// Description : First Project - Part 1
// History     : 04/19/2014: Finish the 1.0 Version
//               04/22/2014: Upgrade to 1.2 Version
//============================================================================

#include "IOSystem53.hpp"

using namespace std;

IOSystem53::IOSystem53(int l, int b)
{

	cout << "Initializing IOSystem53..." << endl;
	MAX_BLOCK_NUM = l;
	MAX_BLOCK_SIZE = b;
	// Dynamic allocate version
	ldisk = new char*[l];
	for (int i = 0; i < b; i++) {
		ldisk[i] = new char[b];
		for (int j = 0; j < b; j++) {
			ldisk[i][j] = '0';
		}
	}

	/* static version
	for (int i = 0; i < l; i++) {
		for (int j = 0; j < b; j++) {
			ldisk[i][j] = '0';
			//cout << ldisk[i][j] << " ";
		}
		//cout << endl;
	}
	*/

}

IOSystem53::~IOSystem53()
{
	for (int i = 0; i < MAX_BLOCK_NUM; i++) {
			delete ldisk[i];
	}

	delete [] ldisk;
}

void IOSystem53::read_block(int i, char* p)
{
	// condition check for i and p, later
	if ( i < 0 || i >= MAX_BLOCK_NUM ) {
		cout << "Error@read_block: invalid block index " << endl;
		return;
	}

	char* cursor = ldisk[i];
	for (int s = 0 ; s < MAX_BLOCK_SIZE; s++) {
		p[s] = cursor[s];
	}
}

void IOSystem53::write_block(int i, char* p)
{
	int arr_length = (int)(sizeof(p)/sizeof(char));
	
	// condition check for i and p, later
	if ( i < 0 || i >= MAX_BLOCK_NUM ) {
		cout << "Error@write_block(): invalid block index " << endl;
		return;
	}
	if ( arr_length > 3 * MAX_BLOCK_SIZE ) {
		cout << "Error@write_block(): invalid block size " << endl;
		return;
	}

	char* cursor = ldisk[i];
	for (int s = 0 ; s < MAX_BLOCK_SIZE; s++) {
		//if ( p[s] == '-' && p[s+1] == '1' )
		//{ break; }
		cursor[s] = p[s];
	}
}

void IOSystem53::save() 
{
	// require some file operations
	ofstream savefile;
	savefile.open ("IOSystem53disk.txt");
	if (savefile.is_open())
	{
		for (int i = 0; i < MAX_BLOCK_NUM; ++i)
		{
			for (int j = 0; j < MAX_BLOCK_SIZE; ++j)
			{
				savefile << ldisk[i][j] << " ";
			}
			savefile << "\n";
		}
		cout << "Save file successfully!" << endl;
		savefile.close();
	}
	else { 
		cout << "Error@save(): Unable to open file" << endl;
		return;
	}
	
}

void IOSystem53::restore()
{
	// require some file operations
	string line;
	ifstream savefile ("IOSystem53.txt");
	if (savefile.is_open())
	{
		for (int i = 0; i < MAX_BLOCK_NUM; ++i)
		{
			getline (savefile,line); 
			for (int j = 0; j < line.size(); ++j) {
				ldisk[i][j] = line[j];
				//cout << ldisk[i][j];
			}
			// for debug;
			//cout << "\n";				
		}
		cout << "Restore file successfully!" << endl;
		savefile.close();
	}

	else {
		cout << "Error@restore(): Unable to open file" << endl;
		return;
	}
}


// for debug;
void IOSystem53::print_disk() {
	for (int i = 0; i < MAX_BLOCK_NUM; ++i)
	{
		for (int j = 0; j < MAX_BLOCK_SIZE; ++j) {
			cout << ldisk[i][j] << "";
		}
		cout << "\n";				
	}
}


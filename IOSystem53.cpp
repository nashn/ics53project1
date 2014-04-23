//============================================================================
// Name        : IOSystem53.cpp
// Authors     : She Nie
//				 Yuhong Li
//				 Qixiang Zhang
// Version     : Ver 1.0
// Copyright   : Team project for ICS53, allright reserved.
// Description : First Project - Part 1
//============================================================================

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class IOSystem53
{
	static const int MAX_BLOCK_NUM = 64;
	static const int MAX_BLOCK_SIZE = 64;

public:
	IOSystem53(int l, int b)
	{
		for (int i = 0; i < l; i++) {
			for (int j = 0; j < b; j++) {
				ldisk[i][j] = '0';
				//cout << ldisk[i][j] << " ";
			}
			//cout << endl;
		}

	}

	~IOSystem53();

	void read_block(int i, char* p)
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

	void write_block(int i, char* p)
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

	void save() 
	{
		// require some file operations
		ofstream savefile;
		savefile.open ("IOSystem53.txt");
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

	void restore()
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
	void print_disk() {
		for (int i = 0; i < MAX_BLOCK_NUM; ++i)
		{
			for (int j = 0; j < MAX_BLOCK_SIZE; ++j) {
				cout << ldisk[i][j] << "";
			}
			cout << "\n";				
		}
	}

	/* data */
private:
	char ldisk[MAX_BLOCK_NUM][MAX_BLOCK_SIZE];

};
//============================================================================
// Name        : UISystem53.cpp
// Authors     : She Nie
//				 Yuhong Li
//				 Qixiang Zhang
// Version     : Ver 1.0
// Copyright   : Team project for ICS53, all rights reserved.
// Description : First Project - Final
// History     : 04/25/2014: Finish System Framework
//
//============================================================================

#include <iostream>
#include <string>
#include <sstream>
#include "FileSystem53.hpp"

using namespace std;

// just for fun
void fuck();

// system helper functions:
void print_info();
void print_commands();
void parse_input_line(string& line);


// command functions by using File System
void create_command(string name);
void delete_command(string name);
void open_command(string name);
void close_command(int index);
void read_command(int index, int count);
void write_command(int num, char symbol, int count);

void seek_command(int index, int position);
void directory_command();
void initialize_command(int disk_cont);
void save_command(int disk_cont);


int main(int argc, char const *argv[])
{
	// system variables:
	FileSystem53* filesystem = new FileSystem53(64, 64, "test");
	string line = "";

	// command variables
	string command = "";
	string name = "";
	int index = 0;
	int count = 0;
	int num = 0;
	char symbol = '0';
	int position = 0;
	int disk_cont = 0;
	
	cout << "Initializing UISystem53..." << endl;
	print_info();
	print_commands();

	while (1) 
	{
		cout << "Enter a Command > ";		
		cin >> line;
		istringstream tmp(line);
		tmp >> command;

		// terminate the command shell
		if ( command == "quit" || command == "exit" ) 
		{
			cout << "All data is saved...\nSystem close" << endl; 
			break; 
		}
		else if ( command == "fuck" )
		{
			fuck();
		}
		else if ( command == "cr" )
		{
			tmp >> name;
			create_command(name);
		}
		else if ( command == "de" )
		{
			tmp >> name;
			delete_command(name);		
		}
		else if ( command == "op" )
		{
			tmp >> name;
			open_command(name);
		}
		else if ( command == "cl" )
		{
			close_command(index);
		}
		else if ( command == "rd" )
		{
			read_command(index, count);
		}
		else if ( command == "wr" )
		{
			write_command(num, symbol, count);
		}
		else if ( command == "sk" )
		{
			seek_command(index, position);
		}
		else if ( command == "dr" )
		{
			directory_command();
		}
		else if ( command == "in" )
		{
			initialize_command(disk_cont);
		}
		else if ( command == "sv" )
		{
			save_command(disk_cont);
		}
		else {
			cout << "Error@UISystem.mian(): Unsupported command" << endl;
		}
	}

	
	return 0;
}

void print_info()
{
	cout << "==================================================================" << endl; 
	cout << "                    System Project for ICS 53                     " << endl;
	cout << "   By She Nie (Nash), Yuhong Li (Stanley), Qixiang Zhang (Kevin)  " << endl;
	cout << "                   © 2014 All rights reserved                     " << endl;
	cout << "==================================================================" << endl;
}

void print_commands()
{
	cout << "      ------------------------------------------------------" << endl;
	cout << "      |               UI System Command Usage              |" << endl;
	cout << "      ------------------------------------------------------" << endl;
	cout << "      | cr <name> : Create a file with <name>              |" << endl;
	cout << "      | de <name> : Delete a file with <name>              |" << endl;
	cout << "      | op <name> : Open a file with <name>                |" << endl;
	cout << "      | cl <index> : Close file with <index>               |" << endl;
	cout << "      | rd <index> <count> : Read from the <index>         |" << endl;
	cout << "      | wr <index> <count> : Write to the <index>          |" << endl;
	cout << "      | sk <index> <pos> : Seek <pos> from <index>         |" << endl;
	cout << "      | dr : List the directory                            |" << endl;
	cout << "      | in <disk_cont> : Initialize the disk               |" << endl;
	cout << "      | sv <disk_cont> : Save the disk                     |" << endl;
	cout << "      | quit/exit : Exit the UI system                     |" << endl;
	cout << "      ------------------------------------------------------" << endl;
}

/////////////////////////////////////////////////////////////////////////////
// just for fun
void fuck()
{
	cout << "Fucking the system..." << endl;
	system("cat ./IOSystem53.txt");
	system("osascript -e \'say \"your whole system has been hacked he he he ho ho ho fa lah lah lah lah lah lah fa lah full hoo blow it\" using \"Cellos\"\'\n");
	cout << "System is fucked...\nDisk data leaks..." << endl;
}
/////////////////////////////////////////////////////////////////////////////

void create_command(string name)
{
	cout << "Building..." << endl;
}

void delete_command(string name)
{
	cout << "Building..." << endl;
}

void open_command(string name)
{
	cout << "Building..." << endl;	
}

void close_command(int index)
{
	cout << "Building..." << endl;
}

void read_command(int index, int count)
{
	cout << "Building..." << endl;
}

void write_command(int num, char symbol, int count)
{
	cout << "Building..." << endl;
}

void seek_command(int index, int position)
{
	cout << "Building..." << endl;
}

void directory_command()
{
	cout << "Building..." << endl;
}

void initialize_command(int disk_cont)
{
	cout << "Building..." << endl;
}

void save_command(int disk_cont)
{
	cout << "Building..." << endl;
}
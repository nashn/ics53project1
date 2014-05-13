//============================================================================
// Name        : UISystem53.cpp
// Authors     : She Nie
//				 Yuhong Li
//				 Qixiang Zhang
// Version     : Ver 1.78
// Copyright   : Team project for ICS53, all rights reserved.
// Description : First Project - Final
// History     : 04/25/2014: Finish System Framework
//				 05/05/2014: Upgrade to Ver 1.2
//				 05/11/2014: Ver 1.35 finsihed, continue debugging
//				 05/11/2014: Ver 1.55 finsihed, continue debugging
//				 05/12/2014: Ver 1.78 finsihed, all testcases pass, clean up for submission
//
//============================================================================

#include <iostream>
#include <string>
#include <sstream>
#include "FileSystem53.hpp"

using namespace std;

// global parameter
FileSystem53* filesystem;

// just for fun
void hack();

// system helper functions:
void print_info();
void print_commands();
void print_version_info();

void parse_input_line(string& line);


// command functions by using File System
void create_command(string& name);
void delete_command(string& name);
void open_command(string& name);
void close_command(int index);
void read_command(int index, int count);
void write_command(int num, char symbol, int count);

void seek_command(int index, int position);
void directory_command();
void initialize_command();

bool file_exist(string filename);

void save_command();
void restore_command();


int main(int argc, char const *argv[])
{
	// system variables:
	filesystem = new FileSystem53(64, 64, "test");
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

	bool initialized_flag = false;

	// using a parameter array instead
	string parameter[4];

	//cout << "Initializing UISystem53..." << endl;
	print_info();
	print_commands();
	print_version_info();

	cout << "Enter above commands to test > " << endl;	
	while (1) 
	{
		cout << "$ ";		
		getline(cin, line);
		//cout << "line: " << line << endl;
		istringstream tmp(line);

		for (int i = 0; i < 4; i++)
			tmp >> parameter[i];

		command = parameter[0];
		//cout << "Command: " << command << endl;

		// terminate the command shell
		if ( command == "q" || command == "quit" || command == "exit" ) 
		{
			filesystem->save();
			//cout << "All data is saved...\nSystem close" << endl; 
			break; 
		}
		else if ( command == "hack" )
		{
			// just for testing the UI functionality
			//hack();
		}
		else if ( command == "cr" )
		{
			//tmp >> name;
			//cout << "Filename@cr: " << name << endl;
			name = parameter[1];
			create_command(name);
		}
		else if ( command == "de" )
		{
			//tmp >> name;
			name = parameter[1];
			delete_command(name);		
		}
		else if ( command == "op" )
		{
			//tmp >> name;
			name = parameter[1];
			open_command(name);
		}
		else if ( command == "cl" )
		{
			istringstream t(parameter[1]);
			t >> index;
			close_command(index);
		}
		else if ( command == "rd" )
		{
			istringstream t1(parameter[1]);
			t1 >> index;
			istringstream t2(parameter[2]);
			t2 >> count;
			read_command(index, count);
		}
		else if ( command == "wr" )
		{
			istringstream t1(parameter[1]);
			t1 >> num;
			istringstream t2(parameter[2]);
			t2 >> symbol;
			istringstream t3(parameter[3]);
			t3 >> count;
			write_command(num, symbol, count);
		}
		else if ( command == "sk" )
		{
			istringstream t1(parameter[1]);
			t1 >> index;
			istringstream t2(parameter[2]);
			t2 >> position;

			seek_command(index, position);
		}
		else if ( command == "dr" )
		{
			directory_command();
		}
		else if ( command == "in" )
		{
			if ( initialized_flag ) {
				restore_command();
			}
			else {
				initialize_command();
				initialized_flag = true;
			}
		}
		else if ( command == "sv" )
		{
			save_command();
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
	cout << "      | in : Initialize the disk                           |" << endl;
	cout << "      | sv : Save the disk                                 |" << endl;
	cout << "      | quit/exit : Exit the UI system                     |" << endl;
	cout << "      ------------------------------------------------------" << endl;
}

void print_version_info()
{
	cout << "==================================================================" << endl; 
	cout << "                  Ver 1.78 Released on 05/12/2014                  " << endl;
	cout << "==================================================================" << endl;
}

/*/////////////////////////////////////////////////////////////////////////////
// just for testing UI functionality
void hack()
{
	cout << "Hacking the system..." << endl;
	system("cat ./IOSystem53disk.txt");
	cout << "System is hacked...\nDisk data leaks..." << endl;
}
////////////////////////////////////////////////////////////////////////////*/

void create_command(string& name)
{
	//cout << "Filename: " << name << endl;
	int res = filesystem->create(name);
	if ( res == -1 ) {
		//cout << "Error@UISystem53.create_command(): no space in disk" << endl;
		cout << "error" << endl;
	}
	else if ( res == -2 ) {
		//cout << "Error@UISystem53.create_command(): duplicate filename" << endl;
		cout << "error" << endl;
	} else {
		cout << "file " << name << " created" << endl;
		//cout << "Create file successfully!" << endl;
	}
}

void delete_command(string& name)
{
	int res = filesystem->deleteFile(name);
	if ( res == -1 ) {
		//cout << "Error@UISystem53.delete_command(): No such file" << endl;
		cout << "error" << endl;
	}
	else {
		cout << "file " << name << " deleted" << endl;
		//cout << "Delete file successfully!" << endl;
	}
}

void open_command(string& name)
{
	int res = filesystem->open(name);
	if ( res == -1 ) {
		//cout << "Error@UISystem53.open_command(): file not found" << endl;
		cout << "error" << endl;
	}
	else if ( res == -2 ) {
		//cout << "Error@UISystem53.open_command(): no empty entry" << endl;
		cout << "error" << endl;
	}
	else {
		cout << "file " << name << " opened, index = " << res << endl;
		//cout << "Open file successfully!" << endl;
	}
}
// problem herer
void close_command(int index)
{
	//cout << "index = " << index << endl;
	filesystem->close(index);
	cout << "file with index " << index << " closed" << endl;
}

void read_command(int index, int count)
{
	char temp[count];
	int res = filesystem->read(index, temp, count);

	if ( res == -1 ) {
		//cout << "Error@UISystem53.read_command(): can't get proper file descriptor" << endl;
		cout << "error" << endl;
	}
	else if ( res == -2 ) {
		//cout << "Error@UISystem53.read_command(): current position==file size" << endl;
		cout << "error" << endl;
	}
	else {
		//cout << "read data successfully!" << endl;
		cout << res << " bytes read: ";

		for (int i = 0 ; i< count; i++)
		{
			cout << temp[i] << "";
		}
		cout << endl;
	}
}

void write_command(int num, char symbol, int count)
{
	int res = filesystem->write(num, symbol, count);
	if ( res == -1 ) {
		//cout << "Error@UISystem53.write_command(): File hasn't been open" << endl;
		cout << "error" << endl;
	}
	else if ( res == -2 ) {
		//cout << "Error@UISystem53.write_command(): Maximum file size reached (not implemented.)" << endl;
		cout << "error" << endl;
	}
	else {
		//cout << "Write file successfully!" << endl;
		cout << res << " bytes written " << endl;
	}
	//filesystem->print_oft();
}

void seek_command(int index, int position)
{
	int res = filesystem->lseek(index, position);

	if ( res == -1 ) {
		//cout << "Error@UISystem53.seek_command(): No such file" << endl;
		cout << "error" << endl;
	}
	else if ( res == 0 ) {
		//cout << "Seek file successfully!" << endl;
		cout << "current position is " << position << endl;

	}
}

void directory_command()
{
	filesystem->directory();
}

void initialize_command()
{
	filesystem = new FileSystem53(64, 64, "test");
	cout << "disk Initialized" << endl;
}

/*
bool file_exist(string filename)
{
	ofstream f(filename);
    if ( !f ) {
        f.close();
        return false;
    } else {
        f.close();
        return true;
    } 
}
*/

void save_command()
{
	filesystem->save();
	cout << "disk saved" << endl;
}

void restore_command()
{
	filesystem->restore();
	cout << "disk restored" << endl;
}
//============================================================================
// Name        : FileSystem53.cpp
// Authors     : She Nie
//				 Yuhong Li
//				 Qixiang Zhang
// Version     : Ver 1.2
// Copyright   : Team project for ICS53, allright reserved.
// Description : First Project - Part 1
// History     : 
//
//============================================================================

#ifndef FILESYSTEM53_HPP
#define FILESYSTEM53_HPP

#include <iostream>
#include <string>

#include "IOSystem53.hpp"

using namespace std;

class FileSystem53 {

	// io system pointer
	IOSystem53* iosystem;

	// Fields
	int B;  //Block length
	int K;  //Number of blocks for descriptor table



	char** oft;
	char** desc_table;  // Descriptor Table (in memory).
	// This is aka cache. It's contents should be
	// maintained to be same as first K blocks in disk.
	// Descriptor table format:
	// +-------------------------------------+
	// | bitmap | dsc_0 | dsc_1 | .. | dsc_N |
	// +-------------------------------------+
	//   bitmap: Each bit represent a block in a disk. MAX_BLOCK_NO/8 bytes
	//   dsc_0 : Root directory descriptor
	//   dsc_i : i'th descriptor. Each descriptor is FILE_SIZE_FIELD+ARRAY_SIZE bytes long.




	// Filesystem format parameters:
	int FILE_SIZE_FIELD = 1;     // Size of file size field in bytes. Maximum file size allowed in this file system is 192.
	int ARRAY_SIZE = 3;          // The length of array of disk block numbers that hold the file contents.
	int DESCR_SIZE = FILE_SIZE_FIELD+ARRAY_SIZE;
	int MAX_FILE_NO = 14;        // Maximum number of files which can be stored by this file system.
	int MAX_BLOCK_NO = 64;       // Maximum number of blocks which can be supported by this file system.
	int MAX_BLOCK_NO_DIV8 = MAX_BLOCK_NO/8;
	int MAX_FILE_NAME_LEN = 32;  // Maximum size of file name in byte.
	int MAX_OPEN_FILE = 3;       // Maximum number of files to open at the same time.
	int FILEIO_BUFFER_SIZE = 64; // Size of file io bufer
	int _EOF= -1;       // End-of-File

public:
	// Constructor of this File system.
	FileSystem53(int l, int b, string storage);

	// functions for OFT
	void OpenFileTable();
	int find_oft();
	void deallocate_oft(int index);

	// Format file system.
	void format();


	// functions for descriptor
	char* read_descriptor(int no);
	void clear_descriptor(int no);
	void write_descriptor(int no, char* desc);
	int find_empty_descriptor();


	// helper functions for reading and writing
	int find_empty_block();
	// Get one character.
	int fgetc(int index);
	// Put one character.
	int fputc(int c, int index);


	// Check for the end of file.
	bool feof(int index);
	// Search for a file
	int search_dir(int index, string symbolic_file_name);
	// Clear a file entry from directory file
	void delete_dir(int index, int start_pos, int len);


	// functions for file ops
	// File creation function:
	int create(string symbolic_file_name);
	int open_desc(int desc_no);
	int open(string symbolic_file_name);
	int read(int index, char* mem_area, int count);
	int write(int index, char value, int count);


	// helper functions for read/write on file
	int lseek(int index, int pos);
	void close(int index);
	int deleteFile(string fileName);

	// Directory listing function:
	void directory();

	/*------------------------------------------------------------------
	  Disk management functions.
	  These functions are not really a part of file system.
	  They are provided for convenience in this emulated file system.
	  ------------------------------------------------------------------
	 Restores the saved disk image in a file to the array.
	 */
	void restore();

	// Saves the array to a file as a disk image.
	void save();

	// Disk dump, from block 'start' to 'start+size-1'.
	void diskdump(int start, int size);
};

#endif
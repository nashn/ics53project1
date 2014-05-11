//============================================================================
// Name        : FileSystem53.cpp
// Authors     : She Nie
//				 Yuhong Li
//				 Qixiang Zhang
// Version     : Ver 1.0
// Copyright   : Team project for ICS53, all rights reserved.
// Description : First Project - Part 2
// History     : 04/22/2014: Start working
//				 05/05/2014: Ver 1.0 finished, start debugging	
//
//============================================================================

#include "FileSystem53.hpp"


FileSystem53::FileSystem53(int l, int b, string storage)
{
	cout << "Initializing FileSystem53..." << endl;
	// initialize IO
	iosystem = new IOSystem53::IOSystem53(l, b);
	
	// set parameters
	B = b;
	K = 7;

	// initialize desc_table
	desc_table = new char*[MAX_FILE_NO+1];
	//desc_table = new char*[MAX_FILE_NO];
	desc_table[0] = new char[B];
	for (int i = 1; i < MAX_FILE_NO+1; i++)
	{
		desc_table[i] = new char[DESCR_SIZE];
	}

	// initialize OFT
	OpenFileTable();

	// format file system
	format();

	//save to disk
	save();

}

// Open File Table(OFT).
void FileSystem53::OpenFileTable()
{
	oft = new char*[MAX_OPEN_FILE+1];
	
	// each entry will have a length of header_size + max block size
	for (int i = 0; i < MAX_OPEN_FILE; i++)
	{
		oft[i] = new char[OFT_ENTRY_SIZE];
		for (int j = 0; j < OFT_ENTRY_SIZE; j++)
		{
			oft[i][j] = 0;
		}
	}

}

// Allocate open file table
int FileSystem53::find_oft()
{
	// start from 1 to avoid checking root directory file
	for (int i = 1; i < MAX_OPEN_FILE+1; i++)
	{
		if ( oft[i][0] == 0 )
			return i;
	}

	// -1000 is error flag
	return -1;
}

//Deallocate
void FileSystem53::deallocate_oft(int index)
{
	if ( index == 0 )
		cout << "Error@FileSystem53.deallocate_oft(): cannot deallocate directory file" << endl;
	if ( index < 0 || index > MAX_OPEN_FILE )
		cout << "Error@FileSystem53.deallocate_oft(): Invalid oft index" << endl;
	else 
	{
		delete [] oft[index];
		oft[index] = new char[OFT_ENTRY_SIZE];
		for (int i = 0; i < OFT_ENTRY_SIZE; i++)
		{
			oft[index][i] = 0;
		}
	}
}

/* Format file system.
 *   1. Initialize the first K blocks with zeros.
 *   2. Create root directory descriptor for directory file.
 * Parameter(s):
 *   none
 * Return:
 *   none
 */
void FileSystem53::format()
{
	// intialize the bytemap at position 0
    /*   Note that since the first 7 blocks of ldisk are preserved
     *     to store descriptors, names, etc., so that the
     *     desc_table[0][0]~desc_table[0][6] are set to 1.
     */
	for (int i = 0; i < B; i++)
	{
        if ( i < 7 )
            desc_table[0][i] = 1;
        else
            desc_table[0][i] = 0;
	}


	// initialize first 14 blocks with zeros
    //   a.k.a. initializing desc_table[0][1]~desc_table[0][14].
	for (int i = 1; i < MAX_FILE_NO+1; i++)
	{
		for (int j = 0; j < DESCR_SIZE; j++)
		{
			desc_table[i][j] = 0;
		}
	}

	// create direcotry file
	char directory_desc[DESCR_SIZE];
	for (int i = 0; i < DESCR_SIZE; i++)
	{ directory_desc[i] = 0; }

	// write it to position 1
	write_descriptor(1, directory_desc);

	// load directory file to oft[0]
	// oft availiable flag
	oft[0][0] = 1;                 // << may cause problem
	for (int i = 0; i < DESCR_SIZE; ++i)
	{
		oft[0][i+1] = directory_desc[i];
	}
	// Set current position cursor to the start to file buffer
	oft[0][5] = 6;


}


/* Read descriptor
 * Parameter(s):
 *    no: Descriptor number to read
 * Return:
 *    Return char[4] of descriptor
 */
char* FileSystem53::read_descriptor(int no)
{
	// boundary check
	if ( no == 0 )
		cout << "Error@FileSystem53.read_descriptor(): Invalid descriptor index" << endl;

	if ( no < 0 || no > MAX_FILE_NO )
		cout << "Error@FileSystem53.read_descriptor(): Descriptor index out of boundary" << endl;


	// descriptot buffer
	char* res = new char[DESCR_SIZE];

	// load from desc_table to buffer
	for (int i = 0; i < DESCR_SIZE; i++)
	{
		res[i] = desc_table[no][i];
	}

	return res;
}


/* Clear descriptor
 *   1. Clear descriptor entry
 *   2. Clear bytemap
 * Parameter(s):
 *    no: Descriptor number to clear
 * Return:
 *    none
 */
void FileSystem53::clear_descriptor(int no)
{
	if ( no == 0 )
		cout << "Error@FileSystem53.clear_descriptor(): Invalid descriptor index" << endl;
	
	if ( no < 0 || no > MAX_FILE_NO )
		cout << "Error@FileSystem53.clear_descriptor(): Descriptor index out of boundary" << endl;


	// set the desc_table[no] to all 0
	for (int i = 0; i < DESCR_SIZE; ++i)
	{
		desc_table[no][i] = 0;
	}

	// mark the byte map
	desc_table[0][no] = 0;

}


/* Write descriptor
 *   1. Update descriptor entry
 *   2. Mark bytemap
 *   3. Write back to disk
 * Parameter(s):
 *    no: Descriptor number to write
 *    desc: descriptor to write
 * Return:
 *    none
 */
void FileSystem53::write_descriptor(int no, char* desc)
{
	if ( no == 0 )
		cout << "Error@FileSystem53.write_descriptor(): Invalid descriptor index" << endl;

	if ( no < 0 || no > MAX_FILE_NO )
		cout << "Error@FileSystem53.write_descriptor(): Descriptor index out of boundary" << endl;

	for (int i = 0; i < DESCR_SIZE; i++)
	{
		desc_table[no][i] = desc[i];
	}

}


/* Search for an unoccupied descriptor.
 * If ARRAY[0] is zero, this descriptor is not occupied.
 * Then it returns descriptor number.
 */
int FileSystem53::find_empty_descriptor()
{
	// start from 2 to avoid directory and bytemap
	for (int i = 2; i < MAX_FILE_NO+1; i++)
	{
		if ( desc_table[i][0] == 0 )
			return i;
	}

	return -1;
}


/* Search for an unoccupied block.
 *   This returns the first unoccupied block in bytemap field.
 *   Return value -1 means all blocks are occupied.
 * Parameter(s):
 *    none
 * Return:
 *    Returns the block number
 *    -1 if not found
 */
int FileSystem53::find_empty_block()
{
	// start from K to 64 to search for unmarked position
	for (int i = K; i < MAX_BLOCK_NO; i++)
	{
		if ( desc_table[0][i] == 0 )
			return i;
	}

	return -1;
}


/* Get one character.
 *    Returns the character currently pointed by the internal file position
 *    indicator of the specified stream. The internal file position indicator
 *    is then advanced to the next character.
 * Parameter(s):
 *    index (descriptor number of the file to be added.)
 * Return:
 *    On success, the character is returned.
 *    If a writing error occurs, EOF is returned.
 */
int FileSystem53::fgetc(int index)
{
	if ( index < 0 || index > MAX_OPEN_FILE ) {
		cout << "Error@FileSystem53.fgetc(): Index out of boundary" << endl;
		return _EOF;
	}

	// get the current cursor of the file buffer
	int cursor = oft[index][OFT_CURRENT_POSITION_INDEX];
	int length = oft[index][1];
	if ( cursor < 6 || cursor > (length + 5) || cursor >= OFT_ENTRY_SIZE )
		return _EOF;

	// increase the cursor
	oft[index][OFT_CURRENT_POSITION_INDEX] = cursor+1;
	return oft[index][cursor];

}


/* Put one character.
 *    Writes a character to the stream and advances the position indicator.
 *    The character is written at the position indicated by the internal position
 *    indicator of the file, which is then automatically advanced by one.
 * Parameter(s):
 *    c: character to write
 *    index (descriptor number of the file to be added.)
 * Return:
 *    On success, the character written is returned.
 *    If a writing error occurs, -2 is returned.
 */
int FileSystem53::fputc(int c, int index)
{
	// the int c should be the ASCII code of the character c
	if ( index < 0 || index > MAX_OPEN_FILE ) {
		cout << "Error@FileSystem53.fputc(): Index out of boundary" << endl;
		return -2;
	}

	int cursor = oft[index][OFT_CURRENT_POSITION_INDEX];
	int length = oft[index][1];
	if ( cursor < 6 || cursor > (length + 5) || cursor >= OFT_ENTRY_SIZE )
		return -2;

	oft[index][cursor] = c;
	
	// increase the cursor position
	oft[index][OFT_CURRENT_POSITION_INDEX] = cursor+1;

	// increase the file size
	oft[index][1] += 1;

	// update the descriptor size
	int desc_index = oft[index][0];
	desc_table[desc_index][0] += 1;

	return oft[index][cursor];

}


/* Check for the end of file.
 * Parameter(s):
 *    index (descriptor number of the file to be added.)
 * Return:
 *    Return true if end-of-file reached.
 */
bool FileSystem53::feof(int index)
{
	int cursor = oft[index][OFT_CURRENT_POSITION_INDEX];
	for (int i = cursor; i < oft[index][1]; i++) {     // checkhere: logic
		if ( oft[index][i] == _EOF )
			return true;
	}

	return false;
}


/* Search for a file
 * Parameter(s):
 *    index: index of open file table
 *    st: The name of file to search.
 * Return:
 *    index: An integer number position of found file entry.
 *    Return -1 if not found.
 */
int FileSystem53::search_dir(int index, string symbolic_file_name)
{
	if ( index < 0 || index > MAX_OPEN_FILE ) {
		cout << "Error@FileSystem53.search_dir(): Index out of boundary" << endl;
		return -1;
	}

	int cursor = oft[index][OFT_CURRENT_POSITION_INDEX];
	int length = oft[index][1];
	if ( cursor < 6 || cursor > (length + 5) || cursor >= OFT_ENTRY_SIZE )
		return -1;

	int res = -1;

	while ( cursor < (length + 5))
	{
		// filename's length
		cursor+=2;
		int nlength = oft[index][cursor];
		char buffer[nlength];
		cursor++;
		for (int i = 0; i < nlength; i++)
		{
			buffer[i] = oft[index][cursor+i];
		}
		
		if ( symbolic_file_name.compare(buffer) == 0 )
			return cursor;

		cursor += nlength-1;	
	}

	return res;
}


/* Clear a file entry from directory file
 *
 * Parameter(s):
 *    index: open file table index
 *    start_pos:
 *    length:
 * Return:
 *    none
 */
void FileSystem53::delete_dir(int index, int start_pos, int len)
{
	if ( index < 0 || index > MAX_FILE_NO ) 
		cout << "Error@FileSystem53.delete_dir(): Index out of boundary" << endl;
	
	if ( start_pos < 6 || start_pos > (len+5) || start_pos >= OFT_ENTRY_SIZE )
		cout << "Error@FileSystem53.delete_dir(): Position out of boundary" << endl;

	for (int i = start_pos; i < start_pos + len; i++)
	{
		oft[index][i] = 0;
	}
}


/* File creation function:
 *    1. creates empty file with file size zero.
 *    2. makes/allocates descriptor.
 *    3. updates directory file.
 * Parameter(s):
 *    symbolic_file_name: The name of file to create.
 * Return:
 *    Return 0 for successful creation.
 *    Return -1 for error (no space in disk)
 *    Return -2 for error (for duplication)
 */
int FileSystem53::create(string symbolic_file_name)
{
	int desc_index = find_empty_descriptor();
	if ( desc_index == -1 ) {
		cout << "Error@FileSystem53.create(): no empty descriptors left" << endl;
		return -1;
	}

	if ( search_dir(0, symbolic_file_name) != -1 ) {
		cout << "Error@FileSystem53.create(): filename exists" << endl;
		return -2;
	}

	// clean up the exist descriptior in desc_table
	for (int i = 0; i < 4; i++)
	{
		desc_table[desc_index][i] = 0;
	}

	// if directory file uses 3 blocked and no more space for the filename
	//	return -1
	int filename_length = symbolic_file_name.size();
	if ( oft[0][1] + filename_length > 192 || filename_length > 10 ) {
		cout << "Error@FileSystem53.create(): filename oversize" << endl;
		return -1;
	}

	// the following part has problem, need to be work out
	// get new empty block to store the content
	//if ( )

	// write filename to directory file in oft
	int start_pos = oft[0][OFT_CURRENT_POSITION_INDEX];
	for (int i = 0; i < filename_length; i++)
	{
		oft[0][start_pos] = symbolic_file_name[i];
		start_pos++;
	}
	oft[0][OFT_CURRENT_POSITION_INDEX] = start_pos;

	// update descriptor in oft
	oft[0][1] += start_pos;


	////////////////////////////////////
	// problem:
	// 	a block with unfully used space
	// len = 56 or len = 67
	// (len % 64 + filename_length < 64)?
	////////////////////////////////////


	// write descriptior to desc_table
	char desc_buffer[DESCR_SIZE];
	for (int i = 0; i < DESCR_SIZE; i++)
	{
		desc_buffer[i] = oft[0][i+1];
	}

	write_descriptor(1, desc_buffer);

	return 0;
}


/* Open file with descriptor number function:
 * Parameter(s):
 *    desc_no: descriptor number
 * Return:
 *    index: index if open file table if successfully allocated.
 *    Return -1 for error.
 */
int FileSystem53::open_desc(int desc_no)
{
	if ( desc_no < 0 || desc_no > MAX_FILE_NO+1 ) {
		cout << "Error@FileSystem53.read(): Index out of boundary" << endl;
		return -1;
	}

	int oft_index = find_oft();
	if ( oft_index == -1 )
		return -1;

	// set oft position to be used
	oft[oft_index][0] = 1;

	// copy descriptor from desc_table to oft
	for (int i = 0; i < DESCR_SIZE; i++)
	{
		oft[oft_index][i+1] = desc_table[desc_no][i];
	}

	return oft_index;
	
}


/* Open file with file name function:
 * Parameter(s):
 *    symbolic_file_name: The name of file to open.
 * Return:
 *    index: An integer number, which is a index number of open file table.
 *    Return -1 or -2 if it cannot be open.
 */
// TODOs:
		// 1. Open directory file
		// 2. Search for a file with given name
		//    Return -1 if not found.
		// 3. Get descriptor number of the found file
		// 4. Looking for unoccupied entry in open file table.
		//    Return -2 if all entry are occupied.
		// 5. Initialize the entry (descriptor number, current position, etc.)
		// 6. Return entry number
int FileSystem53::open(string symbolic_file_name)
{
	if ( symbolic_file_name.size() > 10 ) {
		cout << "Error@FileSystem53.open(): filename size of boundary" << endl;
		return -1;
	}

	// search the direcotry to find the filename and its corresponding index
	int desc_index = search_dir(0, symbolic_file_name);

	if ( desc_index < 0 || desc_index > MAX_FILE_NO+1 ) {
		cout << "Error@FileSystem53.open(): descriptor index " << desc_index << " out of boundary" << endl;
		return -1;
	}

	int oft_index = open_desc(desc_index);

	char block_buffer[B];
	while ( oft[oft_index][1] > 0 )
	{
		for (int i = 1; i < DESCR_SIZE; ++i)
		{
			int block_index = oft[oft_index][i+1];
			if ( block_index != 0 )
			{
				iosystem->read_block(block_index, block_buffer);
				// write to oft buffer
				for (int i = 0; i < B; ++i)
				{
					int tmp = fputc((int)block_buffer[i], 0); // complicated?
					if ( tmp == -1 )
						break;
				}
			}
		}
	}

	return oft_index;
}


/* File Read function:
 *    This reads a number of bytes from the the file indicated by index.
 *    Reading should start from the point pointed by current position of the file.
 *    Current position should be updated accordingly after read.
 * Parameter(s):
 *    index: File index which indicates the file to be read.
 *    mem_area: buffer to be returned
 *    count: number of byte(s) to read
 * Return:
 *    Actual number of bytes returned in mem_area[].
 *    -1 value for error case "File hasn't been open"
 *    -2 value for error case "End-of-file"
 TODOs:
		 1. Read the open file table using index.
		    1.1 Get the file descriptor number and the current position.
		    1.2 Can't get proper file descriptor, return -1.
		 2. Read the file descriptor
		    2.1 Get file size and block array.
		 3. Read 'count' byte(s) from file and store in mem_area[].
		    3.1 If current position crosses block boundary, call read_block() to read the next block.
		    3.2 If current position==file size, stop reading and return.
		    3.3 If this is called when current position==file size, return -2.
		    3.4 If count > mem_area size, only size of mem_area should be read.
		    3.5 Returns actual number of bytes read from file.
		    3.6 Update current position so that next read() can be done from the first byte haven't-been-read.
*/
int FileSystem53::read(int index, char* mem_area, int count)
{
	if ( index < 0 || index > MAX_FILE_NO+1 ) {
		cout << "Error@FileSystem53.read(): Index out of boundary" << endl;
		return -1;
	}

	if ( oft[index][0] == 0 ) {
		cout << "Error@FileSystem53.read(): File has not been opened" << endl;
		return -1;
	}

	int cursor = oft[index][OFT_CURRENT_POSITION_INDEX];
	int length = oft[index][1];
	if ( cursor < 6 || cursor > (length+5) || cursor >= OFT_ENTRY_SIZE ) {
		cout << "Error@FileSystem53.read(): Cursor out of boundary" << endl;
		return -2;
	}

	int read_count = 0;
	for (; read_count < count; read_count++)
	{
		int tmp = fgetc(index);
		if ( tmp == -1 )
		{
			break;
		}
		mem_area[read_count] = (char)tmp;
	}

	return read_count;
}


/* File Write function:
 *    This writes 'count' number of 'value'(s) to the file indicated by index.
 *    Writing should start from the point pointed by current position of the file.
 *    Current position should be updated accordingly.
 * Parameter(s):
 *    index: File index which indicates the file to be read.
 *    value: a character to be written.
 *    count: Number of repetition.
 * Return:
 *    >0 for successful write
 *    -1 value for error case "File hasn't been open"
 *    -2 for error case "Maximum file size reached" (not implemented.)
 */
int FileSystem53::write(int index, char value, int count)
{
	if ( index < 0 || index > MAX_FILE_NO+1 ) {
		cout << "Error@FileSystem53.write(): Index out of boundary" << endl;
		return -1;
	}

	if ( oft[index][0] == 0 ) {
		cout << "Error@FileSystem53.write(): File has not been opened" << endl;
		return -1;
	}

	// previous design
	int cursor = oft[index][OFT_CURRENT_POSITION_INDEX];
	int length = oft[index][1];
	if ( cursor < 6 || cursor > (length+5) || cursor >= OFT_ENTRY_SIZE ) {
		cout << "Error@FileSystem53.write(): Cursor out of boundary" << endl;
		return -2;
	}
	///////////////////////////////////////////

	int write_count = 0;
	for (; write_count < count; write_count++)
	{
		if ( feof(index) )
		{
			cout << "Error@FileSystem53.write(): EOF of writing buffer" << endl;
			return -1;
		}

		int tmp = fputc((int)value, index); // according to the func description, value can be 
		if ( tmp == -2 )					//  more than one char. --> check > LINE 658 <
		{									// 	(solution guess: check if count > 1, if so, 
											//    using index i.e. value[0], value[1]... )
			cout << "Error@FileSystem53.write(): error in writing buffer" << endl;
			return -1;
		}
	}

	return write_count;
}


/* Setting new read/write position function:
 * Parameter(s):
 *    index: File index which indicates the file to be read.
 *    pos: New position in the file. If pos is bigger than file size, set pos to file size.
 * Return:
 *    0 for successful write
 *    -1 value for error case "File hasn't been open"
 */
int FileSystem53::lseek(int index, int pos)
{
	if ( index < 0 || index > MAX_FILE_NO+1 ) {
		cout << "Error@FileSystem53.lseek(): Index out of boundary" << endl;
		return -1;
	}

	if ( oft[index][0] == 0 ) {
		cout << "Error@FileSystem53.lseek(): File has not been opened" << endl;
		return -1;
	}

	if (pos >= OFT_ENTRY_SIZE) {
		pos = OFT_ENTRY_SIZE-1;
	}
	oft[index][OFT_CURRENT_POSITION_INDEX] = pos;

	return 0;
}


/* Close file function:
 * Parameter(s):
 *    index: The index of open file table
 * Return:
 *    none
 */
void FileSystem53::close(int index)
{
	// find the index and store it to the disk
	if ( index < 0 || index > MAX_FILE_NO+1 ) {
		cout << "Error@FileSystem53.close(): Index out of boundary" << endl;
		return;
	}

	if ( oft[index][0] == 0 ) {
		cout << "Error@FileSystem53.close(): File has not been opened" << endl;
		return;
	}

	char buffer[B];
	for (int i = 2; i <= DESCR_SIZE; ++i)
	{
		for (int j = 0; j < B; j++)
		{
			buffer[j] = fgetc(index);
		}

		iosystem->write_block(oft[index][i], buffer);

		for (int j = 0; j < B; j++)
		{
			buffer[j] = 0;
		}
	}
	

}


/* Delete file function:
 *    Delete a file
 * Parameter(s):
 *    symbolic_file_name: a file name to be deleted.
 * Return:
 *    Return 0 with success
 *    Return -1 with error (ie. No such file).
 */
int FileSystem53::deleteFile(string fileName)
{
	int desc_index = search_dir(0, fileName);

	if ( desc_index == -1 ) {
		cout << "Error@FileSystem53.deleteFile(): No such file" << endl;
		return -1;
	}

	// check oft to see wheter it is opened
	for (int i = 1; i < MAX_OPEN_FILE+1; i++)
	{
		if ( oft[i][0] != 0 && oft[i][0] == desc_index )
		{
			return 0;	
		}
	}

	return -1;
}


/* Directory listing function:
 *    List the name and size of files in the directory. (We have only one directory in this project.)
 *    Example of format:
 *       abc 66 bytes, xyz 22 bytes
 * Parameter(s):
 *    None
 * Return:
 *    None
 */
void FileSystem53::directory()
{
	
	int cursor = oft[0][OFT_CURRENT_POSITION_INDEX];
	int length = oft[0][1];

	if ( cursor > length || cursor < 6 || cursor > OFT_ENTRY_SIZE ) {
		cout << "Error@FileSystem53.directory(): Cursor out of boundary" << endl;
		return;
	}
	

	while ( cursor < length )
	{
		int desc_index = cursor++;
		if ( desc_index != 0 )
		{
			int file_length = desc_table[desc_index][0];
			int filename_length = cursor++;

			char name_buffer[filename_length];
			for (int i = 0; i < filename_length; i++)
			{
				name_buffer[i] = oft[0][cursor+i];
			}
			cursor += filename_length;
			cout << name_buffer << " " << file_length << " bytes" << endl;
		}

		cursor++;
	}

}

/*------------------------------------------------------------------
  Disk management functions.
  These functions are not really a part of file system.
  They are provided for convenience in this emulated file system.
  ------------------------------------------------------------------
 Restores the saved disk image in a file to the array.
 */
void FileSystem53::restore()
{
	iosystem->restore();

	// after restore
	// load all the result from the disk to the buffer
}

// Saves the array to a file as a disk image.
void FileSystem53::save()
{
	// before saving to file
	// write all current buffer to the disk
	/*
	for (int i = 1; i < MAX_OPEN_FILE+1; i++)
	{
		// close() will write all the content to disk
		close(i);
	}
	*/

	// writing all descriptior to dist
	char buffer[B];
	for (int i = 1; i < MAX_FILE_NO+1; i++)
	{
		strcat(buffer, desc_table[i]);
	}
	//cout << buffer << endl;
	iosystem->write_block(1, buffer);

	iosystem->save();
}

// Disk dump, from block 'start' to 'start+size-1'.
void FileSystem53::diskdump(int start, int size)
{
	// initialize a clean block
	char* clean = new char[B];
	for (int i = 0; i < B; i++)
	{ clean[i] = '0'; }

	// overwrite to the disk
	for (int i = start; i < start+size-1; i++)
	{
		iosystem->write_block(i, clean);
	}
}

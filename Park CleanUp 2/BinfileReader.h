#pragma once

#include <fstream>
#include <string>
#include <stdint.h>
#include <algorithm>

using namespace std;

std::wstring StringToWString(const std::string& s);
std::string WStringToString(const std::wstring& s);

struct BFR
{
	enum BFREnum
	{
		BEGIN = 0,
		CUR,
		END
	};
};

class BinfileReader
{
private:	
	bool initialized;
	char *buffer;
	int position;
	int filesize;
	bool corrupted;
public:
	int getsize()
	{
		return filesize;
	}
	bool IsReady()
	{
		return initialized;
	}
	bool IsCorrupted()
	{
		return corrupted;
	}

	void open(const string& filename, std::ios::openmode io_mode);

	bool setByte(char byte, size_t loc)
	{
		//System::Diagnostics::Debug::WriteLine(loc + ">" + filesize);
		if(loc >= filesize)
			return false;
		buffer[loc] = byte;
		return true;
	}

	bool setInt(int data, size_t loc)
	{
		union
		{
			int integer;
			char byte[4];
		} charconversion;
		charconversion.integer = data;

		if(loc + 4 >= filesize)
			return false;
		
		for(int i = 0; i < 4; i++)
			setByte(charconversion.byte[i], loc + i);
		return true;
	}
	
	bool setBytes(char* byte, size_t loc, int size)
	{
		//System::Diagnostics::Debug::WriteLine(loc + ">" + filesize);
		if(loc + size >= filesize)
			return false;
		for(int i = 0; i < size; i++)
			buffer[loc + i] = *(char*)byte[i];
		return true;
	}

	uint32_t getu32();
	uint16_t getu16();

	int32_t get32();
	int16_t get16();

	void read(char *data, int32_t size);
	
	string getstring16();
	string getstring32();
	wstring s_getstring32();

	void seek(int offset, int mode);

	void skip(int size)
	{
		position += size;
	}

	uint32_t getpos()
	{
		return position;
	}

	bool good()
	{
		return position <= filesize;
	}

	void ClearBuffer();

	BinfileReader(void);
	~BinfileReader(void);
};


#include "StdAfx.h"
#include "BinfileReader.h"

using namespace System::Windows::Forms;

std::wstring StringToWString(const std::string& s)
{
	std::wstring temp(s.length(),L' ');
	std::copy(s.begin(), s.end(), temp.begin());
	return temp; 
}


std::string WStringToString(const std::wstring& s)
{
	std::string temp(s.length(), ' ');
	std::copy(s.begin(), s.end(), temp.begin());
	return temp; 
}

uint32_t BinfileReader::getu32()
{
	uint32_t tmp;
	if(position + sizeof(tmp) > filesize)
		MessageBox::Show("Alloc error getu32!", "Alloc error!");
	memcpy(&tmp, &buffer[position], sizeof(tmp));
	position += sizeof(tmp);
	return tmp;
}
uint16_t BinfileReader::getu16()
{
	uint16_t tmp;
	if(position + sizeof(tmp) > filesize)
		MessageBox::Show("Alloc error getu16!", "Alloc error!");
	memcpy(&tmp, &buffer[position], sizeof(tmp));
	position += sizeof(tmp);
	return tmp;
}

int32_t BinfileReader::get32()
{
	int32_t tmp;
	if(position + sizeof(tmp) > filesize)
		MessageBox::Show("Alloc error get32!", "Alloc error!");
	memcpy(&tmp, &buffer[position], sizeof(tmp));
	position += sizeof(tmp);
	return tmp;
}
int16_t BinfileReader::get16()
{
	int16_t tmp;
	if(position + sizeof(tmp) > filesize)
		MessageBox::Show("Alloc error get16!", "Alloc error!");
	memcpy(&tmp, &buffer[position], sizeof(tmp));
	position += sizeof(tmp);
	return tmp;
}

void BinfileReader::read(char *data, int32_t size)
{
	if(position + size > filesize)
		MessageBox::Show("Alloc error read()!", "Alloc error!");
	memcpy(data, &buffer[position], size);
	position += size;
}

string BinfileReader::getstring16()
{
	int16_t length = getu16();
	char *data = new char[length+1];
	data[length] = 0;
	memcpy(data, &buffer[position], length);
	position += length;
	string retstr(data);
	delete[] data;
	return retstr;
}

string BinfileReader::getstring32()
{
	int32_t length = getu32();
	char *data = new char[length+1];
	data[length] = 0;
	memcpy(data, &buffer[position], length);
	position += length;
	string retstr(data);
	delete[] data;
	return retstr;
}

wstring BinfileReader::s_getstring32()
{
	uint32_t length = getu32();
	if(length < 4)
		MessageBox::Show("Stringparser error #1: A string has been found which is of unexpected size (corrupted?)");
	//Skip лллл
	skip(4);
	length -= 4;
	if(length > 0)
	{
		wchar_t *data = new wchar_t [length+1];
		data[length/2] = 0;
		memcpy(data, &buffer[position], length);
		position += length;
		wstring str(data);
		delete[] data;

		//str.erase (str.begin(), str.begin()+2);

		//System::Diagnostics::Debug::WriteLine(gcnew System::String(data));
		return str;
	}
	wstring wstr;
	wstr.empty();
	return wstr;
}

void BinfileReader::seek(int offset, int mode)
{
	if(mode == BFR::BEGIN)
		position = offset;
	else if(mode == BFR::CUR)
		position += offset;
	else if(mode == BFR::END)
		position = filesize + offset;
	else
		throw ("BinfileReader::seek: Mode unknown!");
}

void BinfileReader::open(const string& filename, std::ios::openmode io_mode)
{	
	initialized = false;	
	if(buffer != NULL)
	{
		delete[] buffer;
		buffer = NULL;
	}
	ifstream is;
	is.open (filename.c_str(), ios::binary );

	if(is.good())
	{
		position = 0;
		is.seekg (0, ios::end);
		filesize = is.tellg();
		is.seekg (0, ios::beg);

		buffer = new char [filesize+1];
		is.read (buffer,filesize);

		is.close();
		if(buffer != nullptr)
			initialized = true;
		if(filesize > 16)
		{
			seek(filesize-16, BFR::BEGIN);
			if(getu32() == 0 && getu32() == 0 && getu32() == 0 && getu32() == 0)
				corrupted = true;
			seek(0, BFR::BEGIN);
		} else if(filesize == 0)
		{
			corrupted = true;
		}
	}
}

BinfileReader::BinfileReader(void)
{
	initialized = false;
	buffer = NULL;
	corrupted = false;
}


void BinfileReader::ClearBuffer()
{
	try{
		if(buffer != NULL)
		{
			delete[] buffer;
			buffer = NULL;
		}
	} catch (...){
		//Niks
	}
}

BinfileReader::~BinfileReader(void)
{
}

#pragma once
#include <string>
#include <stdint.h>
#include <vector>
#include <assert.h>
#include <fstream>
#include <map>

#include "BinfileReader.h"

#define R3AREADER_VERSION "1.5"

using namespace System;
using namespace System::Diagnostics;
using namespace std;

typedef std::map<std::string, std::string> strmap;
typedef std::map<std::wstring, std::wstring> wstrmap;


/*-----------------
	Format structure
----------------*/

//The header used since Soaked
struct Arc_Header
{
	union
	{
		uint8_t u8[4];
		uint32_t u32;
	} version;
	uint32_t headersize;
	uint32_t unk1;
	uint32_t unk2;
	uint32_t unk3;
	uint32_t unk4;
	uint32_t unk5;
	uint32_t unk6;
	uint32_t unk7;
	uint32_t unk8;
	uint32_t unk9;
	uint32_t unk10;
	uint32_t unk11;
	uint32_t unk12;
};

//The header 1 extension, used if Arc_Header.version.u8[0] == 42
struct Arc_Header2
{
	uint32_t unk1;
	uint32_t unk2;
	uint32_t unk3;
	uint32_t unk4;
};

struct Arc_Settings
{
	int variable_declaration_startpos;
	int variable_declaration_endpos;
};

/*-----------------
	Class prototype/Variable declaration related
----------------*/

//A declaration of a variable
struct Variable_Dec
{
	string name;
	string datatype;
	uint32_t typesize;
	uint32_t subvarcount;
	vector<Variable_Dec> subvars;
};

//The prototype
struct Class_Prototype
{
	string name;
	vector<Variable_Dec> vars;
};

/*-----------------
	Data holders
----------------*/

//A structure containing a unique ID which refers to another block.
struct Reference
{
	uint32_t a;
	uint32_t b;

	uint32_t pos;
	bool valid; // Used for the aggressive compilation reference validation algorithm

	bool IsSame(Reference r)
	{
		if(r.a == a && r.b == b)
			return true;
		false;
	}

	bool IsSame(vector<Reference> r)
	{
		for(int i = 0; i < r.size(); i++)
			if(r[i].a == a && r[i].b == b)
				return true;
		false;
	}

	void operator++ ()
	{
		a++;
		if(a > 0xEFFFFFFF)
		{
			b++;
			a = 0;
		}
	}

	bool operator<(const Reference& compare) const
	{
		if (b > compare.b)
			return false;
		else if (a > compare.a)
			return false;
		return true;
	}
};

struct DataRef //Structure containing a reference to a UID and a datablockID
{
	Reference UID;
	uint32_t datablockID;
};

//Used to create a list of all references, parent refers to the array item ID
struct ParentedReference
{
	uint32_t parent;
	uint32_t a;
	uint32_t b;
};

struct StringData
{
	wstring name;
	wstring value;
};

struct DataRAM
{
	vector<StringData> strs;
	vector<Reference> refs;
	vector<string> refnames;
	vector<uint32_t> datpreview_positions;
	vector<uint32_t> managedimages;
};

//
struct Class_Data_Block
{
	uint32_t prototype_of;
	Reference uid;//Unique ID of the current block
	DataRAM RAM;
	uint32_t start;
	uint32_t end;
};

//For external use only
struct DatablockReference
{
	uint32_t prototype_of;
	Reference uid;
};


/*-----------------
	Park CleanUp reference optimalisation classes
----------------*/

/*struct UpdatedReference
{
	void SetNewReference(Reference nRef)
	{
		newRef.a =		nRef.a;
		newRef.b =		nRef.b;
		newRef.pos =	nRef.pos;
	}

	bool NewReferenceInitialised()
	{
		return (newRef.a >= 0 && newRef.b >= 0 && newRef.pos >= 0);
	}

	void CopyOldToNew()
	{
		newRef.a =		oldRef.a;
		newRef.b =		oldRef.b;
		newRef.pos =	oldRef.pos;
	}

	void InitFromReference(Reference ref)
	{
		oldRef.a =		ref.a;
		oldRef.b =		ref.b;
		oldRef.pos =	ref.pos;
		newRef.a = -1;
		newRef.b = -1;
		newRef.pos = -1;
	}

	Reference oldRef;
	Reference newRef;
};*/

/*-----------------
	Classes
----------------*/

//The R3 General Archive reader class
class R3AReader
{
private:
	bool warninggiven;
	bool errorgiven;

	bool valid;
	string fname;
	bool has_header;

	Arc_Header ah;
	Arc_Header2 ah2;	
	
	vector<Class_Prototype>		class_prototype;
	vector<Class_Data_Block>	class_data;

	bool ReadVariabledeclaration(Variable_Dec &var);
	bool ReadVariabledata(Variable_Dec dec, DataRAM &dr);
	
public:
	Arc_Settings as;
	BinfileReader arc;

	Class_Data_Block getclassdatabyuid(Reference UID)
	{
		Class_Data_Block ret;
		for(int i = 0; i < class_data.size(); i++)
		{
			if(class_data[i].uid.a == UID.a && class_data[i].uid.b == UID.b)
				return class_data[i];
		}
		return ret;
	}

	int getclassidbyuid(Reference UID)
	{
		for(int i = 0; i < class_data.size(); i++)
		{
			if(class_data[i].uid.a == UID.a && class_data[i].uid.b == UID.b)
				return i;
		}
		//TODO: Check if -1 doesn't cause errors
		return -1;
	}

	void Clean()
	{
		arc.ClearBuffer();
	}

	vector<Class_Data_Block> getclassdata()
	{
		return class_data;
	}

	vector<Class_Prototype> getclassprototypes()
	{
		return class_prototype;
	}

	bool IsCorrupted()
	{
		return arc.IsCorrupted();
	}

	bool HasHeader()
	{
		return has_header;
	}

	bool IsValid()
	{
		return valid;
	}

	void Invalidate()
	{
		valid = false;
	}

	string GetFilename()
	{
		return fname;
	}

	uint8_t GetFileVersion()
	{
		if (!has_header)
			return 0;
		return ah.version.u8[0];
	}

	R3AReader();
	R3AReader(string filename);
	~R3AReader(void);
};


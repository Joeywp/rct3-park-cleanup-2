#pragma once
#include <vector>
#include <string>
#include <vcclr.h>

#define OVLREADER_VERSION "1.1"

#pragma warning(disable:4244) // FPOS_T to LONG

using namespace std;
using namespace System;
using namespace System::Diagnostics;

typedef unsigned long ulong;
typedef unsigned short ushort;

//Header part 1
struct OVLHeader
{
	ulong magic;
	ulong headerversion;
	ulong version;	
};

//Header part 2
struct OVLHeader2
{
	ulong unknown;
	ulong totalloaders;
};

//Header part 1 v2
struct OVLHeaderVersion2Extra
{
	ulong unknown;
};

struct OVLHeaderV4Unknown
{
	ulong unknowna;
	ulong unknownb;
};

struct OVLHeaderV5Extra
{
	ulong subversion;
};

struct OVLHeaderV5Unknown
{
	ulong unknowna;
	ulong unknownb;
	ulong unknownc;
};

struct OVLV5LoaderExtra
{
	ulong count;
	ulong order;
};

struct OVLExtendedHeaders
{
	OVLHeaderVersion2Extra		headerversion2extra;
	OVLHeaderV5Extra			ovlv5extra;
	OVLHeaderV5Unknown			ovlv5unknown;
	OVLHeaderV4Unknown			ovlv4unknown;
};

struct V5Extra
{
	std::vector<OVLV5LoaderExtra>		ovlv5extraloaderinfo;
	ulong								unknownperblock[9];
	std::vector<ulong>					unknownafterfileblocks;
	ulong								extrastructcount;
};

struct OVLReference
{
	ushort length;
	string file;
};

struct Loader
{
	string loader;
	string name;
	ulong type;
	string tag;
};

struct Block
{
	size_t size;
	fpos_t position;
	size_t internal_offset;
};

struct Chunk
{
	ulong num_blocks;
	std::vector<Block> blocks;
};

struct SymbolString
{
	fpos_t pos;
	ulong internal_offset;
	string data;
};

struct SymbolStruct {
	ulong stringpointer;
	ulong datapointer;
	ulong ispointer;
	ulong loaderpointer;
	ulong hash;
	ulong internal_offset;
};

struct LoadReference {
	ulong loadernumber;
	ulong datapointer;
	ulong hasextradata;
	ulong symbolstructpointer;
	ulong num_symbolsresolve;
	ulong internal_offset;
};

struct SymbolResolve{
	ulong pointer;
	ulong stringpointer;
	ulong loadpointer;
	ulong stringhash;
	ulong internal_offset;
};

enum
{
	C_OVL =0,
	U_OVL
};

struct LinkedFiles
{
	LoadReference	loaderreference;
	SymbolStruct	symbolresolve;
};

struct PreResolved
{
	ulong offset;
	string name;
	ulong size;

	ulong count1;
	ulong count2;
	ulong count3;
};

struct SVDNameAndPath
{
	string name;
	string path;
};

struct TKSSPlines
{
	string left;
	string right;
	string joinleft;
	string joinright;
	string extraleft;
	string extraright;
};

struct OVLData
{
	string						ovlname;
	bool						hassymbolresolves;
	ulong						num_references;
	OVLHeader					h1;
	OVLHeader2					h2;
	OVLExtendedHeaders			extendedheaders;
	V5Extra						v5extra;

	std::vector<Loader>			loaders;
	std::vector<OVLReference>		references;
	std::vector<Chunk>			chunks;
	std::vector<ulong>			relocations;
	std::vector<ulong>			relocationspointingto;
	std::vector<SymbolString>	symbolstring;

	std::vector<SymbolStruct>	symbolpointers;
	std::vector<LoadReference>	loaderreference;
	std::vector<SymbolResolve>	symbolresolves;

	std::vector<PreResolved>	presolvedfurtherdata;
	std::vector<LinkedFiles>	linkedfiles;	
	OVLData(){
		hassymbolresolves = false;
	}
};

struct PositionReturn
{
	ulong currentOVL;
	ulong position;
};

struct TConvert
{
	union {
		unsigned long ul;
		float fl;
		struct {
			short hi;
			short lo;
		};
		unsigned char c[4];
	};
};

class OVLReader
{
private:
	string		lasterror;	
	bool		ReadOVL(string ovlname, ulong currentOVL, int &offset);
	bool		ovlreaded;
	string		GetStringA(FILE *&ovl, size_t size);
	string		GetStringA(FILE *&ovl);
	wstring		GetStringW(FILE *&ovl);
	OVLData		OVLD[2];
	string		LowerCase(string convert);
public:
	string safefilename;
	std::vector<SVDNameAndPath>	SVDNames;
	std::vector<TKSSPlines>		TKSNames;
	bool HasResource(string resourcename);	
	bool HasLoadTag(string tagname);

	//INTERN	
	std::vector<LinkedFiles>		GetRawLinkedFiles(ulong currentOVL){return OVLD[currentOVL].linkedfiles;};
	std::vector<SymbolStruct>		GetRawSymbolReferences(ulong currentOVL){return OVLD[currentOVL].symbolpointers;};
	std::vector<LoadReference>		GetRawLoaderReferences(ulong currentOVL){return OVLD[currentOVL].loaderreference;};
	std::vector<SymbolResolve>		GetRawSymbolResolves(ulong currentOVL){return OVLD[currentOVL].symbolresolves;};
	bool							IsValid(){return ovlreaded;}
	OVLData							GetOVLD(ulong currentOVL){ return OVLD[currentOVL];}
	string							GetLastReadingError(){return lasterror;}
	//EXTERN
	string							PointerdataAtOffset(ulong offset);
	Loader							GetLoaderByID(ulong id, ulong currentOVL);
	bool							IsReloc(ulong offset);
	PositionReturn					OffsetToPosition(ulong offset);
	string							GetStringFromOffset(ulong offset);
	OVLReader(string filename);	
};


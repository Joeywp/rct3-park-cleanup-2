#include "StdAfx.h"
#include "OVLReader.h"

bool ToString( String^ source, string &target )
{
	pin_ptr<const wchar_t> wch = PtrToStringChars( source );
	int len = (( source->Length+1) * 2);
	char *ch = new char[ len ];
	bool result = wcstombs( ch, wch, len ) != -1;
	target = ch;
	delete ch;
	return result;
}

string OVLReader::LowerCase(string convert)
{
	string retstr;
	String^ tmpconv = gcnew String(convert.c_str());
	tmpconv = tmpconv->ToLower();
	ToString(tmpconv, retstr);
	return retstr;
}

string OVLReader::PointerdataAtOffset(ulong offset)
{
	ulong i = 0, j = 0;
	for(j = 0; j < 2; j++)
	{
		for(i = 0; i < OVLD[j].symbolstring.size(); i++)
		{
			if(OVLD[j].symbolstring[i].internal_offset == offset)
			{
				string rv = "string ";
				rv.append(OVLD[j].symbolstring[i].data);
				return rv;
			}
		}
	}

	for(j = 0; j < 2; j++)
	{
		for(i = 0; i < OVLD[j].symbolpointers.size(); i++)
		{
			if(OVLD[j].symbolpointers[i].internal_offset == offset)
			{
				string rv = "symbolstruct with string ";
				rv.append(GetStringFromOffset(OVLD[j].symbolpointers[i].stringpointer));
				return rv;
			}
		}
	}	

	for(j = 0; j < 2; j++)
	{
		for(i = 0; i < OVLD[j].linkedfiles.size(); i++)
		{
			if(OVLD[j].linkedfiles[i].loaderreference.datapointer == offset)
			{
				string rv = "datablock of ";
				rv.append(GetStringFromOffset(OVLD[j].linkedfiles[i].symbolresolve.stringpointer));
				return rv;
			}
		}
	}

	for(j = 0; j < 2; j++)
	{
		for(i = 0; i < OVLD[j].loaderreference.size(); i++)
		{
			if(OVLD[j].loaderreference[i].internal_offset == offset)
			{
				string rv = "loaderrefence ";
				Loader ldr = GetLoaderByID(OVLD[j].loaderreference[i].loadernumber, j);
				rv.append(ldr.name);
				//Code for symbol pointer to string here?
				return rv;
			}
		}
	}

	for(j = 0; j < 2; j++)
	{
		for(i = 0; i < OVLD[j].loaderreference.size(); i++)
		{
			if(OVLD[j].loaderreference[i].datapointer == offset)
			{
				string rv = "datablock of stringless loaderrefence from loader ";
				Loader ldr = GetLoaderByID(OVLD[j].loaderreference[i].loadernumber, j);
				rv.append(ldr.name);
				return rv;
			}
		}
	}

	for(j = 0; j < 2; j++)
	{
		for(i = 0; i < OVLD[j].symbolresolves.size(); i++)
		{
			if(OVLD[j].symbolresolves[i].pointer == offset)
			{
				string rv = "symbol resolve data ";
				rv.append(GetStringFromOffset(OVLD[j].symbolresolves[i].stringpointer));
				return rv;
			}
		}
	}

	for(j = 0; j < 2; j++)
	{
		for(i = 0; i < OVLD[j].presolvedfurtherdata.size(); i++)
		{
			if(OVLD[j].presolvedfurtherdata[i].offset == offset)
			{
				string rv = "extended data of ";
				rv.append(OVLD[j].presolvedfurtherdata[i].name);
				return rv;
			}
		}
	}

	for(j = 0; j < 2; j++)
	{
		for(i = 0; i < OVLD[j].symbolpointers.size(); i++)
		{
			if(OVLD[j].symbolpointers[i].datapointer == offset)
			{
				string rv = "incoming symbolstruct datapointer with string ";
				rv.append(GetStringFromOffset(OVLD[j].symbolpointers[i].stringpointer));
				return rv;
			}
		}
	}
	return "unknown";
}

bool OVLReader::HasResource(string resourcename)
{
	ulong j, i;
	for(j = 0; j < 2; j++)
	{
		for(i = 0; i < OVLD[j].linkedfiles.size(); i++)
		{
			string currentresouce = GetStringFromOffset(OVLD[j].linkedfiles[i].symbolresolve.stringpointer);
			//Debug::WriteLine("HasResource: " + gcnew String(currentresouce.c_str()) + " == " + gcnew String(resourcename.c_str()));
			if(LowerCase(currentresouce) == LowerCase(resourcename))
				return true;
		}
	}
	return false;
}

bool OVLReader::HasLoadTag(string tagname)
{
	ulong j, i;
	for(j = 0; j < 2; j++)
	{
		for(i = 0; i < OVLD[j].loaderreference.size(); i++)
		{
			Loader ldr = GetLoaderByID(OVLD[j].loaderreference[i].loadernumber, j);
			if(LowerCase(ldr.tag) == LowerCase(tagname))
				return true;
		}
	}
	return false;
}

string OVLReader::GetStringA(FILE *&ovl, size_t size)
{
	string returnstring;
	char *str = new char[size+1];
	str[size] = 0;
	fread(str,size,1,ovl);		

	returnstring = str;
	delete[] str;
	return returnstring;
}

string OVLReader::GetStringA(FILE *&ovl)
{
	ulong size = 0;
	fpos_t readpos;
	fgetpos(ovl, &readpos);
	ulong temp = 0;
	ulong i = 0;
	string returnstring;

	do{
		fread(&temp,1,1,ovl);
		size++;
	} while (temp != 0 && i < 1000);

	fseek(ovl, readpos, SEEK_SET);
	char *str = new char[size+1];
	str[size] = 0;
	fread(str,size,1,ovl);

	returnstring = str;
	delete[] str;
	return returnstring;
}

wstring OVLReader::GetStringW(FILE *&ovl)
{
	ulong size = 0;
	fpos_t readpos;
	fgetpos(ovl, &readpos);
	ulong temp = 0;
	ulong i = 0;
	wstring returnstring;

	bool nulled = false;
	bool nulled2 = false;
	do{
		fread(&temp,1,1,ovl);					 
		if(temp == 0 && nulled == false)
		{
			nulled = true;
			size++;
		}
		else if(temp == 0 && nulled == true)
		{
			nulled2 = true;
		} else {
			nulled = false;
			size++;
		}
	} while (!nulled2 && i < 1000);

	fseek(ovl, readpos, SEEK_SET);
	wchar_t *str = new wchar_t[size];
	str[size] = 0;
	fread(str, (size+2), 1, ovl);

	returnstring = str;
	//delete[] str;
	return returnstring;
}

PositionReturn OVLReader::OffsetToPosition(ulong offset)
{
	PositionReturn pr;
	ulong offsetLeft = offset;
	ulong position = -1;
	ulong currentOVL = 0;

	ulong i = 0;ulong j = 0;
	bool found = false;
	for (currentOVL = 0; currentOVL < 2; currentOVL++)
	{
		for (i= 0; i < 9; i++)
		{
			for(j = 0; j < OVLD[currentOVL].chunks[i].num_blocks; j++)
			{
				if(offsetLeft < OVLD[currentOVL].chunks[i].blocks[j].size && OVLD[currentOVL].chunks[i].blocks[j].size > 0)
				{
					position = OVLD[currentOVL].chunks[i].blocks[j].position + offsetLeft;					
					//Debug::WriteLine("Offset > Position: Found!");
					pr.currentOVL = currentOVL;
					found = true;
					break;
				}
				offsetLeft -= OVLD[currentOVL].chunks[i].blocks[j].size;
				if(found)
					break;
			}
			if(found)
				break;
		}
		if(found)
			break;
	}
	if(!found)
		Debug::WriteLine("Offset not found @OVLReader::OffsetToPosition");
	pr.position = position;
	return pr;
}

bool OVLReader::IsReloc(ulong offset)
{
	ulong i;
	ulong j;
	for (i= 0; i < 2; i++)
	{
		for(j = 0; j < OVLD[i].relocations.size(); j++)
		{
			if(OVLD[i].relocations[j] == offset)
				return true;
		}
	}
	return false;
}

string OVLReader::GetStringFromOffset(ulong offset)
{
	ulong i = 0;
	for(i = 0; i < OVLD[0].symbolstring.size(); i++)
	{
		if(OVLD[0].symbolstring[i].internal_offset == offset)
			return OVLD[0].symbolstring[i].data;
	}
	for(i = 0; i < OVLD[1].symbolstring.size(); i++)
	{
		if(OVLD[1].symbolstring[i].internal_offset == offset)
			return OVLD[1].symbolstring[i].data;
	}
	return "STRINGNOTFOUND";
}

Loader OVLReader::GetLoaderByID(ulong id, ulong currentOVL)
{
	if(id < OVLD[currentOVL].loaders.size())
		return OVLD[currentOVL].loaders[id];
	Loader lr;
	return lr;
}




OVLReader::OVLReader(string filename)
{
	//Debug::WriteLine("Reading OVL " + gcnew String(filename.c_str()));
	if(filename == "null")
		return;
	ovlreaded = false;
	int offset = 0;
	if(!ReadOVL(filename, false, offset))
	{
		//Debug::WriteLine(gcnew String(lasterror.c_str()));
		return;
	}
	if(OVLD[0].h1.version == 6)
		return;
	//Debug::WriteLine("-=-=-=-=-=-=-=-=-=-=-");

	if(!ReadOVL(filename, true, offset))
	{
		//Debug::WriteLine(gcnew String(lasterror.c_str()));
		return;
	}
	ovlreaded = true;
}

bool OVLReader::ReadOVL(string ovlname, ulong currentOVL, int &offset)
{
	ulong i = 0;
	ulong j = 0;
	FILE* ovl;
	ulong ovlversion = 0;
	fpos_t position;
	if(ovlname.find(".ovl") == -1)
	{
		if(currentOVL)
			ovlname += ".unique.ovl";
		else
			ovlname += ".common.ovl";
	}
	OVLD[currentOVL].ovlname = ovlname;
	//Debug::WriteLine("Open " + currentOVL + gcnew String(ovlname.c_str()));
	if(fopen_s(&ovl, ovlname.c_str(), "rb") == EINVAL){
		lasterror = "File EINVAL error";
		return false;
	}
	if(ovl == NULL)
	{
		lasterror = "Could not open overlay ";
		if(currentOVL)
			ovlname.append("unique.ovl");
		else
			ovlname.append("common.ovl");
		return false;
	}

	fread(&OVLD[currentOVL].h1, sizeof(OVLHeader), 1, ovl);

	//MAGIC
	//0x4b524746
	//Debug::WriteLine("Version " + OVLD[currentOVL].h1.version);
	ovlversion = OVLD[currentOVL].h1.version;
	if(OVLD[currentOVL].h1.magic != 0x4b524746)
	{
		lasterror = "File is not an overlay";
		return false;
	}
	if(ovlversion != 1 && ovlversion != 4 && ovlversion != 5 && ovlversion != 6)
	{
		ToString("Bad header version, not readable (version not supported), version is " + ovlversion, lasterror);
		return false;
	}

	//v4/5 code
	if(OVLD[currentOVL].h1.headerversion == 2)
	{
		fread(&OVLD[currentOVL].extendedheaders.headerversion2extra, sizeof(OVLHeaderVersion2Extra), 1, ovl);
		if(OVLD[currentOVL].extendedheaders.headerversion2extra.unknown != 16)
		{
			lasterror = "Bad header version, not readable: headerversion2 extra != 16 (this seems to be some kind of size indicator for the 'pre loader header'";
			return false;
		}
		if(ovlversion == 4)
			fread(&OVLD[currentOVL].num_references, 4, 1, ovl);
	}
	if(ovlversion == 5 || ovlversion == 6)
	{
		fread(&OVLD[currentOVL].extendedheaders.ovlv5extra, sizeof(OVLHeaderV5Extra), 1, ovl);
		//Debug::WriteLine("Subversion " + OVLD[currentOVL].extendedheaders.ovlv5extra.subversion);
		if(OVLD[currentOVL].extendedheaders.ovlv5extra.subversion == 1)
		{
			fread(&OVLD[currentOVL].extendedheaders.ovlv5unknown, sizeof(OVLHeaderV5Unknown), 1, ovl);
			//Debug::WriteLine("Unknown A" + OVLD[currentOVL].extendedheaders.ovlv5unknown.unknowna);
			//Debug::WriteLine("Unknown B" + OVLD[currentOVL].extendedheaders.ovlv5unknown.unknownb);
			//Debug::WriteLine("Unknown C" + OVLD[currentOVL].extendedheaders.ovlv5unknown.unknownc);
			unsigned char tc;
			unsigned int dotemp = 0;
			do {
				fread(&tc, 1, 1, ovl);
				dotemp++;
				if (dotemp == 4) {
					dotemp = 0;
				}
			} while (tc != 0);
			for (j = dotemp; j < 4; j++) {
				fread(&tc, 1, 1, ovl);
			}
		}
		fread(&OVLD[currentOVL].num_references, 4, 1, ovl);
		//Debug::WriteLine("Num references: " + OVLD[currentOVL].num_references);
		if(ovlversion == 6)
		{
			ulong temp, temp2;
			fread(&temp, 4, 1, ovl);
			for (j = 0; j < temp; j++)
			{
				fread(&temp2, 2, 1, ovl);
			}
		}
	}

	if(ovlversion == 6)
	{
		fgetpos(ovl, &position);
		Debug::WriteLine("End position: " + position);
		lasterror = "OVL v6 not fully researched, further structure is unknown (this function is just build-in for research)";
		return false;
	}

	//OVLReferences
	if(ovlversion == 1)
		fread(&OVLD[currentOVL].num_references, 4, 1, ovl);
	for(i = 0; i < OVLD[currentOVL].num_references; i++)
	{
		OVLReference ref;
		fread(&ref.length, 2, 1, ovl);
		ref.file = GetStringA(ovl, ref.length);
		//Debug::WriteLine("OVLReference to " +  gcnew String(ref.file.c_str()));

		OVLD[currentOVL].references.push_back(ref);
	}	

	//Loaders
	fread(&OVLD[currentOVL].h2, sizeof(OVLHeader2), 1, ovl);
	for(i = 0; i < OVLD[currentOVL].h2.totalloaders; i++)
	{
		Loader ldr;
		ushort len;

		fread(&len, 2, 1, ovl);
		ldr.loader = GetStringA(ovl, len);

		fread(&len, 2, 1, ovl);
		ldr.name = GetStringA(ovl, len);

		fread(&ldr.type, 4, 1, ovl);

		fread(&len, 2, 1, ovl);
		ldr.tag = GetStringA(ovl, len);

		OVLD[currentOVL].loaders.push_back(ldr);
		//Debug::WriteLine("Loader " +  gcnew String(ldr.loader.c_str()) + " - " + gcnew String(ldr.name.c_str()) + " - " + gcnew String(ldr.tag.c_str()));
	}	

	//Chunks
	if(ovlversion == 5)
	{
		for(i = 0; i < OVLD[currentOVL].h2.totalloaders; i++)
		{
			OVLV5LoaderExtra extra;
			fread(&extra, sizeof(OVLV5LoaderExtra), 1, ovl);
			OVLD[currentOVL].v5extra.ovlv5extraloaderinfo.push_back(extra);
		}
	}
	for(i = 0; i < 9; i++)
	{
		if(ovlversion == 5)
		{
			ulong len;
			ulong temp;
			Chunk chk;
			fread(&len, 4, 1, ovl);
			fread(&temp, 4, 1, ovl);
			chk.num_blocks = len;
			//Debug::WriteLine("Block[" + (1+i) + "] " + len);
			OVLD[currentOVL].chunks.push_back(chk);
			if(OVLD[currentOVL].extendedheaders.ovlv5extra.subversion == 1)
			{
				fread(&OVLD[currentOVL].v5extra.unknownperblock[i], 4, 1, ovl);
			}
			for(j = 0; j < len; j++)
			{
				Block blk;
				fread(&blk.size, 4, 1, ovl);
				OVLD[currentOVL].chunks[i].blocks.push_back(blk);
				//Debug::WriteLine("Block[" + (1+i) + "][" + j + "].size = " + blk.size);
			}
		} else {
			ulong len;
			Chunk chk;
			fread(&len, 4, 1, ovl);
			if(ovlversion == 4 || ovlversion == 5)
				fread(&len, 4, 1, ovl);
			chk.num_blocks = len;
			//Debug::WriteLine("Block[" + (1+i) + "] " + len);
			OVLD[currentOVL].chunks.push_back(chk);
			if(ovlversion == 4)
			{
				for(j = 0; j < len; j++)
				{
					Block blk;
					fread(&blk.size, 4, 1, ovl);
					OVLD[currentOVL].chunks[i].blocks.push_back(blk);
					//Debug::WriteLine("Block[" + (1+i) + "][" + j + "].size = " + blk.size);
				}
			}
		}
	}

	if(ovlversion == 5)
	{
		ulong temp;
		ulong temp2;

		fread(&temp,4,1,ovl);
		if(temp >= 4){
			fread(&OVLD[currentOVL].v5extra.extrastructcount,4,1,ovl);
			//Debug::WriteLine("ESC" + OVLD[currentOVL].v5extra.extrastructcount);
			if(temp > 4){
				fseek(ovl,temp-4,SEEK_CUR);
			}
		}

		fread(&temp,4,1,ovl);
		if(temp != 0)
			Debug::WriteLine("Info: Unk2 van de fileblocks != 0");
		for(i = 0; i < temp; i++){
			fread(&temp2,4,1,ovl);
			OVLD[currentOVL].v5extra.unknownafterfileblocks.push_back(temp2);
		}
	}	

	if(ovlversion == 4)
	{
		fread(&OVLD[currentOVL].extendedheaders.ovlv4unknown, sizeof(OVLHeaderV4Unknown), 1, ovl);
	}

	//Blocksizes etc inlezen
	for (i=0;i<9;i++){
		for(j=0;j<OVLD[currentOVL].chunks[i].num_blocks;j++){
			if(ovlversion == 1)
			{
				Block blk;
				fread(&blk.size, 4, 1, ovl);
				fgetpos(ovl, &position);
				blk.internal_offset = offset;
				blk.position = position;

				offset += blk.size;
				fseek(ovl, blk.size, SEEK_CUR);
				OVLD[currentOVL].chunks[i].blocks.push_back(blk);
			} else if(ovlversion == 4 || ovlversion == 5)
			{
				fgetpos(ovl, &position);
				//Debug::WriteLine("Size: " + OVLD[currentOVL].chunks[i].blocks[j].size);
				OVLD[currentOVL].chunks[i].blocks[j].internal_offset = offset;
				OVLD[currentOVL].chunks[i].blocks[j].position = position;
				//Debug::WriteLine("Pos: " + position);
				offset += OVLD[currentOVL].chunks[i].blocks[j].size;
				fseek(ovl, OVLD[currentOVL].chunks[i].blocks[j].size, SEEK_CUR);
			}
		}
	}		

	//Relocations
	fread(&j, 4, 1, ovl);
	for (i = 0; i < j; i++){
		ulong relocation;
		fread(&relocation, 4, 1, ovl);
		//Debug::WriteLine("Relocation[" + (1+i) + "] " + relocation);
		OVLD[currentOVL].relocations.push_back(relocation);
	}	
	
	//Stringtable
	if(OVLD[currentOVL].chunks[0].num_blocks >= 1)
	{
		ulong stringoffset = 0;
		fseek(ovl, OVLD[currentOVL].chunks[0].blocks[0].position, SEEK_SET);
		do{
			SymbolString ss;
			ss.data = GetStringA(ovl);
			ss.internal_offset = stringoffset;
			stringoffset += ss.data.length() + 1;

			//Debug::WriteLine("STR: " +gcnew String(ss.data.c_str()));
			OVLD[currentOVL].symbolstring.push_back(ss);
			fgetpos(ovl, &position);
		} while (position < (OVLD[currentOVL].chunks[0].blocks[0].size + OVLD[currentOVL].chunks[0].blocks[0].position));
	}

	//Symbol references
	if(OVLD[currentOVL].chunks[2].num_blocks >= 1)
	{
		ulong count;
		if(ovlversion == 1)
			count = OVLD[currentOVL].chunks[2].blocks[0].size/12;
		else if (ovlversion == 4 || ovlversion == 5)
			count = OVLD[currentOVL].chunks[2].blocks[0].size/16;
		fseek(ovl, OVLD[currentOVL].chunks[2].blocks[0].position, SEEK_SET);
		ulong loaderoffset = OVLD[currentOVL].chunks[2].blocks[0].internal_offset;

		//Debug::WriteLine("Symbol references offset " + loaderoffset);

		for (i = 0; i < count; i++){
			SymbolStruct ss;
			if(ovlversion == 1)
				fread(&ss, 12, 1, ovl);
			else if (ovlversion == 4)
				fread(&ss, 16, 1, ovl);
			else if (ovlversion == 5)
			{
				fread(&ss.stringpointer, 4, 1, ovl);
				fread(&ss.datapointer, 4, 1, ovl);
				fread(&ss.ispointer, 2, 1, ovl);
				fread(&ss.loaderpointer, 2, 1, ovl);
				fread(&ss.hash, 4, 1, ovl);
			}
			ss.internal_offset = loaderoffset;

			//string pointto = GetStringFromOffset(ss.stringpointer);
			//Debug::WriteLine(gcnew String(pointto.c_str()));
			if(ovlversion == 1)
				loaderoffset += 12;
			else if (ovlversion == 4 || ovlversion == 5)
				loaderoffset += 16;
			OVLD[currentOVL].symbolpointers.push_back(ss);
		}
	}
	
	//Loader references
	if(OVLD[currentOVL].chunks[2].num_blocks >= 2)
	{
		ulong count = OVLD[currentOVL].chunks[2].blocks[1].size/20;
		fseek(ovl, OVLD[currentOVL].chunks[2].blocks[1].position, SEEK_SET);
		ulong loaderoffset = OVLD[currentOVL].chunks[2].blocks[1].internal_offset;		

		//Debug::WriteLine("Loader offset " + loaderoffset);

		for (i = 0; i < count; i++){
			LoadReference lr;
			fread(&lr, 20, 1, ovl);
			lr.internal_offset = loaderoffset;

			//string pointto = GetStringFromOffset(ss.stringpointer);
			//Debug::WriteLine(gcnew String(pointto.c_str()));

			loaderoffset += 20;
			OVLD[currentOVL].loaderreference.push_back(lr);
			if(lr.num_symbolsresolve > 0)
				OVLD[currentOVL].hassymbolresolves = true;
		}
	}	
	
	//Symbol resolves
	if(OVLD[currentOVL].chunks[2].num_blocks >= 3 && OVLD[currentOVL].hassymbolresolves)
	{
		ulong count;
		if(ovlversion == 1)
			count = OVLD[currentOVL].chunks[2].blocks[2].size/12;
		else if (ovlversion == 4 || ovlversion == 5)
			count = OVLD[currentOVL].chunks[2].blocks[2].size/16;
		fseek(ovl, OVLD[currentOVL].chunks[2].blocks[2].position, SEEK_SET);
		ulong loaderoffset = OVLD[currentOVL].chunks[2].blocks[2].internal_offset;

		//Debug::WriteLine("Symbol resolves offset " + loaderoffset);

		for (i = 0; i < count; i++){
			SymbolResolve sr;
			if(ovlversion == 1)
				fread(&sr, 12, 1, ovl);
			else if (ovlversion == 4 || ovlversion == 5)
				fread(&sr, 16, 1, ovl);
			sr.internal_offset = loaderoffset;

			//string pointto = GetStringFromOffset(sr.stringpointer);
			//Debug::WriteLine(gcnew String(pointto.c_str()));
			if(ovlversion == 1)
				loaderoffset += 12;
			else if (ovlversion == 4 || ovlversion == 5)
				loaderoffset += 16;
			OVLD[currentOVL].symbolresolves.push_back(sr);
		}
	}

	//Link loaderreferences met symbolreferences
	for (i = 0; i < OVLD[currentOVL].loaderreference.size(); i++)
	{
		LinkedFiles lf;
		lf.loaderreference = OVLD[currentOVL].loaderreference[i];
		bool found = false;
		for (j = 0; j < OVLD[currentOVL].symbolpointers.size(); j++)
		{
			if(OVLD[currentOVL].loaderreference[i].symbolstructpointer == OVLD[currentOVL].symbolpointers[j].internal_offset)
			{
				lf.symbolresolve = OVLD[currentOVL].symbolpointers[j];
				found = true;
				break;
			}
		}

		if(!found)
			if(OVLD[currentOVL].loaderreference[i].symbolstructpointer != 0)
				//Debug::WriteLine("Loader[" + i + "] is pointer to extradata");
			//else
				Debug::WriteLine("Loader[" + i + "] could not find symbol " + OVLD[currentOVL].loaderreference[i].internal_offset);
		if(found)
			OVLD[currentOVL].linkedfiles.push_back(lf);
	}

	//Get the offset of where relocations point to
	for(i = 0; i < OVLD[currentOVL].relocations.size(); i++)
	{
		PositionReturn pr = OffsetToPosition(OVLD[currentOVL].relocations[i]);
		if(pr.position != -1)
		{
			ulong temp;
			fseek(ovl, pr.position, SEEK_SET);
			fread(&temp, 4, 1, ovl);
			PositionReturn pr = OffsetToPosition(temp);
			OVLD[pr.currentOVL].relocationspointingto.push_back(temp);
		} else {
			Debug::WriteLine("Could not find where relocation is pointing, the pointer is out of bounds (unrelocatable)");
		}
	}

	//
	for(i = 0; i < OVLD[currentOVL].loaderreference.size(); i++)
	{		
		Loader ldr = GetLoaderByID(OVLD[currentOVL].loaderreference[i].loadernumber, currentOVL);
		ulong stringpointer = 0xFEFFFFFF;
		for(ulong z = 0; z < OVLD[currentOVL].symbolpointers.size(); z++)
		{
			if(OVLD[currentOVL].symbolpointers[z].internal_offset == OVLD[currentOVL].loaderreference[i].symbolstructpointer)
				stringpointer = OVLD[currentOVL].symbolpointers[z].stringpointer;
		}
		string symbol = (stringpointer != 0xFEFFFFFF)?GetStringFromOffset(stringpointer):"";
		ulong temp;	
		PositionReturn pr = OffsetToPosition(OVLD[currentOVL].loaderreference[i].datapointer);
		fseek(ovl, pr.position, SEEK_SET);
		//Debug::WriteLine("TAG " +gcnew String(ldr.tag.c_str()));
		//First level
		if(ldr.tag == "sid")
		{
			string ovlpath = "";
			ulong svdcount;
			ulong svdpointer;
			fseek(ovl, 80, SEEK_CUR);
			fread(&svdcount, 4, 1, ovl);
			fread(&svdpointer, 4, 1, ovl);

			fseek(ovl, 12, SEEK_CUR);
			fread(&temp, 4, 1, ovl);
			ovlpath = GetStringFromOffset(temp);
			//Debug::WriteLine(gcnew String(ovlpath.c_str()) + " SVDP: " + svdpointer);
			//temp2 =0;

			ulong tempoffset = svdpointer;
			for(j = 0; j < svdcount; j++)
			{
				bool found = false;
				string SVDname = "";
				for(ulong z = 0; z < 2; z++)
				{
					for(ulong k = 0; k < OVLD[z].symbolresolves.size(); k++)
					{
						if(OVLD[z].symbolresolves[k].pointer == tempoffset)
						{
							SVDNameAndPath tmpsvd;
							found = true;
							SVDname = GetStringFromOffset(OVLD[z].symbolresolves[k].stringpointer);
							//Debug::WriteLine("SVDName " + gcnew String(SVDname.c_str()) + " Path " + gcnew String(ovlpath.c_str()));
							tmpsvd.name = SVDname;
							tmpsvd.path = ovlpath;
							SVDNames.push_back(tmpsvd);
						}
					}
				}
				tempoffset += 4;
			}		
		}/* else if(ldr.tag == "tks")
		{
			ulong splineptrs[6];
			fseek(ovl, (8*4), SEEK_CUR);
			for(ulong z = 0; z < 6; z++)
				fread(&splineptrs[z], 4, 1, ovl);

			TKSSPlines tkstemp;
			tkstemp.left = GetStringFromOffset(splineptrs[0]);
			tkstemp.right = GetStringFromOffset(splineptrs[1]);
			tkstemp.joinleft = GetStringFromOffset(splineptrs[2]);
			tkstemp.joinright = GetStringFromOffset(splineptrs[3]);
			tkstemp.extraleft = GetStringFromOffset(splineptrs[4]);
			tkstemp.extraright = GetStringFromOffset(splineptrs[5]);

			//delete[] splineptrs;
		}*/
	}

	fclose(ovl);
	return true;
}

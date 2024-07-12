#include "StdAfx.h"
#include "R3AReader.h"

using namespace std;
using namespace System::Windows::Forms;
using namespace System::Diagnostics;

/*#define T_MSG(text, caption) \
do { \
MessageBox::Show(text + "\r\n\r\nTechnical data related to this bug\r\nFile: " + __FILE__ + "\r\nLine: " + __LINE__, caption, MessageBoxButtons::OK, MessageBoxIcon::Error); \
} while (false)*/

void T_MSG(String^ text, String^ caption)
{

};

void MSG(String^ text, String^ caption)
{
	MessageBox::Show(text, caption);
}

bool R3AReader::ReadVariabledeclaration(Variable_Dec &var)
{
	var.name = arc.getstring16();
	var.datatype = arc.getstring16();
	var.typesize = arc.getu32();
	var.subvarcount = arc.getu32();
	if(var.subvarcount > 200)
		assert(1 && "Too many subchilds in variable declaration");
	//assert(var.subvarcount > 200 && "Too many subchilds in variable declaration");

	for(uint32_t i = 0; i < var.subvarcount; i++)
	{	
		//MSG("Going to read subvars!", "");
		Variable_Dec var_dec;		
		if(ReadVariabledeclaration(var_dec) == false)
		{
			T_MSG("Could not read variable declaration of variable '" + gcnew String(var.name.c_str()) + "' of type '" + gcnew String(var.datatype.c_str()) + "'\r\n\r\nWhat does this mean: Possibly you selected either a non-rct3 file or the park might be corrupted. It is also possible it's just a bug in the archive reader.", "File error");
			return false;
		}
		//var.subvars.push_back(var_dec);

		if(!arc.good())
		{
			T_MSG("Could not read variable declaration of variable '" + gcnew String(var.name.c_str()) + "' of type '" + gcnew String(var.datatype.c_str()) + "'\r\n\r\nWhat does this mean: Possibly you selected either a non-rct3 file or the park might be corrupted. It is also possible it's just a bug in the archive reader.", "File error");
			errorgiven = true;
			return false;
		}
		var.subvars.push_back(var_dec);
	}

	if(!arc.good())
	{
		T_MSG("Could not read variable declaration of variable '" + gcnew String(var.name.c_str()) + "' of type '" + gcnew String(var.datatype.c_str()) + "'\r\n\r\nWhat does this mean: Possibly you selected either a non-rct3 file or the park might be corrupted. It is also possible it's just a bug in the archive reader.", "File error");
		errorgiven = true;
		return false;
	}
	return true;
}

bool R3AReader::ReadVariabledata(Variable_Dec dec, DataRAM &RAM)
{	
	uint32_t file_pos = arc.getpos();
	//Debug::WriteLine("Pos: " + file_pos + "  Type: " + gcnew String(dec.datatype.c_str()) + "  Name: " + gcnew String(dec.name.c_str()));
	uint32_t len = 0;
	uint32_t items = 0;

	if(dec.datatype == "int32")
	{
		arc.skip(4);
	} else if(dec.datatype == "uint32")
	{
		arc.skip(4);
	} else if(dec.datatype == "uint8")
	{
		arc.skip(1);
	} else if(dec.datatype == "int8")
	{
		arc.skip(1);
	} else if(dec.datatype == "bool")
	{
		arc.skip(1);
	} else if(dec.datatype == "lebool")
	{		
		int leboolsize = arc.getu32();
		if(leboolsize > 0)
			T_MSG("'lebool' is greater then 0, the reader will try to skip the data", "Unstable datatype found");
		arc.skip(leboolsize);
	} else if(dec.datatype == "float32")
	{
		arc.skip(4);
	} else if(dec.datatype == "resourcesymbol")
	{
		arc.getstring32();
	} else if(dec.datatype == "reference" || dec.datatype == "managedobjectptr")
	{		
		Reference ref;
		arc.read((char*)&ref.a, 4);
		arc.read((char*)&ref.b, 4);
		ref.pos = file_pos;
		RAM.refs.push_back(ref);
		RAM.refnames.push_back(dec.name);
	} else if(dec.datatype == "string")
	{
		wstring str = arc.s_getstring32();
		StringData sd;
		sd.name = StringToWString(dec.name);
		sd.value = str;
		RAM.strs.push_back(sd);
	} else if(dec.datatype == "vector3")
	{
		arc.skip(12);
	} else if(dec.datatype == "orientation")
	{
		arc.skip(12);
	} else if(dec.datatype == "pathnodearray" || dec.datatype == "BlockingScenery")
	{
		arc.seek(arc.getu32(), BFR::CUR);
	} else if(dec.datatype == "graphedValue")
	{
		arc.skip(12);
		uint32_t size = arc.getu32();
		arc.seek(size*4, BFR::CUR);
	} else if(dec.datatype == "GE_Terrain")
	{
		arc.seek(arc.getu32(), BFR::CUR);
	} else if(dec.datatype == "SkirtTrees")
	{
		arc.seek(arc.getu32(), BFR::CUR);
	} else if(dec.datatype == "PathTileList")
	{
		uint32_t tmp = arc.getu32();
		arc.seek(tmp, BFR::CUR);
		if(tmp > 0)
			T_MSG("PathTileList is greater then 0, the reader will try to skip the data", "File error");
	} else if(dec.datatype == "WaterManager")
	{
		arc.seek(arc.getu32(), BFR::CUR);
	} else if(dec.datatype == "managedImage")
	{
		RAM.managedimages.push_back(file_pos);
		arc.getu32();
		arc.getu32();
		arc.getu32();
		arc.getu32();
		uint32_t size = arc.getu32();
		arc.seek(size*4, BFR::CUR);
	} else if(dec.datatype == "flexicachelist")
	{
		arc.seek(arc.getu32(), BFR::CUR);
	} else if(dec.datatype == "waypointlist")
	{
		arc.seek(arc.getu32(), BFR::CUR);
	} else if(dec.datatype == "matrix44")
	{
		arc.seek((16*4), BFR::CUR);
	} else if(dec.datatype == "array")
	{		
		//Debug::WriteLine("Struct start: " + arc.getpos());
		file_pos = arc.getpos();		
		len = arc.getu32();
		assert(len >= 0);

		//MSG("Struct Length:" + len + "\r\nName: " + gcnew String(dec.name.c_str()) + "\r\nFpos: " + file_pos, "File error");
		file_pos = arc.getpos();
		items = arc.getu32();
		assert(items >= 0);
		//Debug::WriteLine("Struct end: " + arc.getpos());
	} else if(dec.datatype == "list")
	{
		//Debug::WriteLine("Struct start: " + arc.getpos());
		file_pos = arc.getpos();		
		len = arc.getu32();
		assert(len >= 0);

		//MSG("Struct Length:" + len + "\r\nName: " + gcnew String(dec.name.c_str()) + "\r\nFpos: " + file_pos, "File error");
		items = arc.getu32();
		assert(items >= 0);
		//Debug::WriteLine("Struct end: " + arc.getpos());
	} else if( dec.datatype == "struct")
	{
		//Debug::WriteLine("Struct start: " + arc.getpos());
		file_pos = arc.getpos();		
		if(dec.typesize > 0)
			len = dec.typesize;
		else
			len = arc.getu32();
		assert(len >= 0);
	} else {
		MessageBox::Show("Parsing error #1: Type unknown!" + gcnew String(dec.datatype.c_str()));
		errorgiven = true;
		return false;
	}

	if(!arc.good())
	{

	}

	if(dec.datatype == "struct")
	{	
		//Debug::WriteLine("Struct start2: " + arc.getpos());
		file_pos = arc.getpos();
		for(uint32_t i = 0; i < dec.subvars.size(); i++)
		{
			if(ReadVariabledata(dec.subvars[i], RAM) == false)
				return false;
		}
		uint32_t t = arc.getpos();
		uint32_t bytesread = t - file_pos;
		if(bytesread != len)
		{
			T_MSG("Struct fail!\r\nRead:" + bytesread + "\r\nLength:" + len + "\r\nName: " + gcnew String(dec.name.c_str()) + "\r\nFpos: " + file_pos, "File error");
			arc.seek((file_pos + len), BFR::BEGIN);
			//return false;
		}
		//Debug::WriteLine("Struct end2: " + arc.getpos());
	} else if(dec.datatype == "list")
	{
		//T_MSG("Could not handle variable childs", "File error");
		//return false;
		file_pos = arc.getpos();
		if(items > 0)
		{
			for(uint32_t j = 0; j < items; j++)
			{
				for(uint32_t i = 0; i < dec.subvars.size(); i++)
				{
					if(ReadVariabledata(dec.subvars[i], RAM) == false)
						return false;
				}
			}
		}
		uint32_t t = arc.getpos();
		if(t != (file_pos + len - 4))
		{
			T_MSG("Array fail!\r\nLength:" + len + "\r\nName: " + gcnew String(dec.name.c_str()) + "\r\nFpos: " + file_pos, "File error");
			arc.seek((file_pos + len - 4), BFR::BEGIN);
		}
	} else if(dec.datatype == "array")
	{
		//T_MSG("Could not handle variable childs", "File error");
		//return false;
		file_pos = arc.getpos();
		if(dec.name == "DatPreview")
			RAM.datpreview_positions.push_back(file_pos);

		if(items > 0)
		{
			for(uint32_t j = 0; j < items; j++)
			{
				for(uint32_t i = 0; i < dec.subvars.size(); i++)
				{
					if(ReadVariabledata(dec.subvars[i], RAM) == false)
						return false;
				}
			}
		}

		uint32_t t = arc.getpos();
		if(t != (file_pos + len - 4))
		{
			T_MSG("Array fail!\r\nLength:" + len + "\r\nName: " + gcnew String(dec.name.c_str()) + "\r\nFpos: " + file_pos, "File error");
			arc.seek((file_pos + len - 4), BFR::BEGIN);
		}
	} else
	{
		for(uint32_t i = 0; i < dec.subvars.size(); i++)
		{
			if(ReadVariabledata(dec.subvars[i], RAM) == false)
				return false;
		}
	}

	return true;
}

R3AReader::R3AReader()
{
	has_header = false;
	warninggiven = false;
	errorgiven = false;
	valid = false;
};

R3AReader::R3AReader(string filename)
{
	has_header = false;
	warninggiven = false;
	errorgiven = false;
	valid = false;
	fname = filename;
	arc.open( filename , std::ios::in|std::ios::binary );
	if(arc.IsReady())
	{
		uint32_t tmp = arc.getu32();
		uint32_t tmp2 = arc.getu32();
		if(tmp == 0 && tmp2 == 0)
		{
			has_header = true;
			arc.read((char*)&(ah), sizeof(Arc_Header));

			if(ah.version.u8[0] == 42)
				arc.read((char*)&(ah2), sizeof(Arc_Header2));
		} else if(tmp == 0)
		{
			MSG("File seems to contain no data", "File error");
			return;
		} else
		{
			arc.seek(0, BFR::BEGIN);
		}

		if(!arc.good())
		{
			MSG("File error #1: Could not read headers of the selected file.\r\n\r\nWhat does this mean: Possibly you selected either a non-rct3 file or the park might be corrupted. It is also possible it's just a bug in the archive reader.", "File error");
			return;
		}
		//MessageBox::Show("Reading headers");
		as.variable_declaration_startpos = arc.getpos();
		uint32_t classcount = arc.getu32();

		for(uint32_t i = 0; i < classcount; i++)
		{
			//MessageBox::Show("Header #" + i);
			Class_Prototype new_prototype;
			new_prototype.name = arc.getstring16();
			tmp = arc.getu32();
			for(uint32_t j = 0; j < tmp; j++)
			{
				//MessageBox::Show("Var #" + j);
				Variable_Dec var_dec;
				if(ReadVariabledeclaration(var_dec) == false)
				{
					MSG("File error #2: The file could not be read correctly\r\n\r\nWhat does this mean: Most likely this error is caused by either a corrupted parkfile or a bug in the archive reader. If the park also doesn't open open RCT3 the park is likely to be permanently corrupted.", "Code error");
					return;
				}
				new_prototype.vars.push_back(var_dec);
			}

			class_prototype.push_back(new_prototype);
		}

		as.variable_declaration_endpos = arc.getpos();

		uint32_t datacount = arc.getu32();
		//MessageBox::Show("Reading data");		

		for(uint32_t i = 0; i < datacount; i++)
		{
			Class_Data_Block new_data;
			new_data.start = arc.getpos();
			new_data.prototype_of = arc.getu32();
			new_data.uid.a = arc.getu32();
			new_data.uid.b = arc.getu32();
			//System::Diagnostics::Debug::WriteLine(new_data.prototype_of + "|" + class_prototype.size());
			assert(new_data.prototype_of <= class_prototype.size());

			for(uint32_t j = 0; j < class_prototype[new_data.prototype_of].vars.size(); j++)
			{
				//Variable_Data new_actual_data;
				if(ReadVariabledata(class_prototype[new_data.prototype_of].vars[j], new_data.RAM) == false)
				{
					MSG("File error #3: The file could not be read correctly\r\n\r\nWhat does this mean: Most likely this error is caused by either a corrupted parkfile or a bug in the archive reader. If the park also doesn't open open RCT3 the park is likely to be corrupted.", "Code error");
					return;
				}
				//new_data.data.push_back(new_actual_data);
			}

			new_data.end = arc.getpos();
			class_data.push_back(new_data);
		}

		if(!(arc.getpos() == arc.getsize()) && !has_header)		
			T_MSG("No header EOF size = " + (arc.getsize() - arc.getpos()), "File error");
		else if(4 + arc.getpos() > arc.getsize() && has_header)
			MSG("Header EOF size = " + (arc.getsize() - arc.getpos()), "File error");


		valid = true;
	}
}


R3AReader::~R3AReader(void)
{
	//arc.ClearBuffer();
}

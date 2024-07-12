#pragma once
#include <cliext\vector>

using namespace System;
using namespace System::Collections::Generic;

#define R3PATH_VERSION "1.2"

ref class R3Path
{
private:
	String^ r3path;
	List<String^>^ folders;
	String^ currentfilename;
	String^ extension;
public:
	R3Path(void);

	void SetExtension(String^ ext)
	{
		extension = ext;
	}

	void SetR3Path(String^ r3)
	{
		r3path = r3;
	}

	void AddPath(String^ pth)
	{
		AddPath(pth, false);
	}

	void AddPath(String^ pth, bool isfile)
	{
		currentfilename = "";
		cli::array<Char>^chars = {'/','\\','\\\\','//'};
		cli::array<String^>^newfolders = pth->Split( chars );
		for(int i = 0; i < newfolders->Length; i++)
		{
			if(newfolders[i] == "..")
			{
				if(folders->Count > 0)
					folders->RemoveAt(folders->Count-1);
			} else
			{
				if(isfile)
				{
					if(i != newfolders->Length - 1)
						folders->Add(newfolders[i]);
					else
						currentfilename = newfolders[i];
				} else
					folders->Add(newfolders[i]);
			}
		}
	}

	String^ Build()
	{
		String^ ret = "";
		ret += r3path + "/";
		for(int i = 0; i < folders->Count; i++)
			ret += folders[i] + "/";
		if(currentfilename != "")
			ret += currentfilename + extension;
		return ret;
	}

	String^ BuildWithoutBase()
	{
		String^ ret = "";
		for(int i = 0; i < folders->Count; i++)
			ret += folders[i] + "/";
		if(currentfilename != "")
			ret += currentfilename + extension;
		return ret;
	}
};


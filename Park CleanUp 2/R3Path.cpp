#include "stdafx.h"
#include "R3Path.h"


R3Path::R3Path(void)
{
	folders = gcnew List<String^>();
	r3path = "";
	currentfilename = "";
	extension = ".common.ovl";
}

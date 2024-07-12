#include "stdafx.h"
#include "R3FileDialog.h"

#include "Form1.h"


namespace ParkCleanUp2
{
	System::ComponentModel::ComponentResourceManager^ R3FileDialog::GetResourceManager()
	{
		System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
		return resources;
	}
}
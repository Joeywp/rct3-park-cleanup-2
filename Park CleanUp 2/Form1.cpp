#include "stdafx.h"

#include "Form1.h"
#include "TrackEdit.h"
#include "RideEdit.h"
#include "StallEdit.h"
#include "Tutorial.h"
#include "ParkAnalyser.h"

//using namespace ParkCleanUp2;

namespace ParkCleanUp2
{
	R3AReader r3arg;
	//Speed optimization
	vector<Class_Data_Block> cdb;
	vector<Class_Prototype> class_prototype;
	vector<ParentedReference> parented_refs;
	vector<bool> list_checkeditems;
	vector<Reference> TrackUIDs;
	vector<Reference> RideUIDS;
	vector<Reference> StallUIDS;
	vector<BlueprintFile> files;

	uint32_t selectmode = SM::DEFAULT;
	string sm_select_name = "";
	unsigned long dat_preview_position = 0;
	bool defrag = false;
	bool aggressiveCompilation = false;
	TimeConverter time = TimeConverter(-1);
	float timeDeltaPerTick = 0;
	HWND rct;
	DWORD rctBaseAddress = -1;
	int currentChannel = PCU_Channels::DEFAULT;

	string allowedextensions = "Park file|*.dat|Park backup file|*.bak|CarParts file|carparts.pc|Fireworks Display|*.fwd|Firework Effects|*.frw|PlayerFile|*.prf|LaserWriting|*.lwr|LaserEffect|*.las|FileCache|*.fc|WaterJet Description|*.wjt|All|*.*";

	vector<ErrorLog> el;
	bool fastClean = false;
	string currentFileName = "";

	bool pcuReady = false;

	System::Void Form1::ShowTutorial()
	{
		if(AppBusy())
		{
			MessageBox::Show("Please wait until the current task is done!");
			return;
		}
		Tutorial^ pa = gcnew Tutorial();
		pa->ShowDialog();
	}

	System::Void Form1::edit_track(System::Object^  sender, System::EventArgs^  e)
	{
		//MsgExt("I have disabled this function currently as I didn't got it to function properly. Might be enabled again in the future!", "Disabled function", MB_OK);
		//return;
		if(track_list->SelectedIndex  == -1)
			return;
		sr3arg = r3arg;
		strackref = TrackUIDs[track_list->SelectedIndex];
		TrackEdit^ tr = gcnew TrackEdit();
		tr->ShowDialog();
		if(tr->succesfull)
		{
			for(int i = 0; i < tr->scenery_ids.Count; i++)
			{
				if(tr->scenery_ids[i] >= 0)
					List_CheckItem(tr->scenery_ids[i], true);
				debugbox->Text += "Track item remove: " + tr->scenery_ids[i] + Environment::NewLine;
			}
		}
		RecalculateFilesize();
	}

	System::Void Form1::ride_edit(System::Object^  sender, System::EventArgs^  e)
	{
		if(ride_list->SelectedIndex  == -1)
			return;
		sr3arg = r3arg;
		strackref = RideUIDS[ride_list->SelectedIndex];
		RideEdit^ re = gcnew RideEdit();
		re->ShowDialog();
		if(re->succesfull)
		{
			for(int i = 0; i < re->scenery_ids.Count; i++)
			{
				if(re->scenery_ids[i] >= 0)
					List_CheckItem(re->scenery_ids[i], true);
				debugbox->Text += "Ride item remove: " + re->scenery_ids[i] + Environment::NewLine;
			}
		}
		RecalculateFilesize();
	}

	System::Void Form1::stall_edit(System::Object^  sender, System::EventArgs^  e)
	{
		if(stall_list->SelectedIndex  == -1)
			return;
		sr3arg = r3arg;
		strackref = StallUIDS[stall_list->SelectedIndex];
		StallEdit^ se = gcnew StallEdit();
		se->ShowDialog();
		if(se->succesfull)
		{
			for(int i = 0; i < se->scenery_ids.Count; i++)
			{
				if(se->scenery_ids[i] >= 0)
					List_CheckItem(se->scenery_ids[i], true);
				debugbox->Text += "Stall removed: " + se->scenery_ids[i] + Environment::NewLine;
			}
		}
		RecalculateFilesize();
	}

	System::Void Form1::button2_Click(System::Object^  sender, System::EventArgs^  e) {
		if(AppBusy())
		{
			MessageBox::Show("Please wait until the current task is done!");
			return;
		}
		if(!r3arg.IsValid())
		{
			MessageBox::Show("Open a parkfile first!");
			return;
		}
		ParkAnalyser^ pa = gcnew ParkAnalyser();
		pa->ShowDialog();
	}
}

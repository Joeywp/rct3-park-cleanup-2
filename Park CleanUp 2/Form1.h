#pragma once

/*
TODO:
- List of missing references (also errors: Style/themed/test.ovl is missing a dependencie!)
- JS blocks
- Shell extension: get CSO list, clean parkfile, link with .dat files

MAYBE:
- Charts of blocks (SIDDatabase entry etc)

*/

#include <Windows.h>
#include <fstream>
#include <omp.h>
#include <string>
#include <vcclr.h>
#include <math.h>
#include <Shlwapi.h>

#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <TlHelp32.h>

#include <iostream>
#include <chrono>
#include <ctime>

#include "R3AReader.h"
#include "AdditionalStructs.h"
#include "R3Path.h"
#include "OVLReader.h"

#include "R3FileDialog.h"

#include "shared.h"
#include "SHA1.h"

using namespace std;
using namespace Ionic::Zip;
using namespace System::Text;

#define NL "<br/>"

#define OPTIONS_RCT3PATH "rct3path"
#define OPTIONS_SHOW_WARNING "hide_warning"
#define OPTIONS_ADVANCED_ALGORITHM "advanced_algorithm"
#define OPTIONS_SHOW_TUTORIAL "hide_tutorial"
#define OPTIONS_UPDATE "autoupdatecheck"
#define OPTIONS_CHANNEL "channel"
#define OPTIONS_SHOW_DEVWARNING "hide_devwarning"
#define OPTIONS_AGGRESSIVE_COMPILATION "aggressive_compilation"

#define PCU_VERSION_MAJOR 2
#define PCU_VERSION_MINOR 1
#define PCU_VERSION_REVISION 1
#define PCU_VERSION PCU_VERSION_MAJOR + "." + PCU_VERSION_MINOR + "." + PCU_VERSION_REVISION

#define PCU_DEV_BUILD 3

namespace ParkCleanUp2 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	using namespace System::Threading;
	using namespace System::Threading::Tasks;

	using namespace System::IO;
	using namespace System::Net;
	using namespace System::Security::Cryptography;
	using namespace System::Reflection;

	extern R3AReader r3arg;
	//Speed optimization
	extern vector<Class_Data_Block> cdb;
	extern vector<Class_Prototype> class_prototype;
	extern vector<ParentedReference> parented_refs;
	extern vector<bool> list_checkeditems;
	extern vector<Reference> TrackUIDs;
	extern vector<Reference> RideUIDS;
	extern vector<Reference> StallUIDS;
	extern vector<BlueprintFile> files;

	extern uint32_t selectmode;
	extern string sm_select_name;
	extern unsigned long dat_preview_position;
	extern bool defrag;
	extern bool aggressiveCompilation;

	extern string allowedextensions;
	extern TimeConverter time;
	extern float timeDeltaPerTick;
	//extern HWND rct;
	extern DWORD rctBaseAddress;

	extern vector<ErrorLog> el;
	extern bool fastClean;
	extern string currentFileName;

	extern bool pcuReady;
	extern int currentChannel;

	delegate void d_addGraphData(String^ name, int count, int size);

#pragma region Formzooi
	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			clb_classes->Invalidate();
			clb_classes->Update();
			this->Text = "Park CleanUp v" + PCU_VERSION;
			date_label->Text = "Last update: " +  __DATE__;

			if(File::Exists("ParkCleanUpdater.exe"))
				File::Delete("ParkCleanUpdater.exe");
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}

	protected: 



	private: System::Windows::Forms::StatusStrip^  statusStrip1;
	private: System::Windows::Forms::ToolStripStatusLabel^  sl_appstatus;
	private: System::Windows::Forms::ToolStripProgressBar^  pb_status;
	private: System::ComponentModel::BackgroundWorker^  unused_item_selector;
	private: System::ComponentModel::BackgroundWorker^  park_initialiser;
	private: System::Windows::Forms::PictureBox^  loading;


	private: wyDay::Controls::SplitButton^  btn_select_unused_items;
	private: System::Windows::Forms::ContextMenuStrip^  ms_unuseditems;
	private: System::Windows::Forms::Button^  btn_deselect_items;
	private: System::Windows::Forms::ToolStripMenuItem^  sl_gui2datdesc_1;
	private: System::Windows::Forms::ToolStripMenuItem^  sl_FlexiColourGroupCache_1;
	private: System::Windows::Forms::ToolStripMenuItem^  sl_blender_1;
	private: System::Windows::Forms::ToolStripMenuItem^  sl_firework_1;
	private: wyDay::Controls::SplitButton^  btn_save;
	private: System::Windows::Forms::ContextMenuStrip^  save_menu;
	private: System::Windows::Forms::ToolStripMenuItem^  sf_defragged;
	private: System::Windows::Forms::ToolStripMenuItem^  sl_missingcustomcontent_1;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
	private: System::Windows::Forms::ContextMenuStrip^  ms_parkfile_ovlscan;
	private: System::Windows::Forms::ToolStripMenuItem^  scanForOVLFilesThatAreUsedButcurrentlyNotInstalledToolStripMenuItem;

	private: System::Windows::Forms::Button^  debug_dump;
	private: System::Windows::Forms::ToolStripMenuItem^  zIPCompressedParkfileWithAllRequiredDependenciesToolStripMenuItem;
	private: System::Windows::Forms::TabPage^  tab_about;
	private: System::Windows::Forms::TextBox^  credits;
	private: System::Windows::Forms::TabPage^  tab_missing_customcontent;
	private: System::Windows::Forms::Button^  scanlist_copy;
	private: wyDay::Controls::SplitButton^  splitButton1;
	private: System::Windows::Forms::ListBox^  content_listbox;
	private: System::Windows::Forms::TabPage^  tab_opties;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  btn_rct3locbrowser;
	private: System::Windows::Forms::TextBox^  tb_rct3loc;
	private: System::Windows::Forms::CheckBox^  cb_advanced_algorithm;
	private: System::Windows::Forms::TabPage^  tab_cl;
	private: System::Windows::Forms::Label^  lbl_classdata2;
	private: System::Windows::Forms::CheckedListBox^  clb_classes;
	private: System::Windows::Forms::TabPage^  tab_park;

	private: System::Windows::Forms::Label^  lbl_classdata1;
	private: System::Windows::Forms::Label^  tb_parkinfo;
	private: System::Windows::Forms::PictureBox^  park_prev;
	private: System::Windows::Forms::TabControl^  tabctrl;
	private: System::Windows::Forms::TabPage^  tab_debug;

	private: System::Windows::Forms::Label^  label4;
	private: System::ComponentModel::BackgroundWorker^  park_zipper;
	private: System::Windows::Forms::ListBox^  track_list;







	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::TabControl^  tabctrlhidden;
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::TabPage^  tabPage3;

	private: System::Windows::Forms::CheckBox^  cb_deep_check;
	private: System::Windows::Forms::TextBox^  debugbox;

	private: System::ComponentModel::BackgroundWorker^  filescan;
	private: System::Windows::Forms::Label^  content_info;
	private: System::Windows::Forms::ToolStripMenuItem^  cleanUpGuestsmightHaveUnwantedResultsIngameToolStripMenuItem;
	private: System::Windows::Forms::Button^  btn_force_update;
	private: System::Windows::Forms::Timer^  errorExecutor;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::ToolStripMenuItem^  cleanUpDucksToolStripMenuItem;
	private: System::Windows::Forms::TabPage^  tab_live;


	private: System::Windows::Forms::Timer^  timechanger;
	private: System::Windows::Forms::TrackBar^  timeDelta;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::CheckBox^  timeEnabler;

	private: System::IO::FileSystemWatcher^  continuousParkWatcher;
	private: System::Windows::Forms::CheckBox^  cb_AutoCleanUp;
	private: System::Windows::Forms::Button^  button5;
	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::CheckBox^  cb_update;

	private: System::Windows::Forms::Label^  date_label;
	private: System::Windows::Forms::Button^  button7;
	private: wyDay::Controls::SplitButton^  splitButton2;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::ToolStripMenuItem^  pluginsToolStripMenuItem;
	private: System::Windows::Forms::CheckBox^  cb_aggressive;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::ComboBox^  dd_operatingmode;
	private: System::Windows::Forms::Button^  btn_time_hoursmin1;

	private: System::Windows::Forms::TabPage^  tab_removal;



	private: System::Windows::Forms::ListBox^  ride_list;

	private: System::Windows::Forms::Label^  label3;

	private: System::Windows::Forms::ListBox^  stall_list;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  tabPage2;
	private: System::Windows::Forms::TabPage^  tabPage4;
	private: System::Windows::Forms::TabPage^  tabPage5;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Button^  btn_time_hoursplus1;

	private: System::Windows::Forms::Button^  btn_time_minutesplus1;

	private: System::Windows::Forms::Button^  btn_time_minutemin1;

	private: System::ComponentModel::BackgroundWorker^  blueprint_creator;
	private: System::Windows::Forms::ContextMenuStrip^  zip_menu;
	private: System::Windows::Forms::ToolStripMenuItem^  createBlueprintToolStripMenuItem;
	private: System::Windows::Forms::Label^  lbl_debugdata;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^  classgraph;

	private: System::ComponentModel::BackgroundWorker^  graphDrawer;
private: System::Windows::Forms::Label^  label9;









	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^  legend1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			this->ms_parkfile_ovlscan = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->scanForOVLFilesThatAreUsedButcurrentlyNotInstalledToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->debug_dump = (gcnew System::Windows::Forms::Button());
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->pb_status = (gcnew System::Windows::Forms::ToolStripProgressBar());
			this->sl_appstatus = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->unused_item_selector = (gcnew System::ComponentModel::BackgroundWorker());
			this->park_initialiser = (gcnew System::ComponentModel::BackgroundWorker());
			this->loading = (gcnew System::Windows::Forms::PictureBox());
			this->btn_select_unused_items = (gcnew wyDay::Controls::SplitButton());
			this->ms_unuseditems = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->sl_missingcustomcontent_1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->cleanUpGuestsmightHaveUnwantedResultsIngameToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->sl_FlexiColourGroupCache_1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->sl_gui2datdesc_1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->sl_blender_1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->sl_firework_1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cleanUpDucksToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->pluginsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->btn_deselect_items = (gcnew System::Windows::Forms::Button());
			this->btn_save = (gcnew wyDay::Controls::SplitButton());
			this->save_menu = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->sf_defragged = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->zIPCompressedParkfileWithAllRequiredDependenciesToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tab_about = (gcnew System::Windows::Forms::TabPage());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->credits = (gcnew System::Windows::Forms::TextBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->btn_force_update = (gcnew System::Windows::Forms::Button());
			this->tab_missing_customcontent = (gcnew System::Windows::Forms::TabPage());
			this->content_info = (gcnew System::Windows::Forms::Label());
			this->cb_deep_check = (gcnew System::Windows::Forms::CheckBox());
			this->content_listbox = (gcnew System::Windows::Forms::ListBox());
			this->splitButton1 = (gcnew wyDay::Controls::SplitButton());
			this->scanlist_copy = (gcnew System::Windows::Forms::Button());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->track_list = (gcnew System::Windows::Forms::ListBox());
			this->tab_opties = (gcnew System::Windows::Forms::TabPage());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->dd_operatingmode = (gcnew System::Windows::Forms::ComboBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->cb_aggressive = (gcnew System::Windows::Forms::CheckBox());
			this->cb_update = (gcnew System::Windows::Forms::CheckBox());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->cb_AutoCleanUp = (gcnew System::Windows::Forms::CheckBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->btn_rct3locbrowser = (gcnew System::Windows::Forms::Button());
			this->tb_rct3loc = (gcnew System::Windows::Forms::TextBox());
			this->cb_advanced_algorithm = (gcnew System::Windows::Forms::CheckBox());
			this->tab_cl = (gcnew System::Windows::Forms::TabPage());
			this->lbl_classdata2 = (gcnew System::Windows::Forms::Label());
			this->clb_classes = (gcnew System::Windows::Forms::CheckedListBox());
			this->tab_park = (gcnew System::Windows::Forms::TabPage());
			this->classgraph = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->lbl_debugdata = (gcnew System::Windows::Forms::Label());
			this->lbl_classdata1 = (gcnew System::Windows::Forms::Label());
			this->tb_parkinfo = (gcnew System::Windows::Forms::Label());
			this->park_prev = (gcnew System::Windows::Forms::PictureBox());
			this->tabctrlhidden = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->tabPage3 = (gcnew System::Windows::Forms::TabPage());
			this->tabctrl = (gcnew System::Windows::Forms::TabControl());
			this->tab_removal = (gcnew System::Windows::Forms::TabPage());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->ride_list = (gcnew System::Windows::Forms::ListBox());
			this->tabPage4 = (gcnew System::Windows::Forms::TabPage());
			this->tabPage5 = (gcnew System::Windows::Forms::TabPage());
			this->stall_list = (gcnew System::Windows::Forms::ListBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->tab_debug = (gcnew System::Windows::Forms::TabPage());
			this->debugbox = (gcnew System::Windows::Forms::TextBox());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->tab_live = (gcnew System::Windows::Forms::TabPage());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->btn_time_hoursplus1 = (gcnew System::Windows::Forms::Button());
			this->btn_time_minutesplus1 = (gcnew System::Windows::Forms::Button());
			this->btn_time_minutemin1 = (gcnew System::Windows::Forms::Button());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->button7 = (gcnew System::Windows::Forms::Button());
			this->btn_time_hoursmin1 = (gcnew System::Windows::Forms::Button());
			this->timeEnabler = (gcnew System::Windows::Forms::CheckBox());
			this->timeDelta = (gcnew System::Windows::Forms::TrackBar());
			this->park_zipper = (gcnew System::ComponentModel::BackgroundWorker());
			this->filescan = (gcnew System::ComponentModel::BackgroundWorker());
			this->errorExecutor = (gcnew System::Windows::Forms::Timer(this->components));
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->timechanger = (gcnew System::Windows::Forms::Timer(this->components));
			this->continuousParkWatcher = (gcnew System::IO::FileSystemWatcher());
			this->date_label = (gcnew System::Windows::Forms::Label());
			this->splitButton2 = (gcnew wyDay::Controls::SplitButton());
			this->zip_menu = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->createBlueprintToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->blueprint_creator = (gcnew System::ComponentModel::BackgroundWorker());
			this->graphDrawer = (gcnew System::ComponentModel::BackgroundWorker());
			this->ms_parkfile_ovlscan->SuspendLayout();
			this->statusStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->loading))->BeginInit();
			this->ms_unuseditems->SuspendLayout();
			this->save_menu->SuspendLayout();
			this->tab_about->SuspendLayout();
			this->tab_missing_customcontent->SuspendLayout();
			this->tab_opties->SuspendLayout();
			this->tab_cl->SuspendLayout();
			this->tab_park->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->classgraph))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->park_prev))->BeginInit();
			this->tabctrlhidden->SuspendLayout();
			this->tabctrl->SuspendLayout();
			this->tab_removal->SuspendLayout();
			this->tabControl1->SuspendLayout();
			this->tabPage2->SuspendLayout();
			this->tabPage4->SuspendLayout();
			this->tabPage5->SuspendLayout();
			this->tab_debug->SuspendLayout();
			this->tab_live->SuspendLayout();
			this->groupBox1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->timeDelta))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->continuousParkWatcher))->BeginInit();
			this->zip_menu->SuspendLayout();
			this->SuspendLayout();
			// 
			// ms_parkfile_ovlscan
			// 
			this->ms_parkfile_ovlscan->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->scanForOVLFilesThatAreUsedButcurrentlyNotInstalledToolStripMenuItem });
			this->ms_parkfile_ovlscan->Name = L"ms_parkfile_ovlscan";
			this->ms_parkfile_ovlscan->Size = System::Drawing::Size(275, 26);
			// 
			// scanForOVLFilesThatAreUsedButcurrentlyNotInstalledToolStripMenuItem
			// 
			this->scanForOVLFilesThatAreUsedButcurrentlyNotInstalledToolStripMenuItem->Name = L"scanForOVLFilesThatAreUsedButcurrentlyNotInstalledToolStripMenuItem";
			this->scanForOVLFilesThatAreUsedButcurrentlyNotInstalledToolStripMenuItem->Size = System::Drawing::Size(274, 22);
			this->scanForOVLFilesThatAreUsedButcurrentlyNotInstalledToolStripMenuItem->Text = L"Create list of required custom content";
			this->scanForOVLFilesThatAreUsedButcurrentlyNotInstalledToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::scanForOVLFilesThatAreUsedButcurrentlyNotInstalledToolStripMenuItem_Click);
			// 
			// debug_dump
			// 
			this->debug_dump->Location = System::Drawing::Point(6, 6);
			this->debug_dump->Name = L"debug_dump";
			this->debug_dump->Size = System::Drawing::Size(167, 23);
			this->debug_dump->TabIndex = 0;
			this->debug_dump->Text = L"Create a debug-dump";
			this->debug_dump->UseVisualStyleBackColor = true;
			this->debug_dump->Click += gcnew System::EventHandler(this, &Form1::debug_dump_Click);
			// 
			// statusStrip1
			// 
			this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) { this->pb_status, this->sl_appstatus });
			this->statusStrip1->Location = System::Drawing::Point(0, 299);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->Size = System::Drawing::Size(534, 22);
			this->statusStrip1->TabIndex = 4;
			this->statusStrip1->Text = L"statusStrip1";
			// 
			// pb_status
			// 
			this->pb_status->Name = L"pb_status";
			this->pb_status->Size = System::Drawing::Size(150, 16);
			this->pb_status->Visible = false;
			// 
			// sl_appstatus
			// 
			this->sl_appstatus->Name = L"sl_appstatus";
			this->sl_appstatus->Size = System::Drawing::Size(10, 17);
			this->sl_appstatus->Text = L" ";
			// 
			// unused_item_selector
			// 
			this->unused_item_selector->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &Form1::unused_item_selector_DoWork);
			// 
			// park_initialiser
			// 
			this->park_initialiser->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &Form1::park_initialiser_DoWork);
			// 
			// loading
			// 
			this->loading->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->loading->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"loading.Image")));
			this->loading->Location = System::Drawing::Point(301, 301);
			this->loading->Name = L"loading";
			this->loading->Size = System::Drawing::Size(217, 20);
			this->loading->TabIndex = 5;
			this->loading->TabStop = false;
			this->loading->Visible = false;
			// 
			// btn_select_unused_items
			// 
			this->btn_select_unused_items->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->btn_select_unused_items->AutoSize = true;
			this->btn_select_unused_items->ContextMenuStrip = this->ms_unuseditems;
			this->btn_select_unused_items->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F));
			this->btn_select_unused_items->Location = System::Drawing::Point(214, 267);
			this->btn_select_unused_items->Name = L"btn_select_unused_items";
			this->btn_select_unused_items->Size = System::Drawing::Size(154, 28);
			this->btn_select_unused_items->SplitMenuStrip = this->ms_unuseditems;
			this->btn_select_unused_items->TabIndex = 3;
			this->btn_select_unused_items->Text = L"CleanUp the parkfile";
			this->btn_select_unused_items->UseVisualStyleBackColor = true;
			this->btn_select_unused_items->Click += gcnew System::EventHandler(this, &Form1::btn_select_unused_items_Click);
			// 
			// ms_unuseditems
			// 
			this->ms_unuseditems->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(10) {
				this->sl_missingcustomcontent_1,
					this->toolStripSeparator2, this->cleanUpGuestsmightHaveUnwantedResultsIngameToolStripMenuItem, this->sl_FlexiColourGroupCache_1,
					this->sl_gui2datdesc_1, this->sl_blender_1, this->sl_firework_1, this->cleanUpDucksToolStripMenuItem, this->toolStripSeparator1,
					this->pluginsToolStripMenuItem
			});
			this->ms_unuseditems->Name = L"ms_unuseditems";
			this->ms_unuseditems->RenderMode = System::Windows::Forms::ToolStripRenderMode::System;
			this->ms_unuseditems->Size = System::Drawing::Size(406, 192);
			// 
			// sl_missingcustomcontent_1
			// 
			this->sl_missingcustomcontent_1->Enabled = false;
			this->sl_missingcustomcontent_1->Name = L"sl_missingcustomcontent_1";
			this->sl_missingcustomcontent_1->Size = System::Drawing::Size(405, 22);
			this->sl_missingcustomcontent_1->Text = L"CleanUp missing custom content";
			this->sl_missingcustomcontent_1->Visible = false;
			this->sl_missingcustomcontent_1->Click += gcnew System::EventHandler(this, &Form1::sl_missingcustomcontent_1_Click);
			// 
			// toolStripSeparator2
			// 
			this->toolStripSeparator2->Name = L"toolStripSeparator2";
			this->toolStripSeparator2->Size = System::Drawing::Size(402, 6);
			this->toolStripSeparator2->Visible = false;
			// 
			// cleanUpGuestsmightHaveUnwantedResultsIngameToolStripMenuItem
			// 
			this->cleanUpGuestsmightHaveUnwantedResultsIngameToolStripMenuItem->Name = L"cleanUpGuestsmightHaveUnwantedResultsIngameToolStripMenuItem";
			this->cleanUpGuestsmightHaveUnwantedResultsIngameToolStripMenuItem->Size = System::Drawing::Size(405, 22);
			this->cleanUpGuestsmightHaveUnwantedResultsIngameToolStripMenuItem->Text = L"CleanUp Guests (might have unwanted results ingame!)";
			this->cleanUpGuestsmightHaveUnwantedResultsIngameToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::cleanUpGuestsmightHaveUnwantedResultsIngameToolStripMenuItem_Click);
			// 
			// sl_FlexiColourGroupCache_1
			// 
			this->sl_FlexiColourGroupCache_1->Enabled = false;
			this->sl_FlexiColourGroupCache_1->Name = L"sl_FlexiColourGroupCache_1";
			this->sl_FlexiColourGroupCache_1->Size = System::Drawing::Size(405, 22);
			this->sl_FlexiColourGroupCache_1->Text = L"CleanUp FlexiColourGroupCache (unknown ingame effect)";
			this->sl_FlexiColourGroupCache_1->Visible = false;
			this->sl_FlexiColourGroupCache_1->Click += gcnew System::EventHandler(this, &Form1::sl_FlexiColourGroupCache_1_Click);
			// 
			// sl_gui2datdesc_1
			// 
			this->sl_gui2datdesc_1->Name = L"sl_gui2datdesc_1";
			this->sl_gui2datdesc_1->Size = System::Drawing::Size(405, 22);
			this->sl_gui2datdesc_1->Text = L"CleanUp GUI2_DatDesc (might have unwanted results ingame!)";
			this->sl_gui2datdesc_1->Click += gcnew System::EventHandler(this, &Form1::sl_gui2datdesc_1_Click);
			// 
			// sl_blender_1
			// 
			this->sl_blender_1->Name = L"sl_blender_1";
			this->sl_blender_1->Size = System::Drawing::Size(405, 22);
			this->sl_blender_1->Text = L"CleanUp Blender (unknown ingame effect)";
			this->sl_blender_1->Click += gcnew System::EventHandler(this, &Form1::sl_blender_1_Click);
			// 
			// sl_firework_1
			// 
			this->sl_firework_1->Name = L"sl_firework_1";
			this->sl_firework_1->Size = System::Drawing::Size(405, 22);
			this->sl_firework_1->Text = L"CleanUp Firework related items";
			this->sl_firework_1->Click += gcnew System::EventHandler(this, &Form1::sl_firework_1_Click);
			// 
			// cleanUpDucksToolStripMenuItem
			// 
			this->cleanUpDucksToolStripMenuItem->Name = L"cleanUpDucksToolStripMenuItem";
			this->cleanUpDucksToolStripMenuItem->Size = System::Drawing::Size(405, 22);
			this->cleanUpDucksToolStripMenuItem->Text = L"CleanUp Ducks";
			this->cleanUpDucksToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::cleanUpDucksToolStripMenuItem_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(402, 6);
			// 
			// pluginsToolStripMenuItem
			// 
			this->pluginsToolStripMenuItem->Name = L"pluginsToolStripMenuItem";
			this->pluginsToolStripMenuItem->Size = System::Drawing::Size(405, 22);
			this->pluginsToolStripMenuItem->Text = L"Custom";
			// 
			// btn_deselect_items
			// 
			this->btn_deselect_items->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->btn_deselect_items->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F));
			this->btn_deselect_items->Location = System::Drawing::Point(374, 267);
			this->btn_deselect_items->Name = L"btn_deselect_items";
			this->btn_deselect_items->Size = System::Drawing::Size(144, 28);
			this->btn_deselect_items->TabIndex = 7;
			this->btn_deselect_items->Text = L"Deselect all items";
			this->btn_deselect_items->UseVisualStyleBackColor = true;
			this->btn_deselect_items->Click += gcnew System::EventHandler(this, &Form1::btn_deselect_items_Click);
			// 
			// btn_save
			// 
			this->btn_save->AutoSize = true;
			this->btn_save->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F));
			this->btn_save->Location = System::Drawing::Point(114, 3);
			this->btn_save->Name = L"btn_save";
			this->btn_save->Size = System::Drawing::Size(109, 28);
			this->btn_save->TabIndex = 8;
			this->btn_save->Text = L"Save park as...";
			this->btn_save->UseVisualStyleBackColor = true;
			this->btn_save->Click += gcnew System::EventHandler(this, &Form1::btn_save_Click);
			// 
			// save_menu
			// 
			this->save_menu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) { this->sf_defragged, this->zIPCompressedParkfileWithAllRequiredDependenciesToolStripMenuItem });
			this->save_menu->Name = L"contextMenuStrip1";
			this->save_menu->Size = System::Drawing::Size(615, 48);
			// 
			// sf_defragged
			// 
			this->sf_defragged->Enabled = false;
			this->sf_defragged->Name = L"sf_defragged";
			this->sf_defragged->ShortcutKeys = static_cast<System::Windows::Forms::Keys>(((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::Shift)
				| System::Windows::Forms::Keys::S));
			this->sf_defragged->Size = System::Drawing::Size(614, 22);
			this->sf_defragged->Text = L"[EXPERIMENTAL] Save defragged parkfile as.. (DO NOT USE IN PRODUCTION PARKFILES!)"
				L"";
			this->sf_defragged->Click += gcnew System::EventHandler(this, &Form1::sf_defragged_Click);
			// 
			// zIPCompressedParkfileWithAllRequiredDependenciesToolStripMenuItem
			// 
			this->zIPCompressedParkfileWithAllRequiredDependenciesToolStripMenuItem->Name = L"zIPCompressedParkfileWithAllRequiredDependenciesToolStripMenuItem";
			this->zIPCompressedParkfileWithAllRequiredDependenciesToolStripMenuItem->Size = System::Drawing::Size(614, 22);
			this->zIPCompressedParkfileWithAllRequiredDependenciesToolStripMenuItem->Text = L"[WIP] Create a ZIP-file containing all required files in order to run this file o"
				L"n another PC";
			this->zIPCompressedParkfileWithAllRequiredDependenciesToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::zIPCompressedParkfileWithAllRequiredDependenciesToolStripMenuItem_Click);
			// 
			// tab_about
			// 
			this->tab_about->Controls->Add(this->button4);
			this->tab_about->Controls->Add(this->credits);
			this->tab_about->Controls->Add(this->button1);
			this->tab_about->Controls->Add(this->btn_force_update);
			this->tab_about->Location = System::Drawing::Point(4, 22);
			this->tab_about->Name = L"tab_about";
			this->tab_about->Padding = System::Windows::Forms::Padding(3);
			this->tab_about->Size = System::Drawing::Size(502, 202);
			this->tab_about->TabIndex = 5;
			this->tab_about->Text = L"About";
			this->tab_about->UseVisualStyleBackColor = true;
			// 
			// button4
			// 
			this->button4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->button4->Location = System::Drawing::Point(353, 6);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(143, 22);
			this->button4->TabIndex = 6;
			this->button4->Text = L"Park CleanUp tutorial";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &Form1::button4_Click);
			// 
			// credits
			// 
			this->credits->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->credits->Location = System::Drawing::Point(6, 34);
			this->credits->Multiline = true;
			this->credits->Name = L"credits";
			this->credits->ReadOnly = true;
			this->credits->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->credits->Size = System::Drawing::Size(490, 162);
			this->credits->TabIndex = 2;
			this->credits->Text = resources->GetString(L"credits.Text");
			this->credits->TextChanged += gcnew System::EventHandler(this, &Form1::credits_TextChanged);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(6, 6);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(116, 22);
			this->button1->TabIndex = 4;
			this->button1->Text = L"Check for updates";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click_2);
			// 
			// btn_force_update
			// 
			this->btn_force_update->Location = System::Drawing::Point(128, 6);
			this->btn_force_update->Name = L"btn_force_update";
			this->btn_force_update->Size = System::Drawing::Size(110, 22);
			this->btn_force_update->TabIndex = 5;
			this->btn_force_update->Text = L"Force update";
			this->btn_force_update->UseVisualStyleBackColor = true;
			this->btn_force_update->Click += gcnew System::EventHandler(this, &Form1::btn_force_update_Click);
			// 
			// tab_missing_customcontent
			// 
			this->tab_missing_customcontent->Controls->Add(this->content_info);
			this->tab_missing_customcontent->Controls->Add(this->cb_deep_check);
			this->tab_missing_customcontent->Controls->Add(this->content_listbox);
			this->tab_missing_customcontent->Controls->Add(this->splitButton1);
			this->tab_missing_customcontent->Controls->Add(this->scanlist_copy);
			this->tab_missing_customcontent->Location = System::Drawing::Point(4, 22);
			this->tab_missing_customcontent->Name = L"tab_missing_customcontent";
			this->tab_missing_customcontent->Padding = System::Windows::Forms::Padding(3);
			this->tab_missing_customcontent->Size = System::Drawing::Size(502, 202);
			this->tab_missing_customcontent->TabIndex = 6;
			this->tab_missing_customcontent->Text = L"Content";
			this->tab_missing_customcontent->UseVisualStyleBackColor = true;
			// 
			// content_info
			// 
			this->content_info->AutoSize = true;
			this->content_info->Location = System::Drawing::Point(3, 53);
			this->content_info->Name = L"content_info";
			this->content_info->Size = System::Drawing::Size(10, 13);
			this->content_info->TabIndex = 11;
			this->content_info->Text = L" ";
			// 
			// cb_deep_check
			// 
			this->cb_deep_check->AutoSize = true;
			this->cb_deep_check->Location = System::Drawing::Point(6, 33);
			this->cb_deep_check->Name = L"cb_deep_check";
			this->cb_deep_check->Size = System::Drawing::Size(305, 17);
			this->cb_deep_check->TabIndex = 5;
			this->cb_deep_check->Text = L"Do a deep check (shows all specific files, but is slower)";
			this->cb_deep_check->UseVisualStyleBackColor = true;
			// 
			// content_listbox
			// 
			this->content_listbox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->content_listbox->FormattingEnabled = true;
			this->content_listbox->IntegralHeight = false;
			this->content_listbox->Location = System::Drawing::Point(6, 69);
			this->content_listbox->Name = L"content_listbox";
			this->content_listbox->Size = System::Drawing::Size(490, 127);
			this->content_listbox->TabIndex = 1;
			// 
			// splitButton1
			// 
			this->splitButton1->AutoSize = true;
			this->splitButton1->ContextMenuStrip = this->ms_parkfile_ovlscan;
			this->splitButton1->Location = System::Drawing::Point(6, 6);
			this->splitButton1->Name = L"splitButton1";
			this->splitButton1->Size = System::Drawing::Size(232, 25);
			this->splitButton1->SplitMenuStrip = this->ms_parkfile_ovlscan;
			this->splitButton1->TabIndex = 3;
			this->splitButton1->Text = L"Create list of missing custom content";
			this->splitButton1->UseVisualStyleBackColor = true;
			this->splitButton1->Click += gcnew System::EventHandler(this, &Form1::splitButton1_Click);
			// 
			// scanlist_copy
			// 
			this->scanlist_copy->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->scanlist_copy->Location = System::Drawing::Point(421, 43);
			this->scanlist_copy->Name = L"scanlist_copy";
			this->scanlist_copy->Size = System::Drawing::Size(75, 23);
			this->scanlist_copy->TabIndex = 4;
			this->scanlist_copy->Text = L"Copy list";
			this->scanlist_copy->UseVisualStyleBackColor = true;
			this->scanlist_copy->Click += gcnew System::EventHandler(this, &Form1::scanlist_copy_Click);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(6, 3);
			this->label4->MaximumSize = System::Drawing::Size(480, 0);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(379, 13);
			this->label4->TabIndex = 2;
			this->label4->Text = L"List of all tracks in the current parkfile (click a track for removal selection)";
			// 
			// track_list
			// 
			this->track_list->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->track_list->FormattingEnabled = true;
			this->track_list->IntegralHeight = false;
			this->track_list->Location = System::Drawing::Point(6, 19);
			this->track_list->Name = L"track_list";
			this->track_list->Size = System::Drawing::Size(469, 142);
			this->track_list->TabIndex = 6;
			this->track_list->Click += gcnew System::EventHandler(this, &Form1::edit_track);
			this->track_list->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::track_list_SelectedIndexChanged);
			// 
			// tab_opties
			// 
			this->tab_opties->Controls->Add(this->label2);
			this->tab_opties->Controls->Add(this->dd_operatingmode);
			this->tab_opties->Controls->Add(this->label8);
			this->tab_opties->Controls->Add(this->label7);
			this->tab_opties->Controls->Add(this->cb_aggressive);
			this->tab_opties->Controls->Add(this->cb_update);
			this->tab_opties->Controls->Add(this->button5);
			this->tab_opties->Controls->Add(this->cb_AutoCleanUp);
			this->tab_opties->Controls->Add(this->label1);
			this->tab_opties->Controls->Add(this->btn_rct3locbrowser);
			this->tab_opties->Controls->Add(this->tb_rct3loc);
			this->tab_opties->Controls->Add(this->cb_advanced_algorithm);
			this->tab_opties->Location = System::Drawing::Point(4, 22);
			this->tab_opties->Name = L"tab_opties";
			this->tab_opties->Padding = System::Windows::Forms::Padding(3);
			this->tab_opties->Size = System::Drawing::Size(502, 202);
			this->tab_opties->TabIndex = 1;
			this->tab_opties->Text = L"Settings";
			this->tab_opties->UseVisualStyleBackColor = true;
			// 
			// label2
			// 
			this->label2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(334, 156);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(162, 13);
			this->label2->TabIndex = 12;
			this->label2->Text = L"Park CleanUp operating mode";
			// 
			// dd_operatingmode
			// 
			this->dd_operatingmode->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->dd_operatingmode->FormattingEnabled = true;
			this->dd_operatingmode->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"Default", L"Development edition" });
			this->dd_operatingmode->Location = System::Drawing::Point(334, 172);
			this->dd_operatingmode->Name = L"dd_operatingmode";
			this->dd_operatingmode->Size = System::Drawing::Size(162, 21);
			this->dd_operatingmode->TabIndex = 11;
			this->dd_operatingmode->Text = L"Default";
			this->dd_operatingmode->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::comboBox1_SelectedIndexChanged);
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Font = (gcnew System::Drawing::Font(L"Segoe UI", 7));
			this->label8->ForeColor = System::Drawing::SystemColors::ControlDarkDark;
			this->label8->Location = System::Drawing::Point(25, 133);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(257, 12);
			this->label8->TabIndex = 13;
			this->label8->Text = L"Cleaning takes a longer time, but might give better results";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Font = (gcnew System::Drawing::Font(L"Segoe UI", 7));
			this->label7->ForeColor = System::Drawing::SystemColors::ControlDarkDark;
			this->label7->Location = System::Drawing::Point(25, 95);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(325, 12);
			this->label7->TabIndex = 13;
			this->label7->Text = L"Might stabilize RCT3, but in quite some cases it may also corrupt parkfiles";
			// 
			// cb_aggressive
			// 
			this->cb_aggressive->AutoSize = true;
			this->cb_aggressive->Location = System::Drawing::Point(6, 79);
			this->cb_aggressive->Name = L"cb_aggressive";
			this->cb_aggressive->Size = System::Drawing::Size(220, 17);
			this->cb_aggressive->TabIndex = 10;
			this->cb_aggressive->Text = L"[Experimental] Aggressive compilation";
			this->cb_aggressive->UseVisualStyleBackColor = true;
			this->cb_aggressive->CheckedChanged += gcnew System::EventHandler(this, &Form1::cb_aggressive_CheckedChanged);
			// 
			// cb_update
			// 
			this->cb_update->AutoSize = true;
			this->cb_update->Location = System::Drawing::Point(6, 52);
			this->cb_update->Name = L"cb_update";
			this->cb_update->Size = System::Drawing::Size(177, 17);
			this->cb_update->TabIndex = 8;
			this->cb_update->Text = L"Check for updates on startup";
			this->cb_update->UseVisualStyleBackColor = true;
			this->cb_update->CheckedChanged += gcnew System::EventHandler(this, &Form1::cb_update_CheckedChanged);
			// 
			// button5
			// 
			this->button5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->button5->Location = System::Drawing::Point(385, 121);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(111, 22);
			this->button5->TabIndex = 7;
			this->button5->Text = L"Reset all settings";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Click += gcnew System::EventHandler(this, &Form1::button5_Click);
			// 
			// cb_AutoCleanUp
			// 
			this->cb_AutoCleanUp->AutoSize = true;
			this->cb_AutoCleanUp->Enabled = false;
			this->cb_AutoCleanUp->Location = System::Drawing::Point(253, 176);
			this->cb_AutoCleanUp->Name = L"cb_AutoCleanUp";
			this->cb_AutoCleanUp->Size = System::Drawing::Size(125, 17);
			this->cb_AutoCleanUp->TabIndex = 0;
			this->cb_AutoCleanUp->Text = L"[AutoCleanUp_Text]";
			this->cb_AutoCleanUp->UseVisualStyleBackColor = true;
			this->cb_AutoCleanUp->Visible = false;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(6, 4);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(150, 13);
			this->label1->TabIndex = 3;
			this->label1->Text = L"RollerCoaster Tycoon 3 path";
			// 
			// btn_rct3locbrowser
			// 
			this->btn_rct3locbrowser->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->btn_rct3locbrowser->Location = System::Drawing::Point(392, 18);
			this->btn_rct3locbrowser->Name = L"btn_rct3locbrowser";
			this->btn_rct3locbrowser->Size = System::Drawing::Size(104, 23);
			this->btn_rct3locbrowser->TabIndex = 2;
			this->btn_rct3locbrowser->Text = L"Browse";
			this->btn_rct3locbrowser->UseVisualStyleBackColor = true;
			this->btn_rct3locbrowser->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// tb_rct3loc
			// 
			this->tb_rct3loc->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->tb_rct3loc->Location = System::Drawing::Point(9, 20);
			this->tb_rct3loc->Name = L"tb_rct3loc";
			this->tb_rct3loc->ReadOnly = true;
			this->tb_rct3loc->Size = System::Drawing::Size(377, 22);
			this->tb_rct3loc->TabIndex = 1;
			this->tb_rct3loc->TextChanged += gcnew System::EventHandler(this, &Form1::textBox1_TextChanged);
			// 
			// cb_advanced_algorithm
			// 
			this->cb_advanced_algorithm->AutoSize = true;
			this->cb_advanced_algorithm->Location = System::Drawing::Point(6, 117);
			this->cb_advanced_algorithm->Name = L"cb_advanced_algorithm";
			this->cb_advanced_algorithm->Size = System::Drawing::Size(230, 17);
			this->cb_advanced_algorithm->TabIndex = 0;
			this->cb_advanced_algorithm->Text = L"[WIP] Use intelligent cleaning algorithm";
			this->cb_advanced_algorithm->UseVisualStyleBackColor = true;
			this->cb_advanced_algorithm->CheckedChanged += gcnew System::EventHandler(this, &Form1::check_change);
			// 
			// tab_cl
			// 
			this->tab_cl->Controls->Add(this->lbl_classdata2);
			this->tab_cl->Controls->Add(this->clb_classes);
			this->tab_cl->Location = System::Drawing::Point(4, 22);
			this->tab_cl->Name = L"tab_cl";
			this->tab_cl->Padding = System::Windows::Forms::Padding(3);
			this->tab_cl->Size = System::Drawing::Size(502, 202);
			this->tab_cl->TabIndex = 0;
			this->tab_cl->Text = L"Data list";
			this->tab_cl->UseVisualStyleBackColor = true;
			// 
			// lbl_classdata2
			// 
			this->lbl_classdata2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->lbl_classdata2->AutoSize = true;
			this->lbl_classdata2->Location = System::Drawing::Point(6, 186);
			this->lbl_classdata2->Name = L"lbl_classdata2";
			this->lbl_classdata2->Size = System::Drawing::Size(83, 13);
			this->lbl_classdata2->TabIndex = 1;
			this->lbl_classdata2->Text = L"Load a file first";
			// 
			// clb_classes
			// 
			this->clb_classes->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->clb_classes->CheckOnClick = true;
			this->clb_classes->FormattingEnabled = true;
			this->clb_classes->IntegralHeight = false;
			this->clb_classes->Location = System::Drawing::Point(6, 6);
			this->clb_classes->Name = L"clb_classes";
			this->clb_classes->Size = System::Drawing::Size(490, 177);
			this->clb_classes->TabIndex = 0;
			this->clb_classes->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::clb_classes_SelectedIndexChanged);
			// 
			// tab_park
			// 
			this->tab_park->Controls->Add(this->classgraph);
			this->tab_park->Controls->Add(this->lbl_debugdata);
			this->tab_park->Controls->Add(this->lbl_classdata1);
			this->tab_park->Controls->Add(this->tb_parkinfo);
			this->tab_park->Controls->Add(this->park_prev);
			this->tab_park->Location = System::Drawing::Point(4, 22);
			this->tab_park->Name = L"tab_park";
			this->tab_park->Padding = System::Windows::Forms::Padding(3);
			this->tab_park->Size = System::Drawing::Size(502, 202);
			this->tab_park->TabIndex = 3;
			this->tab_park->Text = L"General";
			this->tab_park->UseVisualStyleBackColor = true;
			// 
			// classgraph
			// 
			this->classgraph->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			chartArea1->Name = L"ChartArea1";
			this->classgraph->ChartAreas->Add(chartArea1);
			legend1->Name = L"Legend1";
			this->classgraph->Legends->Add(legend1);
			this->classgraph->Location = System::Drawing::Point(6, 71);
			this->classgraph->Name = L"classgraph";
			this->classgraph->Palette = System::Windows::Forms::DataVisualization::Charting::ChartColorPalette::None;
			this->classgraph->PaletteCustomColors = gcnew cli::array< System::Drawing::Color >(11) {
				System::Drawing::Color::DarkRed, System::Drawing::Color::OrangeRed,
					System::Drawing::Color::DarkOrange, System::Drawing::Color::Moccasin, System::Drawing::Color::OliveDrab, System::Drawing::Color::SpringGreen,
					System::Drawing::Color::CornflowerBlue, System::Drawing::Color::Navy, System::Drawing::Color::BlueViolet, System::Drawing::Color::Orchid,
					System::Drawing::Color::Crimson
			};
			this->classgraph->Size = System::Drawing::Size(490, 128);
			this->classgraph->TabIndex = 12;
			this->classgraph->Text = L"chart1";
			// 
			// lbl_debugdata
			// 
			this->lbl_debugdata->AutoSize = true;
			this->lbl_debugdata->Location = System::Drawing::Point(7, 71);
			this->lbl_debugdata->Name = L"lbl_debugdata";
			this->lbl_debugdata->Size = System::Drawing::Size(61, 13);
			this->lbl_debugdata->TabIndex = 11;
			this->lbl_debugdata->Text = L"                  ";
			// 
			// lbl_classdata1
			// 
			this->lbl_classdata1->AutoSize = true;
			this->lbl_classdata1->Location = System::Drawing::Point(86, 50);
			this->lbl_classdata1->Name = L"lbl_classdata1";
			this->lbl_classdata1->Size = System::Drawing::Size(43, 13);
			this->lbl_classdata1->TabIndex = 2;
			this->lbl_classdata1->Text = L"            ";
			// 
			// tb_parkinfo
			// 
			this->tb_parkinfo->AutoSize = true;
			this->tb_parkinfo->Location = System::Drawing::Point(86, 6);
			this->tb_parkinfo->Name = L"tb_parkinfo";
			this->tb_parkinfo->Size = System::Drawing::Size(80, 13);
			this->tb_parkinfo->TabIndex = 1;
			this->tb_parkinfo->Text = L"No file loaded";
			// 
			// park_prev
			// 
			this->park_prev->Cursor = System::Windows::Forms::Cursors::Hand;
			this->park_prev->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"park_prev.Image")));
			this->park_prev->InitialImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"park_prev.InitialImage")));
			this->park_prev->Location = System::Drawing::Point(6, 6);
			this->park_prev->Name = L"park_prev";
			this->park_prev->Size = System::Drawing::Size(74, 58);
			this->park_prev->TabIndex = 0;
			this->park_prev->TabStop = false;
			this->park_prev->Click += gcnew System::EventHandler(this, &Form1::park_prev_Click);
			// 
			// tabctrlhidden
			// 
			this->tabctrlhidden->Controls->Add(this->tabPage1);
			this->tabctrlhidden->Controls->Add(this->tabPage3);
			this->tabctrlhidden->Location = System::Drawing::Point(476, 3);
			this->tabctrlhidden->Name = L"tabctrlhidden";
			this->tabctrlhidden->SelectedIndex = 0;
			this->tabctrlhidden->Size = System::Drawing::Size(42, 33);
			this->tabctrlhidden->TabIndex = 10;
			this->tabctrlhidden->Visible = false;
			// 
			// tabPage1
			// 
			this->tabPage1->Location = System::Drawing::Point(4, 22);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(34, 7);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"tabPage1";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// tabPage3
			// 
			this->tabPage3->Location = System::Drawing::Point(4, 22);
			this->tabPage3->Name = L"tabPage3";
			this->tabPage3->Padding = System::Windows::Forms::Padding(3);
			this->tabPage3->Size = System::Drawing::Size(34, 7);
			this->tabPage3->TabIndex = 1;
			this->tabPage3->Text = L"tabPage3";
			this->tabPage3->UseVisualStyleBackColor = true;
			// 
			// tabctrl
			// 
			this->tabctrl->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->tabctrl->Controls->Add(this->tab_park);
			this->tabctrl->Controls->Add(this->tab_cl);
			this->tabctrl->Controls->Add(this->tab_missing_customcontent);
			this->tabctrl->Controls->Add(this->tab_removal);
			this->tabctrl->Controls->Add(this->tab_opties);
			this->tabctrl->Controls->Add(this->tab_about);
			this->tabctrl->Controls->Add(this->tab_debug);
			this->tabctrl->Controls->Add(this->tab_live);
			this->tabctrl->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->tabctrl->Location = System::Drawing::Point(12, 33);
			this->tabctrl->Name = L"tabctrl";
			this->tabctrl->SelectedIndex = 0;
			this->tabctrl->Size = System::Drawing::Size(510, 228);
			this->tabctrl->TabIndex = 1;
			// 
			// tab_removal
			// 
			this->tab_removal->Controls->Add(this->tabControl1);
			this->tab_removal->Location = System::Drawing::Point(4, 22);
			this->tab_removal->Name = L"tab_removal";
			this->tab_removal->Padding = System::Windows::Forms::Padding(3);
			this->tab_removal->Size = System::Drawing::Size(502, 202);
			this->tab_removal->TabIndex = 11;
			this->tab_removal->Text = L"Removal aid";
			this->tab_removal->UseVisualStyleBackColor = true;
			// 
			// tabControl1
			// 
			this->tabControl1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Controls->Add(this->tabPage4);
			this->tabControl1->Controls->Add(this->tabPage5);
			this->tabControl1->Location = System::Drawing::Point(7, 6);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(489, 193);
			this->tabControl1->TabIndex = 9;
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->label3);
			this->tabPage2->Controls->Add(this->ride_list);
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(481, 167);
			this->tabPage2->TabIndex = 0;
			this->tabPage2->Text = L"Rides";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(6, 3);
			this->label3->MaximumSize = System::Drawing::Size(480, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(369, 13);
			this->label3->TabIndex = 7;
			this->label3->Text = L"List of all rides in the current parkfile (click a ride for removal selection)";
			// 
			// ride_list
			// 
			this->ride_list->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->ride_list->FormattingEnabled = true;
			this->ride_list->IntegralHeight = false;
			this->ride_list->Location = System::Drawing::Point(6, 19);
			this->ride_list->Name = L"ride_list";
			this->ride_list->Size = System::Drawing::Size(469, 142);
			this->ride_list->TabIndex = 8;
			this->ride_list->Click += gcnew System::EventHandler(this, &Form1::ride_edit);
			// 
			// tabPage4
			// 
			this->tabPage4->Controls->Add(this->track_list);
			this->tabPage4->Controls->Add(this->label4);
			this->tabPage4->Location = System::Drawing::Point(4, 22);
			this->tabPage4->Name = L"tabPage4";
			this->tabPage4->Padding = System::Windows::Forms::Padding(3);
			this->tabPage4->Size = System::Drawing::Size(481, 167);
			this->tabPage4->TabIndex = 1;
			this->tabPage4->Text = L"Tracks";
			this->tabPage4->UseVisualStyleBackColor = true;
			// 
			// tabPage5
			// 
			this->tabPage5->Controls->Add(this->stall_list);
			this->tabPage5->Controls->Add(this->label5);
			this->tabPage5->Location = System::Drawing::Point(4, 22);
			this->tabPage5->Name = L"tabPage5";
			this->tabPage5->Padding = System::Windows::Forms::Padding(3);
			this->tabPage5->Size = System::Drawing::Size(481, 167);
			this->tabPage5->TabIndex = 2;
			this->tabPage5->Text = L"Stalls";
			this->tabPage5->UseVisualStyleBackColor = true;
			// 
			// stall_list
			// 
			this->stall_list->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->stall_list->FormattingEnabled = true;
			this->stall_list->IntegralHeight = false;
			this->stall_list->Location = System::Drawing::Point(6, 19);
			this->stall_list->Name = L"stall_list";
			this->stall_list->Size = System::Drawing::Size(469, 142);
			this->stall_list->TabIndex = 10;
			this->stall_list->Click += gcnew System::EventHandler(this, &Form1::stall_edit);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(6, 3);
			this->label5->MaximumSize = System::Drawing::Size(480, 0);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(370, 13);
			this->label5->TabIndex = 9;
			this->label5->Text = L"List of all stalls in the current parkfile (click a ride for removal selection)";
			// 
			// tab_debug
			// 
			this->tab_debug->Controls->Add(this->debugbox);
			this->tab_debug->Controls->Add(this->button2);
			this->tab_debug->Controls->Add(this->debug_dump);
			this->tab_debug->Location = System::Drawing::Point(4, 22);
			this->tab_debug->Name = L"tab_debug";
			this->tab_debug->Padding = System::Windows::Forms::Padding(3);
			this->tab_debug->Size = System::Drawing::Size(502, 202);
			this->tab_debug->TabIndex = 8;
			this->tab_debug->Text = L"Debug";
			this->tab_debug->UseVisualStyleBackColor = true;
			// 
			// debugbox
			// 
			this->debugbox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->debugbox->Location = System::Drawing::Point(7, 35);
			this->debugbox->Multiline = true;
			this->debugbox->Name = L"debugbox";
			this->debugbox->Size = System::Drawing::Size(489, 161);
			this->debugbox->TabIndex = 8;
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(179, 6);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(104, 23);
			this->button2->TabIndex = 7;
			this->button2->Text = L"Analyse park";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			// 
			// tab_live
			// 
			this->tab_live->Controls->Add(this->label9);
			this->tab_live->Controls->Add(this->groupBox1);
			this->tab_live->Location = System::Drawing::Point(4, 22);
			this->tab_live->Name = L"tab_live";
			this->tab_live->Padding = System::Windows::Forms::Padding(3);
			this->tab_live->Size = System::Drawing::Size(502, 202);
			this->tab_live->TabIndex = 10;
			this->tab_live->Text = L"Live";
			this->tab_live->UseVisualStyleBackColor = true;
			// 
			// label9
			// 
			this->label9->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(7, 183);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(195, 13);
			this->label9->TabIndex = 2;
			this->label9->Text = L"* Might not be 100% correct realtime";
			// 
			// groupBox1
			// 
			this->groupBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->groupBox1->Controls->Add(this->btn_time_hoursplus1);
			this->groupBox1->Controls->Add(this->btn_time_minutesplus1);
			this->groupBox1->Controls->Add(this->btn_time_minutemin1);
			this->groupBox1->Controls->Add(this->label6);
			this->groupBox1->Controls->Add(this->button7);
			this->groupBox1->Controls->Add(this->btn_time_hoursmin1);
			this->groupBox1->Controls->Add(this->timeEnabler);
			this->groupBox1->Controls->Add(this->timeDelta);
			this->groupBox1->Location = System::Drawing::Point(6, 6);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(490, 116);
			this->groupBox1->TabIndex = 1;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Time changer BETA";
			// 
			// btn_time_hoursplus1
			// 
			this->btn_time_hoursplus1->Location = System::Drawing::Point(254, 86);
			this->btn_time_hoursplus1->Name = L"btn_time_hoursplus1";
			this->btn_time_hoursplus1->Size = System::Drawing::Size(75, 23);
			this->btn_time_hoursplus1->TabIndex = 7;
			this->btn_time_hoursplus1->Text = L"Hours + 1";
			this->btn_time_hoursplus1->UseVisualStyleBackColor = true;
			this->btn_time_hoursplus1->Click += gcnew System::EventHandler(this, &Form1::btn_time_hoursplus1_Click);
			// 
			// btn_time_minutesplus1
			// 
			this->btn_time_minutesplus1->Location = System::Drawing::Point(171, 86);
			this->btn_time_minutesplus1->Name = L"btn_time_minutesplus1";
			this->btn_time_minutesplus1->Size = System::Drawing::Size(77, 23);
			this->btn_time_minutesplus1->TabIndex = 6;
			this->btn_time_minutesplus1->Text = L"Minutes + 1";
			this->btn_time_minutesplus1->UseVisualStyleBackColor = true;
			this->btn_time_minutesplus1->Click += gcnew System::EventHandler(this, &Form1::btn_time_minutesplus1_Click);
			// 
			// btn_time_minutemin1
			// 
			this->btn_time_minutemin1->Location = System::Drawing::Point(90, 86);
			this->btn_time_minutemin1->Name = L"btn_time_minutemin1";
			this->btn_time_minutemin1->Size = System::Drawing::Size(75, 23);
			this->btn_time_minutemin1->TabIndex = 5;
			this->btn_time_minutemin1->Text = L"Minutes - 1";
			this->btn_time_minutemin1->UseVisualStyleBackColor = true;
			this->btn_time_minutemin1->Click += gcnew System::EventHandler(this, &Form1::btn_time_minutemin1_Click);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(6, 19);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(135, 13);
			this->label6->TabIndex = 4;
			this->label6->Text = L"Ingame time speed slider";
			// 
			// button7
			// 
			this->button7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->button7->Location = System::Drawing::Point(353, 86);
			this->button7->Name = L"button7";
			this->button7->Size = System::Drawing::Size(128, 23);
			this->button7->TabIndex = 2;
			this->button7->Text = L"Simulate realtime*";
			this->button7->UseVisualStyleBackColor = true;
			this->button7->Click += gcnew System::EventHandler(this, &Form1::button7_Click);
			// 
			// btn_time_hoursmin1
			// 
			this->btn_time_hoursmin1->Location = System::Drawing::Point(9, 86);
			this->btn_time_hoursmin1->Name = L"btn_time_hoursmin1";
			this->btn_time_hoursmin1->Size = System::Drawing::Size(75, 23);
			this->btn_time_hoursmin1->TabIndex = 3;
			this->btn_time_hoursmin1->Text = L"Hours - 1";
			this->btn_time_hoursmin1->UseVisualStyleBackColor = true;
			this->btn_time_hoursmin1->Click += gcnew System::EventHandler(this, &Form1::button6_Click);
			// 
			// timeEnabler
			// 
			this->timeEnabler->AutoSize = true;
			this->timeEnabler->Location = System::Drawing::Point(115, 0);
			this->timeEnabler->Name = L"timeEnabler";
			this->timeEnabler->Size = System::Drawing::Size(61, 17);
			this->timeEnabler->TabIndex = 1;
			this->timeEnabler->Text = L"Enable";
			this->timeEnabler->UseVisualStyleBackColor = true;
			this->timeEnabler->CheckedChanged += gcnew System::EventHandler(this, &Form1::timeEnabler_CheckedChanged);
			// 
			// timeDelta
			// 
			this->timeDelta->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->timeDelta->Location = System::Drawing::Point(6, 35);
			this->timeDelta->Maximum = 1000;
			this->timeDelta->Name = L"timeDelta";
			this->timeDelta->Size = System::Drawing::Size(478, 45);
			this->timeDelta->TabIndex = 0;
			this->timeDelta->TickFrequency = 50;
			this->timeDelta->Scroll += gcnew System::EventHandler(this, &Form1::timeDelta_Scroll);
			// 
			// park_zipper
			// 
			this->park_zipper->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &Form1::park_zipper_DoWork);
			// 
			// filescan
			// 
			this->filescan->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &Form1::filescan_DoWork);
			// 
			// errorExecutor
			// 
			this->errorExecutor->Interval = 500;
			this->errorExecutor->Tick += gcnew System::EventHandler(this, &Form1::errorExecutor_Tick);
			// 
			// button3
			// 
			this->button3->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button3->Location = System::Drawing::Point(12, 3);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(96, 28);
			this->button3->TabIndex = 11;
			this->button3->Text = L"Load park";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &Form1::button3_Click);
			// 
			// timechanger
			// 
			this->timechanger->Interval = 10;
			this->timechanger->Tick += gcnew System::EventHandler(this, &Form1::timechanger_Tick);
			// 
			// continuousParkWatcher
			// 
			this->continuousParkWatcher->EnableRaisingEvents = true;
			this->continuousParkWatcher->SynchronizingObject = this;
			// 
			// date_label
			// 
			this->date_label->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->date_label->AutoSize = true;
			this->date_label->BackColor = System::Drawing::Color::Transparent;
			this->date_label->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->date_label->Location = System::Drawing::Point(19, 271);
			this->date_label->MaximumSize = System::Drawing::Size(150, 0);
			this->date_label->MinimumSize = System::Drawing::Size(150, 0);
			this->date_label->Name = L"date_label";
			this->date_label->Size = System::Drawing::Size(150, 15);
			this->date_label->TabIndex = 12;
			this->date_label->Text = L"May 1 2013";
			// 
			// splitButton2
			// 
			this->splitButton2->AutoSize = true;
			this->splitButton2->ContextMenuStrip = this->zip_menu;
			this->splitButton2->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F));
			this->splitButton2->Location = System::Drawing::Point(229, 3);
			this->splitButton2->Name = L"splitButton2";
			this->splitButton2->Size = System::Drawing::Size(120, 28);
			this->splitButton2->SplitMenuStrip = this->zip_menu;
			this->splitButton2->TabIndex = 13;
			this->splitButton2->Text = L"Create ParkZIP";
			this->splitButton2->UseVisualStyleBackColor = true;
			this->splitButton2->Click += gcnew System::EventHandler(this, &Form1::splitButton2_Click);
			// 
			// zip_menu
			// 
			this->zip_menu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->createBlueprintToolStripMenuItem });
			this->zip_menu->Name = L"zip_menu";
			this->zip_menu->Size = System::Drawing::Size(160, 26);
			// 
			// createBlueprintToolStripMenuItem
			// 
			this->createBlueprintToolStripMenuItem->Name = L"createBlueprintToolStripMenuItem";
			this->createBlueprintToolStripMenuItem->Size = System::Drawing::Size(159, 22);
			this->createBlueprintToolStripMenuItem->Text = L"Create blueprint";
			this->createBlueprintToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::createBlueprintToolStripMenuItem_Click);
			// 
			// blueprint_creator
			// 
			this->blueprint_creator->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &Form1::blueprint_creator_DoWork);
			// 
			// graphDrawer
			// 
			this->graphDrawer->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &Form1::graphDrawer_DoWork);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(534, 321);
			this->Controls->Add(this->splitButton2);
			this->Controls->Add(this->tabctrlhidden);
			this->Controls->Add(this->date_label);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->btn_save);
			this->Controls->Add(this->loading);
			this->Controls->Add(this->btn_deselect_items);
			this->Controls->Add(this->btn_select_unused_items);
			this->Controls->Add(this->statusStrip1);
			this->Controls->Add(this->tabctrl);
			this->DoubleBuffered = true;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->MinimumSize = System::Drawing::Size(550, 360);
			this->Name = L"Form1";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Park CleanUp 2";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->ms_parkfile_ovlscan->ResumeLayout(false);
			this->statusStrip1->ResumeLayout(false);
			this->statusStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->loading))->EndInit();
			this->ms_unuseditems->ResumeLayout(false);
			this->save_menu->ResumeLayout(false);
			this->tab_about->ResumeLayout(false);
			this->tab_about->PerformLayout();
			this->tab_missing_customcontent->ResumeLayout(false);
			this->tab_missing_customcontent->PerformLayout();
			this->tab_opties->ResumeLayout(false);
			this->tab_opties->PerformLayout();
			this->tab_cl->ResumeLayout(false);
			this->tab_cl->PerformLayout();
			this->tab_park->ResumeLayout(false);
			this->tab_park->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->classgraph))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->park_prev))->EndInit();
			this->tabctrlhidden->ResumeLayout(false);
			this->tabctrl->ResumeLayout(false);
			this->tab_removal->ResumeLayout(false);
			this->tabControl1->ResumeLayout(false);
			this->tabPage2->ResumeLayout(false);
			this->tabPage2->PerformLayout();
			this->tabPage4->ResumeLayout(false);
			this->tabPage4->PerformLayout();
			this->tabPage5->ResumeLayout(false);
			this->tabPage5->PerformLayout();
			this->tab_debug->ResumeLayout(false);
			this->tab_debug->PerformLayout();
			this->tab_live->ResumeLayout(false);
			this->tab_live->PerformLayout();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->timeDelta))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->continuousParkWatcher))->EndInit();
			this->zip_menu->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
#pragma endregion

	public:

		delegate void d_AddItem(String^ item);
		delegate void d_ChangeLabel(int label, String^ item);
		delegate void d_ChangeVisible(int label, bool visible);
		delegate void d_ChangeValue(int label, int value);
		delegate void d_AddNode(int label, TreeNode^ node);
		delegate void d_HandleErrorLog();

		void AddItem(String^ item)
		{
			clb_classes->Items->Add(item);
		}
		void ChangeLabel(int label, String^ item)
		{
			if(label == Labels::lbl_classdata1)
				lbl_classdata1->Text = item;
			else if(label == Labels::lbl_classdata2)
				lbl_classdata2->Text = item;
			else if(label == Labels::sl_appstatus)
				sl_appstatus->Text = item;
			else if(label == Labels::tb_parkinfo)
				tb_parkinfo->Text = item;
			else if(label == Labels::pb_status)
				pb_status->Text = item;
		}
		void ChangeVisible(int label, bool visible)
		{
			if(label == Labels::lbl_classdata1)
				lbl_classdata1->Visible = visible;
			else if(label == Labels::lbl_classdata1)
				lbl_classdata2->Visible = visible;
			else if(label == Labels::sl_appstatus)
				sl_appstatus->Visible = visible;
			else if(label == Labels::tb_parkinfo)
				tb_parkinfo->Visible = visible;
			else if(label == Labels::loading)
				loading->Visible = visible;
			else if(label == Labels::pb_status)
				pb_status->Visible = visible;
		}
		void ChangeValue(int label, int value)
		{
			if(label == Labels::pb_status)
				pb_status->Value = value;
		}
		void AddNode(int label, TreeNode^ node)
		{
		}
		void HandleErrorLog()
		{
			/*errortreebinder->Items->Clear();
			if(el.size() > 0)
			{
			vector<string> ovlerrorfiles;
			std::sort(el.begin(), el.end(), el[0]);
			for(int i = 0; i < el.size(); i++)
			{
			bool found = false;
			for(int j = 0; j < ovlerrorfiles.size(); i++)
			{
			if(el[i].filename == ovlerrorfiles[j])
			{
			found = true;
			break;
			}
			}
			if(!found)
			{						
			errortreebinder->Items->Add(stripRCT3Folder(gcnew String(el[i].filename.c_str())));
			//ovlerrorfiles.push_back(el[i].filename);
			}
			}
			}*/
		}

		bool IsDebugEnabled()
		{
			return currentChannel == PCU_Channels::DEBUG;
		}

		System::Void ShowTutorial();

		bool writeBlueprintsToFile(string filename, vector<BlueprintFile> blueprints)
		{
			std::ofstream savefile(filename, std::ios::binary | std::ios::out);
			if(savefile.is_open())
			{
				savefile.write("PCUB", 4);
				int size = blueprints.size();
				savefile.write ((char*)&size, 4);
				for(int i = 0; i < blueprints.size(); i++)
				{
					/*int fullnamesize = blueprints[i].fullfilename.size();					
					savefile.write ((char*)&fullnamesize, 4);
					savefile.write(blueprints[i].fullfilename.c_str(), fullnamesize);*/

					int namelength = blueprints[i].relativename.size();
					savefile.write ((char*)&namelength, 4);
					savefile.write(blueprints[i].relativename.c_str(), namelength);

					savefile.write((char*)&blueprints[i].hash, 4);
				}
				savefile.close();
				return true;
			}
			return false;
		}

		vector<BlueprintFile> readBlueprintsFromFile(string filename)
		{
			vector<BlueprintFile> ret;
			std::ifstream savefile(filename, std::ios::binary);
			if(savefile.is_open())
			{
				savefile.seekg(4);
				int count = 0;
				savefile.read((char*)&count, 4);
				for(int i = 0; i < count; i++)
				{
					BlueprintFile newFile;
					int namelength = 0;
					savefile.read((char*)&namelength, 4);

					char* str = (char*)malloc(namelength + 1);
					str[namelength] = 0;

					savefile.read(str, namelength);

					newFile.relativename = str;
					savefile.read((char*)&newFile.hash, 4);

					delete[] str;
					ret.push_back(newFile);
				}
			}
			return ret;
		}

		int GetFilesNumber(String^ folder, vector<BlueprintFile> &strs){
			cli::array<String^>^ dir = Directory::GetDirectories( folder );
			for (int i=0; i<dir->Length; i++){
				String^ send = dir[i]->Replace(folder+"\\","");
				GetFilesNumber(dir[i], strs);
			}

			String^ rct3Loc = removeTrailingSlash(tb_rct3loc->Text);

			cli::array<String^>^ file = Directory::GetFiles( folder );
			for (int i=0; i<file->Length; i++){					 
				if(file[i]->EndsWith(".common.ovl")){
					BlueprintFile newFile;
					string relativename;
					string fullfilename;
					file[i] = file[i]->Replace(".common.ovl", "");
					String^ curfile = file[i];

					curfile = removeStartingSlash(curfile->Replace(rct3Loc, ""));
					ToString(curfile, relativename);
					ToString(file[i], fullfilename);

					newFile.relativename = relativename;
					newFile.fullfilename = fullfilename;
					newFile.hash = 0;

					strs.push_back(newFile);
				}
			}

			return 1;
		}

		String^ removeTrailingSlash(String^ base)
		{
			base = base->TrimEnd('\\', '/');
			return base;
		}

		String^ removeStartingSlash(String^ base)
		{
			base = base->TrimStart('\\', '/');
			return base;
		}

		String^ stripRCT3Folder(String^ base)
		{
			base = base->ToLower()->Replace("/", "\\")->Replace(tb_rct3loc->Text->ToLower(), "")->TrimStart('\\', '/');
			return base;
		}

		String^ getOVLNameFromPath(String^ ovl)
		{
			ovl = stripRCT3Folder(ovl);
			return ovl;
		}

		String^ getOVLNameFromPath(string ovl)
		{
			return getOVLNameFromPath(gcnew String(ovl.c_str()));
		}

		string getOVLNameFromPathS(string ovl)
		{
			string ret;
			ToString(getOVLNameFromPath(gcnew String(ovl.c_str())), ret);
			return ret;
		}

		unsigned int sqrt32(unsigned long n)  
		{  
			unsigned int c = 0x8000;  
			unsigned int g = 0x8000;  

			for(;;) {  
				if(g*g > n)  
					g ^= c;  
				c >>= 1;  
				if(c == 0)  
					return g;  
				g |= c;  
			}  
		}

		double ByteSizeConverter(double size, string &type)
		{
			type = "bytes";
			double mb = size;
			if(mb > 1024)
			{
				type = "kilobytes";
				mb /= 1024;
			}
			if(mb > 1024)
			{
				type = "megabytes";
				mb /= 1024;
			}
			return mb;
		}

		void Log(String^ msg)
		{
			//debuglog->Text += msg + Environment::NewLine;
		}

		void SetStatusMax(int max)
		{
			pb_status->Maximum = max;
		}

		void SetStatusValue(int val)
		{
			pb_status->Value = val;
		}

		BOOL WritePrivateProfileInt(LPCSTR lpAppName, LPCSTR lpKeyName, int nInteger, LPCSTR lpFileName)
		{
			char lpString[ 1024 ];
			sprintf( lpString, "%d", nInteger );
			return WritePrivateProfileStringA( lpAppName, lpKeyName, lpString, lpFileName );
		}

		void SaveOptions()
		{
			if(!pcuReady)
				return;

			char* settingsfile = ToChar(Path::GetDirectoryName(Application::ExecutablePath) + gcnew String("/options.ini"));
			//WritePrivateProfileStringA
			WritePrivateProfileInt("PARK_CLEANUP", OPTIONS_ADVANCED_ALGORITHM, (cb_advanced_algorithm->Checked) ? 1 : 0, settingsfile);
			WritePrivateProfileInt("PARK_CLEANUP", OPTIONS_UPDATE, (cb_update->Checked) ? 1 : 0, settingsfile);
			WritePrivateProfileInt("PARK_CLEANUP", OPTIONS_AGGRESSIVE_COMPILATION, (cb_aggressive->Checked) ? 1 : 0, settingsfile);
			delete[] settingsfile;
		}

		void SetChannel(int i)
		{
			currentChannel = i;
			char* settingsfile = ToChar(Path::GetDirectoryName(Application::ExecutablePath) + gcnew String("/options.ini"));
			WritePrivateProfileInt("PARK_CLEANUP", OPTIONS_CHANNEL, i, settingsfile);
			delete[] settingsfile;
		}

		void SetChannelOption(int i)
		{
			char* settingsfile = ToChar(Path::GetDirectoryName(Application::ExecutablePath) + gcnew String("/options.ini"));
			WritePrivateProfileInt("PARK_CLEANUP", OPTIONS_CHANNEL, i, settingsfile);
			delete[] settingsfile;
			MsgExt("In order for the channel switch to take full effect you must restart Park CleanUp!", "Restart required", MB_OK, MessageBoxIcon::Information);
		}

		System::Windows::Forms::DialogResult MsgExt(String^ text, String^ caption, int btn){
			MessageBoxButtons buttons = (MessageBoxButtons)btn;
			if((int)buttons == NULL){buttons = MessageBoxButtons::OK;}
			return MessageBox::Show(text,caption,buttons);
		}

		System::Windows::Forms::DialogResult MsgExt(String^ text, String^ caption, int btn, MessageBoxIcon icon){
			MessageBoxButtons buttons = (MessageBoxButtons)btn;
			if((int)buttons == NULL){buttons = MessageBoxButtons::OK;}
			return MessageBox::Show(text,caption,buttons, icon);
		}

		char* ToChar( String^ source) {
			char* target;
			pin_ptr<const wchar_t> wch = PtrToStringChars( source );
			int len = (( source->Length+1) * 2);
			target = new char[ len ];
			wcstombs( target, wch, len );
			return target;
		}

		string wstringtostring(wstring src)
		{
			String^ conv_str = gcnew String(src.c_str());
			char* conv_str2 = ToChar(conv_str);
			string retstr = conv_str2;
			delete[] conv_str2;
			return retstr;
		}

		void UpdateChecklist()
		{
			vector<bool> new_list_checkeditems;
			for(int i = 0; i < clb_classes->Items->Count; i++)
			{
				bool sel = clb_classes->GetItemChecked(i);
				new_list_checkeditems.push_back(sel);
			}
			list_checkeditems = new_list_checkeditems;
		}
		void RenderChecklist()
		{
			clb_classes->Invalidate();
			clb_classes->Update();
			tabctrl->PerformLayout();
			tab_cl->PerformLayout();
		}

		bool AppBusy()
		{
			if(park_initialiser->IsBusy)
				return true;
			if(unused_item_selector->IsBusy)
				return true;
			if(park_zipper->IsBusy)
				return true;
			if(filescan->IsBusy)
				return true;
			if(blueprint_creator->IsBusy)
				return true;
			return false;
		}

		void BusyMessageBox()
		{
			String^ msg = "Please wait until the current task is done!\r\n\r\nCurrently busy are:\r\n";
			if(park_initialiser->IsBusy)
				msg += "- Parkfile reader\r\n";
			if(unused_item_selector->IsBusy)
				msg += "- Item selector (CleanUp)\r\n";
			if(park_zipper->IsBusy)
				msg += "- Park zipper\r\n";
			if(filescan->IsBusy)
				msg += "- Park zipper\r\n";
			MessageBox::Show(msg);
		}

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

		void List_CheckItem(uint32_t i, bool select)
		{
			clb_classes->SetItemChecked(i, select);
		}

		bool IsReferencedToAndChecked(Reference ref)
		{
			if(r3arg.IsValid())
			{
				uint32_t i_lastcheckeditem = 0;
				bool b_lastcheckeditem = clb_classes->GetItemChecked(0);
				for(uint32_t j = 0; j < parented_refs.size(); j++)
				{
					if(parented_refs[j].parent != i_lastcheckeditem)
						b_lastcheckeditem = clb_classes->GetItemChecked(parented_refs[j].parent);
					if(ref.a == parented_refs[j].a && ref.b == parented_refs[j].b && !b_lastcheckeditem)
						return true;
				}
				return false;
			} else {
				return false;
			}
		}

		bool IsReferencedToAndCheckedCached(Reference ref)
		{
			//@CALL BEFORE: UpdateChecklist();
			if(r3arg.IsValid())
			{
				for(uint32_t j = 0; j < parented_refs.size(); j++)
				{
					if(ref.a == parented_refs[j].a && ref.b == parented_refs[j].b && !list_checkeditems[parented_refs[j].parent])
						return true;
				}
				return false;
			} else {
				return false;
			}
		}

		bool IsReferencedTo(Reference ref)
		{
			if(r3arg.IsValid())
			{
				for(uint32_t j = 0; j < parented_refs.size(); j++)
				{
					if(ref.a == parented_refs[j].a && ref.b == parented_refs[j].b)
						return true;
				}
				return false;
			} else {
				return false;
			}
		}

		void RecalculateFilesize()
		{
			if(r3arg.IsValid())
			{
				vector<Class_Data_Block> cdb = r3arg.getclassdata();
				vector<Class_Prototype> class_prototype = r3arg.getclassprototypes();

				double new_file_size = r3arg.as.variable_declaration_endpos;

				for(uint32_t j = 0; j < cdb.size(); j++)
					if(!clb_classes->GetItemChecked(j))
						new_file_size += cdb[j].end - cdb[j].start;

				string new_size;
				new_file_size = ByteSizeConverter(new_file_size, new_size);
				new_file_size = ceil(new_file_size * 100) / 100;

				string new_size2;
				double new_file_size2 = ByteSizeConverter(r3arg.arc.getsize(), new_size2);
				new_file_size2 = ceil(new_file_size2 * 100) / 100;

				cli::array<Object^>^ parms = gcnew cli::array<Object^> { (int)Labels::lbl_classdata1,"Expected filesize is " + new_file_size + " " +  gcnew String(new_size.c_str()) + " of " + new_file_size2 + " " + gcnew String(new_size2.c_str()) };
				lbl_classdata1->Invoke(gcnew d_ChangeLabel(this, &Form1::ChangeLabel), parms);

				//parms = gcnew array<Object^> { 1, lbl_classdata1->Text };
				parms = gcnew cli::array<Object^> { (int)Labels::lbl_classdata2,"Expected filesize is " + new_file_size + " " +  gcnew String(new_size.c_str()) + " of " + new_file_size2 + " " + gcnew String(new_size2.c_str()) };
				lbl_classdata2->Invoke(gcnew d_ChangeLabel(this, &Form1::ChangeLabel), parms);

				//lbl_classdata1->Text = new_file_size + " " +  gcnew String(new_size.c_str()) + " of " + new_file_size2 + " " + gcnew String(new_size2.c_str());
				//lbl_classdata2->Text = lbl_classdata1->Text;
			}
		}

		void DeselectItems()
		{
			for(uint32_t j = 0; j < cdb.size(); j++)
				List_CheckItem(j, false);
			RecalculateFilesize();
		}

		void SelectPredefinedItems()
		{
			if(!r3arg.IsValid())
			{
				MessageBox::Show("Open a parkfile first!");
				return;
			}
			if(AppBusy())
			{
				BusyMessageBox();//MessageBox::Show("Please wait until the current task is done!");
				return;
			}
			loading->Visible = true;
			pb_status->Visible = true;
			pb_status->Value = 0;
			unused_item_selector->RunWorkerAsync();
		}

		void SaveFile()
		{
			if(AppBusy())
			{
				BusyMessageBox();//MessageBox::Show("Please wait until the current task is done!");
				return;
			}
			if(!r3arg.IsValid())
			{
				MessageBox::Show("Open a parkfile first!");
				return;
			}
			SaveFileDialog^ sfd = gcnew SaveFileDialog;
			sfd->Filter = gcnew String(allowedextensions.c_str());//"Park file|*.dat";
			sfd->RestoreDirectory = true;
			if ( sfd->ShowDialog() == System::Windows::Forms::DialogResult::OK )
			{
				string fname;
				ToString(sfd->FileName, fname);
				SaveCompressedParkfileAs(fname);

			}
		}

		void SaveCompressedParkfileAs(string fname)
		{
			ofstream sfile;
			sfile.open (fname.c_str(), std::ios::out|std::ios::binary );
			if (sfile.is_open())
			{
				r3arg.arc.seek(r3arg.as.variable_declaration_startpos, BFR::BEGIN);
				char *tmp = new char[r3arg.as.variable_declaration_endpos - r3arg.as.variable_declaration_startpos];
				r3arg.arc.read(tmp, r3arg.as.variable_declaration_endpos - r3arg.as.variable_declaration_startpos);
				sfile.write(tmp, r3arg.as.variable_declaration_endpos - r3arg.as.variable_declaration_startpos);
				delete[] tmp;

				uint32_t num_datablocks = 0;
				for(int i = 0; i < clb_classes->Items->Count; i++)
					if(!clb_classes->GetItemChecked(i))
						num_datablocks++;

				sfile.write((char*)&num_datablocks, 4);

				if(defrag)
				{
					/*vector<bool> temp_items;
					for(int i = 0; i < clb_classes->Items->Count; i++)
					{
					bool selected = false;
					if(!clb_classes->GetItemChecked(i))
					{
					if(cdb[i].prototype_of <= class_prototype.size())
					{
					string class_prototype_name = class_prototype[cdb[i].prototype_of].name;						 
					if(class_prototype_name == "RCT3Terrain")
					{
					selected = true;
					char *tmp = new char[cdb[i].end - cdb[i].start];
					r3arg.arc.seek(cdb[i].start, BFR::BEGIN);
					r3arg.arc.read(tmp, cdb[i].end - cdb[i].start);
					sfile.write(tmp, cdb[i].end - cdb[i].start);
					delete[] tmp;
					}
					}
					}
					temp_items.push_back(selected);
					}

					for(int i = 0; i < clb_classes->Items->Count; i++)
					{
					if(temp_items[i] == true)
					continue;
					bool selected = false;
					if(!clb_classes->GetItemChecked(i))
					{
					if(cdb[i].prototype_of <= class_prototype.size())
					{
					string class_prototype_name = class_prototype[cdb[i].prototype_of].name;						 
					if(class_prototype_name == "Weather")
					{
					selected = true;
					char *tmp = new char[cdb[i].end - cdb[i].start];
					r3arg.arc.seek(cdb[i].start, BFR::BEGIN);
					r3arg.arc.read(tmp, cdb[i].end - cdb[i].start);
					sfile.write(tmp, cdb[i].end - cdb[i].start);
					delete[] tmp;
					}
					}
					}
					temp_items[i] = selected;
					}

					for(int i = 0; i < clb_classes->Items->Count; i++)
					{
					if(temp_items[i] == true)
					continue;
					bool selected = false;
					if(!clb_classes->GetItemChecked(i))
					{
					if(cdb[i].prototype_of <= class_prototype.size())
					{
					string class_prototype_name = class_prototype[cdb[i].prototype_of].name;						 
					if(class_prototype_name == "FlexiColourGroupCache")
					{
					selected = true;
					char *tmp = new char[cdb[i].end - cdb[i].start];
					r3arg.arc.seek(cdb[i].start, BFR::BEGIN);
					r3arg.arc.read(tmp, cdb[i].end - cdb[i].start);
					sfile.write(tmp, cdb[i].end - cdb[i].start);
					delete[] tmp;
					}
					}
					}
					temp_items[i] = selected;
					}

					for(int i = 0; i < clb_classes->Items->Count; i++)
					{
					if(temp_items[i] == true)
					continue;
					bool selected = false;
					if(!clb_classes->GetItemChecked(i))
					{
					if(cdb[i].prototype_of <= class_prototype.size())
					{
					string class_prototype_name = class_prototype[cdb[i].prototype_of].name;						 
					if(class_prototype_name == "SIDDatabaseEntry")
					{
					selected = true;
					char *tmp = new char[cdb[i].end - cdb[i].start];
					r3arg.arc.seek(cdb[i].start, BFR::BEGIN);
					r3arg.arc.read(tmp, cdb[i].end - cdb[i].start);
					sfile.write(tmp, cdb[i].end - cdb[i].start);
					delete[] tmp;
					}
					}
					}
					temp_items[i] = selected;
					}

					for(int i = 0; i < clb_classes->Items->Count; i++)
					{
					if(temp_items[i] == true)
					continue;
					bool selected = false;
					if(!clb_classes->GetItemChecked(i))
					{
					if(cdb[i].prototype_of <= class_prototype.size())
					{
					string class_prototype_name = class_prototype[cdb[i].prototype_of].name;						 
					if(class_prototype_name == "SceneryItem")
					{
					selected = true;
					char *tmp = new char[cdb[i].end - cdb[i].start];
					r3arg.arc.seek(cdb[i].start, BFR::BEGIN);
					r3arg.arc.read(tmp, cdb[i].end - cdb[i].start);
					sfile.write(tmp, cdb[i].end - cdb[i].start);
					delete[] tmp;
					}
					}
					}
					temp_items[i] = selected;
					}

					for(int i = 0; i < clb_classes->Items->Count; i++)
					{
					if(temp_items[i] == true)
					continue;
					bool selected = false;
					if(!clb_classes->GetItemChecked(i))
					{
					if(cdb[i].prototype_of <= class_prototype.size())
					{
					string class_prototype_name = class_prototype[cdb[i].prototype_of].name;						 
					if(class_prototype_name.find("DatabaseEntry") != string::npos)
					{
					selected = true;
					char *tmp = new char[cdb[i].end - cdb[i].start];
					r3arg.arc.seek(cdb[i].start, BFR::BEGIN);
					r3arg.arc.read(tmp, cdb[i].end - cdb[i].start);
					sfile.write(tmp, cdb[i].end - cdb[i].start);
					delete[] tmp;
					}
					}
					}
					temp_items[i] = selected;
					}

					for(int i = 0; i < clb_classes->Items->Count; i++)
					{
					if(temp_items[i] == true)
					continue;
					bool selected = false;
					if(!clb_classes->GetItemChecked(i))
					{
					if(cdb[i].prototype_of <= class_prototype.size())
					{
					string class_prototype_name = class_prototype[cdb[i].prototype_of].name;						 
					if(class_prototype_name.find("PathTile") != string::npos || class_prototype_name.find("PathFlying") != string::npos || class_prototype_name.find("PathQueque") != string::npos)
					{
					selected = true;
					char *tmp = new char[cdb[i].end - cdb[i].start];
					r3arg.arc.seek(cdb[i].start, BFR::BEGIN);
					r3arg.arc.read(tmp, cdb[i].end - cdb[i].start);
					sfile.write(tmp, cdb[i].end - cdb[i].start);
					delete[] tmp;
					}
					}
					}
					temp_items[i] = selected;
					}

					for(int i = 0; i < clb_classes->Items->Count; i++)
					{
					if(temp_items[i] == true)
					continue;
					bool selected = false;
					if(!clb_classes->GetItemChecked(i))
					{
					if(cdb[i].prototype_of <= class_prototype.size())
					{
					string class_prototype_name = class_prototype[cdb[i].prototype_of].name;						 
					if(class_prototype_name.find("TrackPlatformPiece") != string::npos)
					{
					selected = true;
					char *tmp = new char[cdb[i].end - cdb[i].start];
					r3arg.arc.seek(cdb[i].start, BFR::BEGIN);
					r3arg.arc.read(tmp, cdb[i].end - cdb[i].start);
					sfile.write(tmp, cdb[i].end - cdb[i].start);
					delete[] tmp;
					}
					}
					}
					temp_items[i] = selected;
					}

					for(int i = 0; i < clb_classes->Items->Count; i++)
					{
					if(temp_items[i] == true)
					continue;
					bool selected = false;
					if(!clb_classes->GetItemChecked(i))
					{
					if(cdb[i].prototype_of <= class_prototype.size())
					{
					string class_prototype_name = class_prototype[cdb[i].prototype_of].name;						 
					if(class_prototype_name == "TrackPiece")
					{
					selected = true;
					char *tmp = new char[cdb[i].end - cdb[i].start];
					r3arg.arc.seek(cdb[i].start, BFR::BEGIN);
					r3arg.arc.read(tmp, cdb[i].end - cdb[i].start);
					sfile.write(tmp, cdb[i].end - cdb[i].start);
					delete[] tmp;
					}
					}
					}
					temp_items[i] = selected;
					}			

					//Save all other structures
					for(int i = 0; i < clb_classes->Items->Count; i++)
					{
					if(temp_items[i] == true)
					continue;
					if(!clb_classes->GetItemChecked(i) && temp_items[i] == false)
					{
					char *tmp = new char[cdb[i].end - cdb[i].start];
					r3arg.arc.seek(cdb[i].start, BFR::BEGIN);
					r3arg.arc.read(tmp, cdb[i].end - cdb[i].start);
					sfile.write(tmp, cdb[i].end - cdb[i].start);
					delete[] tmp;
					}
					}*/
				} else {

					if(aggressiveCompilation)
					{
						Reference baseReference;
						baseReference.a = 2;
						baseReference.b = 0;
						baseReference.pos = 0;

						//vector<Reference> reservedUnresolvedReferences
						vector<Reference> newReferences;
						for(uint32_t j = 0; j < cdb.size(); j++)
						{
							if(!clb_classes->GetItemChecked(j))
							{
								for(uint32_t k = 0; k < cdb[j].RAM.refs.size(); k++)
								{
									cdb[j].RAM.refs[k].valid = false;
									newReferences.push_back(cdb[j].RAM.refs[k]);
								}
							}
							Reference classRef;
							classRef.a = cdb[j].uid.a;
							classRef.b = cdb[j].uid.b;
							classRef.pos = cdb[j].start + 4;
							newReferences.push_back(classRef);
						}

						std::sort(newReferences.begin(), newReferences.end());
						//MessageBox::Show(newReferences.size() + "");


						int unresolvableCount = 0;
						ofstream refs;

						if(IsDebugEnabled())
							refs.open("debug_refs.txt");
						for(int i = 0; i < newReferences.size(); i++)
						{								
							if(newReferences[i].a > 0)
							{
								bool found = false;
								for(uint32_t j = 0; j < cdb.size(); j++)
								{
									if(cdb[j].uid.a == newReferences[i].a &&
										cdb[j].uid.b == newReferences[i].b)
									{
										found = true;
										newReferences[i].valid = true;
									}
								}
								if(!found)
								{
									if(IsDebugEnabled())
										refs << newReferences[i].a << " " << newReferences[i].b << "\r\n";
									unresolvableCount++;
								}
							}
						}

						if(IsDebugEnabled())
						{
							refs.close();
							if(unresolvableCount > 0)
								int koek = 0;//MessageBox::Show("[Debug warning] File contains " + unresolvableCount + " unresolvable references, see 'debug_refs.txt' for more info");
							else
								File::Delete("debug_refs.txt");
						}

						if(newReferences.size() > 0)
						{
							Reference currentReference = newReferences[0];
							for(int i = 0; i < newReferences.size(); i++)
							{
								if(newReferences[i].a > 0)
								{
									if(currentReference.a != newReferences[i].a ||
										currentReference.b != newReferences[i].b)
									{
										baseReference++;
										bool doAnotherReferenceCheck = true;
										while(doAnotherReferenceCheck)
										{
											doAnotherReferenceCheck = false;
											for(int x = 0; x < newReferences.size(); x++)
											{
												if(!newReferences[i].valid)
													if(baseReference.a == newReferences[x].a &&
														baseReference.b == newReferences[x].b)
													{
														baseReference++;
														doAnotherReferenceCheck = true;
													}
											}
										}
										currentReference = newReferences[i];
										//MessageBox::Show("Increased!");
									}

									r3arg.arc.seek(newReferences[i].pos, BFR::BEGIN);
									r3arg.arc.setInt(baseReference.a, newReferences[i].pos);
									r3arg.arc.setInt(baseReference.b, newReferences[i].pos + 4);

									//DEBUG
									//r3arg.arc.setInt(1, newReferences[i].pos);
									//r3arg.arc.setInt(1, newReferences[i].pos + 4);
								}								
							}
						}
					} //aggressiveCompilation

					for(int i = 0; i < clb_classes->Items->Count; i++)
					{
						if(!clb_classes->GetItemChecked(i))
						{
							char *tmp = new char[cdb[i].end - cdb[i].start];
							r3arg.arc.seek(cdb[i].start, BFR::BEGIN);
							r3arg.arc.read(tmp, cdb[i].end - cdb[i].start);
							sfile.write(tmp, cdb[i].end - cdb[i].start);
							delete[] tmp;
						}
					}
				}
				defrag = false;

				sfile.close();
			} else {
				MessageBox::Show("Unable to open file, make sure you have writing rights for this file!");
			}
		}

	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {

				 LoadSettings();
				 cb_AutoCleanUp->Text = "[WIP] Automatically optimise parkfiles in the [" + System::Environment::GetFolderPath(System::Environment::SpecialFolder::MyDocuments) + "\\RCT3\\Parks] folder";

				 errorExecutor->Start();
			 }

			 void LoadSettings()
			 {
				 char* settingsfile = ToChar(Path::GetDirectoryName(Application::ExecutablePath) + gcnew String("/options.ini"));

				 // Advanced algorithm
				 cb_advanced_algorithm->Checked = GetPrivateProfileIntA("PARK_CLEANUP", OPTIONS_ADVANCED_ALGORITHM, 0, settingsfile);

				 // Tutorial
				 int showTut = GetPrivateProfileIntA("PARK_CLEANUP", OPTIONS_SHOW_TUTORIAL, 0, settingsfile);

				 cb_update->Checked = showTut;

				 // Auto update value
				 int checkIfUpdate = GetPrivateProfileIntA("PARK_CLEANUP", OPTIONS_UPDATE, 1, settingsfile);
				 currentChannel = GetPrivateProfileIntA("PARK_CLEANUP", OPTIONS_CHANNEL, 0, settingsfile);

				 if(IsDebugEnabled())
				 {
					 dd_operatingmode->Text = "Development edition";
					 cb_advanced_algorithm->Visible = true;
					 cb_advanced_algorithm->Enabled = true;
					 this->Text += " - Development edition";

					 int hideDevWarning = GetPrivateProfileIntA("PARK_CLEANUP", OPTIONS_SHOW_DEVWARNING, 0, settingsfile);
					 if(hideDevWarning == 0){			
						 if(MsgExt("Warning\r\n\r\nYou are currently running Park CleanUp development edition. In this edition Park CleanUp will update automatically to the newest public development editions. This edition might have bugs that are able to corrupt parkfiles and/or your RCT3 installation.\r\n\r\nUSE AT YOUR OWN RISC OR DISABLE THE DEVELOPMENT EDITION!", "Warning about the Park CleanUp debug/development edition", MB_OKCANCEL, MessageBoxIcon::Warning)
							 == System::Windows::Forms::DialogResult::Cancel)
						 {
							 WritePrivateProfileInt("PARK_CLEANUP", OPTIONS_CHANNEL, PCU_Channels::DEFAULT, settingsfile);
							 Application::Restart();
							 return;
						 }
						 WritePrivateProfileStringA("PARK_CLEANUP", OPTIONS_SHOW_DEVWARNING, "1", settingsfile);
					 }

				 } else
				 {
					 dd_operatingmode->Text = "Default";
					 tab_debug->Parent = tabctrlhidden;
					 //tab_live->Parent = tabctrlhidden;
					 //tab_removal->Parent = tabctrlhidden;
				 }

				 if(checkIfUpdate == 1 || IsDebugEnabled())
					 CheckForUpdates(true);				 

				 // Set autoupdate checkbox to right checkedstate
				 if(checkIfUpdate)
					 cb_update->Checked = true;
				 else
					 cb_update->Checked = false;

				 // Aggressive compilation
				 aggressiveCompilation = GetPrivateProfileIntA("PARK_CLEANUP", OPTIONS_AGGRESSIVE_COMPILATION, 0, settingsfile);
				 if(aggressiveCompilation)
					 cb_aggressive->Checked = true;

				 // Write tutorial shown
				 WritePrivateProfileStringA("PARK_CLEANUP", OPTIONS_SHOW_TUTORIAL, "1", settingsfile);

				 // Warning about usage
				 int hidewarning = GetPrivateProfileIntA("PARK_CLEANUP", OPTIONS_SHOW_WARNING, 0, settingsfile);
				 if(hidewarning == 0){			
					 MsgExt("Warning\r\n\r\nTHE USE OF THIS TOOL IS AT YOUR OWN RISK! You are responsible for any damage to your system. Keep in mind that your edited parks may not work as expected!\r\n\r\nThis tool is in no way affiliated or associated with Frontier developments ltd, RollerCoaster Tycoon 3 and/or or it's respective companies.", "Warning about Park CleanUp", MB_OK, MessageBoxIcon::Exclamation);
					 WritePrivateProfileStringA("PARK_CLEANUP", "hide_warning", "1", settingsfile);
				 }


				 char *rct3location = new char[MAX_PATH+1];
				 int rctloc = GetPrivateProfileStringA("PARK_CLEANUP", OPTIONS_RCT3PATH, NULL, rct3location, MAX_PATH, settingsfile);
				 if(rctloc == 0)
				 {				
					 HKEY key;
					 unsigned long length = MAX_PATH;
					 LONG res = RegOpenKeyExA(HKEY_LOCAL_MACHINE,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{907B4640-266B-4A21-92FB-CD1A86CD0F63}",0, KEY_QUERY_VALUE, &key);
					 res = RegQueryValueExA(key, "InstallLocation", 0, NULL, (LPBYTE) rct3location, &length);
					 if (res != ERROR_SUCCESS)
					 {
						 res = RegOpenKeyExA(HKEY_LOCAL_MACHINE,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\RollerCoaster Tycoon 3_is1",0, KEY_QUERY_VALUE, &key);
						 res = RegQueryValueExA(key, "InstallLocation", 0, NULL, (LPBYTE) rct3location, &length);
						 if (res != ERROR_SUCCESS)
						 {
							 MsgExt("RCT3 could not be found on your system. Some advanced features require to know where RCT3 is installed.\r\n\r\nA window will open where you can select your RCT3 installation. If you do not want to do this now you can simply press cancel in the next screen. You can always change the RCT3 location in the options tab.", "Warning", MB_OK);

							 WritePrivateProfileStringA("PARK_CLEANUP", "rct3path", getenv("%ProgramFiles%"), settingsfile);
							 rct3location = getenv("%ProgramFiles%");

							 OpenFileDialog ^ofd = gcnew OpenFileDialog();				 
							 ofd->InitialDirectory = "C:\\";
							 ofd->CheckFileExists = true;
							 try {
								 ofd->Filter = "RCT3.exe|rct3.exe;rct3plus.exe";
								 ofd->Title = "Select RCT3.exe";

								 if (ofd->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
									 char rct3folder[MAX_PATH];
									 sprintf_s(rct3folder, MAX_PATH, "%s", ofd->FileName->Replace(ofd->SafeFileName, ""));
									 WritePrivateProfileStringA("PARK_CLEANUP", "rct3path", rct3folder, settingsfile);
									 rct3location = rct3folder;
								 }
							 }
							 finally {
								 delete ofd;
							 }
						 }
					 } else {
						 WritePrivateProfileStringA("PARK_CLEANUP", "rct3path", rct3location, settingsfile);
					 }
				 }
				 tb_rct3loc->Text = gcnew String(rct3location);
				 tb_rct3loc->Text = removeTrailingSlash(tb_rct3loc->Text->ToLower());
				 //tb_rct3los->Text = RemoveTrailingSlash(tb_rct3loc->Text);
				 delete[] rct3location;
				 delete[] settingsfile;	

				 pcuReady = true;

				 if(showTut != 1)
				 {
					 ShowTutorial();
					 LoadFile(true);
				 }
			 }

			 void LoadFile()
			 {
				 LoadFile(false);
			 }

			 void LoadFile(bool force)
			 {
				 if(AppBusy() && !force)
				 {
					 BusyMessageBox();//MessageBox::Show("Please wait until the current task is done!");
					 return;
				 }
				 string filename;				 

				 R3FileDialog^ test = gcnew R3FileDialog(currentFileName);
				 if(test->ShowDialog() == System::Windows::Forms::DialogResult::OK)
				 {
					 fastClean = test->fastClean;
					 loading->Visible = true;
					 ToString(test->FileName, filename);
					 currentFileName = filename;
					 r3arg.Invalidate();
					 r3arg.arc.ClearBuffer();

					 cli::array<Object^>^ parms = gcnew cli::array<Object^> { test->FileName};
					 park_initialiser->RunWorkerAsync(parms);
					 sl_appstatus->Text = "Loading the selected parkfile..";
				 }
			 }

	private: System::Void park_initialiser_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {	
				 errorExecutor->Enabled = true;
				 cli::array<Object^>^ parms = (cli::array<Object^>^)e->Argument;
				 clb_classes->Items->Clear();
				 track_list->Items->Clear();
				 ride_list->Items->Clear();
				 stall_list->Items->Clear();

				 content_listbox->Items->Clear();
				 string filename;
				 ToString((String^)parms[0], filename);
				 r3arg = R3AReader(filename);

				 el.clear();

				 if(r3arg.IsValid())
				 {
					 if(r3arg.IsCorrupted())
					 {
						 MessageBox::Show("Park CleanUp has detected that the file might be corrupted and Park CleanUp and/or RCT3 might give unexpected results with this parkfile", "Corruption detected");
					 }
					 dat_preview_position = 0;
					 clb_classes->BeginUpdate();
					 track_list->BeginUpdate();

					 cdb = r3arg.getclassdata();
					 class_prototype = r3arg.getclassprototypes();

					 bool preview_found = false;
					 TrackUIDs.clear();
					 for(uint32_t j = 0; j < cdb.size(); j++)
					 {
						 String^ itemname = "";
						 string classname = class_prototype[cdb[j].prototype_of].name;
						 if(cdb[j].prototype_of <= class_prototype.size())
						 {
							 itemname = (gcnew String(class_prototype[cdb[j].prototype_of].name.c_str()));
						 }
						 if(classname == "SIDDatabaseEntry" || classname == "AIDDatabaseEntry" || classname == "ASDDatabaseEntry" || classname == "WASDatabaseEntry" || classname == "WAIDatabaseEntry")
						 {
							 itemname += " (";
							 for(uint32_t i = 0; i < cdb[j].RAM.strs.size(); i++)
							 {
								 if(cdb[j].RAM.strs[i].name == StringToWString("OVERLAYFILENAME"))
								 {
									 itemname += gcnew String(cdb[j].RAM.strs[i].value.c_str()) + " ";
								 } else if(cdb[j].RAM.strs[i].name == StringToWString("SYMBOLNAME"))
								 {
									 itemname += gcnew String(cdb[j].RAM.strs[i].value.c_str()) + ":sid";
								 }
							 }
							 itemname += ")";
						 } else if(classname == "Track")
						 {
							 TrackUIDs.push_back(cdb[j].uid);
							 for(uint32_t i = 0; i < cdb[j].RAM.refnames.size(); i++)
							 {
								 if(cdb[j].RAM.refnames[i] == "TrackedRideInstance")
								 {
									 int tri = r3arg.getclassidbyuid(cdb[j].RAM.refs[i]);


									 String^ name = "UNKNOWN";
									 String^ carname = "UNKNOWN";
									 String^ symbolname = "UNKNOWN";
									 for(uint32_t i = 0; i < cdb[tri].RAM.strs.size(); i++)
									 {
										 if(cdb[tri].RAM.strs[i].name == StringToWString("Name"))
										 {
											 name = gcnew String(cdb[tri].RAM.strs[i].value.c_str());
										 } else if(cdb[tri].RAM.strs[i].name == StringToWString("TrackedRideOverlayName"))
										 {
											 carname = gcnew String(cdb[tri].RAM.strs[i].value.c_str());
										 } else if(cdb[tri].RAM.strs[i].name == StringToWString("TrackedRideSymbolName"))
										 {
											 symbolname = gcnew String(cdb[tri].RAM.strs[i].value.c_str());
										 }
									 }

									 bool sceneryexists = true;
									 R3Path^ pth = gcnew R3Path();
									 pth->SetR3Path(tb_rct3loc->Text);
									 pth->AddPath(carname, true);

									 if(!File::Exists(pth->Build()))
										 sceneryexists = false;
									 track_list->Items->Add("\"" + name + "\" with car \"" + carname + "\" of trackedride type \"" + symbolname + "\" " + ((sceneryexists) ? "" : "(Car OVL is missing)"));


								 }
							 }
						 } else if(classname == "AnimatedRideInstance")
						 {
							 RideUIDS.push_back(cdb[j].uid);

							 String^ name = "UNKNOWN";
							 String^ ovl_symbol = "UNKNOWN";
							 String^ ovl_name = "UNKNOWN";
							 for(uint32_t i = 0; i < cdb[j].RAM.strs.size(); i++)
							 {
								 if(cdb[j].RAM.strs[i].name == StringToWString("AnimatedRideSymbolName"))
								 {
									 ovl_symbol = gcnew String(cdb[j].RAM.strs[i].value.c_str());
								 } else if(cdb[j].RAM.strs[i].name == StringToWString("AnimatedRideOverlayName"))
								 {
									 ovl_name = gcnew String(cdb[j].RAM.strs[i].value.c_str());
								 } else if(cdb[j].RAM.strs[i].name == StringToWString("Name"))
								 {
									 name = gcnew String(cdb[j].RAM.strs[i].value.c_str());
								 }
							 }

							 bool sceneryexists = true;
							 R3Path^ pth = gcnew R3Path();
							 pth->SetR3Path(tb_rct3loc->Text);
							 pth->AddPath(ovl_name, true);

							 if(!File::Exists(pth->Build()))
								 sceneryexists = false;

							 ride_list->Items->Add("\"" + name + "\" of ridetype \"" + ovl_symbol + "\" located in \"" + ovl_name + "\" " + ((sceneryexists) ? "" : "(Ride OVL is missing)"));
						 } else if(classname == "StallInstance")
						 {
							 StallUIDS.push_back(cdb[j].uid);

							 String^ name = "UNKNOWN";
							 String^ ovl_symbol = "UNKNOWN";
							 String^ ovl_name = "UNKNOWN";
							 for(uint32_t i = 0; i < cdb[j].RAM.strs.size(); i++)
							 {
								 if(cdb[j].RAM.strs[i].name == StringToWString("StallSymbolName"))
								 {
									 ovl_symbol = gcnew String(cdb[j].RAM.strs[i].value.c_str());
								 } else if(cdb[j].RAM.strs[i].name == StringToWString("StallOverlayName"))
								 {
									 ovl_name = gcnew String(cdb[j].RAM.strs[i].value.c_str());
								 } else if(cdb[j].RAM.strs[i].name == StringToWString("Name"))
								 {
									 name = gcnew String(cdb[j].RAM.strs[i].value.c_str());
								 }
							 }

							 bool sceneryexists = true;
							 R3Path^ pth = gcnew R3Path();
							 pth->SetR3Path(tb_rct3loc->Text);
							 pth->AddPath(ovl_name, true);

							 if(!File::Exists(pth->Build()))
								 sceneryexists = false;

							 stall_list->Items->Add("\"" + name + "\" of stalltype \"" + ovl_symbol + "\" located in \"" + ovl_name + "\" " + ((sceneryexists) ? "" : "(Stall OVL is missing)"));
						 }

						 if(cdb[j].RAM.datpreview_positions.size() && !preview_found)
						 {
							 dat_preview_position = cdb[j].RAM.datpreview_positions[0];
							 r3arg.arc.seek(cdb[j].RAM.datpreview_positions[0], BFR::BEGIN);

							 Bitmap^ tbmp = gcnew Bitmap(74, 58, System::Drawing::Imaging::PixelFormat::Format24bppRgb);
							 int total = 0;
							 for(unsigned int l = 0; l < 58; l++){
								 for(unsigned int j = 0; j < 74; j++){
									 r3arg.arc.read((char*)&total, 3);
									 Color rep = Color::FromArgb(total);
									 tbmp->SetPixel(j, l, rep);
								 }
							 }							 
							 tbmp->RotateFlip(System::Drawing::RotateFlipType::RotateNoneFlipY);

							 park_prev->Image = tbmp;
							 GraphicsUnit units = GraphicsUnit::Point;
							 Bitmap^ tbmp2 = tbmp->Clone(tbmp->GetBounds(units), tbmp->PixelFormat);
							 SetPreviewImage(tbmp2);
							 preview_found = true;							 
						 }
						 string size_indicator = "";
						 double size = cdb[j].end - cdb[j].start;
						 size = ByteSizeConverter(size, size_indicator);
						 itemname += " - " + size + gcnew String(size_indicator.c_str());

						 //array<Object^>^ parms = gcnew array<Object^> { itemname };
						 //clb_classes->Invoke(gcnew d_AddItem(this, &Form1::AddItem), parms);
						 clb_classes->Items->Add(itemname);
						 /*for(uint32_t i = 0; i < cdb[j].RAM.managedimages.size(); i++)
						 {
						 r3arg.arc.seek(cdb[j].RAM.managedimages[i], BFR::BEGIN);
						 r3arg.arc.getu32();
						 r3arg.arc.getu32();
						 r3arg.arc.getu32();
						 r3arg.arc.getu32();
						 uint32_t size = r3arg.arc.getu32();
						 uint32_t sizert = sqrt32((long)size);

						 //if(sizert*sizert > size+2 || sizert*sizert < size-2)
						 //{
						 MessageBox::Show("Not the right size!");
						 //} else {
						 Bitmap^ tbmp = gcnew Bitmap(sizert, sizert, System::Drawing::Imaging::PixelFormat::Format24bppRgb);
						 int total = 0;
						 for(unsigned int l = 0; l < sizert; l++){
						 for(unsigned int j = 0; j < sizert; j++){
						 r3arg.arc.read((char*)&total, 4);
						 Color rep = Color::FromArgb(total);
						 tbmp->SetPixel(j, l, rep);
						 }
						 }
						 tbmp->RotateFlip(System::Drawing::RotateFlipType::RotateNoneFlipY);
						 wildicon->Image = tbmp;
						 //MessageBox::Show("Right size!");
						 //}
						 }*/
					 }

					 clb_classes->EndUpdate();
					 if(clb_classes->Items->Count > 0)
						 clb_classes->SetSelected(0, true);
					 track_list->EndUpdate();
					 if(track_list->Items->Count > 0)
						 track_list->SetSelected(0, true);


					 String^ fname = Path::GetFileName(gcnew String(filename.c_str()));
					 String^ parkname = "Unknown";

					 bool stop = false;
					 for(uint32_t j = 0; j < cdb.size(); j++)
					 {
						 if(cdb[j].prototype_of <= class_prototype.size())
						 {
							 if(class_prototype[cdb[j].prototype_of].name == "GUI2_DatDesc")
							 {								
								 for(uint32_t i = 0; i < cdb[j].RAM.strs.size(); i++)
								 {
									 if(cdb[j].RAM.strs[i].name == StringToWString("DatParkName"))
									 {
										 parkname = gcnew String(cdb[j].RAM.strs[i].value.c_str());
										 stop = true;
									 }
									 Debug::WriteLine(gcnew String(cdb[j].RAM.strs[i].name.c_str()));
									 if(stop)
										 break;
								 }								 
								 if(stop)
									 break;								 
							 }							 
						 }
						 if(stop)
							 break;
					 }
					 String^ addons = "Vanilla (or a cleaned parkfile)";
					 if(r3arg.GetFileVersion() == 28)
						 addons = "Soaked";
					 else if(r3arg.GetFileVersion() == 42)
						 addons = "Wild";
					 clb_classes->Invoke(gcnew d_ChangeLabel(this, &Form1::ChangeLabel), gcnew cli::array<Object^> { (int)Labels::tb_parkinfo, fname + "\r\n" + parkname + "\r\nPark saved with: " + addons });
					 //tb_parkinfo->Text = fname + "\r\n" + parkname + "\r\nPark saved with: " + addons;

					 clb_classes->Invoke(gcnew d_ChangeLabel(this, &Form1::ChangeLabel), gcnew cli::array<Object^> { (int)Labels::sl_appstatus,"" });
					 //sl_appstatus->Text = "";

					 RecalculateFilesize();

					 vector<ParentedReference> c_parented_refs;
					 for(uint32_t j = 0; j < cdb.size(); j++)
					 {
						 for(uint32_t k = 0; k < cdb[j].RAM.refs.size(); k++)
						 {
							 ParentedReference new_parented_ref;
							 new_parented_ref.parent = j;
							 new_parented_ref.a = cdb[j].RAM.refs[k].a;
							 new_parented_ref.b = cdb[j].RAM.refs[k].b;
							 c_parented_refs.push_back(new_parented_ref);
						 }
					 }
					 parented_refs = c_parented_refs;






					 /*try
					 {
					 //vector<ErrorLog> el;
					 //sl_appstatus->Text = "Error checking..";
					 clb_classes->Invoke(gcnew d_ChangeLabel(this, &Form1::ChangeLabel), gcnew array<Object^> { (int)Labels::sl_appstatus,"Error checking" });
					 vector<wstring> values;
					 for(int i = 0; i < clb_classes->Items->Count; i++)
					 {
					 if(!clb_classes->GetItemChecked(i))
					 {
					 for(unsigned int j = 0; j < cdb[i].RAM.strs.size(); j++)
					 {
					 if(cdb[i].RAM.strs[j].name == L"OVERLAYFILENAME")
					 {
					 wstring tlw = cdb[i].RAM.strs[j].value;
					 std::transform(tlw.begin(), tlw.end(), tlw.begin(), ::tolower);
					 bool found = false;
					 for(unsigned int k = 0; k < values.size(); k++)
					 {
					 if(tlw == values[k])
					 {
					 found = true;									 
					 k = 0xFEFEFEFE;
					 }									 
					 }
					 if(!found)
					 {
					 values.push_back(tlw);
					 }
					 }
					 }					 
					 }
					 }				 

					 String^ zipdirectory_temp = "_TEMP_PARKZIP/";
					 String^ resource_folder = "custom objects/";

					 if(values.size())
					 clb_classes->Invoke(gcnew d_ChangeLabel(this, &Form1::ChangeLabel), gcnew array<Object^> { (int)Labels::sl_appstatus,"[2/3] Resolving internal OVL references.." });
					 //sl_appstatus->Text = "[2/3] Resolving internal OVL references..";
					 vector<string> handledfiles;

					 Stream^ stream = (Assembly::GetExecutingAssembly())->GetManifestResourceStream("strdmp.txt");
					 StreamReader^ streamreader = (gcnew StreamReader(stream));
					 String^ txtdata = streamreader->ReadToEnd();
					 array<String^>^ strs = txtdata->Split('\x0a');

					 for(int i = 0; i < strs->Length; i++)
					 {
					 string toadd;
					 String^ pth = strs[i]->Replace("\x0d", "");
					 pth = pth->Replace(".common.ovl", "");
					 //MessageBox::Show("'" + pth + "'");
					 try
					 {
					 pth = Path::Combine(tb_rct3loc->Text + "\\", pth);
					 pth = pth->Replace("\\\\", "\\");
					 } catch (ArgumentException^ e)
					 {						
					 MessageBox::Show("Argument exception: Could not combine\r\n" + tb_rct3loc->Text + "\r\n" +  pth + "\r\n" +  e->ToString());
					 exit(0);
					 } catch (...)
					 {
					 MessageBox::Show("Could not combine\r\n" + tb_rct3loc->Text + "\r\n" +  pth);
					 exit(0);
					 }
					 ToString(pth, toadd);
					 std::replace(toadd.begin(), toadd.end(), '/', '\\');
					 std::transform(toadd.begin(), toadd.end(), toadd.begin(), ::tolower);
					 handledfiles.push_back(toadd);
					 }
					 streamreader->Close();
					 stream->Close();

					 for(int z = 0; z < values.size(); z++)
					 {
					 if(File::Exists(Path::Combine(tb_rct3loc->Text, gcnew String(values[z].c_str())) + ".common.ovl") &&
					 File::Exists(Path::Combine(tb_rct3loc->Text, gcnew String(values[z].c_str())) + ".unique.ovl"))
					 {
					 vector<string> parents;
					 string curovlname;
					 ToString(Path::Combine(tb_rct3loc->Text, gcnew String(values[z].c_str())), curovlname);						 
					 HandleOVL(curovlname, zipdirectory_temp, resource_folder, el, handledfiles, parents, false);
					 } else {
					 vector<string> parents;
					 el.push_back(ErrorLog(gcnew String(values[z].c_str()), "OVL not found", parents));
					 }
					 int val = (((double) z) / (values.size())*100);
					 SetStatusValue(val);
					 }

					 errortree->Invoke(gcnew d_HandleErrorLog(this, &Form1::HandleErrorLog), gcnew array<Object^> {});
					 /*if(el.size() > 0)
					 {
					 Collections::ArrayList^ errornodes = gcnew Collections::ArrayList();
					 std::sort(el.begin(), el.end(), el[0]);
					 for(int i = 0; i < el.size(); i++)
					 {
					 String^ output = "";
					 if(el[i].parents.size() > 0)
					 output += gcnew String(el[i].parents[0].c_str()) + " referring to ~";
					 output += gcnew String(el[i].filename.c_str()) + ": " + gcnew String(el[i].error.c_str()) + Environment::NewLine;
					 errornodes->Add(gcnew TreeNode(output));
					 clb_classes->Invoke(gcnew d_AddNode(this, &Form1::AddNode), gcnew array<Object^> { (int)Labels::errortree,gcnew TreeNode(output) });
					 //errornodes->n
					 //errortree->Nodes->Add(output);
					 }
					 MessageBox::Show("Errors were detected while zipping the file, see the ErrorLog.txt file in the ZIP for more details.");
					 }*/
					 //errortree->invo
					 /*} catch (...)
					 {
					 MessageBox::Show("Something went wrong when checking the file for errors");
					 }*/




					 clb_classes->Invoke(gcnew d_ChangeLabel(this, &Form1::ChangeLabel), gcnew cli::array<Object^> { (int)Labels::sl_appstatus,"" });

					 UpdateChecklist();
					 RenderChecklist();

					 graphDrawer->RunWorkerAsync();
				 } else {
					 MessageBox::Show("The loaded file was invalid!");
				 }
				 //loading->Visible = false;
				 loading->Invoke(gcnew d_ChangeVisible(this, &Form1::ChangeVisible), gcnew cli::array<Object^> { (int)Labels::loading, false });

				 if(fastClean)
				 {
					 loading->Invoke(gcnew d_ChangeVisible(this, &Form1::ChangeVisible), gcnew cli::array<Object^> { (int)Labels::loading, true });
					 loading->Invoke(gcnew d_ChangeVisible(this, &Form1::ChangeVisible), gcnew cli::array<Object^> { (int)Labels::pb_status, true });
					 pb_status->Value = 0;
					 unused_item_selector->RunWorkerAsync();
				 }				 
			 }
	private: System::Void toolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void unused_item_selector_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
				 errorExecutor->Enabled = true;
				 sl_appstatus->Text = "Busy selecting unused items";
				 int selected_item_count = 0;
				 if(cb_advanced_algorithm->Checked)
					 UpdateChecklist();

DefaultCleanupRescan:
				 if(r3arg.IsValid())
				 {
					 cdb = r3arg.getclassdata();
					 class_prototype = r3arg.getclassprototypes();
					 //clb_classes->Invoke(gcnew d_ChangeValue(this, &Form1::ChangeValue), gcnew array<Object^> { (int)Labels::pb_status, clb_classes->Items->Count });
					 pb_status->Maximum = clb_classes->Items->Count;
					 int progg = 0;
					 if(selectmode == SM::SELECT_NONINSTALLED_CONTENT)
					 {
						 vector<DatablockReference> dbrefs;
						 vector<wstring> values;
						 vector<wstring> error_ovls;
#pragma omp parallel for
						 for(int i = 0; i < clb_classes->Items->Count; i++)
						 {
#pragma omp atomic
							 progg++;
							 if ( progg % 50 == 0 )
								 clb_classes->Invoke(gcnew d_ChangeValue(this, &Form1::ChangeValue), gcnew cli::array<Object^> { (int)Labels::pb_status, progg });
							 //pb_status->Value = progg;
							 if( progg % 500 == 0)
								 RecalculateFilesize();

							 if(!clb_classes->GetItemChecked(i))
							 {
								 for(int j = 0; j < cdb[i].RAM.strs.size(); j++)
								 {
									 if(cdb[i].RAM.strs[j].name == L"OVERLAYFILENAME")
									 {
										 wstring tlw = cdb[i].RAM.strs[j].value;
										 std::transform(tlw.begin(), tlw.end(), tlw.begin(), ::tolower);
										 bool found = false;
										 for(unsigned int k = 0; k < values.size(); k++)
										 {
											 if(tlw == values[k])
											 {
												 found = true;									 
												 k = 0xFEFEFEFE;
											 }									 
										 }
										 if(!found)
										 {							
											 String^ ovlpath = Path::Combine(tb_rct3loc->Text, gcnew String(tlw.c_str()));
											 ovlpath = ovlpath->Replace("/","\\");
											 ovlpath += ".common.ovl";
											 if(!File::Exists(ovlpath))
											 {
												 error_ovls.push_back(tlw);
												 List_CheckItem(i, true);
											 }
											 //content_listbox->Items->Add(gcnew String(tlw.c_str()) + "\r\n");
											 values.push_back(tlw);
										 }
									 }
								 }					 
							 }
						 }
					 } else 
					 {
#pragma omp parallel for

						 for(int i = 0; i < clb_classes->Items->Count; i++)
						 {
#pragma omp atomic
							 progg++;
							 if ( progg % 5 == 0 )
								 clb_classes->Invoke(gcnew d_ChangeValue(this, &Form1::ChangeValue), gcnew cli::array<Object^> { (int)Labels::pb_status, progg });
							 //pb_status->Value = progg;
							 if( progg % 500 == 0)
								 RecalculateFilesize();
							 bool found = false;						 
							 bool name_checked = false;

							 if(!clb_classes->GetItemChecked(i))
							 {
								 if(cb_advanced_algorithm->Checked)
									 found = IsReferencedToAndCheckedCached(cdb[i].uid);
								 else
									 found = IsReferencedTo(cdb[i].uid);
								 if(cdb[i].prototype_of <= class_prototype.size())
								 {
									 string class_prototype_name = class_prototype[cdb[i].prototype_of].name;
									 if(selectmode == SM::DEFAULT)
									 {								 
										 if(class_prototype_name == "SIDDatabaseEntry")
											 name_checked = true;
										 else if(class_prototype_name == "CIDDatabaseEntry")
											 name_checked = true;
										 else if(class_prototype_name == "CEDDatabaseEntry")
											 name_checked = true;
										 else if(class_prototype_name == "WAIDatabaseEntry")
											 name_checked = true;
										 else if(class_prototype_name == "WASDatabaseEntry")
											 name_checked = true;
										 else if(class_prototype_name == "AIDDatabaseEntry")
											 name_checked = true;
										 else if(class_prototype_name == "ASDDatabaseEntry")
											 name_checked = true;
										 else if(class_prototype_name == "PPGDatabaseEntry")
											 name_checked = true;
										 else if(class_prototype_name == "LaserWritingLetter")
											 name_checked = true;
										 else if(class_prototype_name == "FlyingCameraRoute")
											 name_checked = true;
										 else if(class_prototype_name == "SceneryItem")
											 name_checked = true;
										 else if(class_prototype_name == "SavedSelection")
											 name_checked = true;

										 //New in 2.1
										 else if(class_prototype_name == "InventibleAnimation")
											 name_checked = true;
										 else if(class_prototype_name == "PathTypeDatabaseEntry")
											 name_checked = true;
										 else if(class_prototype_name == "QueueTypeDatabaseEntry")
											 name_checked = true;
										 //else if(class_prototype_name == "GUI2_DatDesc")
										 //name_checked = true;
										 else if(class_prototype_name == "EnclosureTypeDatabaseEntry")
											 name_checked = true;

										 // Crashes game
										 /*else if(class_prototype_name == "ParticleEmitterDesc")
										 name_checked = true;
										 else if(class_prototype_name == "ParticleDesc")
										 name_checked = true;
										 else if(class_prototype_name == "ParticleSource")
										 name_checked = true;
										 else if(class_prototype_name == "ParticleBaseDesc")
										 name_checked = true;*/

										 /*else if(class_prototype_name == "PoolSurfaces")
										 name_checked = true;
										 //EXPERIMENTAL!
										 else if(class_prototype_name == "EnclosureManager")
										 name_checked = true;
										 else if(class_prototype_name == "WaterManager")
										 name_checked = true;
										 else if(class_prototype_name == "GUI2_DatDesc")
										 name_checked = true;*/

										 /* Check Vanille/style */
										 for(int j = 0; j < cdb[i].RAM.strs.size(); j++)
										 {
											 if(cdb[i].RAM.strs[j].name == L"OVERLAYFILENAME")
											 {
												 //TODO: Check if doesn't conflict when removed!
												 /*if(cdb[i].RAM.strs[j].value == L"Style\\Vanilla\\style")
												 {
												 name_checked = false;
												 j = 0xFFFFFE;
												 break;
												 }*/
											 }
										 }
									 } else if(selectmode == SM::SELECT_FROM_STRINGNAME)
									 {
										 if(class_prototype_name == sm_select_name)
											 name_checked = true;
									 } else if(selectmode == SM::SELECT_FIREWORK_RELATED)
									 {
										 if(class_prototype_name == "IEUVProvider")
											 name_checked = true;
										 else if(class_prototype_name == "IEUV")
											 name_checked = true;
										 else if(class_prototype_name == "FireworkDisplay")
											 name_checked = true;
										 else if(class_prototype_name == "Blender")
											 name_checked = true;
										 else if(class_prototype_name == "WaterJetBaseDesc")
											 name_checked = true;
										 else if(class_prototype_name == "ColourBlenderElement")
											 name_checked = true;
										 else if(class_prototype_name == "FloatBlenderElement")
											 name_checked = true;
										 else if(class_prototype_name == "WaterJetDesc")
											 name_checked = true;
									 } else if(selectmode == SM::SELECT_GUEST_ITEMS)
									 {
										 found = false;
										 if(class_prototype_name == "GuestGroup")
											 name_checked = true;
										 else if(class_prototype_name == "Guest")
											 name_checked = true;
										 else if(class_prototype_name == "GuestHolders")
											 name_checked = true;
										 else if(class_prototype_name == "PersistentObjectsSeen")
											 name_checked = true;
									 } else if(selectmode == SM::DUCK)
									 {
										 found = false;
										 if(class_prototype_name == "Duck")
											 name_checked = true;
									 }
								 }

								 if(!found && name_checked){
									 List_CheckItem(i, true);
									 selected_item_count++;
								 }
							 }
						 }
					 }
					 RecalculateFilesize();
					 if(cb_advanced_algorithm->Checked && selected_item_count > 10)
					 {
						 System::Windows::Forms::DialogResult result;
						 result = MessageBox::Show("Due to the amount of items selected during the last cleanup scan, you might want to rescan the file once again.\r\n\r\nWould you like to start another cleanup scan?\r\nBy clicking yes another cleanup scan will start and the file might be even more optimised. Usually 2 cleanup scans should be enough", "Multiple passes", MessageBoxButtons::YesNo);
						 if ( result == System::Windows::Forms::DialogResult::Yes )
						 {
							 UpdateChecklist();
							 selected_item_count = 0;
							 goto DefaultCleanupRescan;
						 }
					 }
				 } else {
					 MessageBox::Show("Open a parkfile first!");
				 }
				 RenderChecklist();
				 loading->Visible = false;
				 pb_status->Visible = false;
				 pb_status->Value = 0;
				 sl_appstatus->Text = "";
				 selectmode = SM::DEFAULT;	

				 if(fastClean)
				 {
					 bool overwriteBackup = false;
					 String^ fname = gcnew String(currentFileName.c_str());
					 if(File::Exists(fname->Replace(".dat", ".dat.parkcleanup.bak")))
					 {
						 if(MessageBox::Show("Park CleanUp wants to create a backup of the park before the fastcleaned parkfile will be saved. However, a backup already exists, do you want to overwrite it?", "Overwrite older backup", System::Windows::Forms::MessageBoxButtons::YesNo) == System::Windows::Forms::DialogResult::Yes)
						 {
							 overwriteBackup = true;
						 }
					 } else
						 overwriteBackup = true;

					 if(overwriteBackup)
					 {
						 File::Copy(fname, fname->Replace(".dat", ".dat.parkcleanup.bak"), true);
						 string fname2;
						 ToString(fname, fname2);
						 SaveCompressedParkfileAs(fname2);					 
						 MessageBox::Show("The selected park has been fastcleaned. A backup of the old parkfile has been saved to:\r\n\r\n" + fname->Replace(".dat", ".dat.parkcleanup.bak"));
					 }

					 fastClean = false;
					 LoadFile(true);
				 }
			 }	
	private: System::Void btn_select_unused_items_Click(System::Object^  sender, System::EventArgs^  e) {				 
				 SelectPredefinedItems();
			 }
	private: System::Void btn_deselect_items_Click(System::Object^  sender, System::EventArgs^  e) {
				 DeselectItems();
			 }
	private: System::Void check_change(System::Object^  sender, System::EventArgs^  e) {
				 SaveOptions();
			 }
	private: System::Void clb_classes_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
				 RecalculateFilesize();
			 }
	private: System::Void sl_gui2datdesc_1_Click(System::Object^  sender, System::EventArgs^  e) {
				 sm_select_name = "GUI2_DatDesc";
				 selectmode = SM::SELECT_FROM_STRINGNAME;
				 SelectPredefinedItems();
			 }
	private: System::Void sl_FlexiColourGroupCache_1_Click(System::Object^  sender, System::EventArgs^  e) {
				 sm_select_name = "FlexiColourGroupCache";
				 selectmode = SM::SELECT_FROM_STRINGNAME;
				 SelectPredefinedItems();
			 }
	private: System::Void sl_blender_1_Click(System::Object^  sender, System::EventArgs^  e) {
				 sm_select_name = "Blender";
				 selectmode = SM::SELECT_FROM_STRINGNAME;
				 SelectPredefinedItems();
			 }
	private: System::Void sl_firework_1_Click(System::Object^  sender, System::EventArgs^  e) {
				 selectmode = SM::SELECT_FIREWORK_RELATED;
				 SelectPredefinedItems();
			 }
	private: System::Void btn_save_Click(System::Object^  sender, System::EventArgs^  e) {
				 SaveFile();
			 }
	private: System::Void sf_defragged_Click(System::Object^  sender, System::EventArgs^  e) {
				 SaveFile();
			 }
	private: System::Void sl_missingcustomcontent_1_Click(System::Object^  sender, System::EventArgs^  e) {
				 selectmode = SM::SELECT_NONINSTALLED_CONTENT;
				 SelectPredefinedItems();
			 }
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenFileDialog ^ofd = gcnew OpenFileDialog();				 
				 ofd->InitialDirectory = "C:\\";
				 if(tb_rct3loc->Text != "")
					 ofd->InitialDirectory = tb_rct3loc->Text;
				 ofd->CheckFileExists = true;
				 try {
					 ofd->Filter = "RCT3.exe|rct3.exe;rct3plus.exe";
					 ofd->Title = "Select RCT3.exe";

					 if (ofd->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
						 tb_rct3loc->Text = ofd->FileName->Replace(ofd->SafeFileName, "")->ToLower();
						 tb_rct3loc->Text = removeTrailingSlash(tb_rct3loc->Text);

						 char rct3folder[MAX_PATH];
						 char* settingsfile = ToChar(Path::GetDirectoryName(Application::ExecutablePath) + gcnew String("/options.ini"));
						 sprintf_s(rct3folder, MAX_PATH, "%s", ofd->FileName->Replace(ofd->SafeFileName, ""));
						 WritePrivateProfileStringA("PARK_CLEANUP", "rct3path", rct3folder, settingsfile);
					 }
				 }
				 finally {
					 delete ofd;
				 }
			 }
	private: System::Void textBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void splitButton1_Click(System::Object^  sender, System::EventArgs^  e) {
				 if(AppBusy())
				 {
					 BusyMessageBox();//MessageBox::Show("Please wait until the current task is done!");
					 return;
				 }
				 if(!r3arg.IsValid())
				 {
					 MessageBox::Show("Open a parkfile first!");
					 return;
				 }
				 if (MessageBox::Show("It is recommend to clean your parkfile before creating this list in order to get accurate results.\r\n\r\nDo you want to continue?", "Cleaned parkfile", MessageBoxButtons::YesNo) == System::Windows::Forms::DialogResult::No)
				 {
					 return;
				 }
				 content_listbox->Items->Clear();
				 pb_status->Maximum = 100;
				 loading->Visible = true;
				 pb_status->Visible = true;
				 pb_status->Value = 0;
				 sl_appstatus->Text = "Busy creating list of all required custom content";

				 cli::array<Object^>^ parms = gcnew cli::array<Object^> { false, false, ""};
				 filescan->RunWorkerAsync(parms);
				 return;

				 content_listbox->BeginUpdate();

				 /*

				 vector<string> parents;
				 string curovlname;
				 ToString(Path::Combine(tb_rct3loc->Text, gcnew String(values[z].c_str())), curovlname);						 
				 HandleOVL(curovlname, zipdirectory_temp, resource_folder, el, handledfiles, parents, true);

				 */
				 /* el.clear();

				 vector<wstring> values;
				 for(int i = 0; i < clb_classes->Items->Count; i++)
				 {
				 if(!clb_classes->GetItemChecked(i))
				 {
				 for(unsigned int j = 0; j < cdb[i].RAM.strs.size(); j++)
				 {
				 if(cdb[i].RAM.strs[j].name == L"OVERLAYFILENAME")
				 {
				 wstring tlw = cdb[i].RAM.strs[j].value;
				 std::transform(tlw.begin(), tlw.end(), tlw.begin(), ::tolower);
				 bool found = false;
				 for(unsigned int k = 0; k < values.size(); k++)
				 {
				 if(tlw == values[k])
				 {
				 found = true;									 
				 k = 0xFEFEFEFE;
				 }									 
				 }
				 if(!found)
				 {							
				 String^ ovlpath = Path::Combine(tb_rct3loc->Text, gcnew String(tlw.c_str()));
				 ovlpath = ovlpath->Replace("/","\\");
				 ovlpath += ".common.ovl";
				 if(!File::Exists(ovlpath))
				 {
				 content_listbox->Items->Add(gcnew String(tlw.c_str()) + "\r\n");
				 }
				 /* else {
				 vector<string> parents;
				 vector<string> handledfiles;
				 string curovlname;
				 ToString(ovlpath, curovlname);						 
				 HandleOVL(curovlname, "", "", el, handledfiles, parents, false);
				 }*/
				 /*values.push_back(tlw);
				 }
				 }
				 }					 
				 }
				 }
				 content_listbox->EndUpdate();*/
			 }
	private: System::Void scanForOVLFilesThatAreUsedButcurrentlyNotInstalledToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 if(AppBusy())
				 {
					 BusyMessageBox();//MessageBox::Show("Please wait until the current task is done!");
					 return;
				 }
				 if(!r3arg.IsValid())
				 {
					 MessageBox::Show("Open a parkfile first!");
					 return;
				 }
				 if (MessageBox::Show("It is recommend to clean your parkfile before creating this list in order to get accurate results.\r\n\r\nDo you want to continue?", "Cleaned parkfile", MessageBoxButtons::YesNo) == System::Windows::Forms::DialogResult::No)
				 {
					 return;
				 }

				 content_listbox->Items->Clear();
				 pb_status->Maximum = 100;
				 loading->Visible = true;
				 pb_status->Visible = true;
				 pb_status->Value = 0;
				 sl_appstatus->Text = "Busy creating list of all referenced custom content";

				 cli::array<Object^>^ parms = gcnew cli::array<Object^> { true, false, ""};
				 filescan->RunWorkerAsync(parms);
				 return;

				 content_listbox->BeginUpdate();


				 /*content_listbox->Items->Clear();
				 content_listbox->BeginUpdate();

				 vector<wstring> values;
				 for(int i = 0; i < clb_classes->Items->Count; i++)
				 {
				 if(!clb_classes->GetItemChecked(i))
				 {
				 for(unsigned int j = 0; j < cdb[i].RAM.strs.size(); j++)
				 {
				 if(cdb[i].RAM.strs[j].name == L"OVERLAYFILENAME")
				 {
				 wstring tlw = cdb[i].RAM.strs[j].value;
				 std::transform(tlw.begin(), tlw.end(), tlw.begin(), ::tolower);
				 bool found = false;
				 for(unsigned int k = 0; k < values.size(); k++)
				 {
				 if(tlw == values[k])
				 {
				 found = true;									 
				 k = 0xFEFEFEFE;
				 }									 
				 }
				 if(!found)
				 {							
				 String^ ovlpath = Path::Combine(tb_rct3loc->Text, gcnew String(tlw.c_str()));
				 ovlpath = ovlpath->Replace("/","\\");
				 ovlpath += ".common.ovl";
				 content_listbox->Items->Add(gcnew String(tlw.c_str()) + "\r\n");
				 values.push_back(tlw);
				 }
				 }
				 }					 
				 }
				 }
				 content_listbox->EndUpdate();*/

				 /*if(AppBusy())
				 {
				 MessageBox::Show("Please wait until the current task is done!");
				 return;
				 }
				 if(!r3arg.IsValid())
				 {
				 MessageBox::Show("Open a parkfile first!");
				 return;
				 }
				 if (MessageBox::Show("It is recommend to clean your parkfile before creating this list.\r\n\r\nDo you want to continue?", "Cleaned parkfile", MessageBoxButtons::YesNo) == System::Windows::Forms::DialogResult::No)
				 {
				 return;
				 }
				 content_listbox->Items->Clear();
				 content_listbox->BeginUpdate();

				 vector<wstring> values;
				 for(int i = 0; i < clb_classes->Items->Count; i++)
				 {
				 if(!clb_classes->GetItemChecked(i))
				 {
				 for(unsigned int j = 0; j < cdb[i].RAM.strs.size(); j++)
				 {
				 if(cdb[i].RAM.strs[j].name == L"OVERLAYFILENAME")
				 {
				 wstring tlw = cdb[i].RAM.strs[j].value;
				 std::transform(tlw.begin(), tlw.end(), tlw.begin(), ::tolower);
				 bool found = false;
				 for(unsigned int k = 0; k < values.size(); k++)
				 {
				 if(tlw == values[k])
				 {
				 found = true;									 
				 k = 0xFEFEFEFE;
				 }									 
				 }
				 if(!found)
				 {							

				 content_listbox->Items->Add(gcnew String(tlw.c_str()) + "\r\n");
				 values.push_back(tlw);
				 }
				 }
				 }					 
				 }
				 }
				 content_listbox->EndUpdate();*/
			 }
	private: System::Void scanlist_copy_Click(System::Object^  sender, System::EventArgs^  e) {
				 String^ copy_data = "";
				 if(content_listbox->Items->Count <= 0)
				 {
					 MessageBox::Show("You must generate the list of required OVL files first!");
					 return;
				 }
				 for(int i = 0; i < content_listbox->Items->Count; i++)
				 {
					 copy_data += content_listbox->Items[i] + Environment::NewLine;
				 }
				 Clipboard::SetText(copy_data);
				 MessageBox::Show("The list has been copied to your clipboard", "Text copied!", MessageBoxButtons::OK);
			 }

			 void SetPreviewImage(Bitmap^ dpreplacer)
			 {
				 if(dpreplacer->Width != 74 || dpreplacer->Height != 58){
					 MsgExt("Error: Bitmap is not the correct size of 74*58 pixels", "Error", NULL);
				 }

				 //Graphics^ g = Graphics::FromImage(dpreplacer);
				 //System::Drawing::Font^ drawFont = gcnew System::Drawing::Font( "Arial",8 );
				 //SolidBrush^ drawBrush = gcnew SolidBrush( Color::Black );
				 //PointF drawPoint = PointF(2,5);
				 //g->DrawString( "DEBUGINFO\r\nPCU " + PCU_VERSION + "\r\nR3PATH " + R3PATH_VERSION + "\r\nR3A " + R3AREADER_VERSION /* + "\r\n@" + d1.Hour + ":" + d1.Minute*/, drawFont, drawBrush, drawPoint );


				 dpreplacer->RotateFlip(System::Drawing::RotateFlipType::RotateNoneFlipY);				 
				 r3arg.arc.seek(dat_preview_position, BFR::BEGIN);
				 unsigned long currentpos = dat_preview_position;
				 bool fail = false;
				 for(int y = 0; y < 58; y++){
					 for(int x = 0; x < 74; x++){
						 Color newcol = dpreplacer->GetPixel(x, y);
						 uint8_t blue = newcol.B;
						 uint8_t green = newcol.G;
						 uint8_t red = newcol.R;

						 if(r3arg.arc.setByte(blue, currentpos) == false)
							 fail = true;
						 currentpos++;
						 if(r3arg.arc.setByte(green, currentpos) == false)
							 fail = true;
						 currentpos++;
						 if(r3arg.arc.setByte(red, currentpos) == false)
							 fail = true;
						 currentpos++;
					 }
				 }
				 //g->~Graphics();
				 if(fail)
					 MessageBox::Show("Error: Something went wrong when trying to write the picture to the parkfile!", "Error!");
				 else
				 {
					 dpreplacer->RotateFlip(System::Drawing::RotateFlipType::RotateNoneFlipY);	
					 park_prev->Image = dpreplacer;
				 }

				 /*
				 dat_preview_position = cdb[j].RAM.datpreview_positions[0];
				 r3arg.arc.seek(cdb[j].RAM.datpreview_positions[0], BFR::BEGIN);

				 Bitmap^ tbmp = gcnew Bitmap(74, 58, System::Drawing::Imaging::PixelFormat::Format24bppRgb);
				 int total = 0;
				 for(unsigned int l = 0; l < 58; l++){
				 for(unsigned int j = 0; j < 74; j++){
				 r3arg.arc.read((char*)&total, 3);
				 Color rep = Color::FromArgb(total);
				 tbmp->SetPixel(j, l, rep);
				 }
				 }
				 tbmp->RotateFlip(System::Drawing::RotateFlipType::RotateNoneFlipY);
				 park_prev->Image = tbmp;

				 preview_found = true;
				 */
			 }
	private: System::Void debug_dump_Click(System::Object^  sender, System::EventArgs^  e) {
				 if(AppBusy())
				 {
					 BusyMessageBox();//MessageBox::Show("Please wait until the current task is done!");
					 return;
				 }
				 if(!r3arg.IsValid())
				 {
					 MessageBox::Show("Open a parkfile first!");
					 return;
				 }

				 char* dumpfile = ToChar(Path::GetDirectoryName(Application::ExecutablePath) + gcnew String("/debug_dump.html"));

				 fstream file;
				 file.open(dumpfile, ios::trunc|ios::out);
				 string tmpstr = r3arg.GetFilename();

				 //do an input or output here
				 file << "<!DOCTYPE HTML><html><head><meta charset='utf-8'><title>Park CleanUp debug dumpfile</title><script src='http://ajax.googleapis.com/ajax/libs/jquery/1.8.1/jquery.min.js'></script><style>.list{border:black thin solid;}</style></head><body>";
				 file << "<h3>Automatic debug-dump of " << tmpstr.c_str() << "</h3>";
				 file << "<pre>";
				 string addons = "Vanilla";
				 if(r3arg.GetFileVersion() == 28)
					 addons = "Soaked";
				 else if(r3arg.GetFileVersion() == 42)
					 addons = "Wild";
				 file << "File format: " << (int)r3arg.GetFileVersion() << "&raquo;" << addons.c_str() << NL;
				 file << "General Archive reader version: " << R3AREADER_VERSION << NL;

				 file << NL;

				 file << "Classprototype-count: " << class_prototype.size() << " <a onclick='$(this).next().toggle();'>&raquo;Show/hide</a><div onclick='$(this).toggle();' class='list' style='display:none'>";
				 for(uint32_t j = 0; j < class_prototype.size(); j++)
				 {
					 file << class_prototype[j].name << NL;
				 }
				 file << "</div>" << NL << NL;

				 file << "Datachunks-count: " << cdb.size() << " <a onclick='$(this).next().toggle();'>&raquo;Show/hide</a><div onclick='$(this).toggle();' class='list' style='display:none'>";
				 for(uint32_t j = 0; j < cdb.size(); j++)
				 {
					 file << class_prototype[cdb[j].prototype_of].name << NL;
				 }
				 file << "</div>" << NL << NL;

				 file << "List of referenced strings <a onclick='$(this).next().toggle();'>&raquo;Show/hide</a><div onclick='$(this).toggle();' class='list' style='display:none'>";
				 for(uint32_t j = 0; j < cdb.size(); j++)
				 {
					 for(uint32_t i = 0; i < cdb[j].RAM.strs.size(); i++)
					 {
						 string str1 = wstringtostring(cdb[j].RAM.strs[i].name);
						 string str2 = wstringtostring(cdb[j].RAM.strs[i].value);
						 file << str1.c_str() << "&raquo;" << str2.c_str() << NL;
					 }
				 }
				 file << "</div>" << NL << NL;


				 /*db = r3arg.getclassdata();
				 class_prototype = r3arg.getclassprototypes();

				 bool preview_found = false;
				 for(uint32_t j = 0; j < cdb.size(); j++)
				 {
				 String^ itemname = "";
				 string classname = class_prototype[cdb[j].prototype_of].name;
				 if(cdb[j].prototype_of <= class_prototype.size())
				 {
				 itemname = (gcnew String(class_prototype[cdb[j].prototype_of].name.c_str()));
				 }
				 if(classname == "SIDDatabaseEntry" || classname == "AIDDatabaseEntry" || classname == "ASDDatabaseEntry" || classname == "WASDatabaseEntry" || classname == "WAIDatabaseEntry")
				 {
				 */

				 file << "</pre>";
				 file << "</body></html>";

				 file.close();

				 delete []dumpfile;
			 }
	private: System::Void park_zipper_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
				 errorExecutor->Enabled = true;
				 cli::array<Object^>^ parms = (cli::array<Object^>^)e->Argument;
				 String^ filename = (String^)parms[0];
				 String^ ziplog = "";	

				 MessageBox::Show("Test!");

				 try
				 {
					 vector<BlueprintFile> files;

					 System::Windows::Forms::DialogResult result = MessageBox::Show("Do you want to use a blueprint?\r\n\r\nTip: Using an RCT3 blueprint created with Park CleanUp you can create a ZIP file for your friends using only the files they actually need. Your friends can create this blueprint file in their Park CleanUp, send it to you and then you use it here.", "Use a blueprint", MessageBoxButtons::YesNo);
					 if ( result == System::Windows::Forms::DialogResult::Yes )
					 {
						 OpenFileDialog ^ofd = gcnew OpenFileDialog();
						 ofd->CheckFileExists = true;

						 try {
							 ofd->Filter = "Blueprint file|*.pcub|All|*.*";

							 ofd->Title = "Select a Park CleanUp blueprint";					 

							 if (ofd->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
								 string name;
								 ToString(ofd->FileName, name);
								 files = readBlueprintsFromFile(name);
							 }
							 else {
								 return;
							 }
						 }
						 finally {
							 delete ofd;
						 }
					 }

					 sl_appstatus->Text = "[1/4] Resolving list of referenced OVL files..";
					 vector<wstring> values;
					 for(int i = 0; i < clb_classes->Items->Count; i++)
					 {
						 if(!clb_classes->GetItemChecked(i))
						 {
							 for(unsigned int j = 0; j < cdb[i].RAM.strs.size(); j++)
							 {
								 if(cdb[i].RAM.strs[j].name == L"OVERLAYFILENAME")
								 {
									 wstring tlw = cdb[i].RAM.strs[j].value;
									 std::transform(tlw.begin(), tlw.end(), tlw.begin(), ::tolower);
									 bool found = false;
									 for(unsigned int k = 0; k < values.size(); k++)
									 {
										 if(tlw == values[k])
										 {
											 found = true;									 
											 k = 0xFEFEFEFE;
										 }									 
									 }
									 if(!found)
									 {
										 values.push_back(tlw);
									 }
								 }
							 }					 
						 }
					 }				 

					 String^ zipdirectory_temp = "_TEMP_PARKZIP/";
					 String^ resource_folder = "custom objects/";
					 if ( !Directory::Exists( zipdirectory_temp ) )
					 {
						 Directory::CreateDirectory(zipdirectory_temp);
					 }
					 string parkfile;
					 ToString(zipdirectory_temp, parkfile);
					 parkfile.append("Park.dat");
					 SaveCompressedParkfileAs(parkfile);

					 if(values.size())
						 sl_appstatus->Text = "[2/4] Resolving internal OVL references..";
					 vector<string> handledfiles;

					 Stream^ stream = (Assembly::GetExecutingAssembly())->GetManifestResourceStream("strdmp.txt");
					 StreamReader^ streamreader = (gcnew StreamReader(stream));
					 String^ txtdata = streamreader->ReadToEnd();
					 cli::array<String^>^ strs = txtdata->Split('\x0a');

					 for(int i = 0; i < strs->Length; i++)
					 {
						 try
						 {
							 string toadd;
							 String^ pth = strs[i]->Replace("\x0d", "");
							 pth = pth->Replace(".common.ovl", "");
							 //MessageBox::Show("'" + pth + "'");
							 try
							 {
								 pth = Path::Combine(tb_rct3loc->Text + "\\", pth);
								 pth = pth->Replace("\\\\", "\\");
							 } catch (ArgumentException^ e)
							 {						
								 MessageBox::Show("Argument exception: Could not combine\r\n" + tb_rct3loc->Text + "\r\n" +  pth + "\r\n" +  e->ToString());
								 exit(0);
							 } catch (...)
							 {
								 MessageBox::Show("Could not combine\r\n" + tb_rct3loc->Text + "\r\n" +  pth);
								 exit(0);
							 }
							 ToString(pth, toadd);
							 std::replace(toadd.begin(), toadd.end(), '/', '\\');
							 std::transform(toadd.begin(), toadd.end(), toadd.begin(), ::tolower);
							 handledfiles.push_back(toadd);
						 } catch (...)
						 {
							 MessageBox::Show("Something unknown went wrong trying to create a list of OVL files", "A bug occured");
						 }
					 }

					 for(int i = 0; i < files.size(); i++)
					 {
						 try
						 {
							 string toadd;
							 String^ pth = gcnew String(files[i].relativename.c_str());
							 pth = pth->Replace(".common.ovl", "");
							 //MessageBox::Show("'" + pth + "'");
							 try
							 {
								 pth = Path::Combine(tb_rct3loc->Text + "\\", pth);
								 pth = pth->Replace("\\\\", "\\");
							 } catch (ArgumentException^ e)
							 {						
								 MessageBox::Show("Argument exception: Could not combine\r\n" + tb_rct3loc->Text + "\r\n" +  pth + "\r\n" +  e->ToString());
								 exit(0);
							 } catch (...)
							 {
								 MessageBox::Show("Could not combine\r\n" + tb_rct3loc->Text + "\r\n" +  pth);
								 exit(0);
							 }
							 ToString(pth, toadd);
							 std::replace(toadd.begin(), toadd.end(), '/', '\\');
							 std::transform(toadd.begin(), toadd.end(), toadd.begin(), ::tolower);
							 handledfiles.push_back(toadd);
						 } catch (...)
						 {
							 MessageBox::Show("Something unknown went wrong trying to create a list of OVL files", "A bug occured");
						 }
					 }
					 streamreader->Close();
					 stream->Close();

					 sl_appstatus->Text = "[3/4] Zipping all dependencies..";

					 for(int z = 0; z < values.size(); z++)
					 {
						 try
						 {
							 vector<string> parents;
							 string curovlname;
							 ToString(Path::Combine(tb_rct3loc->Text, gcnew String(values[z].c_str())), curovlname);	

							 if(File::Exists(Path::Combine(tb_rct3loc->Text, gcnew String(values[z].c_str())) + ".common.ovl") &&
								 File::Exists(Path::Combine(tb_rct3loc->Text, gcnew String(values[z].c_str())) + ".unique.ovl"))
							 {			
								 OutputDebugString(values[z].c_str());
								 HandleOVL(curovlname, zipdirectory_temp, resource_folder, el, handledfiles, parents, true);
								 OutputDebugStringA("File handled!");
							 } else {
								 ziplog += "OVL file not found: " + gcnew String(values[z].c_str()) + Environment::NewLine;
							 }
							 int val = (((double) z) / (values.size())*100);
							 SetStatusValue(val);
						 } catch (...)
						 {
							 MessageBox::Show("Something unknown went wrong trying to handle " +  gcnew String(values[z].c_str()), "A bug occured");
						 }
					 }
					 SetStatusValue(100);

					 ZipFile ^ zip;
					 try
					 {
						 zip = gcnew ZipFile();
						 //zip->Password = "Harbinger";
						 //zip->Encryption = EncryptionAlgorithm::WinZipAes128;		
						 for(unsigned int j = 0; j < values.size(); j++)
						 {		
							 try
							 {
								 if(File::Exists(Path::Combine(tb_rct3loc->Text, gcnew String(values[j].c_str())) + ".common.ovl") &&
									 File::Exists(Path::Combine(tb_rct3loc->Text, gcnew String(values[j].c_str())) + ".unique.ovl"))
								 {
									 String^ copydir = Path::Combine(zipdirectory_temp + resource_folder, gcnew String(values[j].c_str()));	
									 Directory::CreateDirectory(Path::GetDirectoryName(copydir));

									 File::Copy(Path::Combine(tb_rct3loc->Text, gcnew String(values[j].c_str())) + ".common.ovl", copydir + ".common.ovl", true);
									 File::Copy(Path::Combine(tb_rct3loc->Text, gcnew String(values[j].c_str())) + ".unique.ovl", copydir + ".unique.ovl", true);
								 } else {
									 ziplog += "OVL pair incomplete: " + gcnew String(values[j].c_str()) + Environment::NewLine;
								 }
							 } catch (...)
							 {
								 MessageBox::Show("Something unknown went wrong trying to copy " +  gcnew String(values[j].c_str()), "A bug occured");
							 }
						 }
						 String^ readme = "ParkZIP by Park CleanUp 2 beta" + Environment::NewLine;
						 readme += "This zip contains a few files:" + Environment::NewLine;
						 readme += "Park.dat: The parkfile which must be placed in \"Documents\\RCT3\\Parks\"" + Environment::NewLine;
						 readme += (ziplog != "") ? "ErrorLog.txt: A file that shows all errors that occured during saving the parkfile" + Environment::NewLine : "";
						 readme += "[custom object]: Folder containing all required resources in order to run the parkfile";
						 zip->AddEntry("Readme.txt", readme);
						 if(el.size() > 0)
						 {

							 std::sort(el.begin(), el.end(), el[0]);
							 for(int i = 0; i < el.size(); i++)
							 {
								 try
								 {
									 /*bool found = false;
									 for(int j = 0; i < el.size(); j++)
									 {
									 if(el[i].filename ==el[j].filename)
									 {
									 found = true;
									 break;
									 }
									 }
									 if(!found)*/
									 if(el[i].parents.size() > 0)
										 ziplog += gcnew String(el[i].parents[0].c_str()) + " referring to ~";
									 ziplog += gcnew String(el[i].filename.c_str()) + ": " + gcnew String(el[i].error.c_str()) + Environment::NewLine;
								 } catch (...)
								 {
									 MessageBox::Show("Something unknown went wrong trying to resolve references/parents of " +  gcnew String(el[i].filename.c_str()), "A bug occured");
								 }
							 }

							 MessageBox::Show("Errors were detected while zipping the file, see the ErrorLog.txt file in the ZIP for more details.");
							 zip->AddEntry("ErrorLog.txt", ziplog);
						 }

						 sl_appstatus->Text = "[4/4] Zipping all dependencies..";

						 zip->SaveProgress += gcnew EventHandler<SaveProgressEventArgs^>(this, &Form1::SaveProgress);
						 zip->AddDirectory(zipdirectory_temp);
						 zip->Save(filename);					 
					 } catch (...)
					 {
						 MessageBox::Show("An unknown error occured while creating the ZIPfile", "A bug occured");
					 }
					 finally
					 {
						 delete zip;
						 Directory::Delete(zipdirectory_temp, true);
					 }

					 ofstream strdmp("values.txt");
					 if(strdmp.is_open())
					 {
						 for(int i = 0; i < handledfiles.size(); i++)
						 {						 
							 strdmp << handledfiles[i] << "\n";
						 }
						 strdmp.close();
					 }


				 } catch (...)
				 {
					 MessageBox::Show("Something went wrong while creating a parkzip (perhaps a bug in the code).\r\n\r\nDon't worry, your PC should still be working! There is just no ZIP file created.");
				 }

				 /*vector<wstring> values;
				 for(int i = 0; i < clb_classes->Items->Count; i++)
				 {
				 if(!clb_classes->GetItemChecked(i))
				 {
				 for(unsigned int j = 0; j < cdb[i].RAM.strs.size(); j++)
				 {
				 if(cdb[i].RAM.strs[j].name == L"OVERLAYFILENAME")
				 {
				 wstring tlw = cdb[i].RAM.strs[j].value;
				 std::transform(tlw.begin(), tlw.end(), tlw.begin(), ::tolower);
				 bool found = false;
				 for(unsigned int k = 0; k < values.size(); k++)
				 {
				 if(tlw == values[k])
				 {
				 found = true;									 
				 k = 0xFEFEFEFE;
				 }									 
				 }
				 if(!found)
				 {
				 values.push_back(tlw);
				 }
				 }
				 }					 
				 }
				 }				 

				 String^ zipdirectory_temp = "_TEMP_PARKZIP/";
				 String^ resource_folder = "custom objects/";
				 if ( !Directory::Exists( zipdirectory_temp ) )
				 {
				 Directory::CreateDirectory(zipdirectory_temp);
				 }
				 string parkfile;
				 ToString(zipdirectory_temp, parkfile);
				 parkfile.append("Park.dat");
				 SaveCompressedParkfileAs(parkfile);

				 if(values.size())
				 sl_appstatus->Text = "[2/3] Resolving internal OVL references..";
				 vector<string> handledfiles;

				 Stream^ stream = (Assembly::GetExecutingAssembly())->GetManifestResourceStream("strdmp.txt");
				 StreamReader^ streamreader = (gcnew StreamReader(stream));
				 String^ txtdata = streamreader->ReadToEnd();
				 array<String^>^ strs = txtdata->Split('\x0a');

				 for(int i = 0; i < strs->Length; i++)
				 {
				 string toadd;
				 String^ pth = strs[i]->Replace("\x0d", "");
				 pth = pth->Replace(".common.ovl", "");
				 //MessageBox::Show("'" + pth + "'");
				 try
				 {
				 pth = Path::Combine(tb_rct3loc->Text + "\\", pth);
				 } catch (ArgumentException^ e)
				 {						
				 MessageBox::Show("Argument exception: Could not combine\r\n" + tb_rct3loc->Text + "\r\n" +  pth + "\r\n" +  e->ToString());
				 } catch (...)
				 {
				 MessageBox::Show("Could not combine\r\n" + tb_rct3loc->Text + "\r\n" +  pth);
				 }
				 ToString(pth, toadd);
				 std::replace(toadd.begin(), toadd.end(), '/', '\\');
				 std::transform(toadd.begin(), toadd.end(), toadd.begin(), ::tolower);
				 handledfiles.push_back(toadd);
				 }
				 streamreader->Close();
				 stream->Close();

				 for(int z = 0; z < values.size(); z++)
				 {					 					 
				 if(File::Exists(Path::Combine(tb_rct3loc->Text, gcnew String(values[z].c_str())) + ".common.ovl") &&
				 File::Exists(Path::Combine(tb_rct3loc->Text, gcnew String(values[z].c_str())) + ".unique.ovl"))
				 {
				 string curovlname;
				 ToString(Path::Combine(tb_rct3loc->Text, gcnew String(values[z].c_str())), curovlname);						 
				 HandleOVL(curovlname, zipdirectory_temp, resource_folder, el, handledfiles);
				 }
				 int val = (((double) z) / (values.size())*100);
				 SetStatusValue(val);
				 }

				 ZipFile ^ zip;
				 try
				 {
				 zip = gcnew ZipFile();
				 //zip->Password = "Harbinger";
				 //zip->Encryption = EncryptionAlgorithm::WinZipAes128;		
				 for(unsigned int j = 0; j < values.size(); j++)
				 {					 					 
				 if(File::Exists(Path::Combine(tb_rct3loc->Text, gcnew String(values[j].c_str())) + ".common.ovl") &&
				 File::Exists(Path::Combine(tb_rct3loc->Text, gcnew String(values[j].c_str())) + ".unique.ovl"))
				 {
				 String^ copydir = Path::Combine(zipdirectory_temp + resource_folder, gcnew String(values[j].c_str()));	
				 Directory::CreateDirectory(Path::GetDirectoryName(copydir));

				 File::Copy(Path::Combine(tb_rct3loc->Text, gcnew String(values[j].c_str())) + ".common.ovl", copydir + ".common.ovl", true);
				 File::Copy(Path::Combine(tb_rct3loc->Text, gcnew String(values[j].c_str())) + ".unique.ovl", copydir + ".unique.ovl", true);
				 } else {
				 ziplog += "OVL pair incomplete: " + gcnew String(values[j].c_str()) + Environment::NewLine;
				 }
				 }
				 String^ readme = "ParkZIP by Park CleanUp 2 beta";
				 //\r\n\r\n" + 
				 //"This zip contains a few files:\r\n" +
				 //"Park.dat: The parkfile which must be placed in \"Documents\RCT3\Parks\"\r\n" +
				 //(ziplog != "") ? "ErrorLog.txt: A file that shows all errors that occured during saving the parkfile\r\n" : "" + 
				 //"[custom object]: Folder containing all required resources in order to run the parkfile\r\n";
				 zip->AddEntry("Readme.txt", readme);
				 if(el.size() > 0)
				 {
				 std::sort(el.begin(), el.end(), el[0]);
				 for(int i = 0; i < el.size(); i++)
				 {
				 /*bool found = false;
				 for(int j = 0; i < el.size(); j++)
				 {
				 if(el[i].filename ==el[j].filename)
				 {
				 found = true;
				 break;
				 }
				 }
				 if(!found)*/
				 /*ziplog += gcnew String(el[i].filename.c_str()) + ": " + gcnew String(el[i].error.c_str()) + Environment::NewLine;
				 }
				 MessageBox::Show("Errors were detected while zipping the file, see the ErrorLog.txt file in the ZIP for more details.");
				 zip->AddEntry("ErrorLog.txt", ziplog);
				 }

				 sl_appstatus->Text = "[3/3] Zipping all dependencies..";

				 zip->SaveProgress += gcnew EventHandler<SaveProgressEventArgs^>(this, &Form1::SaveProgress);
				 zip->AddDirectory(zipdirectory_temp);
				 zip->Save(filename);					 
				 }
				 finally
				 {
				 delete zip;
				 Directory::Delete(zipdirectory_temp, true);
				 }

				 ofstream strdmp("values.txt");
				 if(strdmp.is_open())
				 {
				 for(int i = 0; i < handledfiles.size(); i++)
				 {						 
				 strdmp << handledfiles[i] << "\n";
				 }
				 strdmp.close();
				 }*/

				 loading->Visible = false;
				 pb_status->Visible = false;
				 pb_status->Value = 0;
				 sl_appstatus->Text = "";
			 }

			 void HandleOVL(string curovlname, String^ zipdirectory_temp, String^ resource_folder, vector<ErrorLog> &el, vector<string> &handledfiles, vector<string> &parents, bool copyfiles)
			 {
				 std::transform(curovlname.begin(), curovlname.end(), curovlname.begin(), ::tolower);
				 std::replace(curovlname.begin(), curovlname.end(), '/', '\\');
				 for(int i = 0; i < handledfiles.size(); i++)
				 {
					 if(handledfiles[i] == curovlname)
						 return;
				 }
				 handledfiles.push_back(curovlname);
				 //ziplog += "Reading OVL [" + gcnew String(curovlname.c_str()) + "]\r\n";
				 OVLReader OVL = OVLReader(curovlname);
				 if(! OVL.IsValid())
				 {
					 string err = OVL.GetLastReadingError();
					 MessageBox::Show("Error: Could not open Overlay.\r\r\n" + gcnew String(err.c_str()));
				 } else {
					 //ziplog += "OVL readed:" + OVL.SVDNames.size() + "\r\n";
					 for(unsigned long i = 0; i < OVL.SVDNames.size(); i++)
					 {
						 R3Path^ pth = gcnew R3Path();
						 pth->SetR3Path(tb_rct3loc->Text);
						 pth->AddPath(gcnew String(OVL.SVDNames[i].path.c_str()), true);

						 //ziplog += "SVD.. [" + pth->Build() + "]\r\n";
						 if(File::Exists(pth->Build()))
						 {
							 pth->SetExtension("");
							 if(File::Exists(pth->Build() + ".common.ovl") &&
								 File::Exists(pth->Build() + ".unique.ovl"))
							 {
								 //ziplog += "Handled! [" + pth->Build() + "]\r\n";
								 String^ copydir = Path::Combine(zipdirectory_temp + resource_folder, pth->BuildWithoutBase());
								 if(copyfiles)
									 Directory::CreateDirectory(Path::GetDirectoryName(copydir));

								 if(copyfiles)
									 File::Copy(pth->Build() + ".common.ovl", copydir + ".common.ovl", true);
								 if(copyfiles)
									 File::Copy(pth->Build() + ".unique.ovl", copydir + ".unique.ovl", true);
								 string newovlname;
								 ToString(pth->BuildWithoutBase(), newovlname);
								 parents.push_back(newovlname);
								 ToString(pth->Build(), newovlname);
								 HandleOVL(newovlname, zipdirectory_temp, resource_folder, el, handledfiles, parents, copyfiles);
							 } else {
								 el.push_back(ErrorLog(pth->BuildWithoutBase(), "OVL pair incomplete (either the .common.ovl or unique.ovl seems to be missing", parents));
								 //ziplog += "OVL pair incomplete: " + pth->Build() + Environment::NewLine;
							 }
						 } else {
							 el.push_back(ErrorLog(pth->BuildWithoutBase(), "OVL not found", parents));
							 //MessageBox::Show("Could not find SVD OVL: " + pth->Build() + "\r\n\r\nReferenced from: " + gcnew String(curovlname.c_str()));
						 }
					 }
					 for(ulong j = 0; j < 2; j++)
					 {
						 //break;
						 OVLData OVLD = OVL.GetOVLD(j);
						 for(int i = 0; i < OVLD.references.size(); i++)
						 {
							 string newcurrentovl;
							 string oldcurrentovl = curovlname;
							 oldcurrentovl.append(".common.ovl");
							 MergePaths(oldcurrentovl, OVLD.references[i].file, newcurrentovl);
							 //MessageBox::Show(gcnew String(curovlname.c_str()) + "\r\n" + gcnew String(OVLD.references[i].file.c_str()) + "\r\n" +  gcnew String(newcurrentovl.c_str()));

							 if(File::Exists(gcnew String(newcurrentovl.c_str()) + ".common.ovl") &&
								 File::Exists(gcnew String(newcurrentovl.c_str()) + ".unique.ovl"))
							 {
								 //ziplog += "Handled! [" + pth->Build() + "]\r\n";
								 //String^ copydir = Path::Combine(zipdirectory_temp + resource_folder, gcnew String(newcurrentovl.c_str()));
								 String^ copydir = gcnew String(newcurrentovl.c_str());// = Path::Combine(zipdirectory_temp + resource_folder, gcnew String(values[j].c_str()));
								 copydir = copydir->ToLower();
								 copydir = copydir->Replace(tb_rct3loc->Text->ToLower() + "/", "");
								 copydir = copydir->Replace(tb_rct3loc->Text->ToLower() + "\\", "");
								 copydir = copydir->Replace(tb_rct3loc->Text->ToLower(), "");
								 copydir = copydir->Replace(".common.ovl", "");

								 string newovlname;
								 ToString(copydir, newovlname);
								 parents.push_back(newovlname);

								 //String^ ovlshortname = copydir;
								 //MessageBox::Show("Debug 1: " + copydir);
								 copydir = Path::Combine(zipdirectory_temp + resource_folder, copydir);
								 try
								 {
									 if(copyfiles)
										 Directory::CreateDirectory(Path::GetDirectoryName(copydir));
								 } catch (...)
								 {
									 MessageBox::Show("Createdirectory error: " + copydir);
								 }
								 try
								 {
									 //MessageBox::Show("Debug 2: " + gcnew String(newcurrentovl.c_str()) + "\r\n" + copydir);
									 if(copyfiles)
										 File::Copy(gcnew String(newcurrentovl.c_str()) + ".common.ovl", copydir + ".common.ovl", true);
									 if(copyfiles)
										 File::Copy(gcnew String(newcurrentovl.c_str()) + ".unique.ovl", copydir + ".unique.ovl", true);
								 } catch (...)
								 {
									 MessageBox::Show("Copy error references\r\nFrom: " + gcnew String(newcurrentovl.c_str()) + ".common.ovl\r\bTo: " + copydir + ".common.ovl");
								 }

								 ToString(gcnew String(newcurrentovl.c_str()), newovlname);
								 HandleOVL(newovlname, zipdirectory_temp, resource_folder, el, handledfiles, parents, copyfiles);
							 } else {
								 el.push_back(ErrorLog(gcnew String(newcurrentovl.c_str()), "OVL pair incomplete", parents));

								 //ziplog += "OVL pair incomplete: " + gcnew String(newcurrentovl.c_str()) + Environment::NewLine;
							 }

							 /*R3Path^ pth = gcnew R3Path();
							 pth->SetR3Path(tb_rct3loc->Text);
							 pth->AddPath(gcnew String(values[z].c_str()), true);
							 pth->AddPath(gcnew String(OVLD.references[i].file.c_str()), true);

							 ziplog += "Reference.. [" + pth->Build() + "]\r\n";
							 if(File::Exists(pth->Build()))
							 {
							 if(File::Exists(Path::Combine(tb_rct3loc->Text, gcnew String(values[j].c_str())) + ".common.ovl") &&
							 File::Exists(Path::Combine(tb_rct3loc->Text, gcnew String(values[j].c_str())) + ".unique.ovl"))
							 {
							 ziplog += "Handled! [" + pth->Build() + "]\r\n";
							 String^ copydir = Path::Combine(zipdirectory_temp + resource_folder, gcnew String(values[j].c_str()));	
							 Directory::CreateDirectory(Path::GetDirectoryName(copydir));

							 File::Copy(Path::Combine(tb_rct3loc->Text, gcnew String(values[j].c_str())) + ".common.ovl", copydir + ".common.ovl", true);
							 File::Copy(Path::Combine(tb_rct3loc->Text, gcnew String(values[j].c_str())) + ".unique.ovl", copydir + ".unique.ovl", true);
							 string newovlname;
							 ToString(gcnew String(values[j].c_str()) + ".common.ovl", newovlname);
							 HandleOVL(newovlname, values, z, zipdirectory_temp, resource_folder, ziplog, hashes);
							 } else {
							 ziplog += "OVL pair incomplete: " + gcnew String(values[j].c_str()) + Environment::NewLine;
							 }
							 } else {
							 MessageBox::Show("Could not find OVL: " + pth->Build() + "\r\n\r\nReferenced from: " + gcnew String(curovlname.c_str()));
							 }*/
						 }
					 }
				 }
			 }
	private: System::Void zIPCompressedParkfileWithAllRequiredDependenciesToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 CreateParkZIP();
			 }

			 void CreateParkZIP()
			 {
				 if(AppBusy())
				 {
					 BusyMessageBox();//MessageBox::Show("Please wait until the current task is done!");
					 return;
				 }
				 if(!r3arg.IsValid())
				 {
					 MessageBox::Show("Open a parkfile first!");
					 return;
				 }
				 if (MessageBox::Show("It is recommend to clean your parkfile before zipping a parkfile so only the required OVLs are zipped.\r\n\r\nDo you want to continue?\r\n\r\nNOTE: THIS TOOL ZIPS ALL FILES YOUR PARK IS USING, BE CAREFULL WITH OVL FILES YOU DON'T WANT TO SHARE WITH OTHERS LIKE PRIVATE/WIP SETS!", "Cleaned parkfile", MessageBoxButtons::YesNo) == System::Windows::Forms::DialogResult::No)
				 {
					 return;
				 }

				 files.clear();
				 System::Windows::Forms::DialogResult result = MessageBox::Show("Do you want to use a blueprint?\r\n\r\nTip: Using an RCT3 blueprint created with Park CleanUp you can create a ZIP file for your friends using only the files they actually need. Your friends can create this blueprint file in their Park CleanUp, send it to you and then you use it here.", "Use a blueprint", MessageBoxButtons::YesNo);
				 if ( result == System::Windows::Forms::DialogResult::Yes )
				 {
					 OpenFileDialog ^ofd = gcnew OpenFileDialog();
					 ofd->CheckFileExists = true;

					 try {
						 ofd->Filter = "Blueprint file|*.pcub|All|*.*";

						 ofd->Title = "Select a Park CleanUp blueprint";					 

						 if (ofd->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
							 string name;
							 ToString(ofd->FileName, name);
							 files = readBlueprintsFromFile(name);		
							 MessageBox::Show("Blueprint succesfully opened! Contained " + files.size() + " files\r\n\r\nIn the next window you can select the ZIP file to save to");
						 }
					 } catch (...)
					 {
						 MessageBox::Show("Park CleanUp failed to open the selected blueprint, continuing ParkZIP without blueprint");
					 }
					 finally {
						 delete ofd;
					 }
				 }

				 SaveFileDialog^ sfd = gcnew SaveFileDialog;
				 sfd->Filter = "Park ZIP|*.zip";
				 sfd->InitialDirectory = Environment::GetFolderPath(System::Environment::SpecialFolder::MyDocuments) + "/RCT3/Parks";
				 sfd->RestoreDirectory = true;
				 if ( sfd->ShowDialog() == System::Windows::Forms::DialogResult::OK )
				 {
					 pb_status->Maximum = 100;
					 loading->Visible = true;
					 pb_status->Visible = true;
					 pb_status->Value = 0;
					 sl_appstatus->Text = "Creating list of required custom content";

					 //array<Object^>^ parms = gcnew array<Object^> { sfd->FileName};
					 //park_zipper->RunWorkerAsync(parms);

					 cli::array<Object^>^ parms = gcnew cli::array<Object^> { true, true, sfd->FileName};
					 filescan->RunWorkerAsync(parms);
				 }
			 }

	private: void SaveProgress(Object^ sender, SaveProgressEventArgs^ e)
			 {
				 switch (e->EventType)
				 {
				 case ZipProgressEventType::Saving_BeforeWriteEntry:
					 {
						 Log(e->EntriesSaved + "" + e->EntriesTotal);
						 int val = (((double) e->EntriesSaved) / (0.01 * e->EntriesTotal));
						 SetStatusValue(val);
						 break;
					 }
					 /*case ZipProgressEventType::Saving_Started:
					 {
					 Console::WriteLine("Saving: {0}", e->ArchiveName);
					 break;
					 }
					 case ZipProgressEventType::Saving_BeforeWriteEntry:
					 {
					 Console::WriteLine("  Writing: {0} ({1}/{2})",
					 e->CurrentEntry->FileName,
					 (e->EntriesSaved + 1),
					 e->EntriesTotal);
					 break;
					 }
					 case ZipProgressEventType::Saving_AfterWriteEntry:
					 {
					 if (e->CurrentEntry->InputStreamWasJitProvided)
					 {
					 e->CurrentEntry->InputStream->Close();
					 e->CurrentEntry->InputStream = nullptr;
					 }
					 break;
					 }
					 case ZipProgressEventType::Saving_Completed:
					 {
					 Console::WriteLine();
					 Console::WriteLine("Done: {0}", e->ArchiveName);
					 break;
					 }*/
				 }
			 };

	private: System::Void edit_track(System::Object^  sender, System::EventArgs^  e);
	private: System::Void track_list_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void button1_Click_1(System::Object^  sender, System::EventArgs^  e) {
				 char buffer_1[MAX_PATH] = "C:\\Program Files (x86)\\Atari\\RollerCoaster Tycoon 3\\Style\\Themed\\Adventure.common.ovl";
				 char *lpStr1 = buffer_1;
				 char *lpStr2 = buffer_1;//"C:\\Program Files (x86)\\Atari\\RollerCoaster Tycoon 3\\Style\\Themed\\Adventure.common.ovl";
				 PathRemoveFileSpecA(lpStr2);
				 PathCombineA(lpStr1, lpStr2, "..\\henk");
				 //Path::Combine("C:\\Program Files (x86)\\Atari\\RollerCoaster Tycoon 3\\Style\\Themed\\Adventure","..\\..\\..\\..\\henk");
				 MessageBox::Show(gcnew String(lpStr1));
			 }

			 bool MergePaths(string a, string b, string &c)
			 {
				 std::replace(a.begin(), a.end(), '/', '\\');
				 std::replace(b.begin(), b.end(), '/', '\\');
				 char *str1 = (char*)a.c_str();
				 char *str2 = (char*)b.c_str();
				 char buffer_1[MAX_PATH] = "";
				 char *lpStr1 = buffer_1;

				 PathRemoveFileSpecA(str1);
				 PathCombineA(lpStr1, str1, str2);

				 c = lpStr1;
				 return true;
			 }
	private: System::Void button1_Click_2(System::Object^  sender, System::EventArgs^  e) {
				 CheckForUpdates(false);
			 }

			 void CheckForUpdates(bool showOnUpdateOnly)
			 {
				 try
				 {
					 WebRequest^ request = WebRequest::Create( "http://cloud.wonderplein.nl/pcu/check.php?major=" + PCU_VERSION_MAJOR + "&minor=" + PCU_VERSION_MINOR + "&revision=" + PCU_VERSION_REVISION + ((IsDebugEnabled()) ? "&devbuild=" + PCU_DEV_BUILD : "") );
					 request->Credentials = CredentialCache::DefaultCredentials;
					 request->Timeout = 5000;
					 HttpWebResponse^ response = dynamic_cast<HttpWebResponse^>(request->GetResponse());

					 Stream^ dataStream = response->GetResponseStream();
					 StreamReader^ reader = gcnew StreamReader( dataStream );
					 String^ responseFromServer = reader->ReadToEnd();

					 Debug::WriteLine(responseFromServer);

					 reader->Close();
					 dataStream->Close();
					 response->Close();

					 if(responseFromServer == "1")
					 {					 
						 if (MessageBox::Show("A newer version is available, do you want to install it now?", "Your Park CleanUp is outdated", MessageBoxButtons::YesNo) == System::Windows::Forms::DialogResult::Yes)
						 {
							 if(IsDebugEnabled())
								 File::Create("dev.ldr");
							 ForceUpdate();
						 }
					 } else
					 {
						 if(!showOnUpdateOnly)
							 MessageBox::Show("Your Park CleanUp is up-to-date!");
					 }
				 } catch(...)
				 {
					 if(!showOnUpdateOnly)
						 MessageBox::Show("Hmm, somehow I couldn't check if there are any updates. Maybe you should try it later or visit Wonderplein.nl to get the latest updates.");
				 }
			 }

			 void ForceUpdate()
			 {
				 try
				 {
					 Assembly^_assembly = Assembly::GetExecutingAssembly();
					 Stream^ exestream = _assembly->GetManifestResourceStream("ParkCleanUpdater.exe");
					 Stream^ fileStream = File::Create("ParkCleanUpdater.exe");
					 exestream->Seek(0, IO::SeekOrigin::Begin);
					 exestream->CopyTo(fileStream);
					 fileStream->Close();

					 Diagnostics::Process::Start("ParkCleanUpdater.exe");
					 this->Close();
				 } catch (...)
				 {
					 MessageBox::Show("Could not start the updater!\r\n\r\nTo manually download the newest Park CleanUp go to www.Wonderplein.nl");
				 }
			 }

	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e);

			 void GetErrorList(vector<string> &handledfiles, vector<ErrorLog> &el, bool checkchildren)
			 {
				 //vector<string> parents;
				 Stream^ stream = (Assembly::GetExecutingAssembly())->GetManifestResourceStream("strdmp.txt");
				 StreamReader^ streamreader = (gcnew StreamReader(stream));
				 String^ txtdata = streamreader->ReadToEnd();
				 cli::array<String^>^ strs = txtdata->Split('\x0a');

				 int frontierOffset = 0;

				 for(int i = 0; i < strs->Length; i++)
				 {
					 string toadd;
					 String^ pth = strs[i]->Replace("\x0d", "");
					 try
					 {
						 pth = Path::Combine(removeTrailingSlash(tb_rct3loc->Text) + "\\", pth);
					 } catch (ArgumentException^ e)
					 {						
						 MessageBox::Show("Argument exception: Could not combine\r\n" + tb_rct3loc->Text + "\r\n" +  pth + "\r\n" +  e->ToString());
						 exit(0);
					 } catch (...)
					 {
						 MessageBox::Show("Could not combine\r\n" + tb_rct3loc->Text + "\r\n" +  pth);
						 exit(0);
					 }
					 ToString(pth, toadd);
					 std::replace(toadd.begin(), toadd.end(), '/', '\\');
					 std::transform(toadd.begin(), toadd.end(), toadd.begin(), ::tolower);
					 handledfiles.push_back(toadd);
				 }
				 streamreader->Close();
				 stream->Close();

				 frontierOffset = handledfiles.size();

				 vector<wstring> values;
				 for(int i = 0; i < clb_classes->Items->Count; i++)
				 {
					 if(!clb_classes->GetItemChecked(i))
					 {
						 for(unsigned int j = 0; j < cdb[i].RAM.strs.size(); j++)
						 {
							 if(cdb[i].RAM.strs[j].name == L"OVERLAYFILENAME")
							 {
								 wstring tlw = cdb[i].RAM.strs[j].value;
								 std::transform(tlw.begin(), tlw.end(), tlw.begin(), ::tolower);
								 bool found = false;
								 for(unsigned int k = 0; k < values.size(); k++)
								 {
									 if(tlw == values[k])
									 {
										 found = true;									 
										 k = 0xFEFEFEFE;
									 }									 
								 }
								 if(!found)
								 {
									 values.push_back(tlw);
								 }
							 }
						 }					 
					 }					 
				 }

				 for(int z = 0; z < values.size(); z++)
				 {
					 vector<string> parents;
					 string curovlname;
					 ToString(Path::Combine(tb_rct3loc->Text, gcnew String(values[z].c_str())), curovlname);	

					 if(File::Exists(Path::Combine(tb_rct3loc->Text, gcnew String(values[z].c_str())) + ".common.ovl") &&
						 File::Exists(Path::Combine(tb_rct3loc->Text, gcnew String(values[z].c_str())) + ".unique.ovl"))
					 {	
						 if(checkchildren)
							 HandleOVL(curovlname, parents, handledfiles, el);
						 else
						 {
							 std::transform(curovlname.begin(), curovlname.end(), curovlname.begin(), ::tolower);
							 std::replace(curovlname.begin(), curovlname.end(), '/', '\\');
							 bool found = false;
							 for(int i = 0; i < handledfiles.size(); i++)
							 {
								 if(handledfiles[i] == curovlname)
								 {
									 found = true;
									 i = handledfiles.size();
								 }
							 }
							 if(!found)
								 handledfiles.push_back(curovlname);
						 }
					 } else {
						 el.push_back(ErrorLog(gcnew String(values[z].c_str()), "OVL not found", parents));

						 std::transform(curovlname.begin(), curovlname.end(), curovlname.begin(), ::tolower);
						 std::replace(curovlname.begin(), curovlname.end(), '/', '\\');
						 bool found = false;
						 for(int i = 0; i < handledfiles.size(); i++)
						 {
							 if(handledfiles[i] == curovlname)
							 {
								 found = true;
								 i = handledfiles.size();
							 }
						 }
						 if(!found)
							 handledfiles.push_back(curovlname);
					 }
					 int val = (((double) z) / (values.size())*100);
					 SetStatusValue(val);
				 }

				 vector<string> handledfiles2;

				 for(int i = frontierOffset; i < handledfiles.size(); i++)
					 handledfiles2.push_back(handledfiles[i]);

				 handledfiles = handledfiles2;

				 //handledfiles.erase(handledfiles.begin(), (handledfiles.begin() + (frontierOffset - 1)));// (frontierOffset - 1));
				 //for(int i = 0; i < frontierOffset; i++)
				 //{
				 //handledfiles.erase(handledfiles.begin(), handledfiles.begin() +1);
				 //}
			 }

			 void HandleOVL(string ovlpath, vector<string> &parents, vector<string> &handledfiles, vector<ErrorLog> &el)
			 {
				 std::transform(ovlpath.begin(), ovlpath.end(), ovlpath.begin(), ::tolower);
				 std::replace(ovlpath.begin(), ovlpath.end(), '/', '\\');
				 for(int i = 0; i < handledfiles.size(); i++)
				 {
					 if(handledfiles[i] == ovlpath)
						 return;
				 }
				 handledfiles.push_back(ovlpath);

				 String^ relovlpath = getOVLNameFromPath(ovlpath);

				 OVLReader OVL = OVLReader(ovlpath);
				 if(! OVL.IsValid())
				 {					 
					 string err = OVL.GetLastReadingError();
					 el.push_back(ErrorLog(relovlpath, "OVL appears to be invalid: " + gcnew String(err.c_str()), parents));
				 } else {
					 for(unsigned long i = 0; i < OVL.SVDNames.size(); i++)
					 {
						 R3Path^ pth = gcnew R3Path();
						 pth->SetR3Path(tb_rct3loc->Text);
						 pth->AddPath(gcnew String(OVL.SVDNames[i].path.c_str()), true);

						 if(File::Exists(pth->Build()))
						 {
							 pth->SetExtension("");
							 if(File::Exists(pth->Build() + ".common.ovl") &&
								 File::Exists(pth->Build() + ".unique.ovl"))
							 {
								 string newovlname;
								 ToString(pth->BuildWithoutBase(), newovlname);
								 parents.push_back(newovlname);
								 ToString(pth->Build(), newovlname);
								 HandleOVL(newovlname, parents, handledfiles, el);
							 } else
							 {
								 el.push_back(ErrorLog(relovlpath, "The OVL pair appears to be incomplete (either the unique or common.ovl is missing)", parents));
							 }
						 } else
						 {
							 el.push_back(ErrorLog(relovlpath, "The file does not exist", parents));
						 }
					 }

					 for(ulong j = 0; j < 2; j++)
					 {
						 OVLData OVLD = OVL.GetOVLD(j);
						 for(int i = 0; i < OVLD.references.size(); i++)
						 {
							 string newovlpath;
							 string oldcurrentovl = ovlpath;
							 oldcurrentovl.append(".common.ovl");
							 MergePaths(oldcurrentovl, OVLD.references[i].file, newovlpath);
							 if(File::Exists(gcnew String(newovlpath.c_str()) + ".common.ovl") &&
								 File::Exists(gcnew String(newovlpath.c_str()) + ".unique.ovl"))
							 {
								 string newovlname;
								 ToString(stripRCT3Folder(gcnew String(newovlpath.c_str())), newovlname);
								 parents.push_back(newovlname);
								 HandleOVL(newovlpath, parents, handledfiles, el);
							 } else
							 {
								 if(!(gcnew String(newovlpath.c_str()))->Contains("shy-ms3window"))
									 el.push_back(ErrorLog(stripRCT3Folder(gcnew String(newovlpath.c_str())), "Referenced OVL doesn't exist", parents));
							 }
						 }
					 }
				 }
			 }
	private: System::Void filescan_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
				 errorExecutor->Enabled = true;
				 cli::array<Object^>^ parms = (cli::array<Object^>^)e->Argument;
				 bool showfulllist = (bool)parms[0];
				 bool zipfiles = (bool)parms[1];
				 String^ zipname = (String^)parms[2];
				 vector<string> handledfiles;
				 vector<ErrorLog> el;
				 String^ log = "Park CleanUp " + PCU_VERSION + "\r\nR3PATH " + R3PATH_VERSION + "\r\nR3A " + R3AREADER_VERSION + "\r\nOVL Reader " + OVLREADER_VERSION + Environment::NewLine + "-----------------------" + Environment::NewLine;
				 GetErrorList(handledfiles, el, (cb_deep_check->Checked) ? cb_deep_check->Checked : zipfiles);

				 //clean r3path
				 for(int i = 0; i < handledfiles.size(); i++)
				 {
					 handledfiles[i] = getOVLNameFromPathS(handledfiles[i]);
				 }

				 for(int x= 0; x < files.size(); x++)
				 {
					 handledfiles.push_back(files[x].relativename);
					 /*string toadd;
					 String^ pth = gcnew String(files[x].relativename.c_str());
					 try
					 {
					 pth = Path::Combine(removeTrailingSlash(tb_rct3loc->Text) + "\\", pth);
					 } catch (ArgumentException^ e)
					 {						
					 MessageBox::Show("Argument exception: Could not combine\r\n" + tb_rct3loc->Text + "\r\n" +  pth + "\r\n" +  e->ToString());
					 exit(0);
					 } catch (...)
					 {
					 MessageBox::Show("Could not combine\r\n" + tb_rct3loc->Text + "\r\n" +  pth);
					 exit(0);
					 }
					 ToString(pth, toadd);
					 std::replace(toadd.begin(), toadd.end(), '/', '\\');
					 std::transform(toadd.begin(), toadd.end(), toadd.begin(), ::tolower);
					 handledfiles.push_back(toadd);*/
				 }

				 content_listbox->Items->Clear();
				 content_listbox->BeginUpdate();

				 vector<string> duplicates;
				 for(int i = 0; i < handledfiles.size(); i++)
				 {
					 bool found = false;
					 for(int j = 0; j < duplicates.size(); j++)
					 {
						 if(duplicates[j] == handledfiles[i])
						 {
							 found = true;
							 j = 0xFFFFFFEF;
						 }
					 }
					 if(!found && handledfiles[i] != "")
					 {
						 duplicates.push_back(handledfiles[i]);
					 }
				 }
				 handledfiles = duplicates;

				 vector<ErrorLog> elduplicates;
				 for(int i = 0; i < el.size(); i++)
				 {
					 bool found = false;
					 for(int j = 0; j < elduplicates.size(); j++)
					 {
						 if(elduplicates[j].filename == el[i].filename)
						 {
							 found = true;
							 j = 0xFFFFFFEF;
						 }
					 }
					 if(!found && el[i].filename != "")
					 {
						 elduplicates.push_back(el[i]);
					 }
				 }
				 el = elduplicates;

				 if(showfulllist)
				 {
					 for(int i = 0; i < handledfiles.size(); i++)
					 {
						 content_listbox->Items->Add(gcnew String(handledfiles[i].c_str()));
					 }
					 content_info->Text = handledfiles.size() + " required OVL files were detected";
				 } else
				 {					 
					 for(int i = 0; i < el.size(); i++)
					 {
						 String^ str = gcnew String(el[i].error.c_str()) + " : " + gcnew String(el[i].filename.c_str());
						 if(el[i].parents.size() > 0)
						 {
							 str += ", referenced via " + gcnew String(el[i].parents[0].c_str());
						 }
						 content_listbox->Items->Add(str + Environment::NewLine);
					 }
					 content_info->Text = el.size() + " errors were detected";
				 }
				 content_listbox->EndUpdate();


				 //Clear files that gave errors
				 duplicates.clear();;
				 for(int i = 0; i < handledfiles.size(); i++)
				 {
					 bool found = false;
					 for(int j = 0; j < el.size(); j++)
					 {
						 if(handledfiles[i] == el[j].filename)
						 {
							 found = true;
							 j = 0xFFFFFFEF;
						 }
					 }
					 if(!found && handledfiles[i] != "")
					 {
						 duplicates.push_back(handledfiles[i]);
					 }
				 }
				 handledfiles = duplicates;

				 if(zipfiles)
				 {
					 String^ zipdirectory_temp = "_TEMP_PARKZIP/";
					 String^ resource_folder = "custom objects/";
					 if ( !Directory::Exists( zipdirectory_temp ) )
					 {
						 Directory::CreateDirectory(zipdirectory_temp);
					 }				 

					 ZipFile ^ zip;
					 String^ ziplog;

					 string parkfile;
					 ToString(zipdirectory_temp, parkfile);
					 parkfile.append("Park.dat");
					 SaveCompressedParkfileAs(parkfile);

					 try
					 {
						 sl_appstatus->Text = "[2/3] Copying files to a temporary folder...";
						 OutputDebugStringA("Copying files..");
						 zip = gcnew ZipFile();
						 //zip->Password = "Harbinger";
						 //zip->Encryption = EncryptionAlgorithm::WinZipAes128;		
						 for(unsigned int j = 0; j < handledfiles.size(); j++)
						 {
							 OutputDebugStringA(handledfiles[j].c_str());
							 log += "-------------------" + Environment::NewLine;
							 String^ copydir = Path::Combine(zipdirectory_temp + resource_folder, gcnew String(handledfiles[j].c_str()));

							 try
							 {
								 log += "Copy:" + Path::Combine(tb_rct3loc->Text, gcnew String(handledfiles[j].c_str()))  + Environment::NewLine + "To:" + copydir + Environment::NewLine;
								 String^ copyovlname = Path::Combine(tb_rct3loc->Text, gcnew String(handledfiles[j].c_str()));
								 if(File::Exists(copyovlname + ".common.ovl") && File::Exists(copyovlname + ".unique.ovl"))
								 {
									 Directory::CreateDirectory(Path::GetDirectoryName(copydir));
									 File::Copy(Path::Combine(tb_rct3loc->Text, gcnew String(handledfiles[j].c_str())) + ".common.ovl", copydir + ".common.ovl", true);
									 File::Copy(Path::Combine(tb_rct3loc->Text, gcnew String(handledfiles[j].c_str())) + ".unique.ovl", copydir + ".unique.ovl", true);
								 } else
								 {
									 ziplog += "Incomplete OVL pair" + gcnew String(handledfiles[j].c_str()) + Environment::NewLine;
								 }
							 } catch(...)
							 {
								 ziplog += "An error occured while trying to copy " + gcnew String(handledfiles[j].c_str()) + Environment::NewLine;
							 }
							 OutputDebugStringA("File ok!");
						 }
						 OutputDebugStringA("Files copied!");

						 String^ readme = "ParkZIP by Park CleanUp v" + PCU_VERSION  + Environment::NewLine;
						 //readme += "-- THIS IS NOT FINAL SOFTWARE --" + Environment::NewLine + Environment::NewLine;
						 readme += "This zip contains a few files:" + Environment::NewLine;
						 readme += "log.txt: A logfile containing technical information about the ZIP'ing proces" + Environment::NewLine;
						 readme += "Park.dat: The parkfile which must be placed in \"Documents\RCT3\Parks\"" + Environment::NewLine;
						 readme += (ziplog != "") ? "ErrorLog.txt: A file that shows all errors that occured during saving the parkfile" + Environment::NewLine : "";
						 readme += "[custom object]: Folder containing all required resources in order to run the parkfile" + Environment::NewLine + Environment::NewLine;
						 readme += "---------------------------------"  + Environment::NewLine;
						 readme += "Installation"  + Environment::NewLine;
						 readme += "---------------------------------"  + Environment::NewLine;
						 readme += "Note: It is advised to create a backup of your RCT3 folder prior to installing the custom content of this zip file in case the installation gets screwed up!"  + Environment::NewLine;
						 readme += "1: Copy the contents of custom objects to your RCT3 installation folder (e.g. C:\\Program Files (x86)\\Atari\\RollerCoaster Tycoon 3)"  + Environment::NewLine;
						 readme += "2: If you are asked to merge folders, click yes"  + Environment::NewLine;
						 readme += "	Note: If you already have some of the custom content installed you'll be asked if you want to overwrite files. If you don't want to replace files you can choose not to replace those files. Depending on the situation this may or may not be causing problems with the parkfile from this zip and/or your own parks."  + Environment::NewLine;
						 zip->AddEntry("Readme.txt", readme);

						 OutputDebugStringA("Readme added!");

						 if(ziplog != "" || el.size() > 0)
						 {
							 if(el.size() > 0)
							 {
								 std::sort(el.begin(), el.end(), el[0]);
								 for(int i = 0; i < el.size(); i++)
								 {
									 ziplog += gcnew String(el[i].filename.c_str()) + ": " + gcnew String(el[i].error.c_str()) + Environment::NewLine;
								 }
							 }
							 MessageBox::Show("Some of the custom content required in order to run this park is missing. Basically this means that the park isn't guarenteed to run on another system.\r\n\r\nSee the errorlog.txt within the ZIPfile to see which files raised errors.");
							 zip->AddEntry("ErrorLog.txt", ziplog);
						 }
						 zip->AddEntry("log.txt", log);
						 OutputDebugStringA("ErrorLog added!");

						 sl_appstatus->Text = "[3/3] Zipping all dependencies...";

						 zip->SaveProgress += gcnew EventHandler<SaveProgressEventArgs^>(this, &Form1::SaveProgress);
						 zip->AddDirectory(zipdirectory_temp);
						 zip->Save(zipname);					 
					 }
					 finally
					 {
						 delete zip;
						 Directory::Delete(zipdirectory_temp, true);
					 }
				 }

				 loading->Visible = false;
				 pb_status->Visible = false;
				 pb_status->Value = 0;
				 sl_appstatus->Text = "";
			 }

	private: System::Void cleanUpGuestsmightHaveUnwantedResultsIngameToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 selectmode = SM::SELECT_GUEST_ITEMS;
				 SelectPredefinedItems();
			 }
	private: System::Void btn_force_update_Click(System::Object^  sender, System::EventArgs^  e) {
				 ForceUpdate();
			 }
	private: System::Void errorExecutor_Tick(System::Object^  sender, System::EventArgs^  e) {
				 if(!AppBusy())
				 {
					 if(loading->Visible || pb_status->Visible)
					 {
						 pb_status->Style = System::Windows::Forms::ProgressBarStyle::Blocks;
						 errorExecutor->Enabled = false;
						 MessageBox::Show("Oops, an error occured during the task you were executing. The program will try to continue.\r\n\r\nHere's some usefull debug data:\r\nProgress: " + pb_status->Value, "Fatal error");
						 loading->Visible = false;
						 pb_status->Visible = false;
						 sl_appstatus->Visible = false;						 
					 }
				 }
			 }
	private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
				 LoadFile();
			 }
	private: System::Void cleanUpDucksToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 selectmode = SM::DUCK;
				 SelectPredefinedItems();
			 }
	private: System::Void timeDelta_Scroll(System::Object^  sender, System::EventArgs^  e) {
				 timeDeltaPerTick = (float)timeDelta->Value / 100000;
			 }
	private: System::Void timechanger_Tick(System::Object^  sender, System::EventArgs^  e) {
				 HWND rct = FindWindowA(0, "RCT3");
				 if(rct == NULL){
					 timechanger->Enabled = false;
					 timeEnabler->Checked = false;
					 MessageBox::Show("Please make sure that RCT3 is running or the tool won't work! Time changer has been disabled.");				 
				 } else {
					 if(rctBaseAddress == -1)
					 {
						 Process^ myProcess = gcnew Process;
						 ProcessStartInfo^ myProcessStartInfo = gcnew ProcessStartInfo( tb_rct3loc->Text + "\\RCT3Plus.exe" );
						 myProcess->StartInfo = myProcessStartInfo;

						 myProcess->Start();
						 ProcessModule^ myProcessModule;

						 myProcessModule = myProcess->MainModule;
						 rctBaseAddress = myProcessModule->BaseAddress.ToInt32();
					 }

					 float newtime = time.time;
					 LPMODULEENTRY32 module;
					 DWORD dwProcessId;
					 SIZE_T NumberOfBytesRead;
					 DWORD address = 0x00D06C54;

					 GetWindowThreadProcessId(rct, &dwProcessId);
					 HANDLE han = OpenProcess(PROCESS_ALL_ACCESS, 0, dwProcessId);

					 address += rctBaseAddress;
					 ReadProcessMemory( han, (void*) address, &address, 4,0);
					 address += 0x8;
					 ReadProcessMemory( han, (void*) address, &address, 4,0);
					 address += 0x4;
					 ReadProcessMemory( han, (void*) address, &address, 4,0);
					 address += 0x44;
					 //ReadProcessMemory( han, (void*) address, &address, 4,0);

					 if(newtime == -1)
						 ReadProcessMemory( han, (void*) address, &newtime, 4,0);
					 else
					 {
						 newtime += timeDeltaPerTick;

						 if(newtime > 1.0f)
							 newtime = 0.3f;
						 if(newtime < 0.3f)
							 newtime = 1.0f;

						 WriteProcessMemory(han, (void*) address, &newtime, 4,0);
					 }
					 time.time = newtime;
				 }
			 }
	private: System::Void timeEnabler_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
				 if(timeEnabler->Checked == true)
					 timechanger->Enabled = true;
				 else
					 timechanger->Enabled = false;
			 }
	private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {
				 ShowTutorial();
			 }
	private: System::Void button5_Click(System::Object^  sender, System::EventArgs^  e) {
				 if(AppBusy())
				 {
					 BusyMessageBox();//MessageBox::Show("Please wait until the current task is done!");
					 return;
				 }
				 File::Delete(Path::GetDirectoryName(Application::ExecutablePath) + gcnew String("/options.ini"));
				 LoadSettings();
			 }
	private: System::Void cb_update_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
				 SaveOptions();
			 }
	private: System::Void button7_Click(System::Object^  sender, System::EventArgs^  e) {
				 timeDelta->Value = 0;
				 timeDeltaPerTick = 1/(100*60*60*24);//1.15740741e-7;
			 }
	private: System::Void splitButton2_Click(System::Object^  sender, System::EventArgs^  e) {
				 CreateParkZIP();
			 }
	private: System::Void cb_aggressive_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
				 SaveOptions();
			 }
	private: System::Void comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
				 if(!pcuReady)
					 return;

				 if(dd_operatingmode->Text == "Development edition")
					 SetChannelOption(PCU_Channels::DEBUG);
				 else
					 SetChannelOption(PCU_Channels::DEFAULT);
			 }
	private: System::Void park_prev_Click(System::Object^  sender, System::EventArgs^  e) {
				 if(AppBusy())
				 {
					 BusyMessageBox();//MessageBox::Show("Please wait until the current task is done!");
					 return;
				 }
				 if(!r3arg.IsValid())
				 {
					 MessageBox::Show("Open a parkfile first!");
					 return;
				 }
				 if(dat_preview_position == 0)
				 {
					 MessageBox::Show("The parkpreview could not be found, are you sure the file contains a park?");
					 return;
				 }

				 String^ bmp_replacement = "";
				 OpenFileDialog ^ofd = gcnew OpenFileDialog();
				 ofd->CheckFileExists = true;
				 try {
					 ofd->Filter = "Bitmap|*.bmp";
					 ofd->Title = "Select a bitmap";
					 if (ofd->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
						 bmp_replacement = ofd->FileName;
					 }
					 else {
						 return;
					 }
				 }
				 finally {
					 delete ofd;
				 }
				 Bitmap^ dpreplacer = gcnew Bitmap(bmp_replacement);
				 SetPreviewImage(dpreplacer);
			 }
	private: System::Void button6_Click(System::Object^  sender, System::EventArgs^  e) {
				 time.DecreaseHours();
			 }
	private: System::Void ride_edit(System::Object^  sender, System::EventArgs^  e);
	private: System::Void stall_edit(System::Object^  sender, System::EventArgs^  e);
	private: System::Void btn_time_minutemin1_Click(System::Object^  sender, System::EventArgs^  e) {
				 time.DecreaseMinutes();
			 }
	private: System::Void btn_time_minutesplus1_Click(System::Object^  sender, System::EventArgs^  e) {
				 time.IncreaseMinutes();
			 }
	private: System::Void btn_time_hoursplus1_Click(System::Object^  sender, System::EventArgs^  e) {
				 time.IncreaseHours();
			 }
	private: System::Void button6_Click_1(System::Object^  sender, System::EventArgs^  e) {

			 }
			 void CreateBlueprint()
			 {
				 if(AppBusy())
				 {
					 BusyMessageBox();
					 return;
				 }
				 loading->Visible = true;
				 pb_status->Visible = true;
				 pb_status->Value = 0;
				 blueprint_creator->RunWorkerAsync();
			 }
	private: System::Void blueprint_creator_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
				 errorExecutor->Enabled = true;

				 auto t_start = std::chrono::high_resolution_clock::now();
				 bool timeAdditionDone = false;

				 pb_status->Visible = false;
				 sl_appstatus->Text = "Creating blueprint..";

				 vector<BlueprintFile> blueprints;
				 GetFilesNumber(tb_rct3loc->Text, blueprints);

				 Stream^ stream = (Assembly::GetExecutingAssembly())->GetManifestResourceStream("strdmp.txt");
				 StreamReader^ streamreader = (gcnew StreamReader(stream));
				 String^ txtdata = streamreader->ReadToEnd();
				 cli::array<String^>^ strs = txtdata->Split('\x0a');

				 streamreader->Close();
				 stream->Close();

				 for(int i = blueprints.size() - 1; i >= 0 ; i--)
				 {
					 bool found = false;
					 String^ name = gcnew String(blueprints[i].relativename.c_str());					 
					 for(int j = 0; j < strs->Length; j++)
					 {
						 String^ pth = strs[j]->Replace("\x0d", "");
						 pth = pth->Replace(".common.ovl", "");
						 if(String::Compare(pth, name, true) == 0)
						 {
							 found = true;
							 break;
						 }
					 }
					 auto t_end = std::chrono::high_resolution_clock::now();
					 if(!timeAdditionDone && (std::chrono::duration_cast<std::chrono::milliseconds>(t_end - t_start).count() > 10 * 1000))
					 {
						 sl_appstatus->Text += " - This is going to take some time...";
						 timeAdditionDone = true;
					 }
					 if(found)
					 {						 
						 blueprints.erase(blueprints.begin() + i);
					 }
				 }

				 /*ofstream strdmp("testdump.txt");
				 if(strdmp.is_open())
				 {
				 for(int i = 0; i < blueprints.size(); i++)
				 {
				 strdmp << blueprints[i].relativename << "\n";
				 }
				 strdmp.close();

				 }*/

				 writeBlueprintsToFile("bprints.pcub", blueprints);

				 loading->Visible = false;
				 pb_status->Visible = false;
				 pb_status->Value = 0;
				 sl_appstatus->Text = "";
				 selectmode = SM::DEFAULT;

				 blueprints.clear();

				 MessageBox::Show("Blueprint saved as 'bprints.pcub' in the Park CleanUp folder!");
			 }
	private: System::Void createBlueprintToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 CreateBlueprint();
			 }
	private: System::Void graphDrawer_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
				 classgraph->Series->Clear();
				 vector<IndexedString> data;
				 for(uint32_t j = 0; j < cdb.size(); j++)
				 {
					 bool found = false;
					 String^ itemname = "";
					 string classname = class_prototype[cdb[j].prototype_of].name;
					 for(int i = 0; i < data.size(); i++)
					 {
						 if(data[i].name == classname)
						 {
							 data[i].count++;
							 found = true;
							 break;
						 }
					 }
					 if(!found)
					 {
						 IndexedString str = {0, cdb[j].end - cdb[j].start, classname};
						 data.push_back(str);
					 }
				 }

				 std::sort(data.begin(), data.end(), data[0]);

				 int count = 0;
				 for(int i = 0; i < data.size(); i++)
				 {
					 if(data[i].count > 5)
					 {
						 String^ classname = gcnew String(data[i].name.c_str());
						 if(!classname->Equals("RCT3Terrain") &&
							 !classname->Equals("EnclosureManager"))
						 {
							 String^ name = classname + " size";
							 cli::array<Object^>^ parms = gcnew cli::array<Object^> { name, data[i].count, data[i].size};
							 classgraph->BeginInvoke(gcnew d_addGraphData( this, &Form1::AddGraphData), parms);
						 }
						 count++;
						 if(count > 20)
							 break;
					 }
				 }
			 }

			 void AddGraphData(String^ name, int count, int size)
			 {
				 classgraph->Series->Add(name);
				 //classgraph->Series[name]->Points->Add(size);	
				 classgraph->Series[name]->Points->Add(size);	
			 }
	private: System::Void credits_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 }
};	
}


#pragma once

#include "R3AReader.h"
#include "shared.h"

namespace ParkCleanUp2 { 
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for RideEdit
	/// </summary>
	public ref class RideEdit : public System::Windows::Forms::Form
	{
		ref class Form1;
	public:
		RideEdit(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			succesfull = false;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~RideEdit()
		{
			if (components)
			{
				delete components;
			}
		}


	private: System::Windows::Forms::Button^  btn_save;

	private: System::Windows::Forms::Button^  btn_cancel;


	private: System::ComponentModel::BackgroundWorker^  track_resolver;
	private: System::Windows::Forms::ProgressBar^  prog;
	private: System::Windows::Forms::Label^  lbl;


	protected: 


	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->btn_save = (gcnew System::Windows::Forms::Button());
			this->btn_cancel = (gcnew System::Windows::Forms::Button());
			this->track_resolver = (gcnew System::ComponentModel::BackgroundWorker());
			this->prog = (gcnew System::Windows::Forms::ProgressBar());
			this->lbl = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// btn_save
			// 
			this->btn_save->Enabled = false;
			this->btn_save->Location = System::Drawing::Point(12, 12);
			this->btn_save->Name = L"btn_save";
			this->btn_save->Size = System::Drawing::Size(138, 23);
			this->btn_save->TabIndex = 2;
			this->btn_save->Text = L"Mark ride for removal";
			this->btn_save->UseVisualStyleBackColor = true;
			this->btn_save->Click += gcnew System::EventHandler(this, &RideEdit::button1_Click);
			// 
			// btn_cancel
			// 
			this->btn_cancel->Location = System::Drawing::Point(12, 41);
			this->btn_cancel->Name = L"btn_cancel";
			this->btn_cancel->Size = System::Drawing::Size(138, 23);
			this->btn_cancel->TabIndex = 3;
			this->btn_cancel->Text = L"Cancel";
			this->btn_cancel->UseVisualStyleBackColor = true;
			this->btn_cancel->Click += gcnew System::EventHandler(this, &RideEdit::button2_Click);
			// 
			// track_resolver
			// 
			this->track_resolver->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &RideEdit::track_resolver_DoWork);
			// 
			// prog
			// 
			this->prog->Location = System::Drawing::Point(12, 83);
			this->prog->MarqueeAnimationSpeed = 30;
			this->prog->Name = L"prog";
			this->prog->Size = System::Drawing::Size(138, 28);
			this->prog->Step = 3;
			this->prog->Style = System::Windows::Forms::ProgressBarStyle::Marquee;
			this->prog->TabIndex = 4;
			// 
			// lbl
			// 
			this->lbl->AutoSize = true;
			this->lbl->Location = System::Drawing::Point(12, 67);
			this->lbl->Name = L"lbl";
			this->lbl->Size = System::Drawing::Size(101, 13);
			this->lbl->TabIndex = 5;
			this->lbl->Text = L"Resolving ridedata..";
			// 
			// RideEdit
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(162, 123);
			this->Controls->Add(this->lbl);
			this->Controls->Add(this->prog);
			this->Controls->Add(this->btn_cancel);
			this->Controls->Add(this->btn_save);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->Name = L"RideEdit";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"Ride options";
			this->Load += gcnew System::EventHandler(this, &RideEdit::RideEdit_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	public:
		bool succesfull;

		List<int> scenery_ids;

	private: System::Void track_resolver_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
				 scenery_ids.Clear();
				 vector<Reference> handled;				 
				 R3AReader R3A = sr3arg;
				 Reference trackref = strackref;
				 vector<Class_Data_Block> cdb;
				 //vector<Class_Prototype> class_prototype;
				 cdb = R3A.getclassdata();
				 int j = R3A.getclassidbyuid(trackref);
				 handled.push_back(trackref);

				 vector<int> refList;
				 vector<string> findList;
				 // AnimatedRideInstance related
				 findList.push_back("Entrance");
				 findList.push_back("Exit");
				 findList.push_back("RideStationsObj");
				 findList.push_back("Scenery");
				 findList.push_back("ShowController");
				 findList.push_back("Owner");
				 findList.push_back("SceneryItem");

				 ResolveReferences(handled, refList, findList, j, R3A, trackref, cdb);

				 scenery_ids.Clear();
				 scenery_ids.Add(j);
				 for(int i = 0; i < refList.size(); i++)
					 scenery_ids.Add(refList[i]);


				 btn_save->Enabled = true;
				 prog->Visible = false;
				 //prog->Style = ProgressBarStyle::Blocks;
				 lbl->Text = scenery_ids.Count + " elements marked\r\nfor removal";
			 }

			 vector<int> ResolveReferences(vector<Reference> &handled, vector<int> &referencelist, vector<string> &findlist, int id, R3AReader &R3A, Reference &trackref, vector<Class_Data_Block> &cdb)
			 {
				 for(uint32_t h = 0; h < cdb[id].RAM.refnames.size(); h++)
				 {
					 for(uint32_t j = 0; j < findlist.size(); j++)
					 {
						 if(cdb[id].RAM.refnames[h] == findlist[j])
						 {
							 int tri2 = R3A.getclassidbyuid(cdb[id].RAM.refs[h]);	
							 if(tri2 >= 0)
							 {
								 if(!cdb[id].RAM.refs[h].IsSame(handled))
								 {
									 referencelist.push_back(tri2);
									 handled.push_back(cdb[id].RAM.refs[h]);
									 ResolveReferences(handled, referencelist, findlist, tri2, R3A, trackref, cdb);
								 }
							 }
						 }
					 }
				 }
				 return referencelist;
			 }

	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
				 succesfull = true;
				 this->Close();
			 }
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
				 succesfull = false;
				 this->Close();
			 }
	private: System::Void RideEdit_Load(System::Object^  sender, System::EventArgs^  e) {
				 track_resolver->RunWorkerAsync();
			 }
	};
}

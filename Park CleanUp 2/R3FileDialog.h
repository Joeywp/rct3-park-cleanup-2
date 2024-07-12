#pragma once

#include <Windows.h>
#include <vcclr.h>

#include "R3AReader.h"

namespace ParkCleanUp2 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;

	extern string allowedextensions;

	/// <summary>
	/// Summary for R3FileDialog
	/// </summary>
	public ref class R3FileDialog : public System::Windows::Forms::Form
	{
	private:		
		ref struct R3FileCache;
		List<R3FileCache^>^ filecache;// = gcnew array<String^>(x);
	private: System::ComponentModel::BackgroundWorker^  ImageResolver;
	private: System::Windows::Forms::Button^  button1;

	public:
		bool fastClean;
		String^ FileName;

		R3FileDialog(string curItemName)
		{			
			InitializeComponent();	

			String^ curFileName = gcnew String(curItemName.c_str());

			this->DialogResult = System::Windows::Forms::DialogResult::Abort;

			fastClean = false;
			FileName = "";
			filecache = gcnew List<R3FileCache^>(0);

			this->SuspendLayout();
			this->panel1->SuspendLayout();
			this->panel1->Controls->Clear();
			this->panel1->ResumeLayout(true);
			this->panel1->SuspendLayout();

			String^ path = Environment::GetFolderPath(Environment::SpecialFolder::MyDocuments) + "/RCT3/Parks";
			if(Directory::Exists(path))
			{
				cli::array<String^>^ file = Directory::GetFiles( path );
				System::ComponentModel::ComponentResourceManager^ resources = GetResourceManager();

				int j = 0;			
				for (int i=0; i<file->Length; i++){
					if(file[i]->EndsWith(".dat")){
						R3FileCache^ curCache = gcnew R3FileCache();
						String^ fn = Path::GetFileName(file[i]);

						PictureBox^ img_prev = (gcnew System::Windows::Forms::PictureBox());
						GroupBox^ park = (gcnew System::Windows::Forms::GroupBox());
						Label^ park_label = (gcnew System::Windows::Forms::Label());
						Button^ btn_select = (gcnew System::Windows::Forms::Button());
						Button^ btn_fastclean = (gcnew System::Windows::Forms::Button());

						FileInfo^ fi = gcnew FileInfo ( file[i] );
						string size;
						double newFileSize = ByteSizeConverter(fi->Length, size);
						newFileSize = ceil(newFileSize * 100) / 100;

						park->Controls->Add(btn_fastclean);
						park->Controls->Add(btn_select);
						park->Controls->Add(park_label);
						park->Controls->Add(img_prev);
						park->Location = System::Drawing::Point(3, 3 + (j*93));
						park->Name = L"dat_name";
						park->Size = System::Drawing::Size(260, 94);
						park->TabIndex = 3;
						park->TabStop = false;
						park->Text = (j+1) + L": " +  fn  + " - " + newFileSize + " " + gcnew String(size.c_str());
						// 
						// preview image
						//
						img_prev->Location = System::Drawing::Point(7, 20);
						img_prev->Name = L"img_prev";
						img_prev->Size = System::Drawing::Size(74, 58);
						img_prev->TabIndex = 0;
						img_prev->TabStop = false;
						img_prev->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"park_prev.Image")));
						img_prev->InitialImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"park_prev.InitialImage")));
						// 
						// park name label
						// 
						park_label->AutoSize = true;
						park_label->Location = System::Drawing::Point(84, 20);
						park_label->Name = L"lbl_park_name";
						park_label->Size = System::Drawing::Size(82, 13);
						park_label->TabIndex = 1;
						park_label->Text = L"Park name loading...";
						// 
						// select button
						// 
						btn_select->Location = System::Drawing::Point(87, 55);
						btn_select->Name = file[i];
						btn_select->Size = System::Drawing::Size(75, 23);
						btn_select->TabIndex = 2;
						btn_select->Text = L"Select";
						btn_select->UseVisualStyleBackColor = true;
						btn_select->Click += gcnew System::EventHandler(this, &R3FileDialog::select_file);
						// 
						// fastclean button
						// 
						btn_fastclean->Location = System::Drawing::Point(168, 55);
						btn_fastclean->Name = file[i];
						btn_fastclean->Size = System::Drawing::Size(81, 23);
						btn_fastclean->TabIndex = 3;
						btn_fastclean->Text = L"FastClean";
						btn_fastclean->UseVisualStyleBackColor = true;
						btn_fastclean->Click += gcnew System::EventHandler(this, &R3FileDialog::select_fastclean);

						this->panel1->Controls->Add(park);

						curCache->filename = file[i];
						curCache->picture = img_prev;
						curCache->name = park_label;

						filecache->Add(curCache);
						j++;					
					}
				}
			}


			this->panel1->ResumeLayout(true);
			this->panel1->Update();
			this->ResumeLayout(true);			
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~R3FileDialog()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Panel^  panel1;
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
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->ImageResolver = (gcnew System::ComponentModel::BackgroundWorker());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// panel1
			// 
			this->panel1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left));
			this->panel1->AutoScroll = true;
			this->panel1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->panel1->Location = System::Drawing::Point(12, 12);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(285, 306);
			this->panel1->TabIndex = 2;
			// 
			// ImageResolver
			// 
			this->ImageResolver->WorkerSupportsCancellation = true;
			this->ImageResolver->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &R3FileDialog::ImageResolver_DoWork);
			// 
			// button1
			// 
			this->button1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->button1->Location = System::Drawing::Point(9, 324);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(288, 58);
			this->button1->TabIndex = 3;
			this->button1->Text = L"Browse for (park) files";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &R3FileDialog::button1_Click);
			// 
			// R3FileDialog
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(309, 391);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->panel1);
			this->MaximumSize = System::Drawing::Size(325, 1600);
			this->MinimumSize = System::Drawing::Size(325, 430);
			this->Name = L"R3FileDialog";
			this->ShowIcon = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"Select a park file";
			this->Load += gcnew System::EventHandler(this, &R3FileDialog::R3FileDialog_Load);
			this->ResumeLayout(false);

		}
#pragma endregion

		System::ComponentModel::ComponentResourceManager^ GetResourceManager();

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

	private: System::Void select_file(System::Object^  sender, System::EventArgs^  e) {
				 Button^ testbtn = (System::Windows::Forms::Button^)sender;

				 FileName = testbtn->Name;
				 this->DialogResult = System::Windows::Forms::DialogResult::OK;
			 }

			 System::Void select_fastclean(System::Object^  sender, System::EventArgs^  e) {
				 fastClean = true;
				 Button^ testbtn = (System::Windows::Forms::Button^)sender;

				 FileName = testbtn->Name;
				 this->DialogResult = System::Windows::Forms::DialogResult::OK;
			 }

			 ref struct R3FileCache
			 {
				 String^ filename;
				 Label^ name;
				 PictureBox^ picture;
			 };
	private: System::Void ImageResolver_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
#pragma omp parallel for
				 for(int i = 0; i < filecache->Count; i++)
				 {
					 string filename;
					 ToString(filecache[i]->filename, filename);
					 R3AReader r3arg = R3AReader(filename);

					 if(r3arg.IsValid())
					 {
						 if(r3arg.IsCorrupted())
						 {
							 //TODO: Red background!
						 }
						 int dat_preview_position = 0;

						 vector<Class_Data_Block> cdb = r3arg.getclassdata();
						 vector<Class_Prototype> class_prototype = r3arg.getclassprototypes();

						 for(uint32_t j = 0; j < cdb.size(); j++)
						 {
							 string classname = class_prototype[cdb[j].prototype_of].name;
							 if(cdb[j].RAM.datpreview_positions.size())
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

								 filecache[i]->picture->Image = tbmp;
								 j = cdb.size();
							 }
						 }

						 String^ parkname = "Name unknown";
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
						 try
						 {
							 filecache[i]->name->Text = parkname;
						 } catch (...)
						 {

						 }
					 }
					 r3arg.Clean();
				 }
			 }
	private: System::Void R3FileDialog_Load(System::Object^  sender, System::EventArgs^  e) {
				 ImageResolver->RunWorkerAsync();
			 }
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenFileDialog ^ofd = gcnew OpenFileDialog();
				 ofd->InitialDirectory = Environment::GetFolderPath(System::Environment::SpecialFolder::MyDocuments) + "/RCT3/Parks";
				 ofd->CheckFileExists = true;

				 try {
					 ofd->Filter = gcnew String(allowedextensions.c_str());//"Park file|*.dat|Park backup file|*.bak|All|*.*";

					 ofd->Title = "Select a RCT3 file";					 

					 if (ofd->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
						 FileName = ofd->FileName;
						 this->DialogResult = System::Windows::Forms::DialogResult::OK;
					 }
					 else {
						 return;
					 }
				 }
				 finally {
					 delete ofd;
				 }
			 }
	};
}

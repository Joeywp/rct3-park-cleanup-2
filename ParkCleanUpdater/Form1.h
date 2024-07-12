#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vcclr.h>

#define MAX_SIZE 256

namespace ParkCleanUpdater {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;	

	bool debug = false;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
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
	private: System::Windows::Forms::Button^  update_button;
	protected: 
	private: System::Windows::Forms::ProgressBar^  progressBar1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  button1;

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
			this->update_button = (gcnew System::Windows::Forms::Button());
			this->progressBar1 = (gcnew System::Windows::Forms::ProgressBar());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// update_button
			// 
			this->update_button->Location = System::Drawing::Point(12, 38);
			this->update_button->Name = L"update_button";
			this->update_button->Size = System::Drawing::Size(318, 21);
			this->update_button->TabIndex = 3;
			this->update_button->Text = L"Update";
			this->update_button->UseVisualStyleBackColor = true;
			this->update_button->Click += gcnew System::EventHandler(this, &Form1::update_button_Click);
			// 
			// progressBar1
			// 
			this->progressBar1->Location = System::Drawing::Point(12, 65);
			this->progressBar1->Name = L"progressBar1";
			this->progressBar1->Size = System::Drawing::Size(318, 23);
			this->progressBar1->TabIndex = 2;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(9, 9);
			this->label1->MaximumSize = System::Drawing::Size(320, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(319, 26);
			this->label1->TabIndex = 4;
			this->label1->Text = L"This tool will update Park CleanUp. Make sure that all instances of Park CleanUp " 
				L"are closed before clicking the update button.";
			// 
			// button1
			// 
			this->button1->Enabled = false;
			this->button1->Location = System::Drawing::Point(253, 9);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 5;
			this->button1->Text = L"Encrypt";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Visible = false;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(342, 98);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->update_button);
			this->Controls->Add(this->progressBar1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->Name = L"Form1";
			this->Text = L"Updating Park CleanUp";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void update_button_Click(System::Object^  sender, System::EventArgs^  e) {
				 update_button->Enabled = false;

				 System::Net::WebClient ^_WebClient = gcnew System::Net::WebClient();
				 _WebClient->DownloadFileCompleted += gcnew AsyncCompletedEventHandler(this, &Form1::_DownloadFileCompleted);
				 _WebClient->DownloadProgressChanged += gcnew System::Net::DownloadProgressChangedEventHandler(this, &Form1::_DownloadProgressChanged);
				 if(File::Exists("dev.ldr"))
				 {
					 File::Delete("dev.ldr");
					 _WebClient->DownloadFileAsync(gcnew Uri("http://cloud.wonderplein.nl/pcu/devupdate.our"), "update.our");
				 }
				 else
				 {
					 _WebClient->DownloadFileAsync(gcnew Uri("http://cloud.wonderplein.nl/pcu/update.our"), "update.our");					 
				 }
			 }

			 void Form1::_DownloadFileCompleted(System::Object ^sender, AsyncCompletedEventArgs ^e)
			 {
				 progressBar1->Value = 100;
				 update_button->Enabled = Enabled;

				 FILE* downloaded;
				 FILE* parkcleanup;
				 char* pcu_name;
				 char* downloadname;

				 ToChar("Park CleanUp 2.exe", pcu_name);
				 ToChar("update.our", downloadname);

				 fopen_s(&downloaded, downloadname, "rb");
				 fopen_s(&parkcleanup, pcu_name, "wb");

				 encrypt_data(downloaded, parkcleanup, "null");
				 fclose(downloaded);
				 fclose(parkcleanup);
				 File::Delete("update.our");
				 Diagnostics::Process::Start("Park CleanUp 2.exe");
				 this->Close();
			 }

			 // Occurs when an asynchronous download operation successfully transfers some or all of the data.
			 void Form1::_DownloadProgressChanged(System::Object ^sender, System::Net::DownloadProgressChangedEventArgs ^e)
			 {
				 // Update progress bar
				 progressBar1->Value = e->ProgressPercentage;
			 }

			 void encrypt_data(FILE* input_file, FILE* output_file, char* key)
			 {
				 int key_count = 0; //Used to restart key if strlen(key) < strlen(encrypt)
				 int encrypt_byte;

				 //Loop through each byte of file until EOF
				 while( (encrypt_byte = fgetc(input_file)) != EOF) 
				 {
					 //XOR the data and write it to a file
					 fputc(encrypt_byte ^ key[key_count], output_file);

					 //Increment key_count and start over if necessary
					 key_count++;
					 if(key_count == strlen(key))
						 key_count = 0;
				 }
			 }

			 bool ToChar( String^ source, char*& target ) {
				 pin_ptr<const wchar_t> wch = PtrToStringChars( source );
				 int len = (( source->Length+1) * 2);
				 target = new char[ len ];
				 return wcstombs( target, wch, len ) != -1;
			 }
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
				 FILE* downloaded;
				 FILE* parkcleanup;
				 char* pcu_name;
				 char* downloadname;

				 ToChar("Park CleanUp 2.exe", pcu_name);
				 ToChar("Park CleanUp 2.our", downloadname);

				 fopen_s(&downloaded, downloadname, "wb");
				 fopen_s(&parkcleanup, pcu_name, "rb");

				 encrypt_data(parkcleanup, downloaded, "null");
				 fclose(downloaded);
				 fclose(parkcleanup);
			 }
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
				 if(File::Exists("dev.ldr"))
					 this->Text = this->Text + " - Updating to new development edition";
			 }
	};
}


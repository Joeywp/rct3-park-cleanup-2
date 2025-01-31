#pragma once

#include <Windows.h>
#include <vcclr.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <time.h>
#include <math.h>
#include <string>
#include <vector>

#include <iostream>
#include <fstream>

using namespace std;
using namespace System::IO; 
using namespace System::Diagnostics;
using namespace System::Windows;

namespace FilelistBuilder {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

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
	private: System::Windows::Forms::Button^  button1;
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
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(13, 13);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 0;
			this->button1->Text = L"button1";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 261);
			this->Controls->Add(this->button1);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
				 vector<string> strstowrite;
				 GetFilesNumber("C:\\Program Files (x86)\\Atari\\RollerCoaster Tycoon 3\\", strstowrite);

				 ofstream strdmp("strdmp.txt");
				 if(strdmp.is_open())
				 {
					 for(int i = 0; i < strstowrite.size(); i++)
					 {
						 strdmp << strstowrite[i] << "\n";
					 }
					 strdmp.close();
				 }
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

			 int GetFilesNumber(String^ folder, vector<string> &strs){
				 array<String^>^ dir = Directory::GetDirectories( folder );
				 for (int i=0; i<dir->Length; i++){
					 String^ send = dir[i]->Replace(folder+"\\","");
					 GetFilesNumber(dir[i], strs);
				 }

				 array<String^>^ file = Directory::GetFiles( folder );
				 for (int i=0; i<file->Length; i++){					 
					 if(file[i]->Contains(".common.ovl")){
						 string str;
						 String^ curfile = file[i];
						 curfile = curfile->Replace("C:\\Program Files (x86)\\Atari\\RollerCoaster Tycoon 3\\", "");
						 ToString(curfile, str);
						 strs.push_back(str);
					 }
				 }

				 return 1;
			 }
	};
}


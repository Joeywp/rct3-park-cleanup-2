#pragma once

namespace ParkCleanUp2 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Tutorial
	/// </summary>
	public ref class Tutorial : public System::Windows::Forms::Form
	{
	public:
		Tutorial(void)
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
		~Tutorial()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	protected: 
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::TabControl^  tuttabs;

	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::TabPage^  tabPage2;
	private: System::Windows::Forms::TabPage^  tabPage3;

	private: System::Windows::Forms::Button^  btn_next;
	private: System::Windows::Forms::TabPage^  tabPage5;

	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  cleanup_version;

	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::PictureBox^  pictureBox1;

	private: System::Windows::Forms::PictureBox^  pictureBox4;
	private: System::Windows::Forms::PictureBox^  pictureBox2;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::PictureBox^  pictureBox3;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  label2;





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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Tutorial::typeid));
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->tuttabs = (gcnew System::Windows::Forms::TabControl());
			this->tabPage5 = (gcnew System::Windows::Forms::TabPage());
			this->pictureBox4 = (gcnew System::Windows::Forms::PictureBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->cleanup_version = (gcnew System::Windows::Forms::Label());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->tabPage3 = (gcnew System::Windows::Forms::TabPage());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->pictureBox3 = (gcnew System::Windows::Forms::PictureBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->btn_next = (gcnew System::Windows::Forms::Button());
			this->tuttabs->SuspendLayout();
			this->tabPage5->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox4))->BeginInit();
			this->tabPage1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			this->tabPage2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox2))->BeginInit();
			this->tabPage3->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox3))->BeginInit();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(0, 51);
			this->label1->MaximumSize = System::Drawing::Size(465, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(465, 52);
			this->label1->TabIndex = 0;
			this->label1->Text = resources->GetString(L"label1.Text");
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(369, 311);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(118, 23);
			this->button1->TabIndex = 1;
			this->button1->Text = L"Skip introduction";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Tutorial::button1_Click);
			// 
			// tuttabs
			// 
			this->tuttabs->Controls->Add(this->tabPage5);
			this->tuttabs->Controls->Add(this->tabPage1);
			this->tuttabs->Controls->Add(this->tabPage2);
			this->tuttabs->Controls->Add(this->tabPage3);
			this->tuttabs->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->tuttabs->Location = System::Drawing::Point(13, 12);
			this->tuttabs->Name = L"tuttabs";
			this->tuttabs->SelectedIndex = 0;
			this->tuttabs->Size = System::Drawing::Size(478, 293);
			this->tuttabs->TabIndex = 2;
			// 
			// tabPage5
			// 
			this->tabPage5->Controls->Add(this->pictureBox4);
			this->tabPage5->Controls->Add(this->label3);
			this->tabPage5->Controls->Add(this->cleanup_version);
			this->tabPage5->Controls->Add(this->label1);
			this->tabPage5->Location = System::Drawing::Point(4, 26);
			this->tabPage5->Name = L"tabPage5";
			this->tabPage5->Padding = System::Windows::Forms::Padding(3);
			this->tabPage5->Size = System::Drawing::Size(470, 263);
			this->tabPage5->TabIndex = 4;
			this->tabPage5->Text = L"Introduction";
			this->tabPage5->UseVisualStyleBackColor = true;
			// 
			// pictureBox4
			// 
			this->pictureBox4->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox4.Image")));
			this->pictureBox4->Location = System::Drawing::Point(3, 0);
			this->pictureBox4->Name = L"pictureBox4";
			this->pictureBox4->Size = System::Drawing::Size(48, 48);
			this->pictureBox4->SizeMode = System::Windows::Forms::PictureBoxSizeMode::AutoSize;
			this->pictureBox4->TabIndex = 3;
			this->pictureBox4->TabStop = false;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label3->Location = System::Drawing::Point(3, 118);
			this->label3->MaximumSize = System::Drawing::Size(465, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(464, 26);
			this->label3->TabIndex = 2;
			this->label3->Text = L"This tutorial will explain the basics on how to use Park CleanUp in order to impr" 
				L"ove your RCT3 experience.";
			// 
			// cleanup_version
			// 
			this->cleanup_version->AutoSize = true;
			this->cleanup_version->Font = (gcnew System::Drawing::Font(L"Segoe UI", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->cleanup_version->Location = System::Drawing::Point(57, 7);
			this->cleanup_version->MaximumSize = System::Drawing::Size(475, 0);
			this->cleanup_version->Name = L"cleanup_version";
			this->cleanup_version->Size = System::Drawing::Size(300, 32);
			this->cleanup_version->TabIndex = 1;
			this->cleanup_version->Text = L"Welcome to Park CleanUp!";
			// 
			// tabPage1
			// 
			this->tabPage1->Controls->Add(this->pictureBox1);
			this->tabPage1->Controls->Add(this->label4);
			this->tabPage1->Controls->Add(this->label5);
			this->tabPage1->Location = System::Drawing::Point(4, 26);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(470, 263);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"Loading a parkfile";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// pictureBox1
			// 
			this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox1.Image")));
			this->pictureBox1->Location = System::Drawing::Point(78, 115);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(306, 80);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::AutoSize;
			this->pictureBox1->TabIndex = 4;
			this->pictureBox1->TabStop = false;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Font = (gcnew System::Drawing::Font(L"Segoe UI", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label4->Location = System::Drawing::Point(3, 0);
			this->label4->MaximumSize = System::Drawing::Size(475, 0);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(205, 32);
			this->label4->TabIndex = 3;
			this->label4->Text = L"Loading a parkfile";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label5->Location = System::Drawing::Point(3, 39);
			this->label5->MaximumSize = System::Drawing::Size(465, 0);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(459, 52);
			this->label5->TabIndex = 2;
			this->label5->Text = resources->GetString(L"label5.Text");
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->pictureBox2);
			this->tabPage2->Controls->Add(this->label6);
			this->tabPage2->Controls->Add(this->label7);
			this->tabPage2->Location = System::Drawing::Point(4, 26);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(470, 263);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"Cleaning the parkfile";
			this->tabPage2->UseVisualStyleBackColor = true;
			this->tabPage2->Click += gcnew System::EventHandler(this, &Tutorial::tabPage2_Click);
			// 
			// pictureBox2
			// 
			this->pictureBox2->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox2.Image")));
			this->pictureBox2->Location = System::Drawing::Point(-5, 101);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(534, 111);
			this->pictureBox2->SizeMode = System::Windows::Forms::PictureBoxSizeMode::AutoSize;
			this->pictureBox2->TabIndex = 7;
			this->pictureBox2->TabStop = false;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Font = (gcnew System::Drawing::Font(L"Segoe UI", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label6->Location = System::Drawing::Point(3, 0);
			this->label6->MaximumSize = System::Drawing::Size(475, 0);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(311, 32);
			this->label6->TabIndex = 6;
			this->label6->Text = L"Cleaning the parkfile (basic)";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label7->Location = System::Drawing::Point(3, 39);
			this->label7->MaximumSize = System::Drawing::Size(465, 0);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(453, 52);
			this->label7->TabIndex = 5;
			this->label7->Text = resources->GetString(L"label7.Text");
			// 
			// tabPage3
			// 
			this->tabPage3->Controls->Add(this->label2);
			this->tabPage3->Controls->Add(this->pictureBox3);
			this->tabPage3->Controls->Add(this->label8);
			this->tabPage3->Controls->Add(this->label9);
			this->tabPage3->Location = System::Drawing::Point(4, 26);
			this->tabPage3->Name = L"tabPage3";
			this->tabPage3->Padding = System::Windows::Forms::Padding(3);
			this->tabPage3->Size = System::Drawing::Size(470, 263);
			this->tabPage3->TabIndex = 2;
			this->tabPage3->Text = L"Saving the parkfile";
			this->tabPage3->UseVisualStyleBackColor = true;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(2, 161);
			this->label2->MaximumSize = System::Drawing::Size(465, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(464, 52);
			this->label2->TabIndex = 11;
			this->label2->Text = resources->GetString(L"label2.Text");
			// 
			// pictureBox3
			// 
			this->pictureBox3->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox3.Image")));
			this->pictureBox3->Location = System::Drawing::Point(98, 99);
			this->pictureBox3->Name = L"pictureBox3";
			this->pictureBox3->Size = System::Drawing::Size(244, 53);
			this->pictureBox3->SizeMode = System::Windows::Forms::PictureBoxSizeMode::AutoSize;
			this->pictureBox3->TabIndex = 10;
			this->pictureBox3->TabStop = false;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Font = (gcnew System::Drawing::Font(L"Segoe UI", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label8->Location = System::Drawing::Point(3, 0);
			this->label8->MaximumSize = System::Drawing::Size(475, 0);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(214, 32);
			this->label8->TabIndex = 9;
			this->label8->Text = L"Saving the parkfile";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label9->Location = System::Drawing::Point(3, 39);
			this->label9->MaximumSize = System::Drawing::Size(465, 0);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(465, 52);
			this->label9->TabIndex = 8;
			this->label9->Text = resources->GetString(L"label9.Text");
			// 
			// btn_next
			// 
			this->btn_next->Location = System::Drawing::Point(371, 259);
			this->btn_next->Name = L"btn_next";
			this->btn_next->Size = System::Drawing::Size(112, 39);
			this->btn_next->TabIndex = 3;
			this->btn_next->Text = L"Next step >>";
			this->btn_next->UseVisualStyleBackColor = true;
			this->btn_next->Click += gcnew System::EventHandler(this, &Tutorial::button2_Click);
			// 
			// Tutorial
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(503, 343);
			this->Controls->Add(this->btn_next);
			this->Controls->Add(this->tuttabs);
			this->Controls->Add(this->button1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"Tutorial";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"Introduction to Park CleanUp";
			this->Load += gcnew System::EventHandler(this, &Tutorial::Tutorial_Load);
			this->tuttabs->ResumeLayout(false);
			this->tabPage5->ResumeLayout(false);
			this->tabPage5->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox4))->EndInit();
			this->tabPage1->ResumeLayout(false);
			this->tabPage1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			this->tabPage2->ResumeLayout(false);
			this->tabPage2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox2))->EndInit();
			this->tabPage3->ResumeLayout(false);
			this->tabPage3->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox3))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: String^ GetVersion();
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {				 
				 if(tuttabs->SelectedIndex + 2 == tuttabs->TabCount)
				 {
					 btn_next->Text + "Start Park CleanUp";
				 }
				 if(tuttabs->SelectedIndex + 1 == tuttabs->TabCount)
				 {
					 this->Close();
				 }
				 if(tuttabs->SelectedIndex + 1 < tuttabs->TabCount)
					 tuttabs->SelectedIndex = (tuttabs->SelectedIndex + 1) % tuttabs->TabCount;
			 }
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->Close();
			 }
	private: System::Void Tutorial_Load(System::Object^  sender, System::EventArgs^  e) {
				 cleanup_version->Text = "Welcome to Park CleanUp v" + GetVersion() + "!";
			 }
	private: System::Void tabPage2_Click(System::Object^  sender, System::EventArgs^  e) {
			 }
};
}

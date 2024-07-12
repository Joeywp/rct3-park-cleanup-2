#pragma once
#include "R3AReader.h"
#include "OVLReader.h"
#include "AdditionalStructs.h"

namespace ParkCleanUp2 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Windows::Forms::DataVisualization::Charting;

	extern R3AReader r3arg;
	extern vector<Class_Data_Block> cdb;
	extern vector<Class_Prototype> class_prototype;
	extern vector<ParentedReference> parented_refs;
	extern vector<bool> list_checkeditems;
	extern vector<Reference> TrackUIDs;

	extern uint32_t selectmode;
	extern string sm_select_name;
	extern unsigned long dat_preview_position;
	extern bool defrag;

	extern string allowedextensions;

	/// <summary>
	/// Summary for ParkAnalyser
	/// </summary>
	public ref class ParkAnalyser : public System::Windows::Forms::Form
	{
	public:
		ParkAnalyser(void)
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
		~ParkAnalyser()
		{
			if (components)
			{
				delete components;
			}
		}

	protected: 

	private: System::ComponentModel::BackgroundWorker^  loader;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^  classgraph;





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
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^  legend1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^  series1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			this->loader = (gcnew System::ComponentModel::BackgroundWorker());
			this->classgraph = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->classgraph))->BeginInit();
			this->SuspendLayout();
			// 
			// loader
			// 
			this->loader->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &ParkAnalyser::loader_DoWork);
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
			this->classgraph->Location = System::Drawing::Point(12, 12);
			this->classgraph->Name = L"classgraph";
			series1->ChartArea = L"ChartArea1";
			series1->Legend = L"Legend1";
			series1->Name = L"Series1";
			this->classgraph->Series->Add(series1);
			this->classgraph->Size = System::Drawing::Size(754, 344);
			this->classgraph->TabIndex = 0;
			this->classgraph->Text = L"chart1";
			// 
			// ParkAnalyser
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(778, 368);
			this->Controls->Add(this->classgraph);
			this->Name = L"ParkAnalyser";
			this->Text = L"Park CleanUp analyser";
			this->Load += gcnew System::EventHandler(this, &ParkAnalyser::ParkAnalyser_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->classgraph))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion

	private: System::Void loader_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
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
					 if(data[i].count != 0)
					 {
						 String^ name = gcnew String(data[i].name.c_str());
						 classgraph->Series->Add(name);
						 classgraph->Series[name]->Points->Add(data[i].count);
						 name += " size";
						 classgraph->Series->Add(name);
						 classgraph->Series[name]->Points->Add(data[i].size);
					 }
					 count++;
					 if(count == 10)
						 break;
				 }

				 //classgraph->Size = System::Drawing::Size( data.size() * 100, 300 );
			 }
	private: System::Void ParkAnalyser_Load(System::Object^  sender, System::EventArgs^  e) {
				 loader->RunWorkerAsync();
			 }
};
}

#pragma once
#include <stdint.h>

using namespace std;

const float TC_HOUR = (1.0f/24.0f);
const float TC_MINUTE = (TC_HOUR/60.0f);

struct TimeConverter
{
	float time;

	TimeConverter(float time)
	{
		this->time = time;
	}

	void SetTime(int hours, int minutes)
	{
		time = (hours * TC_HOUR) + (minutes * TC_MINUTE);
	}

	// Hours
	void DecreaseHours()
	{
		time -= TC_HOUR;
	}

	void IncreaseHours()
	{
		time += TC_HOUR;
	}

	void DeltaHours(int count)
	{
		time += count * TC_HOUR;
	}

	// Minutes
	void DecreaseMinutes()
	{
		time -= TC_MINUTE;
	}

	void IncreaseMinutes()
	{
		time += TC_MINUTE;
	}

	void DeltaMinutes(int count)
	{
		time += count * TC_MINUTE;
	}

};

struct BlueprintFile
{
	string fullfilename;
	string relativename;
	int hash;
};

struct u8Color
{
	uint8_t a;
	uint8_t b;
	uint8_t c;
	uint8_t d;
};

struct Labels
{
	enum Named : int32_t
	{
		lbl_classdata1 = 0,
		lbl_classdata2,
		sl_appstatus,
		tb_parkinfo,
		loading,
		pb_status,
	};
};

struct SM
{
	enum SelectionMode
	{
		DEFAULT = 0,
		SELECT_FROM_STRINGNAME,
		SELECT_FIREWORK_RELATED,
		SELECT_NONINSTALLED_CONTENT,
		SELECT_GUEST_ITEMS,
		DUCK
	};
};

struct IndexedString
{
	int count;
	int size;
	std::string name;

	inline bool operator()(const IndexedString& a, const IndexedString& b)
	{
		if(a.size == b.size)
			return a.size < b.size;

		return a.size > b.size;
	}
};

namespace PCU_Channels
{
	enum Channel
	{
		DEFAULT = 0,
		DEBUG
	};
};

struct ErrorLog
{
private:
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
public:
	std::string filename;
	std::string error;
	std::vector<std::string> parents;
	ErrorLog(String^ fname, String^ err, std::vector<std::string> prts)
	{
		this->ToString(fname, this->filename);
		this->ToString(err, this->error);
		this->parents = prts;
	}
	bool operator() (ErrorLog a, ErrorLog b)
	{
		if(a.filename == b.filename)
			return 0;
		return a.filename < b.filename;
	}
};
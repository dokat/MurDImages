#pragma once
#include <vector>
#include <codecvt>
#include <locale>
#include "ofxXmlSettings.h"

using namespace std;

class utils {
public:
	static string utf8ToString(const string utf8Str)
	{
		const locale loc = locale(".1252");

		// UTF-8 to wstring
		wstring_convert<codecvt_utf8<wchar_t>> wconv;
		wstring wstr = wconv.from_bytes(utf8Str);
		// wstring to string
		vector<char> buf(wstr.size());
		use_facet<ctype<wchar_t>>(loc).narrow(wstr.data(), wstr.data() + wstr.size(), '?', buf.data());
		return string(buf.data(), buf.size());
	}

	static void getProjectorPositionAndSize(ofVec2f * position, ofVec2f * size)
	{
		ofxXmlSettings settings;
		settings.loadFile("settings.xml");
		float x = settings.getValue("settings:projector:x", 1920);
		float y = settings.getValue("settings:projector:y", 0);
		float width = settings.getValue("settings:projector:largeur", 1920);
		float height = settings.getValue("settings:projector:hauteur", 1080);

		position->x = x;
		position->y = y;
		size->x = width;
		size->y = height;
	}
};

/*
// trim from start (in place)
static inline void ltrim(std::string &s) {
s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
return !std::isspace(ch);
}));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
return !std::isspace(ch);
}).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
ltrim(s);
rtrim(s);
}

static inline std::string trim_copy(std::string s) {
return rtrim_copy(ltrim_copy(s));
}

// trim from start (copying)
static inline std::string ltrim_copy(std::string s) {
ltrim(s);
return s;
}

// trim from end (copying)
static inline std::string rtrim_copy(std::string s) {
rtrim(s);
return s;
}

static inline std::string toLower(std::string s) {
std::transform(s.begin(), s.end(), s.begin(), ::tolower);
return s;
}
*/
#include "stdafx.h"
#include "CNonDiscreteSquawk2.h"

extern "C" IMAGE_DOS_HEADER __ImageBase;
char DllPathFile[_MAX_PATH];

const int TAG_ITEM_NON_DISCRETE_SSR = 1;

CNonDiscreteSquawk::CNonDiscreteSquawk() : CPlugIn( EuroScopePlugIn::COMPATIBILITY_CODE,
													PLUGIN_NAME,
													PLUGIN_VERSION,
													PLUGIN_AUTHOR,
													PLUGIN_LICENSE)
{
	loadJson();
	RegisterTagItemType("Squawk Message/Warning", TAG_ITEM_NON_DISCRETE_SSR);
}


CNonDiscreteSquawk::~CNonDiscreteSquawk()
{
}

void CNonDiscreteSquawk::OnGetTagItem(EuroScopePlugIn::CFlightPlan flightPlan,
	EuroScopePlugIn::CRadarTarget radarTarget, 
	int itemCode, 
	int tagData, 
	char sItemString[16], 
	int * pColorCode,
	COLORREF * pRGB,
	double * pFontSize)
{
	if (!radarTarget.IsValid())
		return;
	
	const char* ssr = radarTarget.GetPosition().GetSquawk();

	switch (itemCode)
	{
	case TAG_ITEM_NON_DISCRETE_SSR:
		auto it = std::find_if(squawks.begin(), squawks.end(), [ssr](Squawk s) { return s.code == std::string(ssr); });
		if (it != squawks.end()) {
			sprintf_s(sItemString, 16, it->msg.c_str());
			*pColorCode = EuroScopePlugIn::TAG_COLOR_RGB_DEFINED;
			*pRGB = it->color;
		}
	}
}

void CNonDiscreteSquawk::loadJson() {
	std::ifstream f;
	GetModuleFileNameA(HINSTANCE(&__ImageBase), DllPathFile, sizeof(DllPathFile));
	std::string path = DllPathFile;
	path.resize(path.size() - strlen("Squawks.dll"));
	path += "squawks.json";
	f.open(path);
	if (!f.is_open())
		return;

	nlohmann::json j;
	squawks.clear();
	f >> j;
	int r, g, b;
	sscanf_s(j["NonDiscreteColor"].get<std::string>().c_str(), "#%02x%02x%02x", &r, &g, &b);
	COLORREF ndColor = RGB(r, g, b);
	for (nlohmann::json s : j["NonDiscrete"]) {
		squawks.emplace_back(s, j["NonDiscreteMessage"], ndColor);
	}

	for (nlohmann::json s : j["Squawks"]) {
		int r, g, b;
		sscanf_s(s["color"].get<std::string>().c_str(), "#%02x%02x%02x", &r, &g, &b);
		COLORREF color = RGB(r, g, b);
		squawks.emplace_back(s["code"], s["message"], color);
	}

	f.close();
}

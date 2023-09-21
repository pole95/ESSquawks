#pragma once
#include <EuroScopePlugIn.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include "json.hpp"

#define PLUGIN_NAME "Squawk Message/Warning"
#define PLUGIN_VERSION "1.0.1"
#define PLUGIN_AUTHOR "Pol Eyschen"
#define PLUGIN_LICENSE "GPL v3"

struct Squawk {
	std::string code;
	std::string msg;
	COLORREF color;
	Squawk(std::string code, std::string msg, COLORREF color) :code(code), msg(msg), color(color) {}
};

class CNonDiscreteSquawk : public EuroScopePlugIn::CPlugIn
{
public:
	CNonDiscreteSquawk();
	~CNonDiscreteSquawk();
	virtual void OnGetTagItem(EuroScopePlugIn::CFlightPlan flightPlan,
		EuroScopePlugIn::CRadarTarget radarTarget,
		int itemCode,
		int tagData,
		char sItemString[16],
		int *pColorCode,
		COLORREF *pRGB,
		double *pFontSize);
private:
	std::vector<Squawk> squawks;
	void loadJson();
};


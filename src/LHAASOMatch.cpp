#include <stdlib.h>
#include <cmath>
#include <iostream>
#include "LHAASOMatch.h"

using namespace std;

LHAASOMatch::LHAASOMatch()
{
	Initial();
}

LHAASOMatch::~LHAASOMatch()
{
	Initial();
}

void LHAASOMatch::Initial()
{
	is_tel_event=0;
	deltaT.clear();
	matchedEvents.clear();
}

void LHAASOMatch::DoMatch(long long matchTime, int &begin_evt, const std::vector<wfctatime> &televents, long long matchingTstep)
{
	long long dt;
	for(int iiwfcta=begin_evt;iiwfcta<televents.size();iiwfcta++)
	{
		dt = televents.at(iiwfcta).event_alltime-matchTime;
		if(dt>matchingTstep){  break;  }
		if(abs(dt)<matchingTstep)
		{
			is_tel_event = 1;
			deltaT.push_back(dt);
			matchedEvents.push_back(televents.at(iiwfcta).event_ievt);
			begin_evt = iiwfcta+1;
		}
	}
}





















#ifndef LHAASOMATCH_H
#define LHAASOMATCH_H

#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <map>
#include "EventInfo.h"

class LHAASOMatch
{
	public:
		int is_tel_event;
		std::vector<long long> deltaT;
		std::vector<double> matchedEvents;

	public:
		LHAASOMatch();
		~LHAASOMatch();
		void Initial();
		void DoMatch(long long matchTime, int &begin_evt, const std::vector<wfctatime> &televents, long long matchingTstep);

};

#endif // LHAASOMATCH_H

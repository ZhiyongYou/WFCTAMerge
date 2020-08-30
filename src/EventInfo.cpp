#include "EventInfo.h"
int comp(const wfctatime &s1,const wfctatime &s2){
	    return s1.event_alltime<s2.event_alltime;
}

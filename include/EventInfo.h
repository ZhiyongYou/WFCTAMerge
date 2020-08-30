#ifndef EVENTINFO_H
#define EVENTINFO_H
struct wfctatime{
	double event_ievt; //ifile ientry
	long long event_alltime; //event time
};

extern int comp(const wfctatime &s1,const wfctatime &s2);
#endif // EVENTINFO_H

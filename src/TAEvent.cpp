#include <stdlib.h>
#include <cmath>
#include <iostream>
#include "TAEvent.h"

using namespace std;

ClassImp(TAEvent);

TAEvent::TAEvent():TObject()
{
	int MAXPMT = 1024;
	packCheck.resize(MAXPMT);
	eevent.resize(MAXPMT);
	zipmod.resize(MAXPMT);
	iSiPM.resize(MAXPMT);
	winsum.resize(MAXPMT);

	eSatH.resize(MAXPMT);
	eSatL.resize(MAXPMT);

	BaseH.resize(MAXPMT);
	BaseL.resize(MAXPMT);
	LaserBaseH.resize(MAXPMT);
	LaserBaseL.resize(MAXPMT);
	BaseHRMS.resize(MAXPMT);
	BaseLRMS.resize(MAXPMT);
	LaserBaseHRMS.resize(MAXPMT);
	LaserBaseLRMS.resize(MAXPMT);
	AdcH.resize(MAXPMT);
	AdcL.resize(MAXPMT);
	LaserAdcH.resize(MAXPMT);
	LaserAdcL.resize(MAXPMT);

	SatH.resize(MAXPMT);
	SatL.resize(MAXPMT);

	PeakPosShift.resize(MAXPMT);
	PeakPosH.resize(MAXPMT);
	PeakPosL.resize(MAXPMT);
	PeakAmH.resize(MAXPMT);
	PeakAmL.resize(MAXPMT);
	Over_Single_Marker.resize(MAXPMT);
	Over_Record_Marker.resize(MAXPMT);

	EventInitial();

}

TAEvent::~TAEvent()
{
	EventInitial();
}

void TAEvent::EventInitial()
{
	for(int i=0;i<6;i++)
	{
		telTrigger[i]=0;
		rbTime[i]=0;
		rbtime[i]=0;
		status[i]=-1;
	}
	merge_size=-1;
	iEvent=-1;
	eEvent=-1;
	rabbitTime=0;
	rabbittime=0;
	big_pack_lenth=-1;
	n_fired=-1;
	n_Channel=-1;
	packCheck.clear();
	iSiPM.clear();
	zipmod.clear();
	eevent.clear();
	winsum.clear();
	PeakPosShift.clear();
	PeakPosH.clear();
	PeakPosL.clear();
	PeakAmH.clear();
	PeakAmL.clear();
	Over_Single_Marker.clear();
	Over_Record_Marker.clear();
	eSatH.clear();
	eSatL.clear();
	SatH.clear();
	SatL.clear();
	BaseH.clear();
	BaseL.clear();
	LaserBaseH.clear();
	LaserBaseL.clear();
	BaseHRMS.clear();
	BaseLRMS.clear();
	LaserBaseHRMS.clear();
	LaserBaseLRMS.clear();
	AdcH.clear();
	AdcL.clear();
	LaserAdcH.clear();
	LaserAdcL.clear();
}

void TAEvent::SetWFCTAEvent(long long Rb_Time,double Rb_time, WFCTAEvent* telEvent)
{
	int ISIPM=0;
	int tel_id = telEvent->ITel();
	rbTime[tel_id-1] = telEvent->rabbitTime;
	rbtime[tel_id-1] = telEvent->rabbittime;

	rabbitTime = Rb_Time;//event Time
	rabbittime = Rb_time;//event time

	int peak_shift =  (telEvent->rabbitTime-Rb_Time)*1000000000 + (telEvent->rabbittime*20-Rb_time) ;

	for(int ii=0;ii<telEvent->iSiPM.size();ii++)
	{   
		ISIPM = (tel_id-1)*1024 + telEvent->iSiPM.at(ii);

		eevent.push_back(telEvent->eevent.at(ii));
		zipmod.push_back(telEvent->zipmod.at(ii));
		iSiPM.push_back(ISIPM);
		winsum.push_back(telEvent->winsum.at(ii));
		eSatH.push_back(telEvent->eSatH.at(ii));
		eSatL.push_back(telEvent->eSatL.at(ii));
		BaseH.push_back(telEvent->BaseH.at(ii));
		BaseL.push_back(telEvent->BaseL.at(ii));
		BaseHRMS.push_back(telEvent->BaseHRMS.at(ii));
		BaseLRMS.push_back(telEvent->BaseLRMS.at(ii));
		LaserBaseH.push_back(telEvent->LaserBaseH.at(ii));
		LaserBaseL.push_back(telEvent->LaserBaseL.at(ii));
		LaserBaseHRMS.push_back(telEvent->LaserBaseHRMS.at(ii));
		LaserBaseLRMS.push_back(telEvent->LaserBaseLRMS.at(ii));
		AdcH.push_back(telEvent->AdcH.at(ii));
		AdcL.push_back(telEvent->AdcL.at(ii));
		LaserAdcH.push_back(telEvent->LaserAdcH.at(ii));
		LaserAdcL.push_back(telEvent->LaserAdcL.at(ii));
		SatH.push_back(telEvent->SatH.at(ii));
		SatL.push_back(telEvent->SatL.at(ii));
		PeakPosShift.push_back(peak_shift);
		PeakPosH.push_back(telEvent->PeakPosH.at(ii));
		PeakPosL.push_back(telEvent->PeakPosL.at(ii));
		PeakAmH.push_back(telEvent->PeakAmH.at(ii));
		PeakAmL.push_back(telEvent->PeakAmL.at(ii));
		Over_Single_Marker.push_back(telEvent->Over_Single_Marker.at(ii));
		Over_Record_Marker.push_back(telEvent->Over_Record_Marker.at(ii));
	}
}
	/*
void TAEvent::SetWFCTAEvent(int tel,
		long long Rb_Time,
		double Rb_time,
		long long rb_Time,
		double rb_time,
		long long dt,
		std::vector<int> &sipm,
		std::vector<double> &sipmPe,
		std::vector<double> &sipmT,
		std::vector<int> sipmBase,
		std::vector<int> sipmBaserms)
{   
	int ISIPM=0;
	   rbTime[tel-1] = Rb_Time;//single tel Time
	   rbtime[tel-1] = Rb_time;//single tel time
	   rabbitTime = rb_Time;//event Time
	   rabbittime = rb_time;//event time
	   for(int ii=0;ii<sipm.size();ii++)
	   {   
	   ISIPM = (tel-1)*1024 + sipm.at(ii);
	   isipm.push_back(ISIPM);
	   sipmpe.push_back(sipmPe.at(ii));
	   sipmt.push_back(sipmT.at(ii)+dt);
	   sipmbase.push_back(sipmBase.at(ii));
	   sipmbaserms.push_back(sipmBaserms.at(ii));
	   }
}
	   */

void TAEvent::SetTriggerTel(int tel, int trig)
{
	telTrigger[tel-1] = trig;
}
void TAEvent::SetSlcStatus(int tel,int slc_status)
{
	status[tel-1] = slc_status;
}

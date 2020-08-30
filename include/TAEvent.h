#ifndef TAEVENT_H
#define TAEVENT_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "TTree.h"
#include "TBranch.h"
#include "TObject.h"
#include "WFCTAEvent.h"

using namespace std;
class TAEvent : public TObject
{
	protected:
		static TAEvent* _Head;
		static TTree* _Tree;
		static const char * _Name;
		static TBranch* bAll;

	public:
		long rabbitTime;
		double rabbittime;

		int telTrigger[6];
		long long rbTime[6];
		double rbtime[6];
		int status[6];

		short merge_size;
		long iEvent;
		long eEvent;
		int big_pack_lenth;
		short n_fired;
		short n_Channel;
		vector<short> packCheck;

		vector<long> eevent;
		vector<short> zipmod;
		vector<short> iSiPM;
		vector<float> winsum;
		vector<bool> eSatH;
		vector<bool> eSatL;
		vector<float> BaseH;
		vector<float> BaseL;
		vector<float> BaseHRMS;
		vector<float> BaseLRMS;
		vector<float> LaserBaseH;
		vector<float> LaserBaseL;
		vector<float> LaserBaseHRMS;
		vector<float> LaserBaseLRMS;
		vector<float> AdcH;
		vector<float> AdcL;
		vector<float> LaserAdcH;
		vector<float> LaserAdcL;
		vector<bool> SatH;
		vector<bool> SatL;
		vector<int> PeakPosShift;
		vector<short> PeakPosH;
		vector<short> PeakPosL;
		vector<int> PeakAmH;
		vector<int> PeakAmL;
		vector<bool> Over_Single_Marker;
		vector<bool> Over_Record_Marker;


	public:
		TAEvent();
		~TAEvent();
		void EventInitial();

		Short_t Merge_Size() { return merge_size; } 
		Long_t IEvent() { return iEvent; }
		Long_t EEvent() { return eEvent; }
		Long_t RabbitTime() { return rabbitTime; }
		Double_t Rabbittime() { return rabbittime; }
		Int_t Big_Pack_Len() { return big_pack_lenth; }
		Short_t N_fired() { return n_fired; }
		Short_t N_channel() { return n_Channel; }
		vector<short> PackCheck() { return packCheck; }

		vector<long> Eevent() { return eevent; }
		vector<short> Zipmod() { return zipmod; }
		vector<short> SiPMNO() { return iSiPM; }
		vector<float> Win4Sum() { return winsum; }
		vector<bool> eSath() { return eSatH; }
		vector<bool> eSatl() { return eSatL; }
		vector<float> Baseh() { return BaseH; }
		vector<float> Basel() { return BaseL; }
		vector<float> Basehrms() { return BaseHRMS; };
		vector<float> Baselrms() { return BaseLRMS; };
		vector<float> LaserBaseh() { return LaserBaseH; }
		vector<float> LaserBasel() { return LaserBaseL; }
		vector<float> LaserBasehrms() { return LaserBaseHRMS; };
		vector<float> LaserBaselrms() { return LaserBaseLRMS; };
		vector<float> Adch() { return AdcH; }
		vector<float> Adcl() { return AdcL; }
		vector<float> LaserAdch() { return LaserAdcH; }
		vector<float> LaserAdcl() { return LaserAdcL; }
		vector<bool> Sath() { return SatH; }
		vector<bool> Satl() { return SatL; }
		vector<int> PeakPosshift() { return PeakPosShift; }
		vector<short> PeakPosh() { return PeakPosH; }
		vector<short> PeakPosl() { return PeakPosL; }
		vector<int> PeakAmh() { return PeakAmH; }
		vector<int> PeakAml() { return PeakAmL; }
		vector<bool> Single_Marker() { return Over_Single_Marker; }
		vector<bool> Record_Marker() { return Over_Record_Marker; }

		void SetWFCTAEvent(long long Rb_Time,double Rb_time, WFCTAEvent* telEvent);

		void SetTriggerTel(int tel, int trig);
		void SetSlcStatus(int tel,int slc_status);

		ClassDef(TAEvent,1);
};

//ClassImp(TAEvent);

#endif // TAEVENT_H

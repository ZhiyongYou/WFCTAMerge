#ifndef WFCTASLC_H
#define WFCTASLC_H

#include <vector>

class WFCTASLC
{
	public:
		WFCTASLC();
		~WFCTASLC();

	public:
		void ReadSlcFile(int year, int month, int day);
		int GetSlcStatus(long long rbTime, int tel_id);
		//void GetSipmAZ(int isipm, double& azi, double& zen);
		//void GetSipmAZ(int telid, int isipm, double& azi, double& zen);


	private:
		std::vector<std::pair<double,int> > slc_status[6];
//		std::vector<std::pair<double,int> > status_2;
//		std::vector<std::pair<double,int> > status_3;
//		std::vector<std::pair<double,int> > status_4;
//		std::vector<std::pair<double,int> > status_5;
//		std::vector<std::pair<double,int> > status_6;
		void m_ReadSlcFile(int year, int month, int day, int tel);

};

///////////////////////
////return function////
///////////////////////
//inline void WFCTASLC::GetSipmAZ(int telid, int isipm, double& azi, double& zen)
//{
//	int Isipm = (telid-1)*1024 + isipm;
//	azi = SipmA[Isipm];
//	zen = SipmZ[Isipm];
//}


#endif // WFCTASLC_H

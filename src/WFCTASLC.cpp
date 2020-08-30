#include <iostream>
#include <fstream>
#include <algorithm>
#include <string.h>
#include "TMath.h"
#include "TFile.h"
#include "TTree.h"
#include "WFCTASLC.h"

WFCTASLC::WFCTASLC()
{
	std::cout << "wfcta slc constructor ===============>" << std::endl;

	for(int tel=1;tel<7;tel++)
	{
		slc_status[tel-1].clear();
	}

	std::cout << "wfcta slc constructor <===============" << std::endl;
}

WFCTASLC::~WFCTASLC()
{

}

void WFCTASLC::ReadSlcFile(int year, int month, int day)
{
	for(int tel=1;tel<7;tel++)
	{
		m_ReadSlcFile(year, month, day, tel);
	}
}
void WFCTASLC::m_ReadSlcFile(int year, int month, int day, int tel)
{
	std::string slc_file_dir = "/scratchfs/ybj/ketong/workTime/StatisticalTime/outFile/data/separate";
	std::string s_year, s_month, s_day, s_tel;
	s_year = std::to_string(year);
	month<10 ? s_month = "0" + std::to_string(month) : s_month = std::to_string(month);
	day<10 ? s_day = "0" + std::to_string(day) : s_day = std::to_string(day);
	tel<10 ? s_tel = "0" + std::to_string(tel) : s_tel = std::to_string(tel);

	std::string slc_file = slc_file_dir + "/" + s_year + "_" + s_month + "_" + s_day + "_" + s_tel + ".root";
	printf("slc file: %s\n",slc_file.c_str());

	TFile *slcfile = TFile::Open( slc_file.c_str(), "read" );
	// Check file
	if(!slcfile) {
		printf("%s wfcta slc file does note exit!!\n",slc_file.c_str());
		return ;
	}
	if(slcfile->IsZombie()||slcfile->GetEND()<50) {
		printf("%s wfcta slc file error!!\n",slc_file.c_str());
		slcfile->Close();
		return ;
	}
	// Check tree
	TTree *t = (TTree *)slcfile->Get("t");
	if(!t){
		printf("  The SLC file %s tree error!!\n",slc_file.c_str());
		slcfile->Close();
		return ;
	}
	double time_stamp;
	float slow_control_stu;
	float temperature_stu;
	float daq_stu;
	float cloud_stu;
	float phase;
	float delta_angle;
	t->SetBranchAddress("time_stamp",&time_stamp);
	t->SetBranchAddress("slow_control_stu",&slow_control_stu);
	t->SetBranchAddress("temperature_stu",&temperature_stu);
	t->SetBranchAddress("daq_stu",&daq_stu);
	t->SetBranchAddress("cloud_stu",&cloud_stu);
	t->SetBranchAddress("phase",&phase);
	t->SetBranchAddress("delta_angle",&delta_angle);
	int slc_events = t->GetEntries();
	int status_no=0;
	for(int ientry=0;ientry<slc_events;ientry++)
	{
		t->GetEntry(ientry);
		status_no = daq_stu*10000 + slow_control_stu*1000 + temperature_stu*100 + (cloud_stu<-75)*10 + (delta_angle>30)*1;
		//printf("%lf %f %f %f %f %f %f    %d\n",time_stamp,slow_control_stu,temperature_stu,daq_stu,cloud_stu,phase,delta_angle,status_no);
		slc_status[tel-1].push_back(std::make_pair( time_stamp, status_no ) );
	}

	slcfile->Close();

}

int WFCTASLC::GetSlcStatus(long long rbTime, int tel_id)
{
	int istatus;
	int nsta = slc_status[tel_id-1].size();
	if( rbTime < slc_status[tel_id-1].at(0).first )
	{
		printf("SLC start time is later than Event time!\n");
		istatus = 0;
	}
	else if( rbTime >= slc_status[tel_id-1].at(nsta-1).first )
	{
		printf("SLC end time is before than Event time!\n");
		istatus = 0;
	}
	else 
	{
		for( int ista=0; ista<nsta-1; ista++)
		{
			if( rbTime >= slc_status[tel_id-1].at(ista).first && rbTime < slc_status[tel_id-1].at(ista+1).first )
			{
				istatus = slc_status[tel_id-1].at(ista).second;
				break;
			}
		}
	}
	return istatus;
}







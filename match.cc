#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <string>
#include <array>
#include "time.h"
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "WFCTAEvent.h"
#include "TAEvent.h"
#include "LedEvtSel.h"
#include "LHAASOMatch.h"
#include "WFCTASLC.h"
//#include "WFCTARec.h"

using namespace std;
#define WCDASHITF 1574000000000
#define NTELS 6

array<vector<wfctatime>, 20> cosmic_events;
array<vector<wfctatime>, 20> laser_events;
array<vector<wfctatime>, 20> led_events;

long long TimeStart;
long long TimeEnd;
std::string INFILEDIR;

WFCTAEvent *telEvent = new WFCTAEvent();
WFCTASLC* wfctaslc = new WFCTASLC();
LedEvtSel *ledsel = new LedEvtSel();
//WFCTARec* wfctarec = new WFCTARec();

void slaCldj ( int iy, int im, int id, double *djm, int *j );

void changeFileName(std::string& s_filename, char* c_filename);
int ReadEvents(char* f_list, array<TFile*, 100>& WFCTAFile, array<TTree*, 100>& telTree, vector<wfctatime>& cosmic_events, vector<wfctatime>& laser_events, vector<wfctatime>& led_events);
void setEvent(const LHAASOMatch &lhMatch, array<TTree*, 100>& telTree, TAEvent *taEvent, long long TimeStart, long long matchtime);//,int ithtel);

void release_globle();

int main(int argc, char *argv[])
{
	if(argc<0) {
		printf("Usage: %s outroot wfcta01.txt wfcta02.txt wfcta03.txt wfcta04.txt wfcta05.txt wfcta06.txt\n", argv[0]);
		return 1;
	}
	cout<<"begin time:"<<time(0)<<endl;
	//set infile dir
	INFILEDIR = argv[13];

	///////////////////////////////////////////////////////
	//              set output file                 //
	///////////////////////////////////////////////////////
	char Name1[300]="root://eos01.ihep.ac.cn/";
	char of_name[300];
	strcpy(of_name,Name1);
	strcat(of_name,argv[1]);
	strcat(of_name,".root");
	TFile *outfile = TFile::Open(of_name,"recreate");

	//int timeclean_npix;
	TAEvent *taevent = new TAEvent();
	TTree *cosmic_tree = new TTree("event","info of events");
	cosmic_tree->Branch("TAEvent_cosmic", &taevent);
	//---------------------------------------------------------------------;
	TTree *laser_tree = new TTree("laserevent","info of laser events");
	laser_tree->Branch("TAEvent_laser", &taevent);
	//---------------------------------------------------------------------;
	TTree *led_tree = new TTree("ledevent","info of led events");
	led_tree->Branch("WFCTAEvent", &telEvent);

	///////////////////////////////////////////////////////
	//              Get merge time range                 //
	///////////////////////////////////////////////////////
	int year=atoi(argv[8]);//2019;
	int mon=atoi(argv[9]);//11;
	int day=atoi(argv[10]);//29;
	int hour=atoi(argv[11]);//0;
	int minite=atoi(argv[12]);//36;
	double mjd;
	int jjj;
	slaCldj ( year, mon, day, &mjd, &jjj );
	TimeStart = (mjd-40587)*86400 + (hour-8)*3600 + minite*60 + 37;
	TimeEnd = TimeStart + 600;
	printf("TimeStart:%lld TimeEnd:%lld\n",TimeStart,TimeEnd);

	//////////////////////////////////////////////////
	//			Read WFCTA SLC File		         //
	//////////////////////////////////////////////////
	struct tm stm;
	stm.tm_year = year - 1900;
	stm.tm_mon = mon-1;
	stm.tm_mday = day;
	stm.tm_hour = hour;
	stm.tm_min = minite;
	stm.tm_sec = 0;
	time_t utc = mktime(&stm);
	if(hour<12)
		utc -= 86400;
	struct tm *stm_slc = localtime(&utc);
	wfctaslc->ReadSlcFile(stm_slc->tm_year+1900, stm_slc->tm_mon+1, stm_slc->tm_mday);

	////////////////////////////////////////////////
	//     Read Tel 01 - 06     /////////////////////////
	////////////////////////////////////////////////
	int bad_read;
	std::array<std::array<TFile*, 100>, 20> telTFile;
	for(int i=0;i<20;i++)
		for(int j=0;j<100;j++)
			telTFile[i][j] = 0;
	std::array<std::array<TTree*, 100>, 20> telTree;
	for(int i=0;i<20;i++)
		for(int j=0;j<100;j++)
			telTree[i][j] = 0;
	for(int itel=0;itel<NTELS;itel++)
	{
		printf("read tel files:\n");
		bad_read = ReadEvents(argv[itel+2], telTFile[itel], telTree[itel], cosmic_events[itel], laser_events[itel], led_events[itel]);
		if(bad_read){
			outfile->Close();
			delete taevent;
			release_globle();
			exit(1);
		}
	}



	//Merge Laser Event//
	//////////////////////////////////////////////////
	//		match laser events			//
	//////////////////////////////////////////////////
	long long match_Time_end;
	long long matchTime;
	long long matchingTstep=9600;
	matchTime = 0;
	match_Time_end = (TimeEnd-TimeStart)*1000000000LL;
	printf("matchTime:%lld match_Time_end:%lld\n",matchTime,match_Time_end);

	int is_event=0;
	int begin_evt_tel[NTELS]={0};
	long allmatchedlaserevents=0;
	LHAASOMatch lhaasomatch[NTELS];// = new LHAASOMatch();
	while(1)
	{
		//Init
		is_event=0;
		taevent->EventInitial();
		for(int i=0;i<NTELS;i++)	{	lhaasomatch[i].Initial();};
		if(matchTime>=match_Time_end){break;}

		//find matched wfcta events
		for(int i=0;i<NTELS;i++)
		{
			lhaasomatch[i].DoMatch(matchTime, begin_evt_tel[i], laser_events[i], matchingTstep);
			is_event += lhaasomatch[i].is_tel_event;
		}

		//	printf("rabbitTime:%lld  rabbittime:%lf\n",TimeStart + matchTime/1000000000,double(matchTime % 1000000000));
		if(is_event!=0) {
			//set info of wfcta
			long long rb_Time = TimeStart + matchTime/1000000000;
			for(int itel=0;itel<NTELS;itel++)
			{
				setEvent(lhaasomatch[itel], telTree[itel], taevent, TimeStart, matchTime);
				taevent->SetTriggerTel(itel+1,lhaasomatch[itel].is_tel_event);

				int wf_slc_status = wfctaslc->GetSlcStatus(rb_Time, itel+1);
				taevent->SetSlcStatus(itel, wf_slc_status);
			}
			//taevent->MergeWFCTAInfo();
			printf("rbT:%lld  rbt:%lf: (triggertel):",TimeStart + matchTime/1000000000,double(matchTime % 1000000000));
			for(int i=0;i<NTELS;i++) {
				printf("%d ", lhaasomatch[i].is_tel_event);
			}
			printf("\n");

			//fill laser ttree
			laser_tree->Fill();
			allmatchedlaserevents++;
		}
		matchTime += matchingTstep;
	}
	printf("allmatchedlaserevents:%d\n",allmatchedlaserevents);

	//////////////////////////////////////////////////
	//		match cosmic events			//
	//////////////////////////////////////////////////
	matchTime = 0;
	matchingTstep = 1600;
	printf("matchTime:%lld match_Time_end:%lld\n",matchTime,match_Time_end);
	is_event=0;
	for(int i=0;i<NTELS;i++)
		begin_evt_tel[i] = 0;
	long allmatchedevents=0;
	while(1)
	{
		//Init
		is_event=0;
		taevent->EventInitial();
		for(int i=0;i<NTELS;i++)    {   lhaasomatch[i].Initial();};
		if(matchTime>=match_Time_end){break;}

		//find matched wfcta events
		for(int i=0;i<NTELS;i++)
		{   
			lhaasomatch[i].DoMatch(matchTime, begin_evt_tel[i], cosmic_events[i], matchingTstep);
			is_event += lhaasomatch[i].is_tel_event;
		}

		if(is_event!=0)
		{   
			//set info of wfcta
			long long rb_Time = TimeStart + matchTime/1000000000;
			for(int itel=0;itel<NTELS;itel++){
				setEvent(lhaasomatch[itel], telTree[itel], taevent, TimeStart, matchTime);
				taevent->SetTriggerTel(itel+1,lhaasomatch[itel].is_tel_event);

				int wf_slc_status = wfctaslc->GetSlcStatus(rb_Time, itel+1);
				taevent->SetSlcStatus(itel+1, wf_slc_status);
			}
			//lhaasoevent->MergeWFCTAInfo();
			
			//wfcta rec and select events : TimeClean(50 pe), npix<=5 delete 
			//wfctarec->SetWFCTAEvent(lhaasoevent->isipm, lhaasoevent->sipmpe, lhaasoevent->sipmt);
			//wfctarec->TimeClean();
			//int npix = wfctarec->CalcTimeCleanNpix();
			printf("\ntriggered tels:%d--->",is_event);
			for(int ii=0;ii<NTELS;ii++)
				printf("%d ",lhaasomatch[ii].matchedEvents.size());
			printf("rbT:%lld  rbt:%lf\n",TimeStart + matchTime/1000000000,double(matchTime % 1000000000));

			//fill ttree
			cosmic_tree->Fill();
			allmatchedevents++;
		}

		matchTime += matchingTstep;
	}
	printf("allmatchedevents:%d\n",allmatchedevents);
	cout<<"match time:"<<time(0)<<endl<<endl;

	//////////////////////////////////////////////////
	//		match led events			//
	//////////////////////////////////////////////////
	for(int itel=0;itel<NTELS;itel++)
	{
		for(int iled=0;iled<led_events.at(itel).size();iled++)
		{
			int ientry,ifile;
			ientry = int(led_events[itel].at(iled).event_ievt);
			ifile = int((led_events[itel].at(iled).event_ievt-ientry)*100);
			telTree[itel][ifile]->GetEntry(ientry);
			led_tree->Fill();
		}
	}

	//////////////////////////////////////////////////
	//		close tel files			//
	//////////////////////////////////////////////////
	for(int i=0;i<20;i++)
		for(int j=0;j<100;j++)
		{
			if(telTFile[i][j])
			{
				telTFile[i][j]->Close();
				std::cout << "telTFile[" << i << "][" << j <<"]:" << telTFile[i][j] << ": Closed" << std::endl;
			}
		}

	//////////////////////////////////////////////////
	//		write output file			//
	//////////////////////////////////////////////////
	outfile->cd();
	cosmic_tree->Write();
	laser_tree->Write();
	led_tree->Write();
	outfile->Close();
	delete taevent;
	release_globle();

	printf("program message: merge finished, root file closed\n");

	return 0;
}

void release_globle()
{
	delete telEvent;
	delete wfctaslc;
	delete ledsel;
	//	delete wfctarec;
}

void slaCldj ( int iy, int im, int id, double *djm, int *j )
{
	long iyL, imL;
	/* Month lengths in days */
	static int mtab[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	/* Validate year */
	if ( iy < -4699 ) { *j = 1; return; }
	/* Validate month */
	if ( ( im < 1 ) || ( im > 12 ) ) { *j = 2; return; }
	/* Allow for leap year */
	mtab[1] = ( ( ( iy % 4 ) == 0 ) &&
			( ( ( iy % 100 ) != 0 ) || ( ( iy % 400 ) == 0 ) ) ) ?
		29 : 28;
	/* Validate day */
	*j = ( id < 1 || id > mtab[im-1] ) ? 3 : 0;
	/* Lengthen year and month numbers to avoid overflow */
	iyL = (long) iy;
	imL = (long) im;
	/* Perform the conversion */
	*djm = (double)
		( ( 1461L * ( iyL - ( 12L - imL ) / 10L + 4712L ) ) / 4L
		  + ( 306L * ( ( imL + 9L ) % 12L ) + 5L ) / 10L
		  - ( 3L * ( ( iyL - ( 12L - imL ) / 10L + 4900L ) / 100L ) ) / 4L
		  + (long) id - 2399904L );
}

void setEvent(const LHAASOMatch &lhMatch, array<TTree*, 100>& telTree, TAEvent *taEvent, long long TimeStart, long long matchtime)//,int ithtel);
{
	int ientry,ifile;
	long long rb_Time = TimeStart + matchtime/1000000000;
	double rb_time = double(matchtime % 1000000000);
	if(lhMatch.is_tel_event!=0)
	{
		ientry = int(lhMatch.matchedEvents.at(0));
		ifile = int((lhMatch.matchedEvents.at(0)-ientry)*100);
		telTree[ifile]->GetEntry(ientry);
		taEvent->SetWFCTAEvent(rb_Time, rb_time, telEvent);
		/*		taEvent->SetWFCTAEvent(telEvent->iTel,
				telEvent->rabbitTime,
				telEvent->rabbittime,
				rb_Time,
				rb_time,
				lhMatch.deltaT.at(0),
				telEvent->isipm,
				telEvent->sipmpe,
				telEvent->sipmt,
				telEvent->sipmbase,
				telEvent->sipmbaserms);*/
	}
}

int ReadEvents(char* f_list, array<TFile*, 100>& WFCTAFile, array<TTree*, 100>& telTree, vector<wfctatime>& cosmic_events, vector<wfctatime>& laser_events, vector<wfctatime>& led_events)
{
	cosmic_events.clear();
	laser_events.clear();
	led_events.clear();

	double laser2_rb_time = 990035000; //990010000 - 990060000 ns
	double laser3_rb_time = 990850000; //990820000 - 990880000 ns

	FILE *fp = fopen(f_list,"r");
	wfctatime singletelevent;
	char c_telfile[500]; strcpy(c_telfile,"empty");
	std::string telfile = "empty";
	char telFile[500];
	int ifile=0;
	int ievt_selected;
	long long wfcta_time;
	double wfcta_ievent;
	int timeside=0;
	int splits;
	long long fileTimeMax, fileTimeMin;
	while(!feof(fp))
	{
		ievt_selected=0;
		//		fscanf(fp,"%s\n",c_telfile);
		fscanf(fp,"%s %d %lld %lld\n",c_telfile, &splits, &fileTimeMin, &fileTimeMax);
		changeFileName(telfile, c_telfile);
		//		printf("%s %d %lld %lld %lld %lld\n",telfile.c_str(), splits, fileTimeMin, fileTimeMax, TimeStart-60, TimeEnd+60);
		if(strcmp(telfile.c_str(), "empty") == 0){break;}
		if( !( (fileTimeMin>TimeStart-60 && fileTimeMin<TimeEnd+60) || (fileTimeMax>TimeStart-60 && fileTimeMax<TimeEnd+60) || (fileTimeMin<TimeStart-60 && fileTimeMax>TimeEnd+60) ) ){
			continue;
		}
		strcpy(telFile,"root://eos01.ihep.ac.cn/");
		strcat(telFile,telfile.c_str());
		WFCTAFile[ifile] = TFile::Open(telFile,"READ");
		printf("inputing wfcta file -> %s\n",telFile);
		if(!WFCTAFile[ifile] || WFCTAFile[ifile]->IsZombie() || WFCTAFile[ifile]->GetEND()<50)
		{
			printf("%s file error!!\n",telFile);
			fclose(fp);
			return 1;
		}
		telTree[ifile] = (TTree *)WFCTAFile[ifile]->Get("eventShow");
		if(telTree[ifile]==nullptr)
		{
			printf("%s is null file\n",telFile);
			fclose(fp);
			return 1;
		}
		telTree[ifile]->SetBranchAddress("WFCTAEvent",&telEvent);

		int Nwfctaevts = telTree[ifile]->GetEntries();
		printf("	all events: %d\n",Nwfctaevts);

		for(int i=0;i<Nwfctaevts;i++)
		{
			telTree[ifile]->GetEntry(i);

			if(telEvent->rabbitTime<TimeStart-timeside || telEvent->rabbitTime>=TimeEnd+timeside){continue;}
			ievt_selected++;
			wfcta_time = (telEvent->rabbitTime-TimeStart)*1000000000LL + telEvent->rabbittime*20;
			wfcta_ievent = i+ifile*0.01+0.00001;
			singletelevent.event_alltime = wfcta_time;
			singletelevent.event_ievt = wfcta_ievent;
			if(abs(telEvent->Rabbittime()*20-laser2_rb_time)<=25000 || abs(telEvent->Rabbittime()*20-laser3_rb_time)<=30000)
			{
				laser_events.push_back(singletelevent);
			}
			else
			{
				ledsel->SetLed0( telEvent->ITel() );
				if( 13==ledsel->Running(telEvent) ) {
					led_events.push_back(singletelevent);
				}
				else
				{
					cosmic_events.push_back(singletelevent);
				}
			}
		}
		printf("	nevt ifile:%d, selected/all: %d/%d\n",ifile+1,ievt_selected,Nwfctaevts);
		ifile++;
	}
	printf("	nevt ( all selected ): cosmic:%d laser:%d led:%d\n",cosmic_events.size(), laser_events.size(), led_events.size());
	sort(cosmic_events.begin(),cosmic_events.end(),comp);
	sort(laser_events.begin(),laser_events.end(),comp);
	sort(led_events.begin(),led_events.end(),comp);
	fclose(fp);
	cout<<"file read time:"<<time(0)<<endl<<endl;
	return 0;
}

void changeFileName(std::string& s_filename, char* c_filename)
{
	s_filename = c_filename;
	/*
	   std::string filename;
	   filename = c_filename;
	   size_t posES = filename.find("ES.", 0) - 10;
	   size_t poses = filename.find("es-1",0);
	   size_t shift = 0;
	   if(poses!=std::string::npos)
	   shift = 5;
	   std::string basefilename = filename.substr(posES,50+shift);
	   std::string s_year = filename.substr(posES+32+shift,4);
	   std::string s_date = filename.substr(posES+36+shift,4);
	//	std::cout << "basefilename:" << basefilename << " | " << s_year << " | " << s_date << std::endl;
	s_filename = INFILEDIR + "/" + basefilename + ".cal.root";
	*/
}

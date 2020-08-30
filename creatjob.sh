#!/bin/bash

year=2020
Date=$1
#jobdir=/scratchfs/ybj/youzhiyong/LHAASO/matchstep2new/job
jobdir=/scratchfs/ybj/youzhiyong/WFCTAMatch/job

#for Date in `seq 1121 1128`
#do
mkdir -p $jobdir/$year/$Date
h="0 1 2 3 4 5 6 7 17 18 19 20 21 22 23"
m="0 10 20 30 40 50"
#h="21"
#m="0 10"

for ih in ${h}
do
	ih=`printf %02d $ih`
	for im in $m
	do
		im=`printf %02d $im`
		alltime=${year}${Date}${ih}${im}
		sec=`date -d $year$Date +%s`
		sec_yestoday=$((sec - 86400))
		Date_1=`date -d @$sec_yestoday +%m%d`
		mon_1=${Date_1:0:2}
		year_1=`date -d @$sec_yestoday +%Y`
		sec_tomorrow=$((sec + 86400))
		Date1=`date -d @$sec_tomorrow +%m%d`
		mon1=${Date1:0:2}
		year1=`date -d @$sec_tomorrow +%Y`

		if [ $ih -gt 10 ];then
			year_first=$year
			Date_first=$Date
			year_second=$year1
			Date_second=$Date1
		else
			year_first=$year_1
			Date_first=$Date_1
			year_second=$year
			Date_second=$Date
		fi

		echo $alltime
		echo "#!/bin/bash" >$jobdir/$year/$Date/${alltime}.sh
		echo "source /cvmfs/lhaaso.ihep.ac.cn/anysw/slc5_ia64_gcc73/external/envb.sh" >>$jobdir/$year/$Date/${alltime}.sh
		echo "" >>$jobdir/$year/$Date/${alltime}.sh
		echo 'year=2020' >>$jobdir/$year/$Date/${alltime}.sh
		echo "Date=$Date" >>$jobdir/$year/$Date/${alltime}.sh
		echo 'mon=${Date:0:2}' >>$jobdir/$year/$Date/${alltime}.sh
		echo 'day=${Date:2:2}' >>$jobdir/$year/$Date/${alltime}.sh
		echo "hour=$ih" >>$jobdir/$year/$Date/${alltime}.sh
		echo "minite=$im" >>$jobdir/$year/$Date/${alltime}.sh
		echo 'alltime=${year}${mon}${day}${hour}${minite}' >>$jobdir/$year/$Date/${alltime}.sh
		echo "" >>$jobdir/$year/$Date/${alltime}.sh
		echo 'decodedir=/eos/lhaaso/decode/wfcta' >>$jobdir/$year/$Date/${alltime}.sh
		echo "IPTDIR=/afs/ihep.ac.cn/users/w/wfctarec/scratchfs/workshell_v2/joblist/$year_first/${Date_first:0:2}" >>$jobdir/$year/$Date/${alltime}.sh
		echo 'SRCDIR=/eos/user/y/youzhiyong/WFCTAMatch' >>$jobdir/$year/$Date/${alltime}.sh
		echo "OUTDIR=/eos/user/y/youzhiyong/WFCTAMatch/root/$year/$Date" >>$jobdir/$year/$Date/${alltime}.sh
		echo "" >>$jobdir/$year/$Date/${alltime}.sh
		echo 'infilenum=0' >>$jobdir/$year/$Date/${alltime}.sh
		echo 'if [ -f $IPTDIR/wfcta'"${Date_first}_${Date_second}"'_01.list ];then' >>$jobdir/$year/$Date/${alltime}.sh
		echo '	for ifile in `ls $IPTDIR/wfcta'"${Date_first}_${Date_second}"'_0[123456].list`' >>$jobdir/$year/$Date/${alltime}.sh
		echo '	do' >>$jobdir/$year/$Date/${alltime}.sh
		echo '		infilenum=`expr $infilenum + 1`' >>$jobdir/$year/$Date/${alltime}.sh
		echo '	done' >>$jobdir/$year/$Date/${alltime}.sh
		echo 'fi' >>$jobdir/$year/$Date/${alltime}.sh
		echo "" >>$jobdir/$year/$Date/${alltime}.sh
		echo 'if [ $infilenum -eq 6 ];then' >>$jobdir/$year/$Date/${alltime}.sh
		echo "" >>$jobdir/$year/$Date/${alltime}.sh
		echo '	mkdir -p $OUTDIR' >>$jobdir/$year/$Date/${alltime}.sh
		echo "" >>$jobdir/$year/$Date/${alltime}.sh
		echo '	cd $SRCDIR' >>$jobdir/$year/$Date/${alltime}.sh
		echo '	./match $OUTDIR/${alltime} $IPTDIR/wfcta'"${Date_first}_${Date_second}"'_0[123456].list ${year} ${mon} ${day} ${hour} ${minite} $decodedir >$OUTDIR/${alltime}.out' >>$jobdir/$year/$Date/${alltime}.sh
		echo 'else' >>$jobdir/$year/$Date/${alltime}.sh
		echo "" >>$jobdir/$year/$Date/${alltime}.sh
		echo '	mkdir -p $OUTDIR' >>$jobdir/$year/$Date/${alltime}.sh
		echo '	echo "no iptlist for $alltime" >$OUTDIR/${alltime}.out' >>$jobdir/$year/$Date/${alltime}.sh
		echo 'fi' >>$jobdir/$year/$Date/${alltime}.sh
		chmod 755 $jobdir/$year/$Date/${alltime}.sh
	done
done

#done

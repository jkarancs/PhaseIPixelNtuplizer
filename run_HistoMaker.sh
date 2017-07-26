#!/bin/bash
LOWSTAT=1
PHM=1
RAND=`uuid | sed "s;-;_;g"`
CWD=`pwd -P`

echo " ---------------------------------------------------"
for arg in "$@"; do
    if [[ ! $arg == *".py" ]]; then
        echo "$arg is not a crab script"
    else
	CRAB_CFG=$arg
	echo
	echo "Using crab config file: $CRAB_CFG"
	echo
	# read infos from crab config file
        LFN=`grep "config.Data.outLFNDirBase" $CRAB_CFG | sed "s;';;g" | awk '{ print $3 }' | sed 's;/$;;'`
        TASKNAME=`grep "config.Data.outputDatasetTag" $CRAB_CFG | sed "s;';;g" | awk '{ print $3 }'`
	PRIMARY=`grep "config.Data.inputDataset" $CRAB_CFG | sed "s;';;g;s;/; ;g" | awk '{ print $3 }'`
	AUTONAME=`echo $CRAB_CFG | sed "s;crab3_;;;s;.py;;"`
	# Read info from the crab status
	crab status -d crab_$AUTONAME > crab_status_$RAND.txt
	NJOB=`cat crab_status_$RAND.txt | grep -E "%.*\(.*\)" | tail -1 | sed "s;(; ;;s;); ;;s;/; ;" | awk '{ print $NF }'`
	# Set input directory
	TIME=`/data/jkarancs/scripts/se_util.csh ls T2_HU_Budapest:$LFN/$PRIMARY/$TASKNAME | tail -1`
        DL_DIR=`echo $LFN/$PRIMARY/$TASKNAME/$TIME | sed "s;/store/user;/data/gridout;"`
	NCOMP=`ls $DL_DIR/*/*.root | wc -l`
	PERCENT=`echo $(( $NCOMP*100/$NJOB ))`
	# Decide if it is needed to create an SEU list first (BADROC list)
	cd /data/jkarancs/CMSSW/TimingStudy/Latest/src > /dev/null
	cmsenv > /dev/null 2>&1 > /dev/null
	cd /data/jkarancs/CMSSW/PixelHistoMaker_git > /dev/null
	if [ -f PHM_PHASE1_out/BADROC_"$AUTONAME".root ]; then BADROC=0; else BADROC=1; fi
	# Wait until at least 50% of the files download for quick restults on low statistics
	if (( $LOWSTAT )); then
	    while (( $PERCENT < 75 )); do 
	        sleep 5m
	        NCOMP=`ls $DL_DIR/*/*.root | wc -l`
	        PERCENT=`echo $(( $NCOMP*100/$NJOB ))`
	    done
	    echo " ---------------------------------------------------"
	    echo
	    echo "75% of jobs finished, running first a low statistics analysis (on 10% statistics)"
	    echo
	    if (( $BADROC )); then
		echo "- Start creating BADROC list first"
		echo "  Input files:   $DL_DIR/*/*5.root ($NCOMP/$NJOB)"
		echo "  Output file: PHM_PHASE1_out/BADROC_"$AUTONAME".root"
		echo "  Log file:    PHM_PHASE1_out/BADROC_"$AUTONAME".log"
		if (( $PHM )); then
		    # Wait for BADROC list to finish, before running plotting
		    ./Phase1PixelHistoMaker -b -o PHM_PHASE1_out/BADROC_"$AUTONAME".root $DL_DIR/*/*5.root > PHM_PHASE1_out/BADROC_"$AUTONAME".log 2>&1
		else
		    # Otherwise run in background
		    nohup ./Phase1PixelHistoMaker -b -o PHM_PHASE1_out/BADROC_"$AUTONAME".root $DL_DIR/*/*5.root > PHM_PHASE1_out/BADROC_"$AUTONAME".log 2>&1 &
		fi
	    fi
	    if (( $PHM )); then
		echo
		echo "- Start running PixelHistoMaker (in background)"
		echo "  Input files:   $DL_DIR/*/*5.root ($NCOMP/$NJOB)"
		echo "  Output file: PHM_PHASE1_out/HitEffMonitoring_"$AUTONAME"_lowstat.root"
		echo "  Log file:    PHM_PHASE1_out/HitEffMonitoring_"$AUTONAME"_lowstat.log"
		nohup python run_phm.py --outdir="PHM_PHASE1_out/HitEffMonitoring_"$AUTONAME"_lowstat" --nfile=$(($NCOMP/10/7)) --nproc=4 "$DL_DIR/*/*5.root" --run > "PHM_PHASE1_out/HitEffMonitoring_"$AUTONAME"_lowstat.log" 2>&1 &
	    fi
	    echo
	fi
	# Run the full statistics analysis
	while (( $PERCENT < 95 )); do 
	    sleep 5m
	    NCOMP=`ls $DL_DIR/*/*.root | wc -l`
	    PERCENT=`echo $(( $NCOMP*100/$NJOB ))`
	done
	echo " ---------------------------------------------------"
	echo
	echo "95% of jobs finished, running full statistics analysis"
	echo
	if (( $BADROC )); then
	    echo "- Start creating BADROC list first"
	    echo "  Input files:   $DL_DIR/*/*.root ($NCOMP/$NJOB)"
	    echo "  Output file: PHM_PHASE1_out/BADROC_"$AUTONAME".root"
	    echo "  Log file:    PHM_PHASE1_out/BADROC_"$AUTONAME".log"
	    if (( $PHM )); then
	        # Wait for BADROC list to finish, before running plotting
	        ./Phase1PixelHistoMaker -b -o PHM_PHASE1_out/BADROC_"$AUTONAME".root $DL_DIR/*/*.root > PHM_PHASE1_out/BADROC_"$AUTONAME".log 2>&1
	    else
	        # Otherwise run in background
	        nohup ./Phase1PixelHistoMaker -b -o PHM_PHASE1_out/BADROC_"$AUTONAME".root $DL_DIR/*/*.root > PHM_PHASE1_out/BADROC_"$AUTONAME".log 2>&1 &
	    fi
	fi
	if (( $PHM )); then
	    echo
	    echo "- Start running PixelHistoMaker (in background)"
	    echo "  Input files:   $DL_DIR/*/*.root ($NCOMP/$NJOB)"
	    echo "  Output file: PHM_PHASE1_out/HitEffMonitoring_"$AUTONAME".root"
	    echo "  Log file:    PHM_PHASE1_out/HitEffMonitoring_"$AUTONAME".log"
	    nohup python run_phm.py --outdir="PHM_PHASE1_out/HitEffMonitoring_"$AUTONAME --nfile=$(($NCOMP/20)) --nproc=4 "$DL_DIR/*/*.root" --run > "PHM_PHASE1_out/HitEffMonitoring_"$AUTONAME".log" 2>&1 &
	fi
	echo
	echo " ---------------  "$AUTONAME" Done.  -----------------"
	echo
	cd $CWD > /dev/null
	cmsenv > /dev/null 2>&1 > /dev/null
    fi
done

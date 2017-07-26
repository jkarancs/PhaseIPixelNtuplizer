#!/bin/bash
CRAB_CFG=$1

for arg in "$@"; do
    if [[ ! $arg == *".py" ]]; then
        echo "$arg is not a crab script"
    else
	CRAB_CFG=$arg
	# read infos from crab config file
        LFN=`grep "config.Data.outLFNDirBase" $CRAB_CFG | sed "s;';;g" | awk '{ print $3 }' | sed 's;/$;;'`
        TASKNAME=`grep "config.Data.outputDatasetTag" $CRAB_CFG | sed "s;';;g" | awk '{ print $3 }'`
        PRIM_DATASET=`grep "config.Data.inputDataset" $CRAB_CFG | sed "s;';;g;s;/; ;g" | awk '{ print $3 }'`
	SITE=`grep "config.Site.storageSite" $CRAB_CFG | sed "s;';;g" | awk '{ print $3 }'`
	# Set input/output directories
        SE_DIR=$LFN/$PRIM_DATASET/$TASKNAME
        DL_DIR=`echo $LFN/$PRIM_DATASET/$TASKNAME | sed "s;/store/user;/data/gridout;"`
	# download
	echo
	echo "Downloading from crab config file: "$CRAB_CFG
	echo "- Downloading from site: "$SITE
	echo "- Destination local directory:"
	echo "    $DL_DIR"
	echo "- Start synchronizing:"
	echo "    $SE_DIR"
	echo
	for i in $( seq 1 24 ); do sleep 30m; /data/jkarancs/scripts/se_util.csh sync $SITE:$SE_DIR $DL_DIR --run; done
	for i in $( seq 1 12 ); do sleep  1h; /data/jkarancs/scripts/se_util.csh sync $SITE:$SE_DIR $DL_DIR --run; done
	for i in $( seq 1 12 ); do sleep  2h; /data/jkarancs/scripts/se_util.csh sync $SITE:$SE_DIR $DL_DIR --run; done
	echo
	echo "---- All downloads from $CRAB_CFG Done. ----"
    fi
done

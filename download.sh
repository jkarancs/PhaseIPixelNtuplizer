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
        DL_DIR=`echo $LFN/$TASKNAME | sed "s;/store/user;/data/gridout;"`
        TIMESTAMP=`/data/jkarancs/scripts/se_util.csh ls $SITE:$LFN/$PRIM_DATASET/$TASKNAME | tail -1`
        SE_DIR=$LFN/$PRIM_DATASET/$TASKNAME/$TIMESTAMP
	# download
	echo
	echo "Downloading from crab config file: "$CRAB_CFG
	echo "- Downloading from site: "$SITE
	echo "- Destination local directory:"
	echo "    $DL_DIR"
	mkdir -p $DL_DIR
	echo
        for SUBDIR in `/data/jkarancs/scripts/se_util.csh ls $SITE:$SE_DIR`; do
	    echo "- Start downloading from:"
	    echo "    $SE_DIR/$SUBDIR"
	    /data/jkarancs/scripts/se_util.csh dl_mis $SITE:$SE_DIR/$SUBDIR $DL_DIR --run
	    echo
        done
	echo "---- All downloads from $CRAB_CFG Done. ----"
    fi
done

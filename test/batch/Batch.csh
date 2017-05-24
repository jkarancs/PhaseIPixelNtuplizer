#!/bin/tcsh
##########################################################################
# Creator: Janos Karancsi - 2012/04/11 (update 2017/05/17)
#   This is a Script that creates jobs to run PhaseIPixelNtuplizer on the Lxbatch.
#   it uses various other scripts in the test/scripts directory.
#   Make sure to use this on the lxplus in a directory accessible outside!
#   Jobs are submitted via the bsub command - some experience helps
#   The script is foolproof, it doesn't submit anything but
#   produces other scripts in the task directory that you can source
#   Feel free to try it!
#
# Job info: See (eg.) Batch_Job_GENSIM.csh file
#   The Job Script moves output files to EOS:
#   /eos/user/<u>/<username>/...
#   So make sure that you edit USERDIR_EOS first! (see BSub  tutorial)
#   I don't know yet, how to delegate proxy on lxbatch to save to other SE,
#   but there is an option to copy output from EOS to T2_HU_Budapest
#   You can try a test task with Create and then
#   run a test job on one file only (see BSub tutorial below)
#
# BSub tutorial:
#   - if a submit/resubmit script (see Usage) suggest sourcing a file view it first!
#   - make a test job out of the first line like in the example
#     - Create a task by doing Step 0,1 of Usage below (it is a working example)
#     - you can replace the cmscaf1nd queue (Step 1) if you don't have access
#       with 8nm or just delete "-q <queue>" equivalently
#       Available queues for user: bqueues -u <username>
#     - specify a maximum number of events, eg 10
#     eg:
#     cp Batch_Job_GENSIM.csh test.csh
#     chmod 777 test.csh     #This is needed so lxbatch job can access it
#     bsub -J v3735_INC_SPL1_ns11_710pre1_MB_GENSIM_test_JOBtest -oo STDOUT/JOB_test.log -L tcsh jobscript.csh CMSSW_7_1_0_pre1 MC_70_V1 test /store/caf/user/jkarancs/MinBias_TuneZ2star_8TeV_GENSIM_CMSSW_7_1_0pre1/MinBias_8TeV_GEN_SIM_2000k_1_1_FQV.root 10
#    - The above steps are done in the test.csh script of the submit option (Step 2)
#
# Usage - In Steps 0-6:
#
# Step 0 - Check out PixelPhaseIPixelNtuplizer (or just the test directory like below)
#   eg:
#   cmsenv      #in any CMSSW release area (CMSSW_*/src)
#   git clone https://github.com/jkarancs/PhaseIPixelNtuplizer DPGAnalysis/PhaseIPixelNtuplizer
#
#   - This script uses other scripts in the test/scripts directory so stay in the current directory
#   - Create a list of input files that are accessible within CERN (T2/T1/T0_CERN_CH, T2_CH_CAF etc)
#   - Put it inside a file like input.txt
#   eg:
#   dbs search --query "find file where dataset=/ZeroBias1/Commissioning12-PromptReco-v1/RECO and run = 190411" | grep .root > input.txt; 
#   dbs search --query "find file where dataset=/ZeroBias1/Run2012A-PromptReco-v1/RECO and run = 190456" | grep .root >> input.txt; 
#   dbs search --query "find file where dataset=/ZeroBias1/Run2012A-PromptReco-v1/RECO and run = 190538" | grep .root >> input.txt; 
#   dbs search --query "find file where dataset=/ZeroBias1/Run2012A-PromptReco-v1/RECO and run = 190539" | grep .root >> input.txt; 
#   dbs search --query "find file where dataset=/ZeroBias1/Run2012A-PromptReco-v1/RECO and run = 190591" | grep .root >> input.txt; 
#   dbs search --query "find file where dataset=/ZeroBias1/Run2012A-PromptReco-v1/RECO and run = 190592" | grep .root >> input.txt; 
#   dbs search --query "find file where dataset=/ZeroBias1/Run2012A-PromptReco-v1/RECO and run = 190593" | grep .root >> input.txt; 
#   dbs search --query "find file where dataset=/ZeroBias1/Run2012A-PromptReco-v1/RECO and run = 190595" | grep .root >> input.txt
#    
#    basically the input.txt constaints a list of files on the /store/...
#
#
#   A trick to order input files by run and lumisections and use a list of runs from more datasets:
#   dbs search --query "find run, lumi, file where dataset=/ZeroBias*/Run2012A-v1/RAW and (run=190538 or run=190539 or run=190591 or run=190592 or run=190593 or run=190595)" | grep .root | awk '{ printf "%d %.4d %s\n", $1, $2, $3 }' | sort -k3,3 -u | sort | awk '{ print $3 }' > input.txt
#
# Step 1 - Create - Batch.csh [TaskDir] -create [InputFile] [BatchQueue] [JobScript] [CMSSW_Version] [GlobalTag]
# 
#   grid-proxy-init --voms cms -valid 120:00
#   <Grid pass phrase>
#   source test/scripts/Batch.csh v3735_INC_SPL1_ns11_710pre1_MB_GENSIM_test -create test_input.txt cmscaf1nd test/scripts/Batch_Job_GENSIM.csh CMSSW_7_1_0_pre1 MC_70_V1
#
# 
# Step 2 - Submit - Batch.csh [TaskDir] -submit
#
#   source test/scripts/Batch.csh submit v3735_INC_SPL1_ns11_710pre1_MB_GENSIM_test
#   > source v3735_INC_SPL1_ns11_710pre1_MB_GENSIM_test/submit.csh
#
#
# Step 3 - Missing * - Batch.csh [TaskDir] -missing
#
#   source test/scripts/Batch.csh missing v3735_INC_SPL1_ns11_710pre1_MB_GENSIM_test
#   > 21,105,165,345,524,
#
#
# Step 4 - Resubmit * - Batch.csh [TaskDir] -resubmit [list]
#
#   source test/scripts/Batch.csh resubmit v3735_INC_SPL1_ns11_710pre1_MB_GENSIM_test 21,105,165,345,524
#   > source v3735_INC_SPL1_ns11_710pre1_MB_GENSIM_test/resub.csh
# 
#
# Step 5 - Resubmit Missing - Batch.csh [TaskDir] -resubmit_missing
#   * Combines the above two automatically
#
#   source test/scripts/Batch.csh resubmit_missing v3735_INC_SPL1_ns11_710pre1_MB_GENSIM_test
#   > source v3735_INC_SPL1_ns11_710pre1_MB_GENSIM_test/resub.csh
#
#
# Step 6 - Copy to KFKI - Batch.csh [TaskDir] -copy_to_kfki
#
#   source test/scripts/Batch.csh copy_to_kfki v3735_INC_SPL1_ns11_710pre1_MB_GENSIM_test
#   > source v3735_INC_SPL1_ns11_710pre1_MB_GENSIM_test/copy_to_kfki.csh
#
#
# Step 7 - Delete - Batch.csh [TaskDir] -delete
#   - Or you can simply delete the directory now
#
#   source test/scripts/Batch.csh delete v3735_INC_SPL1_ns11_710pre1_MB_GENSIM_test
#
##########################################################################

set USERDIR_EOS = "j/jkarancs/PhaseIPixelNtuplizer"
set USERDIR_KFKI = "jkarancs/PhaseIPixelNtuplizer"


echo "Usage: ">! Usage
echo "Batch.csh [TaskDir] -create [InputFile] [BatchQueue] [JobScript] [CMSSW_Version] [GlobalTag]">> Usage
echo "Batch.csh [TaskDir] -submit">> Usage
echo "Batch.csh [TaskDir] -status">> Usage
echo "Batch.csh [TaskDir] -missing">> Usage
echo "Batch.csh [TaskDir] -resubmit [list]">> Usage
echo "Batch.csh [TaskDir] -resubmit_missing">> Usage
echo "Batch.csh [TaskDir] -copy_to_kfki">> Usage
echo "Batch.csh [TaskDir] -delete">> Usage


if ( "$1" == "" ) then
    echo "No Task Directory Specified"
    cat Usage
    rm Usage
    exit
else
    set TASKDIR = $1
endif

if ( "$2" == "" ) then
    echo "No Option specified"
    cat Usage
    rm Usage
    exit
else
    set OPT = $2
endif

if ( ! -d $TASKDIR ) then
    if ( "$OPT" != "-create" ) then
	echo "Task Directory does not exist"
	rm Usage
	exit
    endif
else
    if ( "$OPT" == "-create" ) then
	echo "Task "$TASKDIR" already exists"
	rm Usage
	exit
    else
	set NJOBS = `wc -l $TASKDIR/alljobs.csh | awk '{ print $1}'`
    endif
endif

# Checking voms proxy
voms-proxy-info > & ! vomsout.txt
if ( `grep "Couldn't find a valid proxy." vomsout.txt` != "" ) then
    echo "---------------- Requesting Voms Proxy ----------------"
    voms-proxy-init --voms cms -valid 120:00
else if ( `grep "Proxy not found" vomsout.txt` != "" ) then
    echo "---------------- Requesting Voms Proxy ----------------"
    voms-proxy-init --voms cms -valid 120:00
else if ( `grep "timeleft" vomsout.txt | awk '{ print $3 }'` == "0:00:00" ) then
    echo "------------ Proxy Expired, Requesting New ------------"
    voms-proxy-init --voms cms -valid 120:00
endif
rm vomsout.txt

if ( "$OPT" == "-create" ) then
    if ( ! -e $3 ) then
	echo "Input file does not exist"
    else
        echo "--------------------------------------------------------------------------------"
        mkdir -p $TASKDIR
        chmod 777 $TASKDIR
        echo "Task Name:       "$TASKDIR
        cp $3 $TASKDIR/input.txt
        echo "Input file:      "$3
        set NJOBS = `wc -l $TASKDIR/input.txt | awk '{ print $1}'`
        echo "Number of Jobs:  "$NJOBS
        echo "Lxbatch queue:   "$4
        sed "s;outdir;"$TASKDIR";;s;userdir;"$USERDIR_EOS";" $5 > $TASKDIR/jobscript.csh
        chmod 777 $TASKDIR/jobscript.csh
        echo "Job Script file: "$5
        echo "CMSSW version:   "$6
        echo "Global Tag:      "$7
        echo
        cd $TASKDIR
        mkdir STDOUT
        chmod 666 STDOUT
        sed = input.txt | sed 'N;s;\n; ;' | awk '{ printf "%.4d %s\n", $1, $2 }' | awk '{ print "bsub -J '$TASKDIR'_JOB"$1" -oo STDOUT/JOB_"$1".log -q '$4' -L tcsh jobscript.csh '$6' '$7' "$1" "$2 }' > alljobs.csh
        cd -
        echo "Task Created"
        echo "--------------------------------------------------------------------------------"
	set queue=$4
    endif
else if ( "$OPT" == "-submit" ) then
    echo "cd "$TASKDIR >! $TASKDIR/submit.csh
    cat $TASKDIR/alljobs.csh >> $TASKDIR/submit.csh
    echo "cd -" >> $TASKDIR/submit.csh
    echo "cd "$TASKDIR >! $TASKDIR/test.csh
    head -1 $TASKDIR/alljobs.csh | sed 's; -q '$queue'; -q 1nh;' | sed 's;$; 10;' | sed "s;JOB0001;JOBtest;;s;JOB_0001;JOB_test;;s; 0001 ; test ;" >> $TASKDIR/test.csh
    echo "cd -" >> $TASKDIR/test.csh
    echo "source "$TASKDIR"/submit.csh"
    echo "Or you can try this test script:"
    echo "source "$TASKDIR"/test.csh"
else if ( "$OPT" == "-status" ) then
    ( bjobs -J "$TASKDIR*" >! taskjobs ) > & /dev/null
    grep PEND taskjobs >! Pending
    grep RUN taskjobs >! Running
    set PEND=`wc -l Pending | awk '{ print $1}'`
    set RUN=`wc -l Running | awk '{ print $1}'`
    ( ls /eos/user/$USERDIR_EOS/$1 | grep .root >! Completed ) > & /dev/null
    ( lcg-ls -b -D srmv2 --vo cms srm://grid143.kfki.hu:8446/srm/managerv2\?SFN=/dpm/kfki.hu/home/cms/phedex/store/user/$USERDIR_KFKI/$1 | grep .root | sed "s;/; ;g" | awk '{ print $NF }' >> Completed ) >& /dev/null
    sort -u Completed >! Comp
    mv Comp Completed
    set COMP=`wc -l Completed | awk '{ print $1}'`
    ls -l $1/STDOUT | grep -v test | grep .log >! Done
    set DONE=`wc -l Done | awk '{ print $1}'`
    rm taskjobs Pending Running Completed Done
    echo "Status of Task "$1" ("$NJOBS" Jobs):"
    echo "Jobs - Pending                : "$PEND
    echo "     - Running                : "$RUN
    echo "     - Done (with STDOUT)     : "$DONE
    echo "------------------------------------"
    echo "     - Completed (has output) : "$COMP
    if ( $DONE > $COMP ) then
        ls -l $TASKDIR/STDOUT | grep -v test | grep .log | awk '{ print "'$TASKDIR'/STDOUT/"$NF }' >! STDOUT_list
        echo -n "" >! quota_list
        set QUOTA = 0
        foreach a ( `cat STDOUT_list` )
            if ( `grep "Unable to access quota space" $a` != "" ) then
                echo $a | sed "s;_; ;g;s;\.; ;" | awk '{ printf "%d,", $(NF-1) }' >> quota_list
        	set QUOTA = `expr $QUOTA + 1`
            endif
        end
        set RESUB_LIST = `sed 's;,$;;' quota_list`
        if ( $QUOTA > 0 ) then
            echo "     - Disk quota problem     : "$QUOTA
            echo "       Free up space on EOS using -move_to_kfki"
            echo "       Then issue -resubmit "$RESUB_LIST
        endif
        rm STDOUT_list quota_list
    endif
else if ( "$OPT" == "-missing" ) then
    lcg-ls -b -D srmv2 --vo cms srm://grid143.kfki.hu:8446/srm/managerv2\?SFN=/dpm/kfki.hu/home/cms/phedex/store/user/$USERDIR_KFKI/$TASKDIR | grep .root | sed "s;/; ;g" | awk '{ print $NF }' > ! output
    ls /eos/user/$USERDIR_EOS/$TASKDIR | grep .root >> output
    cat output | sort -u | sed 's;_; ;g;s;\.; ;g' | awk '{ printf "%d\n", $(NF-1) }' >! jobnums
    seq 1 $NJOBS >! Seq
    diff Seq jobnums | grep "<" | awk '{ printf "%d,", $2 }' | sed 's;,$;\n;'
    rm Seq jobnums output
else if ( "$OPT" == "-resubmit" ) then
    if ( $3 == "" ) then
	echo "No Jobs specified"
    else
	echo "cd "$TASKDIR >! $TASKDIR/resub.csh
	echo $3 | tr ',' '\n' >! list
	echo "" >! list2
	foreach a ( `cat list` )
	    echo $a | grep -v "-" >> list2
	    eval `echo $a | grep - | sed 's;^;seq ;;s;-; ;'` >> list2
	end
	foreach a ( `cat list2` )
	    sed -n $a'p' $TASKDIR/alljobs.csh >> $TASKDIR/resub.csh
	end
	rm list list2
	echo "cd -" >> $TASKDIR/resub.csh
	echo "source "$TASKDIR"/resub.csh"
    endif
else if ( "$OPT" == "-resubmit_missing" ) then
    lcg-ls -b -D srmv2 --vo cms srm://grid143.kfki.hu:8446/srm/managerv2\?SFN=/dpm/kfki.hu/home/cms/phedex/store/user/$USERDIR_KFKI/$1 | grep .root > ! output
    ls /eos/user/$USERDIR_EOS/$1 | grep .root >> output
    cat output | sort -u | sed 's;_; ;g;s;\.; ;g' | awk '{ printf "%d\n", $(NF-1) }' >! jobnums
    seq 1 $NJOBS >! Seq
    diff Seq jobnums | grep "<" | awk '{ print $2 }' >! Missing
    echo "cd "$TASKDIR >! $TASKDIR/resub.csh
    foreach a ( `cat Missing` )
	cat $TASKDIR/alljobs.csh | head -$a | tail -1 >> $TASKDIR/resub.csh
    end
    rm Seq jobnums output Missing
    echo "cd -" >> $TASKDIR/resub.csh
    echo "source "$TASKDIR"/resub.csh"
else if ( "$OPT" == "-move_to_kfki" ) then
    ( ls -l /eos/user/$USERDIR_EOS/$TASKDIR | grep .root | awk '{ print $5" "$NF }' >! eos_list ) >& /dev/null
    ( lcg-ls -l -b -D srmv2 --vo cms srm://grid143.kfki.hu:8446/srm/managerv2\?SFN=/dpm/kfki.hu/home/cms/phedex/store/user/$USERDIR_KFKI/$TASKDIR | grep .root | sed "s;/; ;g" | awk '{ print $5" "$NF }' | sort -k 2 >! kfki_list ) >& /dev/null
    diff eos_list kfki_list | grep "<" | awk '{ print $3 }' | sort -k 2 >! to_move_list
    echo -n "" >! safe_to_delete_from_eos
    foreach a ( `cat eos_list | awk '{ print $2 }'` )
	grep $a kfki_list | awk '{ print "eos rm /eos/user/'$USERDIR_EOS'/'$TASKDIR'/"$2 }' >> safe_to_delete_from_eos
    end
    echo -n "" >! kfki_filesize_mismatch
    foreach a ( `cat to_move_list` )
	grep $a kfki_list | awk '{ print "lcg-del -l -D srmv2 --vo cms srm://grid143.kfki.hu:8446/srm/managerv2\\?SFN=/dpm/kfki.hu/home/cms/phedex/store/user/'$USERDIR_KFKI'/'$TASKDIR'/"$2 }' >> kfki_filesize_mismatch
    end
    cat safe_to_delete_from_eos >! $TASKDIR/move_to_kfki.csh
    cat kfki_filesize_mismatch >> $TASKDIR/move_to_kfki.csh
    awk '{ print "lcg-cp -v -b -D srmv2 --vo cms srm://srm-eoscms.cern.ch:8443/srm/v2/server\\?SFN=/eos/cms/store/caf/user/'$USERDIR_EOS'/'$TASKDIR'/"$1" srm://grid143.kfki.hu:8446/srm/managerv2\\?SFN=/dpm/kfki.hu/home/cms/phedex/store/user/'$USERDIR_KFKI'/'$TASKDIR'/"$1 }' to_move_list >> $TASKDIR/move_to_kfki.csh
    echo "source "$TASKDIR"/move_to_kfki.csh"
    rm eos_list kfki_list to_move_list safe_to_delete_from_eos kfki_filesize_mismatch
else if ( "$OPT" == "-delete" ) then
    rm -r $TASKDIR
else
    echo "Wrong Option"
    cat Usage
    rm Usage
    exit
endif

rm Usage

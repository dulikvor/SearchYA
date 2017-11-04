#!/bin/bash
workingDir=$PWD
MASTER_ADDRESS=${1-"127.0.0.1"}
WORKING_AREA_DIR=${2-"MesosWA"}
RED_COLOR=`tput setaf 1`
NO_COLOR=`tput sgr0`

setEnviorment()
{
    if ! echo $LIBRARY_PATH | grep -q "/Third_Party/lib"; then
        export "LIBRARY_PATH=$LIBRARY_PATH:$workingDir/Third_Party/lib"
    fi

    if ! echo $LD_LIBRARY_PATH | grep -q "/Third_Party/lib"; then
        export "LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$workingDir/Third_Party/lib"
    fi

    if ! echo $PATH | grep -q "/Third_Party/bin"; then
        export "PATH=$PATH:$workingDir/Third_Party/bin"
    fi


    if ! echo $MESOS_WORK_DIR | grep -q "/Third_Party/tmp/mesos"; then
        export MESOS_WORK_DIR=$workingDir/Third_Party/tmp/mesos-"$USER"
    fi

    if ! echo $CPATH | grep -q "/Third_Party/include"; then
        export "CPATH=$CPATH:$workingDir/Third_Party/include/"
    fi
}

#Open all processes in tabs
initiateAll()
{
	gnome-terminal --tab -e "sh -c \"./bin/ClusterManager -workingdir=. \"" --tab -e "sh -c \"./Third_Party/sbin/mesos-master --ip=$MASTER_ADDRESS --work_dir=$WORKING_AREA_DIR\"" --tab -e "sh -c \"sudo ./Third_Party/sbin/mesos-agent --master=$MASTER_ADDRESS:5050 --work_dir=$WORKING_AREA_DIR\"" --tab -e "sh -c \"./Third_Party/bin/redis-server --loadmodule ./bin/libSearchModule.so -workingdir=.\"" --tab -e "sh -c \"./Third_Party/src/node_modules/grpcc/bin/grpcc.js -p ./Communication/IDL/TextualSearchService.proto -a $MASTER_ADDRESS:50051 -i\"" --tab -e "sh -c \"./Third_Party/src/node_modules/grpcc/bin/grpcc.js -p ./Communication/IDL/TextualSearchResultsService.proto -a $MASTER_ADDRESS:50051 -i\""
}

killAll()
{
	sudo pkill -f mesos	
	pkill -f redis
	pkill -f ClusterManager
	pkill -f IndexBuilder
	pkill -f grpcc
	sudo rm -f $WORKING_AREA_DIR/meta/slaves/latest
}

help()
{
	echo "Optional commands are:"
	echo "h - Help"
	echo "i - Will initiate the cluster, mesos and redis processes"
	echo "k - Will kill all processes"
}
	

setEnviorment
echo "Read arguments Master address-$RED_COLOR$MASTER_ADDRESS$NO_COLOR WA-$RED_COLOR$WORKING_AREA_DIR$NO_COLOR"
while : ; do
	echo "Please enter command"
	read -n 1 keypress
	echo ""
	case $keypress in
		i)
		initiateAll
		;;
		h)
		help
		;;
		k)
		killAll
		;;
		*)
			echo "Invalid command, please enter h for help"
		;;
	esac
done

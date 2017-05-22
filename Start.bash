MASTER_ADDRESS=${1-"127.0.0.1"}
WORKING_AREA_DIR=${2-"MesosWA"}
REDIS_LOC=${3-"Third_Party/redis/src"}
RED_COLOR=`tput setaf 1`
NO_COLOR=`tput sgr0`

#Open all processes in tabs
initiateAll()
{
	gnome-terminal --tab -e "sh -c \"./ClusterManager/bin/ClusterManager -workingdir= \"" --tab -e "sh -c \"grpcc -p ./Communication/IDL/TextualSearchService.proto -a $MASTER_ADDRESS:50051 -i\"" --tab -e "sh -c \"mesos master --ip=$MASTER_ADDRESS --work_dir=$WORKING_AREA_DIR\"" --tab -e "sh -c \"mesos agent --master=$MASTER_ADDRESS:5050 --work_dir=$WORKING_AREA_DIR \"" --tab -e "sh -c \"$REDIS_LOC/redis-server --loadmodule ./RedisSearchModule/bin/libSearchModule.so -workingdir=../../../RedisSearchModule/bin\""
}

killAll()
{
	pkill -f mesos	
	pkill -f redis
	pkill -f ClusterManager
	pkill -f IndexBuilder
	pkill -f grpcc
	rm -f $WORKING_AREA_DIR/meta/slaves/latest
}

help()
{
	echo "Optional commands are:"
	echo "h - Help"
	echo "i - Will initiate the cluster, mesos and redis processes"
	echo "k - Will kill all processes"
}
	

echo "Read arguments Master address-$RED_COLOR$MASTER_ADDRESS$NO_COLOR WA-$RED_COLOR$WORKING_AREA_DIR$NO_COLOR Redis Location-$RED_COLOR$REDIS_LOC$NO_COLOR"
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

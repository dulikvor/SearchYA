MASTER_ADDRESS=${1-"127.0.0.1"}
WORKING_AREA_DIR=${2-"MesosWA"}
sh -c "mesos master --ip="$MASTER_ADDRESS" --work_dir="$WORKING_AREA_DIR &
sh -c "mesos agent --master="$MASTER_ADDRESS":5050 --work_dir="$WORKING_AREA_DIR &
gnome-terminal -x sh -c "./ClusterManager/bin/ClusterManager" &
gnome-terminal -x sh -c "grpcc -p ./Communication/IDL/ClusterService.proto -a "$MASTER_ADDRESS":50051 -i" &

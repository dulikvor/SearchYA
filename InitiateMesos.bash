WORKING_AREA_DIR=$1
MASTER_ADDRESS=$2
sh -c "mesos master --ip="$MASTER_ADDRESS" --work_dir="$WORKING_AREA_DIR &
sh -c "mesos agent --master="$MASTER_ADDRESS":5050 --work_dir="$WORKING_AREA_DIR &


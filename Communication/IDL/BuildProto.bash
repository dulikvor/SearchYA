PROJECT_DIR=$1
COMPILER=${PROJECT_DIR}/../Third_Party/bin/protoc
COMPILE_DIR=IDL
OUPUT_DIR=GeneretedFiles
#Create output dir if not exists
mkdir -p ${PROJECT_DIR}/${OUPUT_DIR}
echo "ProtoBuf/GRPC build begins!!!"
for file in $(find $PROJECT_DIR"/"$COMPILE_DIR -name '*.proto');
do
sh -c $COMPILER" -I="$PROJECT_DIR"/"$COMPILE_DIR" --grpc_out="$PROJECT_DIR"/"$OUPUT_DIR" --plugin=protoc-gen-grpc=${PROJECT_DIR}/../Third_Party/bin/grpc_cpp_plugin "$file
sh -c $COMPILER" -I="$PROJECT_DIR"/"$COMPILE_DIR" --cpp_out="$PROJECT_DIR"/"$OUPUT_DIR" "$file
done




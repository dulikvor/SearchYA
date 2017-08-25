PROJECT_DIR=$1
COMPILER=${PROJECT_DIR}/../Third_Party/bin/protoc
COMPILE_DIR=IDL
OUPUT_DIR=GeneretedFiles
#Create output dir if not exists
mkdir -p ${PROJECT_DIR}/${OUPUT_DIR}
for file in $(find $PROJECT_DIR"/"$COMPILE_DIR -name '*.proto');
do
SOURCE_FILES=$file" "$SOURCE_FILES
done
#MESSAGE( STATUS "ProtoBuf/GRPC build begins!!!")
sh -c $COMPILER" -I="$PROJECT_DIR"/"$COMPILE_DIR" --grpc_out="$PROJECT_DIR"/"$OUPUT_DIR" --plugin=protoc-gen-grpc=${PROJECT_DIR}/../Third_Party/bin/grpc_cpp_plugin "$SOURCE_FILES

sh -c $COMPILER" -I="$PROJECT_DIR"/"$COMPILE_DIR" --cpp_out="$PROJECT_DIR"/"$OUPUT_DIR" "$SOURCE_FILES



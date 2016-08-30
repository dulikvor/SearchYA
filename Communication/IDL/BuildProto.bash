COMPILER=protoc
PROJECT_NAME=$1
COMPILE_DIR=IDL
OUPUT_DIR=GeneretedFiles

for file in $(find $PROJECT_NAME"/"$COMPILE_DIR -name '*.proto');
do
SOURCE_FILES=$file" "$SOURCE_FILES
done

sh -c $COMPILER" -I="$PROJECT_NAME"/"$COMPILE_DIR" --grpc_out="$PROJECT_NAME"/"$OUPUT_DIR" --plugin=protoc-gen-grpc=/usr/local/bin/grpc_cpp_plugin "$SOURCE_FILES

sh -c $COMPILER" -I="$PROJECT_NAME"/"$COMPILE_DIR" --cpp_out="$PROJECT_NAME"/"$OUPUT_DIR" "$SOURCE_FILES



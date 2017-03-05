SEARCH="$(find "$PWD" -name "*.cpp")"
INCLUDE_PATH="-I$PWD -I$PWD/Communication -I$PWD/Core -I$PWD/ClusterManager -I$PWD/Thired_Party -I$PWD/../../Downloads/mesos/boost-1.53.0 "
echo $INCLUDE_PATH
g++ -std=c++11 -M $INCLUDE_PATH $SEARCH | sed -e 's/[\\ ]/\n/g' | \
sed -e '/^$/d' -e '/\.o:[ \t]*$/d' | \
ctags -L - --c++-kinds=+p --fields=+iaS --extra=+q

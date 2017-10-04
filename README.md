# SearchYA
A simple distributed textual search engine, based upon apache mesos and redis DB.

## Getting Started
These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites
SearchYa is based upon c++14, its only known supported platform is Ubuntu 16.04.
it uses posix quite frequently (the non portable part), so it might also work on diffirent unix based distributions.

c-ares installation (if not exists, required version - 1.3.0):
```
cd download-dir
wget https://c-ares.haxx.se/download/c-ares-1.13.0.tar.gz
tar -xf c-ares-1.13.0.tar.gz
cd download-dir/c-ares-1.13.0
./configure
make
make install
```

### Installing
A very simplified install, just one command, it will fetch all needed 3rd party dependencies.
```
source INSTALL.sh
```

## Running
A simple start.bash script is provided, the script only supports 3 commands:
1) initiate.
2) kill.
3) help.

use those to bring up the cluster and the interactive client.
```
source ./Start.sh
```




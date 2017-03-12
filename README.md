# MesosBenchMark
A simple textual search engine build upon mesos and redis. the search engine includes the following:
1) Two frameworks - Index FW and Search FW.
2) A scheduler for each FW.
3) Each FW BE is written as a mesos executor.
4) A core and communication layer.
5) Communication to each FW scheduler is done via grpc.
6) All FW internal messaging is done via mesos.
7) Logging infra structure is based upon spdlog.

if (NOT MESOS_FOUND)

    ExternalProject_Add(Mesos
            DOWNLOAD_NAME       mesos-1.1.3-rc1.tar
            URL                 https://github.com/apache/mesos/archive/1.1.3-rc1.tar.gz
            CONFIGURE_COMMAND   cd <SOURCE_DIR> && ./bootstrap && mkdir -p <SOURCE_DIR>/build && cd <SOURCE_DIR>/build && ../configure --disable-java --disable-python --with-protobuf=<INSTALL_DIR> --prefix=<INSTALL_DIR>
            BUILD_COMMAND       make -C <SOURCE_DIR>/build -j 2 V=0
            INSTALL_COMMAND     make -C <SOURCE_DIR>/build install
            TEST_COMMAND        ""
            )

    ExternalProject_Get_Property(Mesos INSTALL_DIR)

    set (MESOS_ROOT_DIR          ${INSTALL_DIR})
    set (MESOS_BINARY_DIR        ${MESOS_ROOT_DIR}/bin)
    set (MESOS_INCLUDE_DIR       ${MESOS_ROOT_DIR}/include)
    set (MESOS_LIBRARY_DIR       ${MESOS_ROOT_DIR}/lib)
    set (MESOS_FOUND             YES)

endif ()

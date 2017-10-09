if (NOT GRPC_FOUND)
    ExternalProject_Add(grpc
            DOWNLOAD_COMMAND    cd <INSTALL_DIR>/src && sh -c "wget https://github.com/grpc/grpc/archive/v1.4.2.tar.gz" && sh -c "tar -xf v1.4.2.tar.gz"
            CONFIGURE_COMMAND   cd <SOURCE_DIR> && git submodule update --init
            BUILD_COMMAND       make -C <INSTALL_DIR>/src/grpc-1.4.2
            INSTALL_COMMAND     make -C <INSTALL_DIR>/src/grpc-1.4.2  prefix=<INSTALL_DIR> install
            TEST_COMMAND        ""
            )
    ExternalProject_Get_Property(grpc INSTALL_DIR)

    set (GRPC_ROOT_DIR          ${INSTALL_DIR})
    set (GRPC_LIBRARY_DIR       ${GRPC_ROOT_DIR}/lib)
    set (GRPC_INCLUDE_DIR       ${GRPC_ROOT_DIR}/include)
    set (GRPC_FOUND             YES)

endif ()

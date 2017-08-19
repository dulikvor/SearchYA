if (NOT GRPC_FOUND)
    ExternalProject_Add(grpc
            DOWNLOAD_COMMAND    cd <INSTALL_DIR>/src && sh -c "git clone -b $(curl -L https://grpc.io/release) https://github.com/grpc/grpc"
            CONFIGURE_COMMAND   cd <SOURCE_DIR> && git submodule update --init
            BUILD_COMMAND       make -C <SOURCE_DIR>
            INSTALL_COMMAND     make -C <SOURCE_DIR>  prefix=<INSTALL_DIR> install
            TEST_COMMAND        ""
            )
    ExternalProject_Get_Property(grpc INSTALL_DIR)

    set (GRPC_ROOT_DIR          ${INSTALL_DIR})
    set (GRPC_LIBRARY_DIR       ${GRPC_ROOT_DIR}/lib)
    set (GRPC_INCLUDE_DIR       ${GRPC_ROOT_DIR}/include)
    set (GRPC_FOUND             YES)

endif ()

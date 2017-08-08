if (NOT PROTOBUF_FOUND)

    ExternalProject_Add(ProtoBuf
            DOWNLOAD_NAME       protobuf-cpp-3.3.0.tar
            URL                 https://github.com/google/protobuf/releases/download/v3.3.0/protobuf-cpp-3.3.0.tar.gz
            CONFIGURE_COMMAND   cd <SOURCE_DIR> && ./autogen.sh && <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>
            BUILD_COMMAND       make -C <SOURCE_DIR>
            INSTALL_COMMAND     make -C <SOURCE_DIR> install
            TEST_COMMAND        ""
            )

    ExternalProject_Get_Property(ProtoBuf INSTALL_DIR)

    set (PROTOBUF_ROOT_DIR          ${INSTALL_DIR})
    set (PROTOBUF_LIBRARY_DIR       ${PROTOBUF_ROOT_DIR}/lib)
    set (PROTOBUF_BINARY_DIR        ${PROTOBUF_ROOT_DIR}/bin)
    set (PROTOBUF_FOUND             YES)

endif ()

if (NOT CPPKIN_FOUND)
    ExternalProject_Add(cppkin
            DOWNLOAD_COMMAND    cd <INSTALL_DIR>/src && sh -c "git clone https://github.com/Dudi119/cppKin <INSTALL_DIR>/src/cppkin_temp"
            CONFIGURE_COMMAND   cp -a <INSTALL_DIR>/src/cppkin_temp/. <INSTALL_DIR>/src/cppkin && rm -rf <INSTALL_DIR>/src/cppkin_temp && cd <SOURCE_DIR> && git submodule update --init
            BUILD_COMMAND       cd <SOURCE_DIR> && sh -c "./INSTALL.sh"
            INSTALL_COMMAND     cp <SOURCE_DIR>/bin/libcppkin.so <INSTALL_DIR>/lib && mkdir -p <INSTALL_DIR>/include/cppkin && cp <SOURCE_DIR>/src/cppkin.h <INSTALL_DIR>/include/cppkin
            TEST_COMMAND        ""
            )
    ExternalProject_Get_Property(cppkin INSTALL_DIR)

    set (CPPKIN_ROOT_DIR          ${INSTALL_DIR})
    set (CPPKIN_FOUND             YES)
    set (CPPKIN_LIBRARY_DIR       YES)
    set (CPPKIN_INCLUDE_DIR       YES)

endif ()

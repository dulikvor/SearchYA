if (NOT GRPCC_FOUND)
    ExternalProject_Add(Grpcc
            DOWNLOAD_COMMAND    cd <INSTALL_DIR>/src && sh -c "npm install grpcc"
            CONFIGURE_COMMAND   ""
            BUILD_COMMAND       ""
            INSTALL_COMMAND     ""
            TEST_COMMAND        ""
            )

    ExternalProject_Get_Property(Grpcc INSTALL_DIR)

    set (GRPCC_ROOT_DIR          ${INSTALL_DIR})
    set (GRPCC_INCLUDE_DIR       ${GRPCC_ROOT_DIR}/src/node_modules/grpcc/bin)
    set (GRPCC_FOUND             YES)

endif ()

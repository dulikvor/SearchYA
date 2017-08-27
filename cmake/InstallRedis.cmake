if (NOT REDIS_FOUND)

    ExternalProject_Add(Redis
            DOWNLOAD_NAME       redis-4.0.0.tar.gz
            URL                 https://github.com/antirez/redis/archive/4.0.0.tar.gz
            CONFIGURE_COMMAND   ""
            BUILD_COMMAND       make -C <SOURCE_DIR>
            INSTALL_COMMAND     make -C <SOURCE_DIR> PREFIX=<INSTALL_DIR> install
            TEST_COMMAND        ""
            )


    ExternalProject_Add_Step(Redis Redis_Install_Headers
            COMMAND     sh -c "mkdir -p <INSTALL_DIR>/include/Redis && find <SOURCE_DIR>/src -name '*.h' -exec cp --target-directory=<INSTALL_DIR>/include/Redis/ {} +"
            DEPENDEES   install
            )


    ExternalProject_Add_Step(Redis Redis_Patch_Header
            COMMAND     sh -c "cp <INSTALL_DIR>/Patch/redismodule.h <SOURCE_DIR>/src"
            DEPENDEES  download
            )

    ExternalProject_Get_Property(Redis INSTALL_DIR)

    set (REDIS_ROOT_DIR          ${INSTALL_DIR})
    set (REDIS_INCLUDE_DIR       ${REDIS_ROOT_DIR}/include)
    set (REDIS_FOUND             YES)

#    mark_as_advanced(GLOG_LIBRARY GLOG_INCLUDE_PATH)

endif ()

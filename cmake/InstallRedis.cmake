if (NOT REDIS_FOUND)

    ExternalProject_Add(Redis
            DOWNLOAD_NAME       redis-4.0.0.tar.gz
            URL                 https://github.com/antirez/redis/archive/4.0.0.tar.gz
            CONFIGURE_COMMAND   ""
            BUILD_COMMAND       make -C <SOURCE_DIR>
            INSTALL_COMMAND     make -C <SOURCE_DIR> PREFIX=<INSTALL_DIR> install
            TEST_COMMAND        ""
            )

    ExternalProject_Get_Property(Redis INSTALL_DIR)

    set (REDIS_ROOT_DIR          ${INSTALL_DIR})
    set (REDIS_FOUND             YES)

#    add_library(GLOG_LIBRARY STATIC IMPORTED)
#    add_dependencies(GLOG_LIBRARY GLog)
#    mark_as_advanced(GLOG_LIBRARY GLOG_INCLUDE_PATH)

endif ()

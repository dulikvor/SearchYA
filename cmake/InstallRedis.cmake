if (NOT REDIS_FOUND)

    ExternalProject_Add(Redis
            DOWNLOAD_NAME       redis-4.0.0.tar.gz
            URL                 https://github.com/antirez/redis/archive/4.0.0.tar.gz
            CONFIGURE_COMMAND   ""
            BUILD_COMMAND       cd <SOURCE_DIR> && make
            INSTALL_COMMAND     ""
            TEST_COMMAND        ""
            )

    ExternalProject_Get_Property(Redis INSTALL_DIR)

#    set (GLOG_ROOT_DIR          ${INSTALL_DIR})
#   set (GLOG_INCLUDE_PATH      ${GLOG_ROOT_DIR}/include)
#    set (GLOG_LIBRARY           ${GLOG_ROOT_DIR}/lib/libglog.a)
#    set (GLOG_FOUND             YES)

#    add_library(GLOG_LIBRARY STATIC IMPORTED)
#    add_dependencies(GLOG_LIBRARY GLog)
#    mark_as_advanced(GLOG_LIBRARY GLOG_INCLUDE_PATH)

#    message(STATUS "Use bundled glog v${GLOG_VERSION}")
endif ()

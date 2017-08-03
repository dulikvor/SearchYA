find_library(REDIS_LIBRARY NAMES redis)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(redis REQUIRED_VARS REDIS_LIBRARY)

if(REDIS_FOUND)
    message(STATUS "Found libraries - ${REDIS_LIBRARY}")
else(REDIS_FOUND)
    #message(FATAL_ERROR "Redis libraries not found")
endif(REDIS_FOUND)

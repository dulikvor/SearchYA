find_path(GRPCC_INCLUDE_DIR NAMES grpcc.js PATHS ${EXTERNAL_PROJECT_DIR}/src/node_modules/grpcc/bin NO_DEFAULT_PATH)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(grpcc REQUIRED_VARS GRPCC_INCLUDE_DIR)

if(GRPCC_FOUND)
    message(STATUS "Grpcc Include dir - ${GRPCC_INCLUDE_DIR}")
else()
    message(WARNING "Grpcc not found")
endif()

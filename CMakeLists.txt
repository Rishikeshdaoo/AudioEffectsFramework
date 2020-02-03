cmake_minimum_required (VERSION 3.0)
project (AudioFxFramework
        VERSION 0.0.0.1)



#######################################################################################################
## includes
# where to look for the default stuff...
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake.modules")

# platform detection
include (SetPlatform)

# output directories
include (SetOutputDir)

# oslibs
include (SetOsLibs)

# helper functions
include(AddSrcDirectories)
include(AddIncDirectories)


#######################################################################################################
## project specific stuff

# set directories
set (PROJECT_DIRECTORIES)
list (APPEND PROJECT_DIRECTORIES ${CMAKE_SOURCE_DIR}/src/AudioFxFrameworkExec)       
list (APPEND PROJECT_DIRECTORIES ${CMAKE_SOURCE_DIR}/src/AudioEffect) 
    
# set includes
set (PROJECT_INCLUDES)
list (APPEND PROJECT_INCLUDES ${CMAKE_SOURCE_DIR}/inc)       
list (APPEND PROJECT_INCLUDES ${CMAKE_SOURCE_DIR}/src/inc)       
list (APPEND PROJECT_INCLUDES ${CMAKE_BINARY_DIR}/inc)       

# add directories
add_src_directories ()


# output version number variable
message(STATUS ${PROJECT_NAME} Version: ${PROJECT_VERSION})
message(STATUS "")

# generate platform config file
configure_file(${CMAKE_SOURCE_DIR}/src/${PROJECT_NAME}Config.h.in ${CMAKE_BINARY_DIR}/inc/${PROJECT_NAME}Config.h)

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
list (APPEND PROJECT_DIRECTORIES ${CMAKE_SOURCE_DIR}/src/AudioFileIO)
list (APPEND PROJECT_DIRECTORIES ${CMAKE_SOURCE_DIR}/src/Tests/TestExec)  
    
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
# integrate cppcheck targets in IDE
option (WITH_CPPCHECKTARGET "Add target for cppcheck testing" OFF)
#######################################################################################################
# ## tests

# with tests or not
option (WITH_TESTS "Use UnitTest++ for testing" ON)

if (WITH_TESTS)
    include(AddCppcheckTest)
    enable_testing()
    
    # add tests here
    add_test(AudioIo ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/TestExec AudioIo "${CMAKE_SOURCE_DIR}/src/Tests/TestData/")
    add_test(AudioEffect ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/TestExec AudioEffect)
    
    add_cppcheck_test ()
endif (WITH_TESTS)

#######################################################################################################
## add documentation
#-- Add an Option to toggle the generation of the API documentation
option(WITH_DOXYGENTARGET "Use Doxygen to create the documentation" OFF)
if(WITH_DOXYGENTARGET)
	FIND_PACKAGE(Doxygen)
	if (NOT DOXYGEN_FOUND)
	message(FATAL_ERROR 
	  "Doxygen is needed to build the documentation. Please install it correctly")
	endif(NOT DOXYGEN_FOUND)
  
	#-- Configure the Template doxy.config for our specific project
	configure_file(${CMAKE_SOURCE_DIR}/src/doxy.config.in 
				 ${PROJECT_BINARY_DIR}/docs/doxy.config  @ONLY IMMEDIATE)
	#-- Add a custom target to run Doxygen when ever the project is built
	add_custom_target (Doxy_documentation  
			COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/docs/doxy.config
			SOURCES ${PROJECT_BINARY_DIR}/docs/doxy.config)
endif(WITH_DOXYGENTARGET)



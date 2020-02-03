set(CurrLib sndlib)

# get sndlib source and header files
file(GLOB ${CurrLib}_SOURCES RELATIVE ${CMAKE_SOURCE_DIR}/3rdparty/${CurrLib} *.c;*.cpp;*.txt)
file(GLOB ${CurrLib}_HEADERS RELATIVE ${CMAKE_SOURCE_DIR}/3rdparty/${CurrLib} *.h;../incl/*.h)

# remove the files with main functions
file(GLOB to_remove RELATIVE ${CMAKE_SOURCE_DIR}/3rdparty/${CurrLib} *sndplay.c;*sndinfo.c)
list(REMOVE_ITEM ${CurrLib}_SOURCES ${to_remove})

# set include path
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/${CurrLib})

# build a static library
add_library(${CurrLib} STATIC ${${CurrLib}_SOURCES} ${${CurrLib}_HEADERS})
set_target_properties(${CurrLib} PROPERTIES CLEAN_DIRECT_OUTPUT 1)

# set output name
set_target_properties(${CurrLib} PROPERTIES OUTPUT_NAME "${CurrLib}")
set_target_properties(${CurrLib} PROPERTIES PREFIX "lib")

if(MSVC)
    set_target_properties(${CurrLib} PROPERTIES COMPILE_FLAGS "/W1 /DMUS_WINDOZE")
endif(MSVC)

if (APPLE)
	set_target_properties(${CurrLib} PROPERTIES COMPILE_FLAGS "-w")
endif(APPLE)

set(CurrLib AudioEffect)

# get all source files
file(GLOB ${CurrLib}_SOURCES RELATIVE ${CMAKE_SOURCE_DIR}/src/${CurrLib} *.cpp;*.txt)
file(GLOB ${CurrLib}_HEADERS RELATIVE ${CMAKE_SOURCE_DIR}/src/${CurrLib} *.h;../inc/*.h)

## add include directories
add_inc_directories ()

# build a static library
add_library(${CurrLib} STATIC ${${CurrLib}_SOURCES} ${${CurrLib}_HEADERS})
set_target_properties(${CurrLib} PROPERTIES CLEAN_DIRECT_OUTPUT 1)

# set output name
set_target_properties(${CurrLib} PROPERTIES OUTPUT_NAME "${CurrLib}")
set_target_properties(${CurrLib} PROPERTIES PREFIX "lib")

if (WITH_CPPCHECKTARGET)
    # cppcheck target in the IDE
    file(GLOB to_remove RELATIVE ${CMAKE_SOURCE_DIR}/src/${CurrLib} *.txt)
    list(REMOVE_ITEM ${CurrLib}_SOURCES ${to_remove})
    include (GenerateCppcheck)
    GENERATE_CPPCHECK(SOURCES ${${CurrLib}_SOURCES}
                        TARGET_NAME "CppCheck_${CurrLib}"
                      INCLUDES ${PROJECT_INCLUDES})
endif(WITH_CPPCHECKTARGET)

set (APP AudioFxFrameworkExec)

set (AudioLIB AudioFileIO)
set (AudioEffectLIB AudioEffect)

file(GLOB ${APP}_SOURCES RELATIVE ${CMAKE_SOURCE_DIR}/src/${APP} *.c;*.cpp;*.txt)
file(GLOB ${APP}_HEADERS RELATIVE ${CMAKE_SOURCE_DIR}/src/${APP} *.h;../inc/*.h)

## add include directories
add_inc_directories ()

## create the target
add_executable(${APP} ${${APP}_SOURCES} ${${APP}_HEADERS})

# link the libraries
target_link_libraries(${APP} ${LIB} ${TestLIB} ${AudioEffectLIB} ${AudioLIB} ${SndLIB} ${OS_LIBS})

if (WITH_CPPCHECKTARGET)
    # cppcheck target in the IDE
    file(GLOB to_remove RELATIVE ${CMAKE_SOURCE_DIR}/src/${APP} *.txt)
    list(REMOVE_ITEM ${APP}_SOURCES ${to_remove})
    include (GenerateCppcheck)
    GENERATE_CPPCHECK(SOURCES ${${APP}_SOURCES}
                        TARGET_NAME "CppCheck_${APP}"
                      INCLUDES ${PROJECT_INCLUDES})
endif(WITH_CPPCHECKTARGET)

set (APP AudioFxFrameworkExec)

set (AudioEffectLIB AudioEffect)

file(GLOB ${APP}_SOURCES RELATIVE ${CMAKE_SOURCE_DIR}/src/${APP} *.c;*.cpp;*.txt)
file(GLOB ${APP}_HEADERS RELATIVE ${CMAKE_SOURCE_DIR}/src/${APP} *.h;../inc/*.h)

## add include directories
add_inc_directories ()

## create the target
add_executable(${APP} ${${APP}_SOURCES} ${${APP}_HEADERS})

# link the libraries
target_link_libraries(${APP} ${AudioEffectLIB} ${SndLIB} ${OS_LIBS})
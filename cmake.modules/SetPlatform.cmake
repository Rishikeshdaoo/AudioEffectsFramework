## set defines for operating system and platform

if (APPLE)
	set(GTCMT_MACOSX 1)
endif (APPLE)

if (UNIX AND NOT APPLE)
	## get bit depth
	if ( CMAKE_SIZEOF_VOID_P EQUAL 4)
		set(GTCMT_LINUX32 1)
	else ( CMAKE_SIZEOF_VOID_P EQUAL 4 )
		set(GTCMT_LINUX64 1)
	endif ( CMAKE_SIZEOF_VOID_P EQUAL 4 )
endif (UNIX AND NOT APPLE)

if(MINGW)
	if ( CMAKE_SIZEOF_VOID_P EQUAL 4)
		set(GTCMT_WIN32 1)
	else ( CMAKE_SIZEOF_VOID_P EQUAL 4 )
		set(GTCMT_WIN64 1)
	endif ( CMAKE_SIZEOF_VOID_P EQUAL 4 )
endif(MINGW)

if (MSVC)
	if ( CMAKE_CL_64 EQUAL 0 )
		set(GTCMT_WIN32 1)
	else ( CMAKE_CL_64 EQUAL 0 )
		set(GTCMT_WIN64 1)
	endif ( CMAKE_CL_64 EQUAL 0 )
    
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od /fp:precise /EHsc /W4")
endif (MSVC)
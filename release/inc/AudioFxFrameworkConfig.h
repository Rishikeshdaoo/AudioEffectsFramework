#ifndef __AudioFxFramework_config_hdr__
#define __AudioFxFramework_config_hdr__

// set platform defines
#ifndef GTCMT_LINUX32
/* #undef GTCMT_LINUX32 */
#endif // GTCMT_LINUX32
#ifndef GTCMT_LINUX64
/* #undef GTCMT_LINUX64 */
#endif // GTCMT_LINUX64
#ifndef GTCMT_MACOSX
#define GTCMT_MACOSX
#endif // GTCMT_MACOSX
#ifndef GTCMT_WIN32
/* #undef GTCMT_WIN32 */
#endif // GTCMT_WIN32
#ifndef GTCMT_WIN64
/* #undef GTCMT_WIN64 */
#endif // GTCMT_WIN64

//////////////////////////////////////////////////////////
// set module switches
#ifndef WITH_TESTS //!< UnitTest++ should be used for for testing
/* #undef WITH_TESTS */
#endif // WITH_TESTS

// set the version number
#define AudioFxFramework_VERSION_MAJOR 0
#define AudioFxFramework_VERSION_MINOR 0
#define AudioFxFramework_VERSION_PATCH 0

#endif // __MigEdit_config_hdr__

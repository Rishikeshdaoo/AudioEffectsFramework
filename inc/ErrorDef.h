#if !defined(__ErrorDef_hdr__)
#define __ErrorDef_hdr__

enum Error_t
{
    kNoError,    //!< No error

    kFileOpenError,    //!< Error in opening file
    kFileAccessError,   //!< Error accessing file

    kFunctionInvalidArgsError,  //!< Invalid arguments passed to function

    kNotInitializedError,   //!< Effect instance not initialized properly
    kFunctionIllegalCallError,  //!< Illegal call to function
    kInvalidString, //!< Invalid string passed

    kMemError,  //!< Problem with memory allocation

    kUnknownError,  //!< Unknown error occured

    kNumErrors //!< Number error
};
#endif // #if !defined(__ErrorDef_hdr__)




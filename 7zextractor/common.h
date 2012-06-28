#ifndef SEVENZIP_EXTRACTOR_COMMON_H_
#define SEVENZIP_EXTRACTOR_COMMON_H_

// callback function
typedef void   (__stdcall *SetTotalProc)
                          (unsigned __int64 totalsize);
typedef void   (__stdcall *SetCompletedProc)
                          (unsigned __int64 completesize);
typedef void   (__stdcall *SetOperationResultProc)
                          (int operation_result);

// library function
typedef int    (__stdcall *InitProc)
                          (const wchar_t* xadll);
typedef int    (__stdcall *OpenProc)
                          (const wchar_t* filename);
typedef int    (__stdcall *OpenFromMemoryProc)
                          (const void* data, unsigned __int64 len);
typedef int    (__stdcall *ExtractProc)
                          (const wchar_t* path,
                           SetTotalProc SetTotal,
                           SetCompletedProc SetCompleted,
                           SetOperationResultProc SetOperationResult);

namespace extractresult {

namespace operation {
  enum {
    kOK = 0,
    kUnSupportedMethod,
    kDataError,
    kCRCError
  };
}

namespace init {
  enum {
    kOK = 0,
    kLoadLibraryError,
    kLibraryInsideError,
    kGet7zObjectError
  };
}

namespace open {
  enum {
    kOK = 0,
    kOpenFileError,
    kOpenArchiveError,
    kPasswordError
  };
}

namespace extract {
  enum {
    kOK = 0,
    kDeleteExistFileError,
    kCreateFileError,
    kPasswordError,
    kUnknownError
  };
}
} //namespace extractresult

#endif
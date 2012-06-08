#pragma once

#include "ArchiveOpenCallBack.h"
#include "ArchiveExtractCallBack.h"

#include "ult\library.h"

#include <string>

typedef void (__stdcall *SetTotalProc)(unsigned __int64 totalsize);
typedef void (__stdcall *SetCompleteProc)(unsigned __int64 completesize);
typedef void (__stdcall *SetOperationResultProc)(int operation_result);
typedef UINT32 (__stdcall *CreateObjectProc)(const GUID* clsid, const GUID* interfaceid, void**outobject);

namespace extractresult {

namespace pperation {
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
}


class Extractor {

public:

  Extractor(void);
  ~Extractor(void);

  int Init(void);

  int Open(const std::wstring& filename);

  int Extract(const std::wstring& path,
              SetTotalProc SetTotal,
              SetCompleteProc SetComplete,
              SetOperationResultProc SetOperationResult);
  
private:

  ult::Library lib_;

  CMyComPtr<IInArchive> archive_;
  InFileStream* file_spec_;
  CMyComPtr<IInStream> file_;
  ArchiveOpenCallBack* open_callback_spec_;
  CMyComPtr<IArchiveOpenCallback> open_callback_;
  ArchiveExtractCallBack* extract_callback_spec_;
  CMyComPtr<IArchiveExtractCallback> extract_callback_;

  static const wchar_t* kDllName_;
};
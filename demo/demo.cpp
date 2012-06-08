// demo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ult\library.h"
#include "ult\file-dir.h"

typedef void (__stdcall *SetTotalProc)(unsigned __int64 totalsize);
typedef void (__stdcall *SetCompleteProc)(unsigned __int64 completesize);
typedef void (__stdcall *SetOperationResultProc)(int operation_result);
typedef int  (__stdcall *InitProc)(void);
typedef int  (__stdcall *OpenProc)(const wchar_t* filename);
typedef int  (__stdcall *ExtractProc)(const wchar_t* path,
                                      SetTotalProc SetTotal,
                                      SetCompleteProc SetComplete,
                                      SetOperationResultProc SetOperationResult);

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

void __stdcall SetTotal(unsigned __int64 totalsize) {

}
void __stdcall SetComplete(unsigned __int64 completesize) {

}
void __stdcall SetOperationResult(int operation_result) {

}

int _tmain(int argc, _TCHAR* argv[])
{
  std::wstring self_path;
  ult::GetSelfModulePath(&self_path);
  std::wstring dll_path = self_path + L"7zextractor.dll";
  ult::Library lib;
  lib.Load(dll_path.c_str());
  if (!lib.IsLoaded()) {
    return 1;
  }

  InitProc Init = (InitProc)lib.GetProc("Init");
  OpenProc Open = (OpenProc)lib.GetProc("Open");
  ExtractProc Extract = (ExtractProc)lib.GetProc("Extract");

  if (Init() != extractresult::init::kOK) {
  }
  if (Open(L"E:\\temp\\test.7z") != extractresult::open::kOK) {
  }
  if (Extract(L"E:\\temp\\a\\", SetTotal, SetComplete, SetOperationResult) != extractresult::extract::kOK) {
  }
	return 0;
}
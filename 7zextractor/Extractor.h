#pragma once

#include "common.h"
#include "ArchiveOpenCallBack.h"
#include "ArchiveExtractCallBack.h"

#include "ult\library.h"

#include <string>

typedef UINT32 (__stdcall *CreateObjectProc)
                          (const GUID* clsid, const GUID* interfaceid, void**outobject);

class Extractor {

public:

  Extractor(void);
  ~Extractor(void);

  int Init(const wchar_t* xadll);

  int Open(const std::wstring& filename);

  int OpenFromMemory(const void* data, unsigned __int64 len);

  int Extract(const std::wstring& path,
              SetTotalProc SetTotal,
              SetCompletedProc SetComplete,
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
  static const wchar_t* kSelfDllName_;
};
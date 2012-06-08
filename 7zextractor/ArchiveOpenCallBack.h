#pragma once

#include "7z/cpp/common/MyInitGuid.h"
#include "7z/cpp/7zip/IPassword.h"
#include "7z/cpp/7zip/archive/IArchive.h"
#include "7z/cpp/common/MyCom.h"

#include <string>

class ArchiveOpenCallBack :
    public IArchiveOpenCallback,
    public ICryptoGetTextPassword,
    public CMyUnknownImp {

public:

  MY_UNKNOWN_IMP1(ICryptoGetTextPassword)

  STDMETHOD(SetTotal)(const UInt64* files, const UInt64* bytes);
  STDMETHOD(SetCompleted)(const UInt64* files, const UInt64* bytes);

  STDMETHOD(CryptoGetTextPassword)(BSTR* password);

  bool password_is_defined_;
  std::wstring password_;

  ArchiveOpenCallBack(void);
};
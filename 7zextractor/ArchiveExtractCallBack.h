#pragma once

#include "7zFileStream.h"

#include "7z/cpp/common/MyInitGuid.h"
#include "7z/cpp/7zip/IPassword.h"
#include "7z/cpp/7zip/archive/IArchive.h"
#include "7z/cpp/common/MyCom.h"

#include <string>

class ArchiveExtractCallBack :
    public IArchiveExtractCallback,
    public ICryptoGetTextPassword,
    public CMyUnknownImp {

public:

  void Init(IInArchive* archive_handler, const std::wstring& directory_paty);

  MY_UNKNOWN_IMP1(ICryptoGetTextPassword)
  
  //IProgress
  STDMETHOD(SetTotal)(UInt64 size);
  STDMETHOD(SetCompleted)(const UInt64* complete_value);

  //IArchiveExtractCallBack
  STDMETHOD(GetStream)(UInt32 index, ISequentialOutStream **out_stream, Int32 ask_extract_mode);
  STDMETHOD(PrepareOperation)(Int32 ask_extract_mode);
  STDMETHOD(SetOperationResult)(Int32 operation_result);

  //ICryptoGetTextPassword
  STDMETHOD(CryptoGetTextPassword)(BSTR* password);

  bool password_is_defined_;
  std::wstring password_;

  ArchiveExtractCallBack(void);

private:

  struct ProcessedFileInfo {
    FILETIME mtime;
    DWORD attrib;
    bool isdir;
    bool attrib_defined;
    bool mtime_defined;
  } processed_fileinfo_;

  CMyComPtr<IInArchive> archive_handler_;
  OutFileStream* out_filestream_spec_;
  CMyComPtr<ISequentialOutStream> out_filestream_;

  std::wstring file_path_; //name inside archive
  std::wstring directory_path_; //output directory
  std::wstring disk_file_path_; //full path to file on disk

  bool extract_mode_;

  static const wchar_t* kEmptyFileAlias_;

};
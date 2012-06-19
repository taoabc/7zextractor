#include "StdAfx.h"
#include "Extractor.h"

#include "ult\file-dir.h"
#include "ult\file-io.h"

DEFINE_GUID(CLSID_CFormat7z,
    0x23170F69, 0x40C1, 0x278A, 0x10, 0x00, 0x00, 0x01, 0x10, 0x07, 0x00, 0x00);

const wchar_t* Extractor::kDllName_     = L"7zxa.dll";
const wchar_t* Extractor::kSelfDllName_ = L"7zextractor.dll";

Extractor::Extractor(void) {
}


Extractor::~Extractor(void) {
}

int Extractor::Init(const wchar_t* xadll) {
  std::wstring dll_path;
  if (xadll == NULL) {
    std::wstring self_path;
    ult::GetSelfModulePath(&self_path);
    dll_path = self_path + kDllName_;
    if (!ult::IsPathFileExist(dll_path)) {
      std::wstring dll_self_path;
      ult::GetModulePathByName(kSelfDllName_, &dll_self_path);
      dll_path = dll_self_path + kDllName_;
    }
  } else {
    dll_path.assign(xadll);
  }
  lib_.Load(dll_path.c_str());
  if (!lib_.IsLoaded()) {
    return extractresult::init::kLoadLibraryError;
  }
  CreateObjectProc CreateObject = (CreateObjectProc)lib_.GetProc("CreateObject");
  if (CreateObject == NULL) {
    return extractresult::init::kLibraryInsideError;
  }

  if (CreateObject(&CLSID_CFormat7z, &IID_IInArchive, (void**)&archive_) != S_OK) {
    return extractresult::init::kGet7zObjectError;
  }

  //init pointer
  file_spec_ = new InFileStream;
  file_ = file_spec_;

  open_callback_spec_ = new ArchiveOpenCallBack;
  open_callback_ = open_callback_spec_;
  open_callback_spec_->password_is_defined_ = false;

  extract_callback_spec_ = new ArchiveExtractCallBack;
  extract_callback_ = extract_callback_spec_;

  return extractresult::init::kOK;
}

int Extractor::Open(const std::wstring& filename) {
  if (!file_spec_->Open(filename)) {
    return extractresult::open::kOpenFileError;
  }

  if (archive_->Open(file_, 0, open_callback_) != S_OK) {
    return extractresult::open::kOpenArchiveError;
  }
  return extractresult::open::kOK;
}

int Extractor::Extract(const std::wstring& path,
                       SetTotalProc SetTotal,
                       SetCompletedProc SetCompleted,
                       SetOperationResultProc SetOperationResult) {
    extract_callback_spec_->Init(archive_, path);
    extract_callback_spec_->SetCallback(SetTotal, SetCompleted, SetOperationResult);
    HRESULT result = archive_->Extract(NULL, (UInt32)(Int32)-1, false, extract_callback_);
    if (result != S_OK) {
      if (extract_callback_spec_->GetErrorCode() != extractresult::extract::kOK) {
        return extract_callback_spec_->GetErrorCode();
      }
      return extractresult::extract::kUnknownError;
    }
    return extractresult::extract::kOK;
}
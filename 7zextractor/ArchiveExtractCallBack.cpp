#include "StdAfx.h"
#include "ArchiveExtractCallBack.h"
#include "ult\file-dir.h"

static HRESULT IsArchiveItemProp(IInArchive* archive, UInt32 index, PROPID propID, bool* result) {
  PROPVARIANT prop;
  memset(&prop, 0, sizeof(prop));
  RINOK(archive->GetProperty(index, propID, &prop));
  if (prop.vt == VT_BOOL)
    *result = (prop.boolVal != VARIANT_FALSE);
  else if (prop.vt == VT_EMPTY)
    *result = false;
  else
    return E_FAIL;
  return S_OK;
}

static HRESULT IsArchiveItemFolder(IInArchive* archive, UInt32 index, bool* result)
{
  return IsArchiveItemProp(archive, index, kpidIsDir, result);
}

static UInt64 ConvertPropVariantToUInt64(const PROPVARIANT &prop)
{
  switch (prop.vt)
  {
  case VT_UI1: return prop.bVal;
  case VT_UI2: return prop.uiVal;
  case VT_UI4: return prop.ulVal;
  case VT_UI8: return (UInt64)prop.uhVal.QuadPart;
  default: throw 151199;
  }
}

void ArchiveExtractCallBack::Init(IInArchive* archive_handler, const std::wstring& directory_path) {
  archive_handler_ = archive_handler;
  directory_path_ = directory_path;
  ult::CanonicalizeDirPathPrefix(&directory_path_);
}

const wchar_t* ArchiveExtractCallBack::kEmptyFileAlias_ = L"[Content]";

ArchiveExtractCallBack::ArchiveExtractCallBack(void) :
    password_is_defined_(false) {
}

STDMETHODIMP ArchiveExtractCallBack::SetTotal(UInt64 size) {
  return S_OK;
}

STDMETHODIMP ArchiveExtractCallBack::SetCompleted(const UInt64* complete_value) {
  return S_OK;
}

STDMETHODIMP ArchiveExtractCallBack::GetStream(UInt32 index,
                                               ISequentialOutStream **out_stream,
                                               Int32 ask_extract_mode) {
  *out_stream = 0;
  out_filestream_.Release();

  PROPVARIANT prop;
  memset(&prop, 0, sizeof (prop));
  RINOK(archive_handler_->GetProperty(index, kpidPath, &prop));
  {
    //Get name
    std::wstring full_path;
    if (prop.vt == VT_EMPTY) {
      full_path = kEmptyFileAlias_;
    } else {
      if (prop.vt != VT_BSTR) {
        return E_FAIL;
      }
      full_path = prop.bstrVal;
    }
    file_path_ = full_path;
  }

  if (ask_extract_mode != NArchive::NExtract::NAskMode::kExtract)
    return S_OK;

  {
    //Get attrib
    memset(&prop, 0, sizeof (prop));
    RINOK(archive_handler_->GetProperty(index, kpidAttrib, &prop));
    if (prop.vt == VT_EMPTY) {
      processed_fileinfo_.attrib = 0;
      processed_fileinfo_.attrib_defined = false;
    } else {
      if (prop.vt != VT_UI4) {
        return E_FAIL;
      }
      processed_fileinfo_.attrib = prop.ulVal;
      processed_fileinfo_.attrib_defined = true;
    }
  }

  RINOK(IsArchiveItemFolder(archive_handler_, index, &processed_fileinfo_.isdir));
  
  {
    //Get modified time
    memset(&prop, 0, sizeof (prop));
    RINOK(archive_handler_->GetProperty(index, kpidMTime, &prop));
    processed_fileinfo_.mtime_defined = false;
    switch (prop.vt) {
    case VT_EMPTY:
      break;
    case VT_FILETIME:
      processed_fileinfo_.mtime = prop.filetime;
      processed_fileinfo_.mtime_defined = true;
      break;
    default:
      return E_FAIL;
    }
  }
  
  {
    //Get size
    memset(&prop, 0, sizeof (prop));
    RINOK(archive_handler_->GetProperty(index, kpidSize, &prop));
    UInt64 new_filesize;
    if (prop.vt != VT_EMPTY) {
      new_filesize = ConvertPropVariantToUInt64(prop);
    }
  }

  {
    //Create folders for file
    int pos = file_path_.rfind(L"\\");
    if (pos != std::wstring::npos) {
      ult::MakeSureFolderExist(directory_path_ + file_path_.substr(0, pos));
    }
  }

  std::wstring full_processed_path = directory_path_ + file_path_;
  disk_file_path_ = full_processed_path;
  if (processed_fileinfo_.isdir) {
    ult::MakeSureFolderExist(full_processed_path);
  } else {
    if (ult::IsPathFileExist(full_processed_path)) {
      if (!ult::DeleteFileAlways(full_processed_path)) {
        return E_ABORT;
      }
    }
    out_filestream_spec_ = new OutFileStream;
    CMyComPtr<ISequentialOutStream> out_stream_loc(out_filestream_spec_);
    if (!out_filestream_spec_->Create(full_processed_path, true)) {
      return E_ABORT;
    }
    out_filestream_ = out_stream_loc;
    *out_stream = out_stream_loc.Detach();
  }
  return S_OK;
}

STDMETHODIMP ArchiveExtractCallBack::PrepareOperation(Int32 ask_extract_mode) {
  extract_mode_ = false;
  switch (ask_extract_mode) {
  case NArchive::NExtract::NAskMode::kExtract:
    extract_mode_ = true;
    break;
  }
  return S_OK;
}

STDMETHODIMP ArchiveExtractCallBack::SetOperationResult(Int32 operation_result) {
  if (out_filestream_ != NULL) {
    if (processed_fileinfo_.mtime_defined) {
      //set modified time back, otherwise there were be currenttime
      out_filestream_spec_->SetMTime(&processed_fileinfo_.mtime);
    }
    RINOK(out_filestream_spec_->Close());
  }
  out_filestream_.Release();
  if (extract_mode_ && processed_fileinfo_.attrib_defined) {
    ult::SetFileAttributes(disk_file_path_, processed_fileinfo_.attrib);
  }
  return S_OK;
}

STDMETHODIMP ArchiveExtractCallBack::CryptoGetTextPassword(BSTR* password) {
  if (!password_is_defined_) {
    return E_ABORT;
  }
  return StringToBstr(password_.c_str(), password);
}

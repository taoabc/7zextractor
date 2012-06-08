#include "stdafx.h"
#include "7zFileStream.h"

HRESULT ConvertBoolToHRESULT(bool result) {
  if (result) {
    return S_OK;
  }
  DWORD lasterror = ::GetLastError();
  if (0 == lasterror) {
    return E_FAIL;
  }
  return HRESULT_FROM_WIN32(lasterror);
}

bool InFileStream::Open(const std::wstring& filename) {
  return file_.Open(filename);
}

STDMETHODIMP InFileStream::Read(void* data, UInt32 size, UInt32* processed_size) {
  DWORD real_processed;
  bool result = file_.ReadPart(data, size, &real_processed);
  if (processed_size != NULL) {
    *processed_size = real_processed;
  }
  return ConvertBoolToHRESULT(result);
}

STDMETHODIMP InFileStream::Seek(Int64 offset, UInt32 seek_origin, UInt64* new_position) {
  if (seek_origin > 3) {
    return STG_E_INVALIDFUNCTION;
  }
  unsigned __int64 real_new_position;
  bool result = file_.Seek(offset, &real_new_position, seek_origin);
  if (new_position != NULL) {
    *new_position = real_new_position;
  }
  return ConvertBoolToHRESULT(result);
}

STDMETHODIMP InFileStream::GetSize(UInt64 *size) {
  *size = file_.GetSize();
  return S_OK;
}

bool OutFileStream::Create(const std::wstring& filename, bool create_always) {
  return file_.Create(filename, create_always);
}

STDMETHODIMP OutFileStream::Write(const void*data, UInt32 size, UInt32* processed_size) {
  DWORD real_processed;
  bool result = file_.WritePart(data, size, &real_processed);
  if (processed_size != NULL) {
    *processed_size = real_processed;
  }
  return ConvertBoolToHRESULT(result);
}

STDMETHODIMP OutFileStream::Seek(Int64 offset, UInt32 seek_origin, UInt64* new_position) {
  if (seek_origin > 3) {
    return STG_E_INVALIDFUNCTION;
  }
  unsigned __int64 real_new_position;
  bool result = file_.Seek(offset, &real_new_position, seek_origin);
  if (new_position != NULL) {
    *new_position = real_new_position;
  }
  return ConvertBoolToHRESULT(result);
}

STDMETHODIMP OutFileStream::SetSize(UInt64 new_size) {
  unsigned __int64 current_pos;
  if (!file_.Seek(0, &current_pos, FILE_CURRENT)) {
    return E_FAIL;
  }
  bool result = file_.SetSize(new_size);
  unsigned __int64 current_pos2;
  result = result && file_.Seek(current_pos, &current_pos2);
  return result ? S_OK : E_FAIL;
}

bool OutFileStream::SetMTime( const FILETIME* mtime )
{
  return file_.SetFileTime(NULL, NULL, mtime);
}

HRESULT OutFileStream::Close( void )
{
  return ConvertBoolToHRESULT(file_.Close());
}

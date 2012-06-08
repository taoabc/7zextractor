#include "StdAfx.h"
#include "ArchiveOpenCallBack.h"

ArchiveOpenCallBack::ArchiveOpenCallBack(void) :
    password_is_defined_(false) {
}

STDMETHODIMP ArchiveOpenCallBack::SetTotal(const UInt64* files, const UInt64* bytes) {
  return S_OK;
}

STDMETHODIMP ArchiveOpenCallBack::SetCompleted(const UInt64* files, const UInt64* bytes) {
  return S_OK;
}

STDMETHODIMP ArchiveOpenCallBack::CryptoGetTextPassword(BSTR* password) {
  if (!password_is_defined_) {
    return E_ABORT;
  }
  return StringToBstr(password_.c_str(), password);
}
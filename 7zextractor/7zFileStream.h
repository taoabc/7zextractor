#ifndef SEVENZIP_FILE_STREAM_H_
#define SEVENZIP_FILE_STREAM_H_

#include "7z/cpp/7zip/IStream.h"
#include "7z/cpp/common/MyCom.h"

#include "ult\file-io.h"

#include <string>

class InFileStream :
    public IInStream,
    public IStreamGetSize,
    public CMyUnknownImp {

public:

  bool Open(const std::wstring& filename);

  MY_UNKNOWN_IMP2(IInStream, IStreamGetSize)

  //IInStream
  STDMETHOD(Read)(void *data, UInt32 size, UInt32 *processed_size);
  STDMETHOD(Seek)(Int64 offset, UInt32 seek_origin, UInt64 *new_position);

  //IStreamGetSize
  STDMETHOD(GetSize)(UInt64 *size);

private:

  ult::File file_;

};

class OutFileStream :
    public IOutStream,
    public CMyUnknownImp {

public:

  bool Create(const std::wstring& filename, bool create_always);
  bool SetMTime(const FILETIME* mtime);
  HRESULT Close(void);

  MY_UNKNOWN_IMP1(IOutStream)

  STDMETHOD(Write)(const void*data, UInt32 size, UInt32* processed_size);
  STDMETHOD(Seek)(Int64 offset, UInt32 seek_origin, UInt64* new_position);
  STDMETHOD(SetSize)(UInt64 new_size);

private:
  
  ult::File file_;

};

#endif
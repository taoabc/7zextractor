/*
** 提供文件及磁盘操作
** author
**   taoabc@gmail.com
*/
#ifndef ULT_FILE_DIR_H_
#define ULT_FILE_DIR_H_

#include <string>
#include <windows.h>
#include <shlobj.h>

namespace ult {
namespace filedir {

inline void SplitToPureNameAnddExtension(const std::wstring& fullname,
                                         std::wstring* purename,
                                         std::wstring* extension) {
  int pos = fullname.rfind(L".");
  if (pos == std::wstring::npos) {
    purename->assign(fullname);
    extension->clear();
  } else {
    purename->assign(fullname.substr(0, pos));
    extension->assign(fullname.substr(pos+1));
  }
}

inline void SplitToPathPrefixAndFileName(const std::wstring& fullpath,
                                         const std::wstring& pathseparator,
                                         std::wstring* pathprefix,
                                         std::wstring* filename) {
  int separator_len = pathseparator.length();
  int pos = fullpath.rfind(pathseparator);
  if (pos == std::wstring::npos) {
    pathprefix->clear();
    filename->assign(fullpath);
  } else {
    pathprefix->assign(fullpath.substr(0, pos+separator_len));
    filename->assign(fullpath.substr(pos+separator_len));
  }
}

inline void CanonicalizeDirPathPrefix(std::wstring* dirpath) {
  if (dirpath->empty()) {
    return;
  }
  if (dirpath->rfind(L"\\") != dirpath->length()-1) {
    dirpath->append(L"\\");
  }
}

inline void GetMaxFreeSpaceDrive(std::wstring* drive,
                                 unsigned __int64* freesize) {
  DWORD buf_len = ::GetLogicalDriveStrings(0, NULL);
  wchar_t* buf = new wchar_t [buf_len];

  ULARGE_INTEGER freespace;
  ULONGLONG maxfree = 0;

  if (0 != ::GetLogicalDriveStrings(buf_len, buf)) {
    wchar_t* drive_tmp = buf;
    DWORD i = 0;
    while (i <= buf_len) {
      if (DRIVE_FIXED == ::GetDriveType(drive_tmp)) {
        ::GetDiskFreeSpaceEx(drive_tmp, &freespace, NULL, NULL);
        ULONGLONG t = freespace.QuadPart;
        if (t > maxfree) {
          maxfree = t;
          drive->assign(drive_tmp);
        }
      }
      i += static_cast<DWORD>(wcslen(drive_tmp)) + 1;
      drive_tmp += i;
    }
  }
  delete[] buf;
  *freesize = maxfree;
}

inline void GetProgramFilesDirectory(std::wstring* directory) {
  wchar_t buf[MAX_PATH];
  ::SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES | CSIDL_FLAG_CREATE, NULL, SHGFP_TYPE_CURRENT, buf);
  directory->assign(buf);
}

inline void GetAppDataDirectory(std::wstring* directory) {
  wchar_t buf[MAX_PATH];
  ::SHGetFolderPath(NULL, CSIDL_APPDATA | CSIDL_FLAG_CREATE, NULL, SHGFP_TYPE_CURRENT, buf);
  directory->assign(buf);
}

inline bool IsPathFileExist(const std::wstring& pathfile) {
  if (-1 != _waccess(pathfile.c_str(), 0)) {
    return true;
  } else {
    return false;
  }
}

inline bool GetUpperPath(const std::wstring& path, std::wstring* upper_path) {
  upper_path->clear();
  int pos = path.rfind(L'\\');
  if (pos == std::wstring::npos) {
    return false;
  }
  if (pos == (path.length()-1)) {
    return GetUpperPath(std::wstring(path.c_str(), path.length()-1), upper_path);
  }
  upper_path->assign(path.c_str(), pos+1);
  return true;
}

inline void GetSelfModulePath(std::wstring* path) {
  wchar_t buf[MAX_PATH];
  ::GetModuleFileName(NULL, buf, MAX_PATH);
  GetUpperPath(buf, path);
}

inline bool MakeSureFolderExist(const std::wstring& folder_path) {
  int index = 0;
  bool ret = false;
  std::wstring normalize_path(folder_path);
  CanonicalizeDirPathPrefix(&normalize_path);
  while ((index = normalize_path.find(L'\\', index)) != std::string::npos) {
    index++;
    std::wstring path = normalize_path.substr(0, index);
    if (0 == CreateDirectory(path.c_str(), NULL)) {
      if (GetLastError() != ERROR_ALREADY_EXISTS) {
        ret = true;
      } else {
        ret = false;
      }
    } else {
      ret = true;
    }
  }
  return ret;
}

inline bool DeleteFileAlways(const std::wstring& filename) {
  if (::SetFileAttributes(filename.c_str(), 0) == 0) {
    return false;
  }
  if (::DeleteFile(filename.c_str()) == 0) {
    return false;
  }
  return true;
}

inline bool SetFileAttributes(const std::wstring& filename, DWORD file_attributes) {
  return (::SetFileAttributes(filename.c_str(), file_attributes) != 0);
}

} //namespace filedir

using namespace filedir;

} //namespace ult

#endif // ULT_FILE_DIR_H_
#ifndef ULT_LIBRARY_H_
#define ULT_LIBRARY_H_

#include <windows.h>

namespace ult {
namespace library {

class Library {

public:

  Library(void) : module_(NULL) {
  };

  ~Library(void) {
    Free();
  }

  operator HMODULE() const {
    return module_;
  }

  HMODULE* operator&() {
    return &module_;
  }

  bool IsLoaded(void) const {
    return (NULL != module_);
  }

  void Attach(HMODULE m) {
    Free();
    module_ = m;
  }

  HMODULE Detach(void) {
    HMODULE m = module_;
    module_ = NULL;
    return m;
  }

  bool Free(void) {
    if (NULL == module_) {
      return true;
    }
    if (!::FreeLibrary(module_)) {
      return false;
    }
    module_ = NULL;
    return true;
  }

  bool Load(const wchar_t* filename) {
    return LoadOperations(::LoadLibrary(filename));
  }

  bool LoadEx(const wchar_t* filename, DWORD flags) {
    return LoadOperations(::LoadLibraryEx(filename, NULL, flags));
  }

  FARPROC GetProc(const char* procname) const {
    return ::GetProcAddress(module_, procname);
  }

private:

  bool LoadOperations(HMODULE new_module) {
    if (NULL == new_module) {
      return false;
    }
    if (!Free()) {
      return false;
    }
    module_ = new_module;
    return true;
  }

  HMODULE module_;

};
} //namespace library

using namespace library;

} //namespace ult

#endif
// 7zextractor.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include "Extractor.h"

Extractor g_extractor_spec;

int __stdcall Init(const wchar_t* xadll) {
  return g_extractor_spec.Init(xadll);
}

int __stdcall Open(const wchar_t* filename) {
  return g_extractor_spec.Open(filename);
}

int __stdcall Extract(const wchar_t* path,
                      SetTotalProc SetTotal,
                      SetCompletedProc SetComplete,
                      SetOperationResultProc SetOperationResult) {
  return g_extractor_spec.Extract(path, SetTotal, SetComplete, SetOperationResult);
}
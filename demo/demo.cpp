// demo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "ult\library.h"
#include "ult\file-dir.h"

#include "../7zextractor/common.h"
#include "ult\number.h"

unsigned __int64 g_totalsize;

void __stdcall SetTotal(unsigned __int64 totalsize) {
  g_totalsize = totalsize;
}
void __stdcall SetCompleted(unsigned __int64 completesize) {
  int progress = ult::UIntMultDiv(completesize, 100, g_totalsize);
  printf("%d%%\n", progress);
}
void __stdcall SetOperationResult(int operation_result) {
  printf("%d\n", operation_result);
}

int _tmain(int argc, _TCHAR* argv[])
{
  std::wstring self_path;
  ult::GetSelfModulePath(&self_path);
  std::wstring dll_path = self_path + L"7zextractor.dll";
  ult::Library lib;
  lib.Load(dll_path.c_str());
  if (!lib.IsLoaded()) {
    return 1;
  }

  InitProc Init = (InitProc)lib.GetProc("Init");
  OpenProc Open = (OpenProc)lib.GetProc("Open");
  ExtractProc Extract = (ExtractProc)lib.GetProc("Extract");

  if (Init(NULL) != extractresult::init::kOK) {
  }
  if (Open(L"E:\\temp\\test.7z") != extractresult::open::kOK) {
  }
  if (Extract(L"E:\\temp\\a\\", SetTotal, SetCompleted, SetOperationResult) != extractresult::extract::kOK) {
  }
  system("pause");
	return 0;
}
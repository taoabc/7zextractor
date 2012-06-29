// demo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "resource.h"
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
  //printf("%d\n", operation_result);
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

  ult::sevenzip::InitProc Init = (ult::sevenzip::InitProc)lib.GetProc("Init");
  ult::sevenzip::OpenProc Open = (ult::sevenzip::OpenProc)lib.GetProc("Open");
  ult::sevenzip::OpenFromMemoryProc OpenFromMemory = (ult::sevenzip::OpenFromMemoryProc)lib.GetProc("OpenFromMemory");
  ult::sevenzip::ExtractProc Extract = (ult::sevenzip::ExtractProc)lib.GetProc("Extract");

  if (Init(NULL) != ult::sevenzip::extractresult::init::kOK) {
  }
  /*if (Open(L"E:\\temp\\test.7z") != extractresult::open::kOK) {
  }*/
  HRSRC hrsrc = ::FindResource(NULL, MAKEINTRESOURCE(IDR_GAMEBOX_PACK1), L"GAMEBOX_PACK");
  HGLOBAL hpack = ::LoadResource(NULL, hrsrc);
  LPVOID pack_data = ::LockResource(hpack);
  DWORD pack_size = ::SizeofResource(NULL, hrsrc);
  if (OpenFromMemory((const char*)pack_data, pack_size) != ult::sevenzip::extractresult::open::kOK) {

  }
  if (Extract(L"E:\\temp\\a\\", SetTotal, SetCompleted, SetOperationResult) != ult::sevenzip::extractresult::extract::kOK) {
  }
  ::FreeResource(hpack);
  system("pause");
	return 0;
}
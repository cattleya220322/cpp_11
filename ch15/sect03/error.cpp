/// @file error.cpp
/// @brief エラーハンドラの実装

#include "error.h"
#include <iostream>

int Error::number_of_errors{0};
double Error::Error(std::string const &s)
{
  number_of_errors++;
  std::cerr << "error: " << s << '\n';

  // ここでのエラーの殆どが、式の評価中に発生
  // 以降のエラーを発生させないよう値を返す
  return 1;
}
/// @file error.h
/// @brief エラーハンドラのインターフェイス

#ifndef DESKCALCULATOR_ERROR_H_
#define DESKCALCULATOR_ERROR_H_

#include <string>

/// @brief エラーハンドラモジュール
namespace Error
{
  /// @brief エラーの回数
  extern int number_of_errors;
  /// @brief エラー回数をカウント、エラーメッセージを出力
  /// @param s 出力エラーメッセージ
  /// @return 以降のエラーを発生させないよう値を返す
  double Error(std::string const &s);
}

#endif // DESKCALCULATOR_ERROR_H_

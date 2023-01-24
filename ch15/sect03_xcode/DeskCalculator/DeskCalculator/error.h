/// @file error.h
/// @brief エラーハンドラのインターフェイス

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

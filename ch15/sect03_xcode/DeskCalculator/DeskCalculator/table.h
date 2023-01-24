/// @file table.h
/// @brief シンボルテーブルのインターフェイス

#include <map>
#include <string>

/// @brief シンボルテーブルモジュール
namespace Table
{
  /// @brief シンボルテーブル（定義済名を利用可能にする）
  extern std::map<std::string, double> table;
}

/// @file table.h
/// @brief シンボルテーブルのインターフェイス

#ifndef DESKCALCULATOR_TABLE_H_
#define DESKCALCULATOR_TABLE_H_

#include <map>
#include <string>

/// @brief シンボルテーブルモジュール
namespace Table
{
  /// @brief シンボルテーブル（定義済名を利用可能にする）
  extern std::map<std::string, double> table;
}

#endif // DESKCALCULATOR_TABLE_H_

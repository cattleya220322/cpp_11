/// @file parser.h
/// @brief パーサ（構文解析器）のユーザに提供する外部インターフェイス（ユーザ用インターフェイス）

#ifndef DESKCALCULATOR_PARSER_H_
#define DESKCALCULATOR_PARSER_H_

/// @brief パーサモジュール
namespace Parser
{
  /// @brief 式：加算と減算
  /// @param get 次のトークンを取り出す TokenStream::Get() を呼び出す必要の有無
  /// @return 自分自身の式を評価して、その値を返す（Exprは加算と減算を処理）
  double Expr(bool get);
}

#endif // DESKCALCULATOR_PARSER_H_

/// @file parser_impl.h
/// @brief パーサ（構文解析器）を実装する関数用の共通環境（実装者用インターフェイス）

#include "parser.h"
#include "lexer.h"
#include "error.h"

namespace Parser
{
  /// @brief 一次式を処理する parser（構文解析器）最下層
  /// @param get 次のトークンを取り出すTokenStream::Get()を呼び出す必要の有無
  /// @return 一次式の結果
  double Prim(bool get);
  /// @brief
  /// 終端：乗算と除算（非終端記号（置換され得る記号）を認識する構文解析器 (parser) ）
  /// @param get 次のトークンを取り出すTokenStream::Get()を呼び出す必要の有無
  /// @return 自分自身の式を評価して、その値を返す（Termは乗算と除算を処理）
  double Term(bool get);
  double Expr(bool get);
}
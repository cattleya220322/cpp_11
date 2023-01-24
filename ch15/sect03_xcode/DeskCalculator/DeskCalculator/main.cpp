/// @file main.cpp
/// @brief エントリポイント & ドライバ

#include "parser.h"
#include "lexer.h" // ts を利用可能にする
#include "table.h" // 定義済名を利用可能にする
#include "error.h"
#include <sstream> // main() の引数を文字列ストリームに置けるようにする

/// @brief ドライバモジュール
namespace Driver
{
  void Calculate()
  {
    for (;;)
    {
      Lexer::ts.Get();
      // ts.Get() が入力エラーやファイル終端に出会った際に、ループ終了
      if (Lexer::ts.Current().kind_ == Lexer::Kind::kEnd)
        break;
      // Expr() は空の式を処理しない
      if (Lexer::ts.Current().kind_ == Lexer::Kind::kPrint)
        continue;
      // ts.Get() を呼び出して処理対象トークンを読み取る必要がない
      std::cout << Parser::Expr(false) << '\n';
    }
  }
}

/// @brief エントリポイント
/// @param argc コマンドライン引数の個数（実行ファイルの後ろに一つのみ文字列を渡せる）
/// @param argv コマンドライン引数の文字列へのポインタ（e.g. "val=9.5;val*2;val/5;3*(val+1.0)"）
/// @return 異常終了であれば Non Zero
auto main(int argc, char *argv[]) -> int
{
  // 予約語
  Table::table["pi"] = 3.1415926535897932385;
  Table::table["e"] = 2.7182818284590452354;

  // コマンドライン引数対応
  switch (argc)
  {
  case 1:
    break;
  case 2:
    Lexer::ts.SetInput(new std::istringstream{argv[1]});
    break;
  default:
    Error::Error("too many arguments");
    return 1;
  }

  Driver::Calculate();

  // 異常終了: Non Zero を返す
  return Error::number_of_errors;
}
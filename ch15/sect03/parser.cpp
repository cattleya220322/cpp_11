/// @file parser.cpp
/// @brief パーサの実装

#include "parser_impl.h"
#include "table.h"

double Parser::Prim(bool get)
{
  if (get)
    Lexer::ts.Get(); // 次のトークンを取り出す
  switch (Lexer::ts.Current().kind_)
  {
  case Lexer::Kind::kNumber:
  {
    double v = Lexer::ts.Current().number_value_;
    Lexer::ts.Get();
    return v;
  }
  case Lexer::Kind::kName:
  {
    double &v = Table::table[Lexer::ts.Current().string_value_];
    if (Lexer::ts.Get().kind_ == Lexer::Kind::kAssign)
      v = Expr(true); // 次のトークンが = の場合：代入
    return v;
  }
  case Lexer::Kind::kMinus:
    return -Prim(true);
  case Lexer::Kind::kLp:
  {
    auto e = Expr(true);
    if (Lexer::ts.Current().kind_ != Lexer::Kind::kRp)
      return Error::Error("')' expected.");
    Lexer::ts.Get(); // ')'を読み飛ばす
    return e;
  }
  default:
    return Error::Error("primary expected.");
  }
}

double Parser::Term(bool get)
{
  double left = Prim(get);
  while (true)
  {
    switch (Lexer::ts.Current().kind_)
    {
    case Lexer::Kind::kMul:
      left *= Prim(true);
      break;
    case Lexer::Kind::kDiv:
      if (auto d = Prim(true))
      { // ここ Non Zero を判定している
        left /= d;
        break;
      }
      return Error::Error("divide by 0");
    default:
      return left;
    }
  }
}

double Parser::Expr(bool get)
{
  double left = Term(get);
  for (;;)
  {
    switch (Lexer::ts.Current().kind_)
    {
    case Lexer::Kind::kPlus:
      left += Term(true);
      break;
    case Lexer::Kind::kMinus:
      left -= Term(true);
      break;
    default:
      return left;
    }
  }
}

/// @file lexer.cpp
/// @brief レクサの実装

#include "lexer.h"
#include "error.h"
#include <cctype>

Lexer::TokenStream Lexer::ts{std::cin}; // デフォルトで cin から読み取る

Lexer::TokenStream::TokenStream(std::istream &s) : p_ist_{&s}, owns_{false}, ct_{Kind::kEnd} {}
Lexer::TokenStream::TokenStream(std::istream *p) : p_ist_{p}, owns_{true}, ct_{Kind::kEnd} {}
Lexer::TokenStream::~TokenStream() { Close(); }
Lexer::Token Lexer::TokenStream::Get()
{
  char ch{0};
  // セミコロンと同じように、改行に対して式の終了を表す意味を与える
  do
  {                       // '\n' 以外の空白をスキップ
    if (!p_ist_->get(ch)) // cin から文字を読み取れなかった場合
      return ct_ = {Kind::kEnd};
  } while (ch != '\n' && std::isspace(ch));

  switch (ch)
  {
  case '\n':
  case ';':
    return ct_ = {Kind::kEnd};
  case '*':
  case '/':
  case '+':
  case '-':
  case '(':
  case ')':
  case '=':
    return ct_ = {static_cast<Kind>(ch)};
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
  case '.':
    p_ist_->putback(ch);          // 最初の数字（または . ）を入力ストリームに戻す
    *p_ist_ >> ct_.number_value_; // 数値を ct_ に読み込む
    ct_.kind_ = Kind::kNumber;
    return ct_;
  default: // 名前｜名前 =｜エラー
    if (std::isalpha(ch))
    {
      // >> を利用しているために空白なしでは文字列を区切れない問題に対して、
      // アルファベット文字か数字以外の文字に出会うまで文字を１個ずつ読み取ることで解決
      ct_.string_value_ = ch;
      while (p_ist_->get(ch))
        if (std::isalnum(ch))
          ct_.string_value_ += ch; // string_value_ の後ろに ch を連結
        else
        {
          p_ist_->putback(ch); // 文字を入力ストリーム先頭に戻す
          break;
        }
      ct_.kind_ = Kind::kName;
      return ct_;
    }
    Error::Error("bad token");
    return ct_ = {Kind::kPrint};
  }
}
Lexer::Token const &Lexer::TokenStream::Current() { return ct_; }
void Lexer::TokenStream::SetInput(std::istream &s)
{
  Close();
  p_ist_ = &s;
  owns_ = false;
}
void Lexer::TokenStream::SetInput(std::istream *p)
{
  Close();
  p_ist_ = p;
  owns_ = true;
}
void Lexer::TokenStream::Close()
{
  if (owns_)
  {
    delete p_ist_;
    p_ist_ = nullptr;
  }
}
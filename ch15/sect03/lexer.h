/// @file lexer.h
/// @brief レクサ（字句解析器）のインターフェイス

#include <string>
#include <iostream>

/// @brief レクサ（字句解析器）モジュール
namespace Lexer
{
  /// @brief トークン種別
  enum class Kind : char
  {
    /// @brief 名前（何らかの定義）
    kName,
    /// @brief 整数リテラル or 浮動小数点数リテラル
    kNumber,
    /// @brief 計算処理を終了させる
    kEnd,
    kPlus = '+',
    kMinus = '-',
    kMul = '*',
    kDiv = '/',
    kPrint = ';',
    kAssign = '=',
    kLp = '(',
    kRp = ')'
  };

  /// @brief トークン種別と値のペア
  struct Token
  {
    /// @brief トークン種別
    Kind kind_;
    /// @brief トークン種別が kName であれば、その文字列は string_value_ に置かれる
    std::string string_value_;
    /// @brief トークン種別が kNumber であれば、その値が number_value_ に置かれる
    double number_value_{0};
  };

  /// @brief 123.45 のような文字ストリームを {kNumber, 123.45} のようなトークンに変換する
  class TokenStream
  {
  public:
    /// @brief Constructor (overload)
    /// @param s 文字を読み取る対象である入力ストリーム
    TokenStream(std::istream &s);
    /// @brief Constructor (overload)
    /// @param p ポインタとして渡された istream
    TokenStream(std::istream *p);
    /// @brief Destructor
    ~TokenStream();
    /// @brief 次のトークンを読み取る
    /// @return 次のトークン
    Token Get();
    /// @brief 最後に読み込んだトークンを返す
    /// @return 最後に読み込んだトークン
    Token const &Current();
    /// @brief ?
    /// @param s ?
    void SetInput(std::istream &s);
    /// @brief コマンドライン引数で式を渡す時に使う
    /// @param p 式の文字列ストリーム
    void SetInput(std::istream *p);

  private:
    /// @brief istream を所有している場合、これを破棄
    void Close();
    /// @brief 入力ストリームへのポインタ
    std::istream *p_ist_;
    /// @brief 入力ストリームの所有権を表す論理値
    bool owns_;
    /// @brief カレントトークン
    Token ct_;
  };

  extern TokenStream ts;
}
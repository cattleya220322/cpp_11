#include <cctype>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

// recursive descent parser

/// @brief
/// トークン（ソースコードに出現する文字列の中で意味を持つ最小単位）の種別(Kind)
enum class Kind : char {
  name,
  /// @brief 浮動小数点定数
  number,
  end,
  plus = '+',
  minus = '-',
  mul = '*',
  div = '/',
  print = ';',
  assign = '=',
  lp = '(',
  rp = ')'
};

/// @brief トークン種別と値のペア
struct Token {
  Kind kind_;
  std::string string_value_;
  double number_value_;
};

/// @brief トークン化を行う
class TokenStream {
 public:
  /// @brief constructor (overload)
  /// @param s 文字を読み取る対象である入力ストリーム
  TokenStream(std::istream& s) : ip_{&s}, owns_{false} {}
  /// @brief constructor (overload)
  /// @param p ポインタとして渡されたistream
  TokenStream(std::istream* p) : ip_{p}, owns_{true} {}
  /// @brief destructor
  ~TokenStream() { Close(); }
  /// @brief 次のトークンを読み取って返却
  /// @return 次のトークン
  Token Get();
  /// @brief 最後に読み込んだトークン
  /// @return 最後に読み込んだトークン
  const Token& Current() { return ct_; }

  void SetInput(std::istream& s) {
    Close();
    ip_ = &s;
    owns_ = false;
  }
  void SetInput(std::istream* p) {
    Close();
    ip_ = p;
    owns_ = true;
  }

 private:
  void Close() {
    if (owns_) {
      delete ip_;
      ip_ = nullptr;
    }
  }
  /// @brief 入力ストリームへのポインタ
  std::istream* ip_{nullptr};
  /// @brief TokenStreamはistreamを所有しているか
  bool owns_;
  /// @brief カレントトークン
  Token ct_{Kind::end};
};

TokenStream ts{std::cin};

/// @brief
/// 終端：乗算と除算（非終端記号（置換され得る記号）を認識する構文解析器(parser)）
/// @param get
/// 次のトークンを取り出すTokenStream::Get()を呼び出す必要の有無を示す
/// @return 自分自身の式を評価して、その値を返す（Termは乗算と除算を処理）
double Term(bool get);

/// @brief 式：加算と減算
/// @param get
/// 次のトークンを取り出すTokenStream::Get()を呼び出す必要の有無を示す
/// @return 自分自身の式を評価して、その値を返す（Exprは加算と減算を処理）
double Expr(bool get) {
  double left = Term(get);

  while (true) {
    switch (ts.Current().kind_) {
      case Kind::plus:
        left += Term(true);
        break;
      case Kind::minus:
        left -= Term(true);
        break;
      default:
        return left;
    }
  }
}

/// @brief 一次式を処理するparser（構文解析器）最下層
/// @param get
/// 次のトークンを取り出すTokenStream::Get()を呼び出す必要の有無を示す
/// @return 一次式の結果
double Prim(bool get);

/// @brief エラー回数をカウント、エラーメッセージを出力
/// @param s 出力エラーメッセージ
/// @return 以降のエラーを発生させないよう値を返す
double Error(std::string const& s);

double Term(bool get) {
  double left = Prim(get);
  while (true) {
    switch (ts.Current().kind_) {
      case Kind::mul:
        left *= Prim(true);
        break;
      case Kind::div:
        if (auto d = Prim(true)) {  // ここ Non Zero を判定している
          left /= d;
          break;
        }
        return Error("divide by 0");
      default:
        return left;
    }
  }
}

/// @brief シンボルテーブル
std::map<std::string, double> table;

double Prim(bool get) {
  if (get) ts.Get();  // 次のトークンを取り出す
  switch (ts.Current().kind_) {
    case Kind::number: {
      double v = ts.Current().number_value_;
      ts.Get();
      return v;
    }
    case Kind::name: {
      double& v = table[ts.Current().string_value_];
      if (ts.Get().kind_ == Kind::assign)
        v = Expr(true);  // 次のトークンが=の場合：代入
      return v;
    }
    case Kind::minus:
      return -Prim(true);
    case Kind::lp: {
      auto e = Expr(true);
      if (ts.Current().kind_ != Kind::rp) return Error("')' expected.");
      ts.Get();  // ')'を読み飛ばす
      return e;
    }
    default:
      return Error("primary expected.");
  }
}

Token TokenStream::Get() {
  char ch{0};

  // *ip_ >> ch;

  // セミコロンと同じように、改行に対して式の終了を表す意味を与える
  do {                  // '\n' 以外の空白をスキップ
    if (!ip_->get(ch))  // cin から文字を読み取れなかった場合
      return ct_ = {Kind::end};
  } while (ch != '\n' && std::isspace(ch));

  switch (ch) {
    case '\n':
    case ';':
      return ct_ = {Kind::end};
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
      ip_->putback(ch);  // 最初の数字（または . ）を入力ストリームに戻す
      *ip_ >> ct_.number_value_;  // 数値を ct_ に読み込む
      ct_.kind_ = Kind::number;
      return ct_;
    default:  // 名前｜名前 =｜エラー
      if (std::isalpha(ch)) {
        // >> を利用しているために空白なしでは文字列を区切れない問題に対して、
        // アルファベット文字か数字以外の文字に出会うまで文字を１個ずつ読み取ることで解決
        ct_.string_value_ = ch;
        while (ip_->get(ch))
          if (std::isalnum(ch))
            ct_.string_value_ += ch;  // string_value_ の後ろに ch を連結
          else {
            ip_->putback(ch);  // 文字を入力ストリーム先頭に戻す
            break;
          }

        // ip_->putback(ch);  // 最初の文字を入力ストリームに戻す
        // *ip_ >> ct_.string_value_;  // 文字列を ct_ に読み込む

        ct_.kind_ = Kind::name;
        return ct_;
      }
      Error("bad token");
      return ct_ = {Kind::print};
  }
}

int no_of_errors{0};

double Error(std::string const& s) {
  no_of_errors++;
  std::cerr << "error: " << s << '\n';
  // ここでのエラーの殆どが、式の評価中に発生
  // 以降のエラーを発生させないよう値を返す
  return 1;
}

void Calculate() {
  for (;;) {
    ts.Get();
    // ts.Get() が入力エラーやファイル終端に出会った際に、ループ終了
    if (ts.Current().kind_ == Kind::end) break;
    // Expr() は空の式を処理しない
    if (ts.Current().kind_ == Kind::print) continue;
    // ts.Get() を呼び出して処理対象トークンを読み取る必要がない
    std::cout << Expr(false) << '\n';
  }
}

auto main(int argc, char* argv[]) -> int {
  table["pi"] = 3.1415926535897932385;
  table["e"] = 2.7182818284590452354;

  // コマンドライン引数対応
  switch (argc) {
    case 1:
      break;
    case 2:
      ts.SetInput(new std::istringstream{argv[1]});
      break;
    default:
      Error("too many arguments");
      return 1;
  }

  Calculate();
  // 以上終了: Non Zero を返す
  return no_of_errors;
}
//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// Node.cpp -- concrete node types implementation
//
//------------------------------------------------------------------------------

module node;

import<iostream>;
import<stdexcept>;
import<string>;
import<vector>;

import symtab;

// this import required because GlobalTable have unordered_map in ctor
import<unordered_map>;

Symtab GlobalTable;
Node::IScope *CurrentScope = nullptr;

namespace Node {

// Integer value
class Value final : public INode {
  int Val_;

  // INode interface
public:
  int calc() override { return Val_; }
  void dump() const override {
    std::cout << "Node Value: " << Val_ << std::endl;
  }

public:
  Value(int V) : Val_(V) {}
};

// Declaration
class Decl final : public INode {
  int Val_;
  std::string Varname_;

  // INode interface
public:
  Decl(const std::string &Name) : Varname_(Name) {}
  int calc() override { return Val_; }
  void dump() const override {
    std::cout << "Node Decl: " << Val_ << std::endl;
  }

public:
  Decl() = default;
  void SetValue(int Val) { Val_ = Val; }
};

// Scope
class Scope final : public IScope {
  std::vector<INode *> Branches_;
  IScope *PrevScope_;

  // INode interface
public:
  int calc() override {
    for (auto *B : Branches_)
      B->calc();
    return 0;
  }

  void dump() const override {
    std::cout << "Node Scope: " << std::endl;
    for (const auto *X : Branches_)
      X->dump();
  }

  // IScope interface
public:
  IScope *push() override { return new Scope(this); }
  IScope *resetScope() const override { return PrevScope_; }
  void addBranch(INode *Branch) override { Branches_.push_back(Branch); }
  INode *access(const std::string &VarName) override;
  INode *visible(const std::string &VarName) const override;

public:
  Scope(Scope *PrevScope) : PrevScope_(PrevScope) {}
  ~Scope();
};

// Operand
class Operand final : public INode {
  INode *Left_;
  INode *Right_;
  Operation Op_;

  // INode interface
public:
  int calc() override;
  void dump() const override;

public:
  Operand(INode *Left, Operation Op, INode *Right)
      : Left_(Left), Right_(Right), Op_(Op) {}
  ~Operand();
};

// While loop node
class While final : public INode {
  INode *Op_ = nullptr;
  INode *Scope_ = nullptr;

  // INode interface
public:
  int calc() override {
    while (Op_->calc())
      Scope_->calc();
    return 0;
  }
  void dump() const override {
    std::cout << "Node While " << std::endl;
    Scope_->dump();
  }

public:
  While(INode *Op, INode *Scope) : Op_(Op), Scope_(Scope) {}
  ~While() {
    // we can not delete decl here
    if (Op_ && typeid(Decl) != typeid(*Op_))
      delete Op_;
    delete Scope_;
  }
};

// If node
class If final : public INode {
  INode *Op_;
  INode *Scope_;

  // INode interface
public:
  int calc() override {
    if (Op_->calc())
      Scope_->calc();
    return 0;
  }
  void dump() const override {
    std::cout << "Node If " << std::endl;
    Scope_->dump();
  }

public:
  If(INode *Op, INode *Scope) : Op_(Op), Scope_(Scope) {}
  ~If() {
    if (Op_ && typeid(Decl) != typeid(*Op_))
      delete Op_;
    delete Scope_;
  }
};

} // namespace Node

Node::INode *makeValue(int V) { return new Node::Value{V}; }
Node::INode *makeOperand(Node::INode *Lhs, Node::Operation Op,
                         Node::INode *Rhs) {
  return new Node::Operand{Lhs, Op, Rhs};
}
Node::INode *makeWhile(Node::INode *Op, Node::INode *Scope) {
  return new Node::While{Op, Scope};
}
Node::INode *makeIf(Node::INode *Op, Node::INode *Scope) {
  return new Node::If{Op, Scope};
}
Node::IScope *createScope() { return new Node::Scope{nullptr}; }

namespace Node {

// SCOPE

INode *Scope::access(std::string const &VarName) {
  INode *Existing = visible(VarName);
  if (Existing)
    return Existing;
  INode *NewDecl = new Decl{VarName};
  GlobalTable.add(this, VarName, NewDecl);
  return NewDecl;
}

INode *Scope::visible(std::string const &VarName) const {
  INode *F = GlobalTable.exists(this, VarName);
  if (F)
    return F;
  if (PrevScope_)
    F = PrevScope_->visible(VarName);
  return F;
}

Scope::~Scope() {
  for (auto *B : Branches_)
    delete B;

  GlobalTable.free(this);
}

int Operand::calc() {
  int Val = 0;
  switch (Op_) {
  case Operation::Plus:
    if (Left_ == nullptr)
      return Right_->calc();
    return Left_->calc() + Right_->calc();
  case Operation::Minus:
    if (Left_ == nullptr)
      return -Right_->calc();
    return Left_->calc() - Right_->calc();
  case Operation::Greater:
    return Left_->calc() > Right_->calc();
  case Operation::Less:
    return Left_->calc() < Right_->calc();
  case Operation::GreaterEq:
    return Left_->calc() >= Right_->calc();
  case Operation::LessEq:
    return Left_->calc() <= Right_->calc();
  case Operation::Assign:
    Val = Right_->calc();
    static_cast<Decl *>(Left_)->SetValue(Val);
    return Val;
  case Operation::StdOut:
    std::cout << Right_->calc() << std::endl;
    break;
  case Operation::StdIn:
    std::cin >> Val;
    return Val;
  case Operation::Equal:
    return (Left_->calc() == Right_->calc());
  case Operation::NotEqual:
    return (Left_->calc() != Right_->calc());
  case Operation::Not:
    return !static_cast<bool>(Right_->calc());
  case Operation::And:
    return Left_->calc() && Right_->calc();
  case Operation::Or:
    return Left_->calc() || Right_->calc();
  case Operation::Div:
    Val = Right_->calc();
    if (!Val)
      throw std::runtime_error("Division by zero");
    return Left_->calc() / Right_->calc();
  case Operation::Mul:
    return Left_->calc() * Right_->calc();
  case Operation::Mod:
    return Left_->calc() % Right_->calc();
  default:
    throw std::runtime_error("Operation not supported");
  }
  return Val;
}

void Operand::dump() const {
  std::cout << "Node Operand: ";
  switch (Op_) {
  case Operation::Plus:
    std::cout << "Plus" << std::endl;
    break;
  case Operation::Minus:
    std::cout << "Minus" << std::endl;
    break;
  case Operation::Greater:
    std::cout << "Greater" << std::endl;
    break;
  case Operation::Less:
    std::cout << "Less" << std::endl;
    break;
  case Operation::Assign:
    std::cout << "Assign" << std::endl;
    break;
  case Operation::StdOut:
    std::cout << "StdOut" << std::endl;
    break;
  case Operation::StdIn:
    std::cout << "StdIn" << std::endl;
    break;
  case Operation::Equal:
    std::cout << "Equal" << std::endl;
    break;
  default:
    std::cout << "Unknown" << std::endl;
  }
  if (Left_ != nullptr)
    Left_->dump();
  Right_->dump();
}

// another design flaw: dtor is recursive
Operand::~Operand() {
  if (Left_ && typeid(Decl) != typeid(*Left_))
    delete Left_;
  if (Right_ && typeid(Decl) != typeid(*Right_))
    delete Right_;
}

} // namespace Node
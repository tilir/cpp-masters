//------------------------------------------------------------------------------
//
// Option parsing alternative version (without boost)
//
//------------------------------------------------------------------------------
//
// This file is licensed after LGPL v3
// Look at: https://www.gnu.org/licenses/lgpl-3.0.en.html for details
//
//------------------------------------------------------------------------------

#pragma once

#include <cassert>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace options {

// Basic information about option (except name)
struct ValDesc {
  std::string value;
  std::string description;
  bool exists;
};

// Option parser incapsulates logic. This is non-boost version.
class Parser final {
  bool Parsed_ = false;

  // name -> value + description + exists
  std::unordered_map<std::string, ValDesc> Values_;

  // process -help option and illegal cases
  void process_help_notfound(std::string Opt) {
    bool IsHelp = (Opt.find("help") != Opt.npos);
    if (IsHelp || !Values_.count(Opt)) {
      if (!IsHelp)
        std::cerr << "ERROR: illegal option detected: " << Opt << std::endl;
      std::cout << "Available options:" << std::endl;
      for (auto &&X : Values_) {
        auto &Name = X.first;
        auto &Desc = X.second;
        std::cout << ("-" + Name) << "( = " << Desc.value
                  << ") : " << Desc.description << std::endl;
      }
      if (!IsHelp)
        throw std::runtime_error("Wrong options");
      exit(0);
    }
    Values_[Opt].exists = true;
  }

public:
  Parser() = default;

  template <typename T>
  void add(std::string Name, T Defval, std::string Description = "") {
    std::ostringstream Os;
    Os << Defval;
    auto &Desc = Values_[Name];
    Desc.value = Os.str();
    Desc.description = Description;
    Desc.exists = false;
  }

  template <typename T> T get(std::string Name) const {
    if (!Parsed_)
      throw std::runtime_error(
          "Please do not query options before they are parsed");
    if (!Values_.count(Name))
      throw std::runtime_error("Option not present in list, use add");
    const auto &Desc = Values_.find(Name)->second;
    const auto &Val = Desc.value;
    std::istringstream Is{Val};
    T Ret;
    Is >> Ret;
    return Ret;
  }

  bool exists(std::string Name) const {
    if (!Parsed_)
      throw std::runtime_error(
          "Please do not query options before they are parsed");
    if (!Values_.count(Name))
      throw std::runtime_error("Option not present in list, use add");
    const auto &Desc = Values_.find(Name)->second;
    return Desc.exists;
  }

  void parse(int argc, char **argv) {
    for (int I = 1; I < argc; ++I) {
      std::string_view Optview = argv[I];
      auto TrimPos = Optview.find_first_not_of('-');
      Optview.remove_prefix(TrimPos);
      TrimPos = Optview.find('=');
      bool HaveVal = (TrimPos != Optview.npos);
      std::string_view Valview = Optview;
      if (HaveVal)
        Optview.remove_suffix(Optview.size() - TrimPos);
      std::string Opt{Optview};
      process_help_notfound(Opt);
      if (HaveVal) {
        Valview.remove_prefix(TrimPos + 1);
        Values_[Opt].value = Valview;
      }
    }
    Parsed_ = true;
  }

  bool parsed() const noexcept { return Parsed_; }
};

} // namespace options
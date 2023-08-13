//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// MPL-like case on stadard C++
//
//-----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <array>
#include <string>

namespace {

struct TagEntity {
  bool isValid = false;
  std::string value;
};

template <int N> struct ct_ints {
  std::array<int, N> arr_;

  template <int... Is>
  constexpr ct_ints(std::integer_sequence<int, Is...>) : arr_{Is...} {}

  constexpr int find(int tag) const {
    for (int i = 0; i < N; ++i)
      if (arr_[i] == tag)
        return i;
    return N + 1;
  }
};

template <typename CommonTags> struct FixMsg {
  static constexpr int CommonTagsCount = CommonTags::size();

  template <int Tag> TagEntity &getTagEntity() {
    constexpr ct_ints<CommonTagsCount> vec(CommonTags{});
    constexpr int idx = vec.find(Tag);
    static_assert(static_cast<int>(idx) < static_cast<int>(CommonTagsCount),
                  "tag not found");
    return commonTags_[idx];
  }

  std::array<TagEntity, CommonTagsCount> commonTags_;
};

} // namespace

TEST(cexpr, mplike) {
  using MyCommonTags = std::integer_sequence<int, 11, 35, 10914, 10916>;
  FixMsg<MyCommonTags> fixMsg;

  auto &tagEntity = fixMsg.getTagEntity<11>();
  tagEntity.isValid = true;
  tagEntity.value = "someval";

  EXPECT_EQ(fixMsg.getTagEntity<35>().isValid, false);
  EXPECT_EQ(fixMsg.getTagEntity<11>().isValid, true);
}
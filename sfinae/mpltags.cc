//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// boost MPL example
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <array>
#include <boost/mpl/find.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/vector_c.hpp>
#include <concepts>
#include <string>
#include <type_traits>

namespace mpl = boost::mpl;

struct TagEntity {
  bool isValid = false;
  std::string value;
};

template <typename CommonTags> struct FixMsg {
  enum { CommonTagsCount = mpl::size<CommonTags>::type::value };

  // convert tag to index in compile time
  template <int Tag> struct index {
    enum {
      value = mpl::find<CommonTags, mpl::integral_c<int, Tag>>::type::pos::value
    };
    static_assert(static_cast<int>(value) < static_cast<int>(CommonTagsCount),
                  "tag not found");
  };

  template <int Tag> TagEntity &getTagEntity() {
    int idx = index<Tag>::value;
    return commonTags_[idx];
  }
  std::array<TagEntity, CommonTagsCount> commonTags_;
};

TEST(sfinae, mpl) {
  using MyCommonTags = mpl::vector_c<int, 11, 35, 10914, 10916>;
  FixMsg<MyCommonTags> fixMsg;
  auto &tagEntity11 = fixMsg.getTagEntity<11>();
  EXPECT_EQ(tagEntity11.isValid, false);
  tagEntity11.isValid = true;
  auto &tagEntity35 = fixMsg.getTagEntity<35>();
  EXPECT_EQ(tagEntity35.isValid, false);
}
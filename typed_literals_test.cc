#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <typed_literals.h>

#include <string_view>

namespace {

using ::testing::StrEq;

TEST(TypedLiteral, FromCLiteral) {
  auto fromCLiteral = TYPED_LITERAL("hello");
  using LiteralType = decltype(fromCLiteral);
  static_assert(
      std::is_same_v<LiteralType, typed_literals_internal::ConstStringLiteral<
                                      'h', 'e', 'l', 'l', 'o'>>);
  static_assert(sizeof(LiteralType::data) == 6);
  static_assert(LiteralType::size == 5);
  EXPECT_THAT(fromCLiteral.data, StrEq("hello"));
}

TEST(TypedLiteral, EmptyCLiteral) {
  auto fromCLiteral = TYPED_LITERAL("");
  using LiteralType = decltype(fromCLiteral);
  static_assert(std::is_same_v<LiteralType,
                               typed_literals_internal::ConstStringLiteral<>>);
  static_assert(sizeof(LiteralType::data) == 1);
  static_assert(LiteralType::size == 0);
  EXPECT_THAT(fromCLiteral.data, StrEq(""));
}

TEST(TypedLiteral, FromCArray) {
  static constexpr const char kArray[] = {'a', 'b', 'c', 0, 0};
  // 1. Typed literal only reads until the first zero.
  // 2. An array that does not contain a zero will cause compilation error.
  auto fromCArray = TYPED_LITERAL(kArray);
  using LiteralType = decltype(fromCArray);
  static_assert(std::is_same_v<
                LiteralType,
                typed_literals_internal::ConstStringLiteral<'a', 'b', 'c'>>);
  static_assert(sizeof(LiteralType::data) == 4);
  static_assert(LiteralType::size == 3);
  EXPECT_THAT(fromCArray.data, StrEq("abc"));
}

TEST(TypedLiteral, FromStringView) {
  static constexpr std::string_view sv = "lorem ipsum";
  auto fromSV = TYPED_LITERAL(sv);
  using LiteralType = decltype(fromSV);
  static_assert(
      std::is_same_v<LiteralType, typed_literals_internal::ConstStringLiteral<
                                      'l', 'o', 'r', 'e', 'm', ' ', 'i', 'p',
                                      's', 'u', 'm'>>);
  static_assert(sizeof(LiteralType::data) == 12);
  static_assert(LiteralType::size == 11);
  EXPECT_THAT(fromSV.data, StrEq("lorem ipsum"));
}

}  // namespace
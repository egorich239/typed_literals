#pragma once

#include <cstdlib>
#include <string_view>
#include <type_traits>
#include <utility>

namespace typed_literals_internal {

template <char... Chars>
struct ConstStringLiteral {
  static constexpr const char data[] = {Chars..., '\0'};
  static constexpr const std::size_t size = sizeof...(Chars);
};

template <std::size_t... Idx, typename L>
constexpr auto for_each_index(std::index_sequence<Idx...>, L l) {
  return l(std::integral_constant<std::size_t, Idx>{}...);
};

constexpr std::size_t strlen_constexpr(std::string_view sv) {
  return sv.size();
}

constexpr std::size_t strlen_constexpr(const char *c) {
  size_t t = 0;
  while (*c++) ++t;
  return t;
}

}  // namespace typed_literals_internal

#define TYPED_LITERAL(lit_)                                               \
  typed_literals_internal::for_each_index(                                \
      std::make_index_sequence<typed_literals_internal::strlen_constexpr( \
          lit_)>{},                                                       \
      [](auto... indices) constexpr {                                     \
        return typed_literals_internal::ConstStringLiteral<(              \
            lit_)[decltype(indices)::value]...>();                        \
      })

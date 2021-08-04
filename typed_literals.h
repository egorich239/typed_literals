// Copyright 2021 Ivan Egorov
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/// An illustration of `TYPED_LITERAL(l)` - a macro, producing a value of unique
/// type for every string literal `l`.
#pragma once

#include <cstdlib>
#include <string_view>
#include <type_traits>
#include <utility>

namespace typed_literals_internal {

// Compile-time string literal:
// - `Chars` encode the string literal without the trailing zero;
// - `data` is always NUL-terminated;
// - `size` does not account for the terminal zero.
template <char... Chars>
struct ConstStringLiteral {
  static constexpr const char data[] = {Chars..., '\0'};
  static constexpr const std::size_t size = sizeof...(Chars);
};

template <std::size_t... Idx, typename L>
constexpr auto for_each_index(std::index_sequence<Idx...>, L l) noexcept {
  return l(std::integral_constant<std::size_t, Idx>{}...);
};

// constexpr version of `strlen` for string views.
constexpr std::size_t strlen_constexpr(std::string_view sv) noexcept {
  return sv.size();
}

// constexpr version of `strlen` for C literals, also accepts char arrays.
// A non-NULL-terminated input results in compile error.
constexpr std::size_t strlen_constexpr(const char *c) noexcept {
  size_t t = 0;
  while (*c++) ++t;
  return t;
}

}  // namespace typed_literals_internal

/// `TYPED_LITERAL(lit_)` produces a value of `lit_`-dependent type.
///
/// This value can then be used to deduce a type that can be used as template
/// argument:
///
///   auto l = TYPED_LITERAL("lorem");
///   F<decltype(l)> foo{};
///
/// NOTE: Unfortunately, `TYPED_LITERAL` itself uses a lambda and cannot be a
/// direct parameter to such a `decltype()`.
#define TYPED_LITERAL(lit_)                                               \
  typed_literals_internal::for_each_index(                                \
      std::make_index_sequence<typed_literals_internal::strlen_constexpr( \
          lit_)>{},                                                       \
      [](auto... indices) constexpr noexcept {                            \
        return typed_literals_internal::ConstStringLiteral<(              \
            lit_)[decltype(indices)::value]...>();                        \
      })

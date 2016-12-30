/// The MIT License (MIT)
/// Copyright (c) 2016 Peter Goldsborough and Markus Engel
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to
/// deal in the Software without restriction, including without limitation the
/// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
/// sell copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
/// IN THE SOFTWARE.

#ifndef BASE_UNORDERED_ITERATOR_HPP
#define BASE_UNORDERED_ITERATOR_HPP

#include <algorithm>
#include <iterator>
#include <type_traits>

#include "lru/internal/base-iterator.hpp"
#include "lru/internal/definitions.hpp"
#include "lru/internal/optional.hpp"
#include "lru/pair.hpp"


namespace LRU {
namespace Internal {

template <typename Cache, typename UnderlyingIterator>
using BaseForBaseUnorderedIterator =
    BaseIterator<std::forward_iterator_tag,
                 decltype(UnderlyingIterator()->first),
                 decltype(UnderlyingIterator()->second.value),
                 Cache,
                 UnderlyingIterator>;

template <typename Cache, typename UnderlyingIterator>
class BaseUnorderedIterator
    : public BaseForBaseUnorderedIterator<Cache, UnderlyingIterator> {
 protected:
  using super = BaseForBaseUnorderedIterator<Cache, UnderlyingIterator>;
  using PRIVATE_BASE_ITERATOR_MEMBERS;

 public:
  using Tag = std::false_type;
  using PUBLIC_BASE_ITERATOR_MEMBERS;

  BaseUnorderedIterator() noexcept = default;

  explicit BaseUnorderedIterator(Cache& cache,
                                 const UnderlyingIterator& iterator)
  : super(cache, iterator) {
  }

  template <typename AnyCache, typename AnyIterator>
  bool
  operator==(const BaseUnorderedIterator<AnyCache, AnyIterator>& other) const
      noexcept {
    return this->_iterator == other._iterator;
  }

  template <typename AnyCache, typename AnyIterator>
  bool
  operator!=(const BaseUnorderedIterator<AnyCache, AnyIterator>& other) const
      noexcept {
    return !(*this == other);
  }

  BaseUnorderedIterator& operator++() {
    ++_iterator;
    _pair.reset();
    return *this;
  }

  BaseUnorderedIterator operator++(int) {
    auto previous = *this;
    ++*this;
    return previous;
  }

  Pair& pair() noexcept override {
    if (!_pair.has_value()) {
      _pair.emplace(key(), value());
    }

    return *_pair;
  }

  Value& value() noexcept override {
    return _iterator->second.value;
  }

  const Key& key() noexcept override {
    return _iterator->first;
  }

 protected:
  template <typename, typename, typename>
  friend class BaseOrderedIterator;
};
}  // namespace Internal
}  // namespace LRU

#endif  // BASE_UNORDERED_ITERATOR_HPP
#ifndef INDICES_H
#define INDICES_H

#include <cstddef>

// Define holder for indices...
template <std::size_t... Indices>
struct indices {};

template <std::size_t index_to_add, typename Indices = indices<>>
struct make_indices_impl;

// Recursive case
template <std::size_t index_to_add, std::size_t... Indices>
struct make_indices_impl<index_to_add, indices<Indices...>>
{
  typedef
    typename make_indices_impl<
      index_to_add-1,
      indices<index_to_add-1, Indices...>
    >::type
    type
  ;
};

// Termination case
template <std::size_t... Indices>
struct make_indices_impl<0, indices<Indices...>>
{
  typedef indices<Indices...> type;
};

template <std::size_t max_index>
inline typename make_indices_impl<max_index>::type make_indices()
{
  return typename make_indices_impl<max_index>::type();
}

#endif

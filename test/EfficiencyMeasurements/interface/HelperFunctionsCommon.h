#ifndef HELPER_FUNCTIONS_COMMON_H
#define HELPER_FUNCTIONS_COMMON_H

#include <fstream>
#include <vector>
#include <stdexcept>
#include <exception>
#include <algorithm>
#include <iterator>
#include <type_traits>

template <typename IntType>
std::vector<IntType> range(IntType start, IntType stop, IntType step)
{
	if (step == IntType(0))
	{
		throw std::invalid_argument("step for range must be non-zero");
	}
	std::vector<IntType> result;
	IntType i = start;
	while ((step > 0) ? (i < stop) : (i > stop))
	{
		result.push_back(i);
		i += step;
	}
	return result;
}
template <typename IntType>
std::vector<IntType> range(IntType start, IntType stop)
{
	return range(start, stop, IntType(1));
}
template <typename IntType>
std::vector<IntType> range(IntType stop)
{
	return range(IntType(0), stop, IntType(1));
}

// template <typename T, class UnaryPredicate>
// std::vector<T> filter(std::vector<T> vectorToFilter, UnaryPredicate pred)
// {
// 	std::vector<T> filteredVector(vectorToFilter.size());
// 	std::copy_if(vectorToFilter.begin(), vectorToFilter.end(), filteredVector.begin(), std::move(pred));
// 	filteredVector.shrink_to_fit();
// 	return filteredVector;
// }

template <class T, class UnaryPredicate>
std::vector<T> filter(const std::vector<T>& vectorToFilter, UnaryPredicate&& predicate)
{
	std::vector<T> result;
	for(auto it = vectorToFilter.begin(); it != vectorToFilter.end(); ++it)
	{
		if(predicate(*it)) result.push_back(*it);
	}
	return result;
}

template <class T, class UnaryPredicate>
std::pair<std::vector<T>, std::vector<T>> dualFilter(const std::vector<T>& vectorToFilter, UnaryPredicate predicate)
{
	std::pair<std::vector<T>, std::vector<T>> results;
	for(auto it = vectorToFilter.begin(); it != vectorToFilter.end(); ++it)
	{
		if(predicate(*it)) results.first.push_back(*it);
		else               results.second.push_back(*it);
	}
	return results;
}

// template <class T, class UnaryFunction>
// std::vector<T> map(const std::vector<T>& vectorToMap, UnaryFunction operation)
// {
// 	std::vector<typename std::result_of<UnaryFunction(T)>::type> result;
// 	result.reserve(vectorToMap.size());
// 	std::transform(vectorToMap.begin(), vectorToMap.end(),
// 		std::back_inserter(result), [&operation] (T item) { return operation(item); });
// 	return result;
// }

template<template<class...>class Z, class C, class Op>
struct calculate_return_type {
  using dC = typename std::decay<C>::type;
  using T_in = typename dC::reference;
  using T_out = typename std::decay< typename std::result_of< Op&(T_in) >::type >::type;
  using R = Z<T_out>;
};
template <template<class...>class Z=std::vector, class C, class UnaryOperator>
auto fmap(C&& c_in, UnaryOperator&& operation) -> typename calculate_return_type<Z, C, UnaryOperator>::R
{
  using R = typename calculate_return_type<Z, C, UnaryOperator>::R;
  R result;
  result.reserve(c_in.size());
  using T_in = typename calculate_return_type<Z, C, UnaryOperator>::T_in;

  using std::begin; using std::end;
  std::transform(
    begin(c_in), end(c_in),
    std::back_inserter(result),
    [&] (T_in item) { return operation(decltype(item)(item)); }
  );
  return result;
}

// template <class T, std::size_t...Idx>
// auto deref_impl(T &&tuple, std::index_sequence<Idx...>) 
// {
//     return std::tuple<decltype(*std::get<Idx>(std::forward<T>(tuple)))...>(*std::get<Idx>(std::forward<T>(tuple))...);
// }

// template <class T>
// auto deref(T &&tuple)
//     -> decltype(deref_impl(std::forward<T>(tuple), std::make_index_sequence<std::tuple_size<std::remove_reference_t<T>>::value>{})) 
// {
//     return deref_impl(std::forward<T>(tuple), std::make_index_sequence<std::tuple_size<std::remove_reference_t<T>>::value>{});
// }

template <class Iter, class R = typename std::iterator_traits<Iter>::reference>
std::pair<R, R> deref_minmax_element(Iter first, Iter last)
{
    auto iters = std::minmax_element(first, last);
    return std::pair<R, R>{*iters.first, *iters.second};
}

std::string fileToString(const std::string& filename) 
{
	std::ifstream file(filename);
	std::string result = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
	file.close(); 
	return result;
}

#endif
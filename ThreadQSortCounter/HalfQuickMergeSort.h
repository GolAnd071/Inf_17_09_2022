#pragma once

#include <thread>

#include "BetterQuickSort.h"

template <class _Compare, class _RandomAccessIterator>
void __half_quick_merge_sort(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
	typedef typename std::iterator_traits<_RandomAccessIterator>::difference_type difference_type;
	typedef typename std::iterator_traits<_RandomAccessIterator>::value_type value_type;

	difference_type __len = __last - __first;

	// if we can not divide into two parts
	if (__len < 2)
		return;

	difference_type __l2 = __len / 2;
	_RandomAccessIterator __mid = __first + __l2;

	// sort two halves of the array
	__better_quick_sort<_Compare>(__first, __mid, __comp);
	__better_quick_sort<_Compare>(__mid, __last, __comp);
	
	// merge this two halves
	std::vector<value_type> __leftArray, __rightArray;
	std::copy(__first, __mid, std::back_inserter(__leftArray));
	std::copy(__mid, __last, std::back_inserter(__rightArray));
	__merge<_Compare>(__leftArray.begin(), __leftArray.end(),
		__rightArray.begin(), __rightArray.end(), __first, __comp);
}

template <class _Compare, class _RandomAccessIterator>
void __better_half_quick_merge_sort(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
	typedef typename std::iterator_traits<_RandomAccessIterator>::difference_type difference_type;
	typedef typename std::iterator_traits<_RandomAccessIterator>::value_type value_type;

	difference_type __len = __last - __first;

	// if we can not divide into two parts
	if (__len < 2)
		return;

	difference_type __l2 = __len / 2;
	_RandomAccessIterator __mid = __first + __l2;

	// sort two halves of the array
	std::thread __half1(__better_quick_sort<_Compare, _RandomAccessIterator>, __first, __mid, __comp);
	std::thread __half2(__better_quick_sort<_Compare, _RandomAccessIterator>, __mid, __last, __comp);
	__half1.join();
	__half2.join();
	
	// merge this two halves
	std::vector<value_type> __leftArray, __rightArray;
	std::copy(__first, __mid, std::back_inserter(__leftArray));
	std::copy(__mid, __last, std::back_inserter(__rightArray));
	__merge<_Compare>(__leftArray.begin(), __leftArray.end(),
		__rightArray.begin(), __rightArray.end(), __first, __comp);
}

template <class _RandomAccessIterator>
void halfQuickMergeSort(_RandomAccessIterator __first, _RandomAccessIterator __last)
{
	__half_quick_merge_sort(__first, __last,
		__less<typename std::iterator_traits<_RandomAccessIterator>::value_type>());
}

template <class _Compare, class _RandomAccessIterator>
void halfQuickMergeSort(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
	__half_quick_merge_sort<_Compare>(__first, __last, __comp);
}

template <class _RandomAccessIterator>
void betterHalfQuickMergeSort(_RandomAccessIterator __first, _RandomAccessIterator __last)
{
	__better_half_quick_merge_sort(__first, __last,
		__less<typename std::iterator_traits<_RandomAccessIterator>::value_type>());
}

template <class _Compare, class _RandomAccessIterator>
void betterHalfQuickMergeSort(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
	__better_half_quick_merge_sort<_Compare>(__first, __last, __comp);
}
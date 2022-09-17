#pragma once

#include <iterator>
#include <utility>

#include <vector>
#include <map>

// utility for determine standard compare function
template <class _T1, class _T2 = _T1>
struct __less
{
	bool operator()(const _T1& __x, const _T1& __y) const { return __x < __y; }

	bool operator()(const _T1& __x, const _T2& __y) const { return __x < __y; }

	bool operator()(const _T2& __x, const _T1& __y) const { return __x < __y; }

	bool operator()(const _T2& __x, const _T2& __y) const { return __x < __y; }
};

template <class _T1>
struct __less<_T1, _T1>
{
	bool operator()(const _T1& __x, const _T1& __y) const { return __x < __y; }
};

template <class _T1>
struct __less<const _T1, _T1>
{
	bool operator()(const _T1& __x, const _T1& __y) const { return __x < __y; }
};

template <class _T1>
struct __less<_T1, const _T1>
{
	bool operator()(const _T1& __x, const _T1& __y) const { return __x < __y; }
};

// bubble sort
template <class _Compare, class _RandomAccessIterator>
void __bubble_sort(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
	typedef typename std::iterator_traits<_RandomAccessIterator>::difference_type difference_type;

	if (__first == __last)
		return;

	// last unsorted element
	difference_type __n = __last - __first;
	do {
		difference_type __newn = 0;
		for (_RandomAccessIterator __it = __first + 1; __it != __first + __n; ++__it)
			// if this pair is out of order, swap it
			if (!__comp(*(__it - 1), *__it)) {
				std::swap(*(__it - 1), *__it);
				__newn = __it - __first;
			}
		__n = __newn;
	} while (__n > 1);
}

// merge sort
template <class _Compare, class _InputIterator1, class _InputIterator2, class _OutputIterator>
_OutputIterator __merge(_InputIterator1 __first1, _InputIterator1 __last1,
	_InputIterator2 __first2, _InputIterator2 __last2,
	_OutputIterator __result, _Compare __comp)
{
	// while the first array is not empty
	for (; __first1 != __last1; ++__result) {
		// if the second array ended earlier than the first
		if (__first2 == __last2) {
			return std::copy(__first1, __last1, __result);
		}
		// merge
		if (__comp(*__first2, *__first1)) {
			*__result = *__first2;
			++__first2;
		}
		else {
			*__result = *__first1;
			++__first1;
		}
	}
	// write remaining elements
	return std::copy(__first2, __last2, __result);
}

template <class _Compare, class _RandomAccessIterator>
void __merge_sort(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
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
	__merge_sort<_Compare>(__first, __mid, __comp);
	__merge_sort<_Compare>(__mid, __last, __comp);

	// merge this two halves
	std::vector<value_type> __leftArray, __rightArray;
	std::copy(__first, __mid, std::back_inserter(__leftArray));
	std::copy(__mid, __last, std::back_inserter(__rightArray));
	__merge<_Compare>(__leftArray.begin(), __leftArray.end(),
		__rightArray.begin(), __rightArray.end(), __first, __comp);
}

// heap sort
template <class _Compare, class _RandomAccessIterator>
void __sift_down(_RandomAccessIterator __first, _RandomAccessIterator /*__last*/, _Compare __comp,
	typename std::iterator_traits<_RandomAccessIterator>::difference_type __len,
	_RandomAccessIterator __start)
{
	typedef typename std::iterator_traits<_RandomAccessIterator>::difference_type difference_type;
	typedef typename std::iterator_traits<_RandomAccessIterator>::value_type value_type;

	// left-child of __start is at 2 * __start + 1
	// right-child of __start is at 2 * __start + 2
	difference_type __child = __start - __first;

	if (__len < 2 || (__len - 2) / 2 < __child)
		return;

	__child = 2 * __child + 1;
	_RandomAccessIterator __child_i = __first + __child;

	if ((__child + 1) < __len && __comp(*__child_i, *(__child_i + 1))) {
		// right-child exists and is greater than left-child
		++__child_i;
		++__child;
	}

	// check if we are in heap-order
	if (__comp(*__child_i, *__start))
		// we are, __start is larger than it's largest child
		return;

	value_type __top(std::move(*__start));
	do {
		// we are not in heap-order, swap the parent with it's largest child
		*__start = std::move(*__child_i);
		__start = __child_i;

		if ((__len - 2) / 2 < __child)
			break;

		// recompute the child based off of the updated parent
		__child = 2 * __child + 1;
		__child_i = __first + __child;

		if ((__child + 1) < __len && __comp(*__child_i, *(__child_i + 1))) {
			// right-child exists and is greater than left-child
			++__child_i;
			++__child;
		}

		// check if we are in heap-order
	} while (!__comp(*__child_i, __top));
	*__start = std::move(__top);
}

template <class _Compare, class _RandomAccessIterator>
void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp,
	typename std::iterator_traits<_RandomAccessIterator>::difference_type __len)
{
	if (__len > 1) {
		std::swap(*__first, *--__last);
		__sift_down<_Compare>(__first, __last, __comp, __len - 1, __first);
	}
}

template <class _Compare, class _RandomAccessIterator>
void __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
	typedef typename std::iterator_traits<_RandomAccessIterator>::difference_type difference_type;

	difference_type __n = __last - __first;

	if (__n > 1) {
		// start from the first parent, there is no need to consider children
		for (difference_type __start = (__n - 2) / 2; __start >= 0; --__start)
			__sift_down<_Compare>(__first, __last, __comp, __n, __first + __start);
	}
}

template <class _Compare, class _RandomAccessIterator>
void __sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
	typedef typename std::iterator_traits<_RandomAccessIterator>::difference_type difference_type;

	for (difference_type __n = __last - __first; __n > 1; --__last, --__n)
		__pop_heap<_Compare>(__first, __last, __comp, __n);
}

template <class _Compare, class _RandomAccessIterator>
void __heap_sort(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
	// make a heap from array
	__make_heap<_Compare>(__first, __last, __comp);
	// sort it
	__sort_heap<_Compare>(__first, __last, __comp);
}

// quick sort
template <class _Compare, class _RandomAccessIterator>
_RandomAccessIterator __partition(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
	typedef typename std::iterator_traits<_RandomAccessIterator>::difference_type difference_type;
	typedef typename std::iterator_traits<_RandomAccessIterator>::value_type value_type;

	value_type __pivot = *(__last - 1);
	// indicates position of pivot
	_RandomAccessIterator __it = __first;

	for (; __first != __last - 1; ++__first)
		// if current element smaller than pivot
		if (__comp(*__first, __pivot))
			std::swap(*__it++, *__first);

	std::swap(*__it, *(__last - 1));
	return __it;
}

template <class _Compare, class _RandomAccessIterator>
void __quick_sort(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
	typedef typename std::iterator_traits<_RandomAccessIterator>::difference_type difference_type;

	difference_type __len = __last - __first;

	// if there is one element then brake
	if (__len < 2)
		return;

	// find element on right place
	_RandomAccessIterator __pi = __partition<_Compare>(__first, __last, __comp);

	// separately sort two part of the array
	__quick_sort<_Compare>(__first, __pi, __comp);
	__quick_sort<_Compare>(__pi + 1, __last, __comp);
}

// counting sort
template <class _Compare, class _RandomAccessIterator>
void __counting_sort(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
	typedef typename std::iterator_traits<_RandomAccessIterator>::value_type value_type;

	// map for counting different elements
	std::map<value_type, size_t, _Compare> __c(__comp);

	// count element
	for (_RandomAccessIterator __it = __first; __it != __last; ++__it) {
		// increment if element is already in map
		if (__c.find(*__it) != __c.end())
			++__c[*__it];
		// insert element if not
		else
			__c[*__it] = 1;
	}

	// copy map to output array
	for (auto __i : __c)
		for (size_t __j = 0; __j < __i.second; ++__j)
			*__first++ = __i.first;
}

// Declarations
template <class _RandomAccessIterator>
void bubbleSort(_RandomAccessIterator __first, _RandomAccessIterator __last)
{
	__bubble_sort(__first, __last,
		__less<typename std::iterator_traits<_RandomAccessIterator>::value_type>());
}

template <class _Compare, class _RandomAccessIterator>
void bubbleSort(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
	__bubble_sort<_Compare>(__first, __last, __comp);
}

template <class _RandomAccessIterator>
void mergeSort(_RandomAccessIterator __first, _RandomAccessIterator __last)
{
	__merge_sort(__first, __last,
		__less<typename std::iterator_traits<_RandomAccessIterator>::value_type>());
}

template <class _Compare, class _RandomAccessIterator>
void mergeSort(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
	__merge_sort<_Compare>(__first, __last, __comp);
}

template <class _RandomAccessIterator>
void heapSort(_RandomAccessIterator __first, _RandomAccessIterator __last)
{
	__heap_sort(__first, __last,
		__less<typename std::iterator_traits<_RandomAccessIterator>::value_type>());
}

template <class _Compare, class _RandomAccessIterator>
void heapSort(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
	__heap_sort<_Compare>(__first, __last, __comp);
}

template <class _RandomAccessIterator>
void quickSort(_RandomAccessIterator __first, _RandomAccessIterator __last)
{
	__quick_sort(__first, __last,
		__less<typename std::iterator_traits<_RandomAccessIterator>::value_type>());
}

template <class _Compare, class _RandomAccessIterator>
void quickSort(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
	__quick_sort<_Compare>(__first, __last, __comp);
}

template <class _RandomAccessIterator>
void countingSort(_RandomAccessIterator __first, _RandomAccessIterator __last)
{
	__counting_sort(__first, __last,
		__less<typename std::iterator_traits<_RandomAccessIterator>::value_type>());
}

template <class _Compare, class _RandomAccessIterator>
void countingSort(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
	__counting_sort<_Compare>(__first, __last, __comp);
}
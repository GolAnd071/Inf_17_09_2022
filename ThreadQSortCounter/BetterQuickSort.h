#pragma once

#include "Sorts.h"

const size_t __CUTOFF = 8;

template <typename _Ty>
void __swap(_Ty& __x, _Ty& __y);

template <class _Compare, class _RandomAccessIterator>
void __insertion_sort(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
	for (_RandomAccessIterator __it0 = __first; __it0 != __last; ++__it0)
		for (_RandomAccessIterator __it1 = __it0; __it1 != __first && __comp(*__it1, *(__it1 - 1)); --__it1)
			__swap(*__it1, *(__it1 - 1));
}

template <class _Compare, class _RandomAccessIterator>
_RandomAccessIterator __median3(_RandomAccessIterator __first, _RandomAccessIterator __second, _RandomAccessIterator __third, _Compare __comp)
{
	return __comp(*__first, *__second) ? __comp(*__second, *__third) ? __second : __comp(*__first, *__third) ? __third : __first : 
										 __comp(*__third, *__second) ? __second : __comp(*__third, *__first) ? __third : __first;
}

template <class _Compare, class _RandomAccessIterator>
void __better_quick_sort(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
	typedef typename std::iterator_traits<_RandomAccessIterator>::difference_type difference_type;

	difference_type __len = __last - __first;

	// cutoff to insertion sort
	if (__len <= __CUTOFF) {
		__insertion_sort<_Compare, _RandomAccessIterator>(__first, __last, __comp);
		return;
	}

	// use median-of-3 as partitioning element
	if (__len <= 40) {
		_RandomAccessIterator __median = __median3<_Compare, _RandomAccessIterator>(__first, __first + __len / 2, __last - 1, __comp);
		__swap(*__median, *__first);
	}

	// use tukey ninther as partitioning element
	else {
		difference_type __eps = __len / 8;
		_RandomAccessIterator __mid = __first + __len / 2;
		_RandomAccessIterator __m_first = __median3<_Compare, _RandomAccessIterator>(__first, __first + __eps, __first + __eps + __eps, __comp);
		_RandomAccessIterator __m_mid = __median3<_Compare, _RandomAccessIterator>(__mid - __eps, __mid, __mid + __eps, __comp);
		_RandomAccessIterator __m_last = __median3<_Compare, _RandomAccessIterator>(__last - 1 - __eps - __eps, __last - 1 - __eps, __last - 1, __comp);
		_RandomAccessIterator __ninther = __median3<_Compare, _RandomAccessIterator>(__m_first, __m_mid, __m_last, __comp);
		__swap(*__first, *__ninther);
	}

	// bentley-mcilroy 3-way partitioning
	_RandomAccessIterator __i = __first, __j = __last;
	_RandomAccessIterator __p = __first, __q = __last;

	for (;;) {
		auto val = *__first;
		while (__comp(*(++__i), val))
			if (*__i == *(__last - 1))
				break;

		while (__comp(val, *(--__j)))
			if (*__j == *__first)
				break;

		if (__i > __j)
			break;

		__swap(*__i, *__j);

		if (*__i == val)
			__swap(*(++__p), *__i);

		if (*__j == val)
			__swap(*(--__q), *__j);
	}

	__swap(*__first, *__j);

	__i = __j-- + 1;

	for (_RandomAccessIterator __k = __first + 1; __k <= __p; ++__k)
		__swap(*__k, *(__j--));
	for (_RandomAccessIterator __k = __last - 1; __k >= __q; --__k)
		__swap(*__k, *(__i++));

	__better_quick_sort<_Compare, _RandomAccessIterator>(__first, __j + 1, __comp);
	__better_quick_sort<_Compare, _RandomAccessIterator>(__i, __last, __comp);
}

template <class _RandomAccessIterator>
void betterQuickSort(_RandomAccessIterator __first, _RandomAccessIterator __last)
{
	__better_quick_sort(__first, __last,
		__less<typename std::iterator_traits<_RandomAccessIterator>::value_type>());
}

template <class _Compare, class _RandomAccessIterator>
void betterQuickSort(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
	__better_quick_sort<_Compare>(__first, __last, __comp);
}
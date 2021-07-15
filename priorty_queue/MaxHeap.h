#ifndef _H_MaxHeap
#define _H_MaxHeap

#include<iostream>
#include<assert.h>
#include<vector>

using namespace std;

template<class T>
struct Less
{
	bool operator()(const T& left, const T& right) const
	{
		return left < right;
	}
};

template<class T>
struct Greater
{
	bool operator()(const T& left, const T& right) const
	{
		return left > right;
	}
};

template<class T, class Compare = Greater<T>>
class MaxHeap {
private:
	vector<T>_heap;
	size_t _capacity;
	size_t _CurrentSize;
public:
	MaxHeap() {}

	MaxHeap(T* arr, size_t length) {};

	~MaxHeap() {}

	void SiftUp(size_t child) {};

	void SiftDown(size_t parent) {};

	void Push(const T& elem) {};

	void Pop() {};

	T& Top() {};
	
};

#endif

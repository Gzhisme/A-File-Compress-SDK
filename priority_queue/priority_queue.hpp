#ifndef _H_PriorityQueue
#define _H_PriorityQueue

#include <iostream>
#include <vector>
#include <assert.h>

template <typename T1>
class less
{
public:
	bool operator()(T1 elem1, T1 elem2)
	{
		return elem1 < elem2;
	}
};

template <typename T2>
class greater
{
public:
	bool operator()(T2 elem1, T2 elem2)
	{
		return elem1 > elem2;
	}
};

template <typename T, typename Compare = less<T> >
class Heap
{
public:
	Heap()
	{}

	Heap(T *arr, int size)
	{
		assert(NULL != arr && size >= 0);		
		
		for (int i = 0; i < size; ++i)
		{
			_heap.push_back(arr[i]);
		}

		int root = _heap.size() / 2 - 1; 
		for (int i = root; i >= 0; --i)
			AdjustDown(i);
	}
	
	void push_back(const T & data)
	{
		_heap.push_back(data);
		AdjustUp(_heap.size() - 1);
	}

	void pop()
	{
		_heap[0] = _heap[_heap.size() - 1];
		_heap.pop_back();
		AdjustDown(0);
	}

	const T & top()
	{
		assert(_heap.size() != 0);
		return _heap[0];
	}
	
	size_t Size()
	{
		return _heap.size();
	}

private:
	void AdjustUp(int root)
	{
		int size = _heap.size();
		int child = root;
		int parent;

		while (0 != child)
		{
			parent = (child - 1) / 2;
			if (compare(_heap[child], _heap[parent]))
			{
				std::swap(_heap[child], _heap[parent]);
				child = parent;
			}
			else
				break;
		}	
	}

	void AdjustDown(int root)
	{
		int size = _heap.size();
		if (size <= 1)
			return;

		while (root <= size / 2 - 1)
		{
			int child = root * 2 + 1;
			if (child + 1 < size && com(_heap[child + 1], _heap[child]))
				++child;
			
			int parent = (child - 1) / 2;
			if (compare(_heap[child], _heap[parent]))
			{
				std::swap(_heap[child], _heap[parent]);
				root = child;
			}
			else
				break;
		}
	}

private:
	std::vector<T> _heap;
	Compare compare;
};

template <typename T, typename Compare = less<T> >
class PriorityQueue
{
public:
	PriorityQueue()
	{}
	
	PriorityQueue(int *arr, int n)
		: q(arr, n)
	{}

	void push_back(const T &data)
	{
		q.push_back(data);
	}

	void pop()
	{
		q.pop();
	}

	const T & top()
	{
		return q.top();
	}

	size_t Size()
	{
		return q.Size();
	}
	

private:
	Heap<int, Compare> q;
};


#endif

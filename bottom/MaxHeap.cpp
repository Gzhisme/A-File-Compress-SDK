#include<MaxHeap.h>

MaxHeap::MaxHeap(T* arr, size_t length) {
	_capacity = length + 1;
	_CurrentSize = 0;
	_heap.reserve(_capacity);
	for (size_t i = 1; i <= length; i++) {
		_heap[i] = arr[i - 1];
	}
	for (size_t i = length; i > 0; i--) {
		SiftUp(i);
	}
	_CurrentSize = length;
}
/*���ϵ���*/
void MaxHeap::SiftUp(size_t child) {
	Compare compare;
	size_t parent = (child - 1) / 2;
	while (parent >= 0) {
		if (compare(_heap[child], _heap[parent])) {
			swap(_heap[child], _heap[parent]);
			child = parent;
			parent = (child - 1) / 2;
		}
		else break;
	}
}
/*���µ���*/
void MaxHeap::SiftDown(size_t parent) {
	Compare compare;
	size_t child = parent * 2 + 1;
	while (child < _capacity) {
		if (child + 1 < _capacity && compare(_heap[child + 1], _heap[child])) {
			child++;
		}
		if (compare(_heap[child], _heap[parent])) {
			swap(_heap[child], _heap[parent]);
			perant = child;
			child = parent * 2 + 1;
		}
		else break;
	}
}
/*����Ԫ��*/
void MaxHeap::Push(const T& elem) {
	_heap.push_back(elem);
	SiftUp(++_CurrentSize);
}
/*ɾ�������*/
void MaxHeap::Pop() {
	assert(!_heap.empty());
	swap(_heap[0], _heap[_CurrentSize]);
	_heap.pop_back();
	SiftDown(1);
}
/*ȡ�Ѷ�Ԫ��*/
T& MaxHeap::Top() {
	assert(!_heap.empty());
	return _heap[1];
}
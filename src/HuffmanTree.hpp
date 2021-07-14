#pragma once

#include <iostream>
#include <assert.h>
#include <queue>
#include <vector>

using namespace std;

/*HuffmanTree���*/
template <typename T>
struct HuffmanTreeNode
{
	HuffmanTreeNode(const T &data)
		: _weight(data)//Ȩֵ
		, pLeft(NULL)//���ӽڵ�	
		, pRight(NULL)//���ӽڵ�	
		, pParent(NULL)//���ڵ�
	{}

	T 		_weight;
	HuffmanTreeNode *pLeft;
	HuffmanTreeNode *pRight;
	HuffmanTreeNode *pParent;
};

/*�ȽϺ���*/
template <typename T>
struct greater
{
	bool operator()(const T &left, const T &right)
	{
		return left->_weight > right->_weight;
	}
};

/*HuffmanTree*/
template <typename T>
class HuffmanTree
{
	HuffmanTreeNode<T>* pRoot;
	T					_invalid;
public:
	HuffmanTree(const T *weight, int size, const T &invalid)
		: pRoot(NULL)
		, _invalid(invalid)
	{
		assert(NULL != weight && size >= 0);
		_Create(weight, size);
	}

	~HuffmanTree()
	{
		_Destroy(pRoot);
	}

	void LevelTraverse()
	{
		queue<HuffmanTreeNode<T> *> q;
		if (NULL != pRoot)
			q.push(pRoot);

		while (!q.empty())//�������
		{
			HuffmanTreeNode<T> *pCur = q.front();
			q.pop();
			cout << pCur->_weight << " ";

			if (NULL != pCur->pLeft)
				q.push(pCur->pLeft);
			if (NULL != pCur->pRight)
				q.push(pCur->pRight);
		}

		cout << endl;
	}

	HuffmanTreeNode<T> * GetRoot()
	{
		return pRoot;
	}

private:
	void _Destroy(HuffmanTreeNode<T> * &pRoot)
	{
		if (NULL != pRoot)
		{
			_Destroy(pRoot->pLeft);
			_Destroy(pRoot->pRight);
			delete pRoot;
			pRoot = NULL;
		}
	}

	void _Create(const T *weight, int size)
	{
		if (0 == size)
			return;
		else if (1 == size)
		{
			if (*weight != _invalid)
			pRoot = new HuffmanTreeNode<T>(*weight);
		}
		else
		{
			priority_queue<HuffmanTreeNode<T> *, vector<HuffmanTreeNode<T>*>, greater<HuffmanTreeNode<T>*>> heap;

			for (int i = 0; i < size; ++i)
			{
				if (weight[i] != _invalid)
				{
					HuffmanTreeNode<T> *tmp = new HuffmanTreeNode<T>(weight[i]);
					heap.push(tmp);
				}
			}

			HuffmanTreeNode<T> *pLeft, *pRight;
			while (heap.size() >= 2)
			{
				pLeft = heap.top();
				heap.pop();
				pRight = heap.top();
				heap.pop();

				HuffmanTreeNode<T> *pParent = new HuffmanTreeNode<T>(pLeft->_weight + pRight->_weight);
				pParent->pLeft = pLeft;
				pParent->pRight = pRight;
				pLeft->pParent = pParent;
				pRight->pParent = pParent;

				heap.push(pParent);
			}
			if (!heap.empty())
				pRoot = heap.top();
		}
	}
};

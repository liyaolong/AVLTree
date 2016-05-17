#pragma once
using namespace std;

template<class K,class V>
struct AVLTreeNode
{
	K _key;
	V _value;
	int bf;
	AVLTreeNode<K, V>* _left;
	AVLTreeNode<K, V>* _right;
	AVLTreeNode<K, V>* _parent;
	AVLTreeNode(const K& key, const V& value)
		:_key(key)
		, _value(value)
		,bf(0)
		, _left(nullptr)
		, _right(nullptr)
		, _parent(nullptr)
	{}
};

template<class K,class V>
class AVLTree
{
public:
	typedef AVLTreeNode<K, V> Node;

	AVLTree()
		:_root(nullptr)
	{}

	bool Insert(const K& key, const V& value)
	{
		//1.空树
		if (_root == nullptr)
		{
			_root = new Node(key, value);
			return true;
		}
		else
		{
			Node* parent = nullptr;
			Node* cur = _root;
			//2.查找位置
			while (cur)
			{
				if (cur->_key > key)
				{
					parent = cur;
					cur = cur->_left;
				}
				else if (cur->_key < key)
				{
					parent = cur;
					cur = cur->_right;
				}
				else
					return false;
			}
			//3.插入节点
			cur = new Node(key, value);
			cur->_parent = parent;
			if (parent->_key < key)
				parent->_right = cur;
			else
				parent->_left = cur;

			bool IsRotate = false;
			//4.更新平衡因子，调整树
			while (parent)
			{
				if (parent->_right == cur)
					parent->bf++;
				else
					parent->bf--;
				if (parent->bf == 0)
					break;
				else if (parent->bf == 1 || parent->bf == -1)
				{
					cur = parent;
					parent = parent->_parent;
				}
				else
				{
					IsRotate = true;
					if (parent->bf == 2)
					{
						if (cur->bf == 1)//左旋
						{
							RotateL(parent);
						}
						else//右左旋转
						{
							RotateRL(parent);
						}
					}
					if (parent->bf == -2)
					{
						if (cur->bf == -1)//右旋
						{
							RotateR(parent);
						}
						else//左右旋转
						{
							RotateLR(parent);
						}
					}
					break;
				}
			}
			if (IsRotate)
			{
				Node* ppNode = parent->_parent;
				if (ppNode == nullptr)
					_root = parent;
				else
				{
					if (ppNode->_key < parent->_key)
						ppNode->_right = parent;
					else
						ppNode->_left = parent;
				}
			}
		}
		return true;
	}
	void Inorder()
	{
		_Inorder(_root);
		cout << endl;
	}
	bool IsBalance()
	{
		return _IsBalance(_root);
	}
protected:
	bool _IsBalance(Node*& root)
	{
		if (root == nullptr)
			return true;

		int left = Height(root->_left);
		int right = Height(root->_right);

		int _bf = abs(left - right);
		if (_bf > 1)
			return false;
		if (_bf != abs(root->bf))
		{
			cout << "bf have a trouble:" << root->_key << endl;
			return false;
		}

		return _IsBalance(root->_left) && _IsBalance(root->_right);
	}
	int Height(Node*& root)
	{
		if (root == nullptr)
			return 0;

		int left = Height(root->_left) + 1;
		int right = Height(root->_right) + 1;

		return left > right ? left : right;
	}
	void RotateL(Node*& parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;

		parent->_right = subRL;
		if (subRL)
			subRL->_parent = parent;
		subR->_parent = parent->_parent;
		subR->_left = parent;
		parent->_parent = subR;
		parent->bf = subR->bf = 0;

		parent = subR;
	}
	void RotateR(Node*& parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;

		parent->_left = subLR;
		if (subLR)
			subLR->_parent = parent;
		subL->_parent = parent->_parent;
		subL->_right = parent;
		parent->_parent = subL;
		parent->bf = subL->bf = 0;

		parent = subL;
	}
	//void RotateLR(Node*& parent)
	//{
	//	RotateL(parent->_left);
	//	RotateR(parent);
	//}
	void RotateLR(Node*& parent)
	{
		bool IsSubRR = true;
		Node* leftTemp = parent->_left;
		Node* subR = leftTemp->_right;
		Node* subRL = subR->_left;

		leftTemp->_right = subRL;
		if (subRL)
		{
			subRL->_parent = leftTemp;
			if (subR->_left == nullptr)
				IsSubRR = false;
		}
		subR->_parent = leftTemp->_parent;
		subR->_left = leftTemp;
		leftTemp->_parent = subR;
		leftTemp->bf = subR->bf = 0;

		leftTemp = subR;
		parent->_left = leftTemp;

		Node* subL = parent->_left;
		Node* subLR = subL->_right;

		parent->_left = subLR;
		if (subLR)
			subLR->_parent = parent;
		subL->_parent = parent->_parent;
		subL->_right = parent;
		parent->_parent = subL;
		if (IsSubRR)
		{
			parent->bf = subL->bf = 0;
		}
		else
		{
			parent->bf = 1;
			subL->bf = 0;
		}

		parent = subL;

	}
	//void RotateRL(Node*& parent)
	//{
	//	RotateR(parent->_right);
	//	RotateL(parent);
	//}
	void RotateRL(Node*& parent)
	{
		bool IsSubLL=true;
		Node* rightTemp = parent->_right;
		Node* subL = rightTemp->_left;
		Node* subLR = subL->_right;

		rightTemp->_left = subLR;
		if (subLR)
		{
			subLR->_parent = rightTemp;
			if (subL->_left == nullptr)
				IsSubLL = false;
		}
		subL->_parent = rightTemp->_parent;
		subL->_right = rightTemp;
		rightTemp->_parent = subL;
		rightTemp->bf = subL->bf = 0;

		rightTemp = subL;
		parent->_right = rightTemp;

		Node* subR = parent->_right;
		Node* subRL = subR->_left;

		parent->_right = subRL;
		if (subRL)
			subRL->_parent = parent;
		subR->_parent = parent->_parent;
		subR->_left = parent;
		parent->_parent = subR;
		if (IsSubLL)
		{
			parent->bf = subR->bf = 0;
		}
		else
		{
			parent->bf = -1;
			subR->bf = 0;
		}

		parent = subR;
	}
	void _Inorder(Node*& root)
	{
		if (root == nullptr)
			return;
		_Inorder(root->_left);
		cout << root->_key << " ";
		_Inorder(root->_right);
	}
private:
	Node* _root;
};

void Test()
{
	AVLTree<int, int> t;
	int a[] = {30,10,20,25,18,16,15,17,14,13,12,9,8,7,6,5,4,3};
	for (size_t i = 0;i < sizeof(a) / sizeof(int);++i)
	{
		t.Insert(a[i], i);
	}
	t.Inorder();
	
	cout << t.IsBalance() << endl;
}
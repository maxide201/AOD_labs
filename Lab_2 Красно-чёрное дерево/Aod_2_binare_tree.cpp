#include <iostream>
using namespace std;
class RBtree {
	struct node_st
	{
		node_st* p1, * p2;
		int value;
		bool red;
	};
	node_st* tree_root;
	int nodes_count;
private:
	node_st* NewNode(int value);
	void DelNode(node_st*);
	void Clear(node_st*);
	node_st* RotateLeft(node_st*);
	node_st* RotateRight(node_st*);
	void BalanceInsert(node_st**);
	bool BalanceRemove1(node_st**);
	bool BalanceRemove2(node_st**);
	bool Insert(int, node_st**);
	bool GetMin(node_st**, node_st**);
	bool Remove(node_st**, int);
	void Show(node_st*, int, char);
public:
	RBtree();
	~RBtree();
	void Clear();
	bool Find(int);
	void Insert(int);
	void Remove(int);
	int GetNodesCount();
	void Show();
};


RBtree::RBtree()
{
	tree_root = 0;
	nodes_count = 0;
}

RBtree::~RBtree()
{
	Clear(tree_root);
}

int RBtree::GetNodesCount()
{
	return nodes_count;
}

RBtree::node_st* RBtree::NewNode(int value)
{
	nodes_count++;
	node_st* node = new node_st;
	node->value = value;
	node->p1 = node->p2 = 0;
	node->red = true;
	return node;
}


void RBtree::DelNode(node_st* node)
{
	nodes_count--;
	delete node;
}

void RBtree::Clear(node_st* node)
{
	if (!node) return;
	Clear(node->p1);
	Clear(node->p2);
	DelNode(node);
}

void RBtree::Show(node_st* node, int depth, char dir)
{
	int n;
	if (!node) return;
	for (n = 0; n < depth; n++) putchar(' ');
	printf("%c[%d] (%s)\n", dir, node->value, node->red ? "red" : "black");
	Show(node->p1, depth + 2, '-');
	Show(node->p2, depth + 2, '+');
}


RBtree::node_st* RBtree::RotateLeft(node_st* node)
{
	node_st* p2 = node->p2;
	node_st* p21 = p2->p1;
	p2->p1 = node;
	node->p2 = p21;
	return p2;
}
RBtree::node_st* RBtree::RotateRight(node_st* node)
{
	node_st* p1 = node->p1;
	node_st* p12 = p1->p2;
	p1->p2 = node;
	node->p1 = p12;
	return p1;
}


void RBtree::BalanceInsert(node_st** root)
{
	node_st* p1, * p2, * px1, * px2;
	node_st* node = *root;
	if (node->red) return;
	p1 = node->p1;
	p2 = node->p2;
	if (p1 && p1->red) {
		px2 = p1->p2;
		if (px2 && px2->red) p1 = node->p1 = RotateLeft(p1);
		px1 = p1->p1;
		if (px1 && px1->red) {
			node->red = true;
			p1->red = false;
			if (p2 && p2->red) {
				px1->red = true;
				p2->red = false;
				return;
			}
			*root = RotateRight(node);
			return;
		}
	}
	if (p2 && p2->red) {
		px1 = p2->p1;
		if (px1 && px1->red) p2 = node->p2 = RotateRight(p2);
		px2 = p2->p2;
		if (px2 && px2->red) {
			node->red = true;
			p2->red = false;
			if (p1 && p1->red) {
				px2->red = true;
				p1->red = false;
				return;
			}
			*root = RotateLeft(node);
			return;
		}
	}
}


bool RBtree::BalanceRemove1(node_st** root)
{
	node_st* node = *root;
	node_st* p1 = node->p1;
	node_st* p2 = node->p2;
	if (p1 && p1->red) {
		p1->red = false; return false;
	}
	if (p2 && p2->red) { // случай 1
		node->red = true;
		p2->red = false;
		node = *root = RotateLeft(node);
		if (BalanceRemove1(&node->p1)) node->p1->red = false;
		return false;
	}
	unsigned int mask = 0;
	node_st* p21 = p2->p1;
	node_st* p22 = p2->p2;
	if (p21 && p21->red) mask |= 1;
	if (p22 && p22->red) mask |= 2;
	switch (mask)
	{
	case 0:     // случай 2 - if((!p21 || !p21->red) && (!p22 || !p22->red))
		p2->red = true;
		return true;
	case 1:
	case 3:     // случай 3 - if(p21 && p21->red)
		p2->red = true;
		p21->red = false;
		p2 = node->p2 = RotateRight(p2);
		p22 = p2->p2;
	case 2:     // случай 4 - if(p22 && p22->red)
		p2->red = node->red;
		p22->red = node->red = false;
		*root = RotateLeft(node);
	}
	return false;
}

bool RBtree::BalanceRemove2(node_st** root)
{
	node_st* node = *root;
	node_st* p1 = node->p1;
	node_st* p2 = node->p2;
	if (p2 && p2->red) { p2->red = false; return false; }
	if (p1 && p1->red) { // случай 1
		node->red = true;
		p1->red = false;
		node = *root = RotateRight(node);
		if (BalanceRemove2(&node->p2)) node->p2->red = false;
		return false;
	}
	unsigned int mask = 0;
	node_st* p11 = p1->p1;
	node_st* p12 = p1->p2;
	if (p11 && p11->red) mask |= 1;
	if (p12 && p12->red) mask |= 2;
	switch (mask) {
	case 0:     // случай 2 - if((!p12 || !p12->red) && (!p11 || !p11->red))
		p1->red = true;
		return true;
	case 2:
	case 3:     // случай 3 - if(p12 && p12->red)
		p1->red = true;
		p12->red = false;
		p1 = node->p1 = RotateLeft(p1);
		p11 = p1->p1;
	case 1:     // случай 4 - if(p11 && p11->red)
		p1->red = node->red;
		p11->red = node->red = false;
		*root = RotateRight(node);
	}
	return false;
}


bool RBtree::Find(int value)
{
	node_st* node = tree_root;
	while (node) {
		if (node->value == value) return true;
		node = node->value > value ? node->p1 : node->p2;
	}
	return false;
}

bool RBtree::Insert(int value, node_st** root)
{
	node_st* node = *root;
	if (!node) *root = NewNode(value);
	else {
		if (value == node->value) return true;
		if (Insert(value, value < node->value ? &node->p1 : &node->p2)) return true;
		BalanceInsert(root);
	}
	return false;
}

bool RBtree::GetMin(node_st** root, node_st** res)
{
	node_st* node = *root;
	if (node->p1) {
		if (GetMin(&node->p1, res)) return BalanceRemove1(root);
	}
	else {
		*root = node->p2;
		*res = node;
		return !node->red;
	}
	return false;
}


bool RBtree::Remove(node_st** root, int value)
{
	node_st* t, * node = *root;
	if (!node) return false;
	if (node->value < value) {
		if (Remove(&node->p2, value))   return BalanceRemove2(root);
	}
	else if (node->value > value) {
		if (Remove(&node->p1, value))   return BalanceRemove1(root);
	}
	else {
		bool res;
		if (!node->p2) {
			*root = node->p1;
			res = !node->red;
		}
		else {
			res = GetMin(&node->p2, root);
			t = *root;
			t->red = node->red;
			t->p1 = node->p1;
			t->p2 = node->p2;
			if (res) res = BalanceRemove2(root);
		}
		DelNode(node);
		return res;
	}
	return 0;
}


void RBtree::Show()
{
	printf("TREE:\n");
	Show(tree_root, 0, '*');
}

void RBtree::Insert(int value)
{
	Insert(value, &tree_root);
	if (tree_root) tree_root->red = false;
}


void RBtree::Remove(int value)
{
	Remove(&tree_root, value);
}

void RBtree::Clear()
{
	Clear(tree_root);
	tree_root = 0;
}

int main()
{
	RBtree tree;
	int inp, n;
	cin >> n;
	for (int i = 0; i < n; i++)
	{
		cin >> inp;
		tree.Insert(inp);
	}
	tree.Show();
	return 0;
}
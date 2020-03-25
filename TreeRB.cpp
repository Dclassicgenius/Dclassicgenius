#pragma once

#include <iostream>
#include <vector>

using namespace std;

template <typename K, typename T>
class RBTreeMap {
	const bool red = true;
	const bool black = false;

	struct Node {
		K key;
		T value; // value of element
		bool color; // color bit

		Node* left; // pointer to the left subtree
		Node* right; // pointer to the right subtree

		Node* parent; // parent
	};

	Node* root; // root

	Node* Sibling(Node* node) const; 

	void RotateLeft(Node* node); 
	void RotateRight(Node* node); 

	void FixInsertion(Node* node); // node adjustment to satisfy all the properties of red-black tree after insertion
	void FixRemoving(Node* node); // node adjustment to satisfy all the properties of red-black tree after deleting

	Node* FindNode(const K& key) const; // search 
	void RemoveNode(Node* node); // deleting a node

	void Copy(Node* node, Node*& newNode);
	void Clear(Node*& node); // clear the tree

	void GetKeys(Node* node, vector<K>& keys) const; // getting the keys
	void GetValues(Node* node, vector<T>& values) const; // getting the values

	void Print(ostream& os, Node* node) const; 

public:
	RBTreeMap(); // constructor
	RBTreeMap(const RBTreeMap& tree); 

	RBTreeMap& operator=(const RBTreeMap& tree); // assignment operator

	void Insert(const K& key, const T& value); // inserting an element to the tree
	bool Remove(const K& key); // deleting an element from the tree
	bool Find(const K& key) const; // searching an element from the tree
	void Clear(); // deleting all element from the three

	vector<K> GetKeys() const; // getting the keys
	vector<T> GetValues() const; // getting the values

	T& operator[](const K& key); // getting element by key

	~RBTreeMap(); // destructor

	template <typename K1, typename T1>
	friend ostream& operator<<(ostream& os, const RBTreeMap<K1, T1>& tree); 
};

// getting the sibling
template <typename K, typename T>
typename RBTreeMap<K, T>::Node* RBTreeMap<K, T>::Sibling(Node* node) const {
	if (node->parent == nullptr)
		return nullptr;

	Node* parent = node->parent;

	return node == parent->left ? parent->right : parent->left;
}

// left
template <typename K, typename T>
void RBTreeMap<K, T>::RotateLeft(Node* node) {
	Node* right = node->right;
	node->right = right->left;

	if (node->right != nullptr)
		node->right->parent = node;

	right->parent = node->parent;

	if (node->parent == nullptr) {
		root = right;
	}
	else if (node == node->parent->left) {
		node->parent->left = right;
	}
	else {
		node->parent->right = right;
	}

	right->left = node;
	node->parent = right;
}

// right
template <typename K, typename T>
void RBTreeMap<K, T>::RotateRight(Node* node) {
	Node* left = node->left;
	node->left = left->right;

	if (node->left)
		node->left->parent = node;

	left->parent = node->parent;

	if (node->parent == nullptr) {
		root = left;
	}
	else if (node == node->parent->left) {
		node->parent->left = left;
	}
	else {
		node->parent->right = left;
	}

	left->right = node;
	node->parent = left;
}

//node adjustment to satisfy all the properties of red-black tree after insertion
template <typename K, typename T>
void RBTreeMap<K, T>::FixInsertion(Node* node) {
	while (node != root && node->color != black && node->parent->color == red) {
		Node* parent = node->parent;
		Node* grandparent = node->parent->parent; 
		Node* uncle = parent == grandparent->left ? grandparent->right : grandparent->left; 

		if (uncle && uncle->color == red) {
			grandparent->color = red;
			parent->color = black;
			uncle->color = black;

			node = grandparent;
		}
		else {
			if (parent == grandparent->left) {
				if (node == parent->right) {
					RotateLeft(parent);
					node = parent;
					parent = node->parent;
				}

				RotateRight(grandparent);
			}
			else {
				if (node == parent->left) {
					RotateRight(parent);
					node = parent;
					parent = node->parent;
				}

				RotateLeft(grandparent);
			}

			swap(parent->color, grandparent->color);

			node = parent;
		}
	}

	root->color = black;
}

// node adjustment to satisfy all the properties of red-black tree after deleting
template <typename K, typename T>
void RBTreeMap<K, T>::FixRemoving(Node* node) {
	if (node == root) 
		return;

	Node* sibling = Sibling(node);
	Node* parent = node->parent;

	if (sibling == nullptr) { 
		FixRemoving(parent);
	}
	else {
		if (sibling->color == red) {  
			parent->color = red;
			sibling->color = black;

			if (sibling == sibling->parent->left) {
				RotateRight(parent);
			}
			else {
				RotateLeft(parent);
			}

			FixRemoving(node);
		}
		else { 
			if ((sibling->left && sibling->left->color == red) || (sibling->right && sibling->right->color == red)) { 
				if (sibling->left && sibling->left->color == red) {
					if (sibling == sibling->parent->left) {
						sibling->left->color = sibling->color;
						sibling->color = parent->color;

						RotateRight(parent);
					}
					else {
						sibling->left->color = parent->color;

						RotateRight(sibling);
						RotateLeft(parent);
					}
				}
				else {
					if (sibling == sibling->parent->left) {
						sibling->right->color = parent->color;

						RotateLeft(sibling);
						RotateRight(parent);
					}
					else {
						sibling->right->color = sibling->color;
						sibling->color = parent->color;

						RotateLeft(parent);
					}
				}

				parent->color = black;
			}
			else { 
				sibling->color = red;

				if (parent->color == black) {
					FixRemoving(parent);
				}
				else {
					parent->color = black;
				}
			}
		}
	}
}

// search by value
template <typename K, typename T>
typename RBTreeMap<K, T>::Node* RBTreeMap<K, T>::FindNode(const K& key) const {
	Node* node = root; 

	
	while (node) {
		
		if (node->key == key)
			return node; 

		
		if (key < node->key) {
			node = node->left; 
		}
		else {
			node = node->right; 
		}
	}

	return nullptr;
}

// deleting a node
template <typename K, typename T>
void RBTreeMap<K, T>::RemoveNode(Node* node) {
	Node* u = nullptr;

	
	if (node->left && node->right) {
		Node* tmp = node->right;

		while (tmp->left)
			tmp = tmp->left;

		u = tmp;
	}
	else {
		if (node->left) { 
			u = node->left;
		}
		else if (node->right) {
			u = node->right;
		}
	}

	bool uvBlack = (u == nullptr || u->color == black) && (node->color == black); 
	Node* parent = node->parent;

	
	if (u == nullptr) {
		if (node == root) {
			root = nullptr; 
		}
		else {
			if (uvBlack) { 
				FixRemoving(node);
			}
			else { 
				Node* sibling = Sibling(node);

				if (sibling) {
					sibling->color = red;
				}
			}

			
			if (node == node->parent->left) {
				parent->left = nullptr;
			}
			else {
				parent->right = nullptr;
			}
		}

		delete node;
		return;
	}

	
	if (node->left == nullptr || node->right == nullptr) {
		if (node == root) { 
			node->value = u->value;
			node->key = u->key;
			node->left = nullptr;
			node->right = nullptr;

			delete u;
		}
		else {
			 
			if (node == node->parent->left) {
				parent->left = u;
			}
			else {
				parent->right = u;
			}

			delete node;

			u->parent = parent;

			if (uvBlack) {
				FixRemoving(u);
			}
			else {
				u->color = black;
			}
		}

		return;
	}

	swap(u->value, node->value);
	swap(u->key, node->key);
	RemoveNode(u);
}


template <typename K, typename T>
void RBTreeMap<K, T>::Copy(Node* node, Node*& newNode) {
	if (node == nullptr)
		return;

	newNode = new Node;

	newNode->key = node->key;
	newNode->value = node->value;
	newNode->color = node->color;
	newNode->left = nullptr;
	newNode->right = nullptr;

	Copy(node->left, newNode->left); 
	Copy(node->right, newNode->right); 
}

// clear
template <typename K, typename T>
void RBTreeMap<K, T>::Clear(Node*& node) {
	if (node == nullptr)
		return;

	Clear(node->left); 
	Clear(node->right); 

	delete node; 
	node = nullptr; 
}

// getting the keys
template <typename K, typename T>
void RBTreeMap<K, T>::GetKeys(Node* node, vector<K>& keys) const {
	if (node == nullptr)
	{
		return;
		throw out_of_range("invalid argument");
	}
	else
	{
		GetKeys(node->left, keys);
		keys.push_back(node->key);
		GetKeys(node->right, keys);
		return;
	}
}

// getting the values
template <typename K, typename T>
void RBTreeMap<K, T>::GetValues(Node* node, vector<T>& values) const {
	if (node == nullptr) {
		return ;
		throw out_of_range("invalid argument");
	}
	else
	{
		GetValues(node->left, values);
		values.push_back(node->value);
		GetValues(node->right, values);
		return;
	}
}

// print
template <typename K, typename T>
void RBTreeMap<K, T>::Print(ostream& os, Node* node) const {
	if (node == nullptr) {
		throw out_of_range("invalid argument");
	}
		return;

	Print(os, node->left);
	os << "    " << node->key << ": " << node->value << endl;
	Print(os, node->right);
}

// constructor
template <typename K, typename T>
RBTreeMap<K, T>::RBTreeMap() {
	root = nullptr; 
}


template <typename K, typename T>
RBTreeMap<K, T>::RBTreeMap(const RBTreeMap& tree) {
	Copy(tree.root, root);
}


template <typename K, typename T>
RBTreeMap<K, T>& RBTreeMap<K, T>::operator=(const RBTreeMap& tree) {
	if (this == &tree)
		return *this;

	Clear(root); 
	Copy(tree.root, root); 

	return *this;
}

template <typename K, typename T>
void RBTreeMap<K, T>::Insert(const K& key, const T& value) {
	Node* current = root; 
	Node* parent = nullptr; 

	
	while (current != nullptr) {
		parent = current; 

		
		if (key < current->key) {
			current = current->left;
		}
		else {
			current = current->right;
		}
	}

	
	Node* node = new Node;

	node->key = key;
	node->value = value;
	node->parent = parent;
	node->left = nullptr;
	node->right = nullptr;
	node->color = red;

	
	if (parent) {
		if (key < parent->key) {
			parent->left = node;
		}
		else {
			parent->right = node;
		}
	}
	else {
		root = node; 
	}

	FixInsertion(node); 
}

template <typename K, typename T>
bool RBTreeMap<K, T>::Remove(const K& key) {
	Node* node = FindNode(key);

	
	if (node == nullptr)
		throw out_of_range("invalid argument");

	RemoveNode(node);

	return ; 
		
}


template <typename K, typename T>
void RBTreeMap<K, T>::Clear() {
	Clear(root); 
}


template <typename K, typename T>
bool RBTreeMap<K, T>::Find(const K& key) const {
	return FindNode(key) != nullptr;
}


template <typename K, typename T>
vector<K> RBTreeMap<K, T>::GetKeys() const {
	vector<K> keys;
	GetKeys(root, keys);
	return keys;
}


template <typename K, typename T>
vector<T> RBTreeMap<K, T>::GetValues() const {
	vector<T> values;
	GetValues(root, values);
	return values;
}


template <typename K, typename T>
T& RBTreeMap<K, T>::operator[](const K& key) {
	Node* node = FindNode(key);

	if (node == nullptr)
		throw out_of_range("invalid argument");

	return node->value;
}


template <typename K, typename T>
RBTreeMap<K, T>::~RBTreeMap() {
	Clear(root);
}


template <typename K, typename T>
ostream& operator<<(ostream& os, const RBTreeMap<K, T>& tree) {
	os << "{" << endl;
	tree.Print(os, tree.root);
	os << "}" << endl;
	return os;
}
void main()
{


}
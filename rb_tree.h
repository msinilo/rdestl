#ifndef RDESTL_RB_TREE_H
#define RDESTL_RB_TREE_H

#include "rdestl/rdestl.h"

namespace rde
{
namespace internal
{
	struct rb_tree_base
	{
		typedef size_t	size_type;
		enum color_e
		{
			red,
			black
		};
	};
} // internal

template<typename T>
class rb_tree : private internal::rb_tree_base
{
public:
	typedef T	key_type;

	struct node
	{
		color_e	color;
		T		key;
		node*	left;
		node*	right;
		node*	parent;
	};

	rb_tree()
	{
		m_sentinel.color	= black;
		m_sentinel.left		= &m_sentinel;
		m_sentinel.right	= &m_sentinel;
		m_sentinel.parent	= &m_sentinel;
		m_root				= &m_sentinel;
	}

	void insert(const key_type& key)
	{
		node* iter(m_root);
		node* parent(&m_sentinel);
		while (iter != &m_sentinel)
		{
			parent = iter;
			if (key > iter->key)
				iter = iter->right;
			else if (key < iter->key)
				iter = iter->left;
			else	// key == iter->key
				return;
		}

		node* new_node = alloc_node();
		new_node->color = red;
		new_node->key	= key;
		new_node->left	= &m_sentinel;
		new_node->right	= &m_sentinel;
		new_node->parent = parent;
		if (parent != &m_sentinel)
		{
			if (key > parent->key)
				parent->right = new_node;
			else
				parent->left = new_node;
		}
		else	// empty tree
			m_root = new_node;

		rebalance(new_node);
		validate();
	}

	bool empty() const	{ return m_root == &m_sentinel; }
	size_type size() const
	{
		return num_nodes(m_root);
	}

private:
	size_type num_nodes(const node* n) const
	{
		return n == &m_sentinel ? 0 : 1 + num_nodes(n->left) + num_nodes(n->right);
	}
	void rebalance(node* new_node)
	{
		RDE_ASSERT(new_node->color == red);
		node* iter(new_node);
		while (iter->parent->color == red)
		{
			node* grandparent(iter->parent->parent);
			if (iter->parent == grandparent->left)
			{
				node* uncle = grandparent->right;
				if (uncle->color == red)
				{
					iter->parent->color = black;
					uncle->color = black;
					grandparent->color = red;
					iter = grandparent;
				}
				else 
				{
					if (iter == iter->parent->right)
					{
						iter = iter->parent;
						rotate_left(iter);
					}
					grandparent = iter->parent->parent;
					iter->parent->color = black;
					grandparent->color = red;
					rotate_right(grandparent);
				}
			}
			else
			{
				node* uncle = grandparent->left;
				if (uncle->color == red)
				{
					grandparent->color = red;
					iter->parent->color = black;
					uncle->color = black;
					iter = grandparent;
				}
				else
				{
					if (iter == iter->parent->left)
					{
						iter = iter->parent;
						rotate_right(iter);
					}
					grandparent = iter->parent->parent;
					iter->parent->color = black;
					grandparent->color = red;
					rotate_left(grandparent);
				}
			}
		}
		m_root->color = black;
	}

	void validate() const
	{
		RDE_ASSERT(m_root->color == black);
		validate_node(m_root);

	}
	void validate_node(node* n) const
	{
		// - we're child of our parent.
		RDE_ASSERT(n->parent == &m_sentinel ||
			n->parent->left == n || n->parent->right == n);
		// - both children of red node are black
		if (n->color == red)
		{
			RDE_ASSERT(n->left->color == black);
			RDE_ASSERT(n->right->color == black);
		}
		if (n->left != &m_sentinel)
			validate_node(n->left);
		if (n->right != &m_sentinel)
			validate_node(n->right);
	}

	// n's right child replaces n and the right child's left child
	// becomes n's right child.
	void rotate_left(node* n)
	{
		// Right child's left child becomes n's right child.
		node* rightChild = n->right;
		n->right = rightChild->left;
		if (n->right != &m_sentinel)
			n->right->parent = n;

		// n's right child replaces n
		rightChild->parent = n->parent;
		if (n->parent == &m_sentinel)
		{
			m_root = rightChild;
		}
		else
		{
			if (n == n->parent->left)
				n->parent->left = rightChild;
			else
				n->parent->right = rightChild;
		}
		rightChild->left = n;
		n->parent = rightChild;
	}
	void rotate_right(node* n)
	{
		node* leftChild(n->left);
		n->left = leftChild->right;
		if (n->left != &m_sentinel)
			n->left->parent = n;

		leftChild->parent = n->parent;
		if (n->parent == &m_sentinel)
		{
			m_root = leftChild;
		}
		else
		{
			// Substitute us in the parent list with left child.
			if (n == n->parent->left)
				n->parent->left = leftChild;
			else
				n->parent->right = leftChild;
		}
		leftChild->right = n;
		n->parent = leftChild;
	}
	node* alloc_node()
	{
		return new node();
	}

	node	m_sentinel;
	node*	m_root;
};

} // rde

#endif // #ifndef RDESTL_RB_TREE_H

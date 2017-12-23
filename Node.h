#pragma once
template <class T>
class Node
{
public:
	T data;
	Node<T>* next;
	Node<T>* prep;
	Node(T item, Node<T>* nextnode = NULL, Node<T>* prepnode = NULL);
	Node() {};
	~Node() {};
};

template <class T>
Node<T>::Node(T item, Node<T>* nextnode, Node<T>* prepnode)
{
	data = item;
	next = nextnode;
	prep = prepnode;
}

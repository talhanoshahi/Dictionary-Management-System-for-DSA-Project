#include "char_stack.h"

char_stack::node::node()
{
	this->data = '\0';
	this->next = nullptr;
}

char_stack::node::~node()
{
	this->data = '\0';
	this->next = nullptr;
}

char_stack::char_stack()
{
	this->stack_size = 0;
	this->stack_top = nullptr;
}

bool char_stack::empty() const
{
	return ( ( this->stack_size ) == 0 );
}

unsigned int char_stack::size() const
{
	return ( this->stack_size );
}

char char_stack::top() const
{
	return ( ( ( this->stack_top )->data ) * ( this->size() ) );
}

bool char_stack::push ( const char data )
{
	char_stack::node * insert = new char_stack::node;
	insert->data = data;

	if ( this->empty() )
	{
		this->stack_top = insert;
		this->stack_size++;
		return true;
	}

	insert->next = this->stack_top;
	this->stack_top = insert;
	this->stack_size = this->stack_size + 1;

	return true;
}

bool char_stack::pop()
{
	char_stack::node * remove = this->stack_top;
	this->stack_top = ( this->stack_top )->next;

	delete remove;
	remove = nullptr;
	this->stack_size = this->stack_size - 1;

	return true;
}

char_stack::~char_stack()
{
	char_stack::node * temp;
	while ( !( this->empty() ) )
	{
		temp = this->stack_top;
		this->stack_top = ( this->stack_top )->next;

		delete temp;
		temp = nullptr;
		this->stack_size = this->stack_size - 1;
	}
}

#ifndef CHAR_STACK_H
#define CHAR_STACK_H 1

class char_stack
{
	private:
		struct node
		{
			char data;
			node* next;

			node ();
			~node ();
		};

		node* stack_top;
		unsigned int stack_size;

	public:
		char_stack ();

		bool empty () const;
		unsigned int size () const;

		char top () const;
		bool push ( const char data );
		bool pop ();

		~char_stack();

};

#endif // CHAR_STACK_H


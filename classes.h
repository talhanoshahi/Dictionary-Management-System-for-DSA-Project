#ifndef DICTIONARY_CLASSES
#define DICTIONARY_CLASSES 1

#include <string>

struct a_word
{
	std::string word;
	unsigned int key;
};

class user
{
	private:
		std::string name;
		std::string password;
		bool admin_prevliges;

	public:
		user ();
		user (const std::string & , const std::string &);
		user (const std::string & , const std::string & , const bool );

		void set_username ( const std::string & );
		void set_password ( const std::string & );
		void set_admin_prevliges ( const bool );

		std::string get_username () const;
		std::string get_password () const;
		bool get_admin_prevliges () const;

		~user ();

};

#endif // DICTIONARY_CLASSES


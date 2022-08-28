#include "classes.h"
#include <string>

user::user ()
{
	this->name = '\0';
	this->password = '\0';
	this->admin_prevliges = false;
}

user::user ( const std::string & name , const std::string & password )
{
	this->name = name;
	this-> password = password;
	this->admin_prevliges = false;
}

user::user ( const std::string & name , const std::string & password , const bool admin_prevliges)
{
	this->name = name;
	this-> password = password;
	this->admin_prevliges = admin_prevliges;
}

void user::set_username(const std::string & name)
{
	this->name = name;
}

void user::set_password(const std::string & password)
{
	this->password = password;
}

void user::set_admin_prevliges(const bool admin_prevliges)
{
	this->admin_prevliges = admin_prevliges;
}

std::string user::get_username() const
{
	return this->name;
}

std::string user::get_password() const
{
	return this->password;
}

bool user::get_admin_prevliges() const
{
	return this->admin_prevliges;
}

user::~user() {}

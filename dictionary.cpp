#include <fstream>
#include <termios.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <experimental/filesystem>
#include <map>
#include <climits>
#include <vector>
#include "classes.h"
#include "char_stack.h"

namespace fs = std::experimental::filesystem;

// functions or classes added from namespace std
using std::cout;
using std::cin;
using std::ws;
using std::cerr;
using std::string;
using std::stoul;
using std::vector;
using std::map;
using std::ifstream;
using std::ofstream;
using std::ios;

// functions or classes added from namespace fs (std::experimental::filesystem)
using fs::exists;
using fs::create_directory;
using fs::is_empty;

// functions before using signin or signup
void intro();
void select_signin_or_signup ();
void switch_signin_or_signup ( const int choice );
void signin();
void signup();
string return_admin_privleges ( const bool is_admin );

string input_password ();

// functions regarding file handling
void directory_existence ( const string & directory_name );
void database_existance ( );

// String related functions
string to_lowercase ( const string & to_uncaptalize );
string split_string( const string & string_to_split , const char symbol_to_split_at , const int position_to_return );
vector<string> split_string( const string & string_to_split , const char symbol_to_split_at );
string remove_symbols ( const string & source_string , const char symbol_to_remove );
string remove_boundary_spaces ( const string & source_string );
string find_in_string ( const string & source , const string & word_to_find );
int number_of_words_in_string ( const string & source );
bool sort_strings ( vector<string> & to_sort );

// variable to store user data at runtime
string username;
map<string, user> user_account;

// functions after user login or signup
void select_main_menu ();
void admin_main_menu ();
void non_admin_main_menu ();

// Database handling and dictionary's main functions
void select_action ( const int action );
void selected_insertion ();
void selected_deletion ();
void selected_updation ();
void selected_searching ();
void selected_palindrome ();
void selected_reversion ();

void insert_word ( const string & the_word , const unsigned int hash_key );
int delete_word ( const string & word_to_delete );
int update_word ( const string & word_to_update , const string & updated_word );
int search_word ( const string & word_to_find );
bool palindrome_check ( const string & check );
string reverse_word ( const string & );
void print_database ( );

// hash table functions
unsigned int hash_table ( const string & word_to_hash );

int main ()
{
	intro();
	select_signin_or_signup ();

	select_main_menu ();

	return 0;
}

void intro()
{
	system ( "clear" );

	std::cout << "Welcome to Dictionary Management System\n";
	std::cout << "\nThis program was developed by:\n";

	std::ifstream developers_file ("./developers/developers.dat");

	std::string read_roll_no;
	std::string read_name;

	if ( developers_file.is_open() )
	{
		while ( developers_file >> read_roll_no >> read_name )
		{
			cout << "\t" << read_roll_no << ": ";

			for ( int i = 0 ; i < read_name.size() ; i++ )
			{
				if ( read_name[i] == '_' )
				{
					cout << " ";

					continue;
				}

				cout << read_name[i];

			}

			cout << "\n";

		}

		developers_file.close();

		cin.get();

	}

}

void select_signin_or_signup ()
{
	system ( "clear" );

	int choice;
	bool condition;

	do
	{
		cout << "Please select the desired option: \n";
		cout << "\t1. Sign In\n";
		cout << "\t2. Sign Up\n";

		cout << "\n\tChoice: ";
		cin >> choice;

		condition = choice != 1 && choice != 2;

		if ( condition )
		{
			system ( "clear" );
			cerr << "error: choice should be 1 or 2.\n\n";
		}

	} while ( condition );

	switch_signin_or_signup(choice);

}

void switch_signin_or_signup ( const int choice )
{
	switch ( choice )
	{
		case 1:
			signin();
			break;

		case 2:
			signup();
			break;

		default:
			cerr << "error: wrong choice.\n";
	}
}

void signin ()
{
	system ( "clear" );

	directory_existence( "./accounts" ); // custom function to check if the passed directory exists

	string entered_username;
	string entered_password;

	int choice;
	bool condition; // a variable for storing conditions for do-while loop

	// taking input of username
	cout << "Username: ";
	cin >> entered_username;

	if ( !exists ( "./accounts/" + entered_username + ".acc" ) ) // checking if a file of given username exists in ./accounts directory
	{
		system ( "clear" );
		cerr << "error: account with username \"" << entered_username << "\" not found.\n";

		do
		{
			cout << "Do you want to:\n";

			cout << "\t1. Sign in again with a different account.\n";
			cout << "\t2. Sign up with a new account.\n";

			cout << "\n\tChoice: ";
			cin >> choice;

			condition = choice != 1 && choice != 2;

			if (condition)
			{
				system ( "clear" );
				cerr << "error: choice should be either 1 or 2.\n";
			}

		}while (condition);

		if ( choice == 1 )
		{
			signin();
			return;
		}

		if ( choice == 2 )
		{
			signup ();
			return;
		}

	}

	// reading user data from the file to compare to the data entered by the user
	ifstream read_user ( "./accounts/" + entered_username + ".acc" );
	string read_username;
	string read_password;
	string read_permissions;

	if ( read_user.is_open() )
	{
		read_user >> read_username;
		read_user >> read_password;
		read_user >> read_permissions;

		read_user.close();
	}


	if ( read_username == entered_username )
	{
		cout << "Password: ";
		entered_password = input_password();
	}

	user_account[entered_username] = user ( read_username , read_password , read_permissions == "is_admin" );

	cout << "\nSign in successfull.\n";
	username = entered_username;
	sleep ( 2 );

}

void signup ()
{
	system ( "clear" );

	directory_existence( "./accounts" ); // custom function to check if the passed directory exists

	int choice;
	bool condition;
	bool already_exists; // a variable to check if user already exists

	// taking input of username
	string enter_username;

	cout << "Username: ";
	cin >> enter_username;

	already_exists = exists ( "./accounts/" + enter_username + ".acc" );
	if ( already_exists ) // checking if a file of given username exists in ./accounts directory
	{
		system ( "clear" );
		cerr << "error: account with username \"" << enter_username << "\" already exist.\n";

		do
		{
			cout << "Do you want to:\n";

			cout << "\t1. Sign in with this account.\n";
			cout << "\t2. Sign up with a different account.\n";

			cout << "\n\tChoice: ";
			cin >> choice;

			condition = choice != 1 && choice != 2;

			if (condition)
			{
				system ( "clear" );
				cerr << "error: choice should be either 1 or 2.\n";
			}

		}while (condition);

		if ( choice == 1 )
		{
			signin();
			return;
		}

		if ( choice == 2 )
		{
			signup ();
			return;
		}

	}

	bool password_match;

	string enter_password;
	string confirm_password;

	do
	{
		cout << "Password: ";
		enter_password = input_password();

		cout << "\nConfirm Password: ";
		confirm_password = input_password();


		password_match = enter_password == confirm_password;

		if ( !password_match )
		{
			cout << "\nPasswords does not match. Please try again.\n\n";
		}
	}while ( !password_match );

	bool repeat_loop;
	char answer;

	do
	{
		cout << "\nDoes the account have admin previleges (Y/n): ";
		cin >> answer;
		cin.ignore();

		repeat_loop = answer != 'Y'  && answer != 'y' && answer != 'N' && answer != 'n';

		if ( repeat_loop )
		{
			cerr << "error: the answer should be yes (Y) or no (n).\n\n";
		}
	}while ( repeat_loop );

	string admin_password;
	string read_admin_password;

	ifstream admin_file( "./accounts/admin.acc" );

	if ( admin_file.is_open() )
	{
		getline ( admin_file , read_admin_password );
		getline ( admin_file , read_admin_password );

		admin_file.close();
	}

	int choose_account_type;

	if ( answer == 'Y' || answer == 'y' )
	{
		do
		{
			cout << "\nEnter admin password: ";
			admin_password = input_password();

			if ( admin_password != read_admin_password )
			{
				system ( "clear" );

				cerr << "error: entered admin password incorrect.\n";

				cout << "Do you want to:\n";
				cout << "\t1. Enter the password again.\n";
				cout << "\t2. Set the account type as non-admin.\n";
				cout << "\n\tChoice: ";
				cin >> choose_account_type;
			}

		}while ( admin_password != read_admin_password && choose_account_type == 1 );

	}

	string acc_type;
	acc_type = return_admin_privleges( admin_password == read_admin_password );

	user_account[enter_username] = user ( enter_username , enter_password , acc_type == "is_admin" );

	// storing user data to the file
	ofstream create_user_file ( "./accounts/" + enter_username + ".acc" );

	if ( create_user_file.is_open() )
	{
		create_user_file << enter_username << "\n";
		create_user_file << enter_password << "\n";
		create_user_file << acc_type;

		create_user_file.close();
	}

	username = enter_username;
	cout << "\nSign up successfull.\n";
	sleep( 2 );

}

string input_password ()
{
	termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    termios newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	cin.clear ( );
    string password;
	cin >> password;

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

	return password;
}

void directory_existence ( const string & directory_name )
{
	if ( !exists ( directory_name ) )
	{
		create_directory ( directory_name );
	}

	return;
}

string return_admin_privleges ( const bool admin_account )
{
	if ( admin_account )
	{
		return "is_admin";
	}

	return "non_admin";
}

void select_main_menu ()
{
	switch ( user_account[ username ].get_admin_prevliges() )
	{
		case true:
			admin_main_menu();
			break;

		case false:
			non_admin_main_menu();
			break;

		default:
			non_admin_main_menu();

	}
}

void admin_main_menu ()
{
	int step_to_take;

	do
	{
		system ( "clear" );

		do
		{
			cout << "Select the steps you want to take:\n";
			cout << "\t1. Insert a word in the database.\n";
			cout << "\t2. Delete a word from the database.\n";
			cout << "\t3. Update a word in the database.\n";
			cout << "\t4. Search (find) a word in the database.\n";
			cout << "\t5. Display all words from the database.\n";
			cout << "\t6. Exit.\n";
			cout << "\n\tStep: ";

			cin >> step_to_take;

			if ( step_to_take < 1 && step_to_take > 5 )
			{
				system ( "clear" );

				cerr << "error: step to take should be in between 1 and 5.\n\n";
			}

		}while ( step_to_take < 1 && step_to_take > 5 );

		if ( step_to_take < 6 )
			select_action (step_to_take);
	}while ( step_to_take < 6 );
}

void non_admin_main_menu ()
{
	int step_to_take;

	do
	{
		system ( "clear" );

		do
		{
			cout << "Select the steps you want to take:\n";
			cout << "\t1. Search (find) a word in the database.\n";
			cout << "\t2. Display all words from the database.\n";
			cout << "\t3. Check if a word is a palindrome.\n";
			cout << "\t4. Find the reverse of a word\n";
			cout << "\t5.Exit.\n";
			cout << "\n\tStep: ";

			cin >> step_to_take;

			if ( step_to_take < 1 && step_to_take > 4 )
			{
				system ( "clear" );

				cerr << "error: step to take should be in between 1 and 4.\n\n";
			}

		}while ( step_to_take < 1 && step_to_take > 4 );

		if ( step_to_take < 5 )
			select_action ( step_to_take + 3 );

	}while ( step_to_take < 5 );
}

void select_action ( const int action )
{
	switch ( action )
	{
		case 1:
			selected_insertion ( );

			break;

		case 2:
			selected_deletion ( );

			break;

		case 3:
			selected_updation ( );

			break;

		case 4:
			selected_searching ( );

			break;

		case 5:
			print_database ( );

			break;

		case 6:
			selected_palindrome ( );

			break;

		case 7:
			selected_reversion ( );

			break;

		default:
			cerr << "error: action unavailable.\n";
	}
}

void selected_insertion ()
{
	system ( "clear" );

	a_word word_to_insert;
	cout << "Enter the word to insert: ";
	getline ( cin >> ws , word_to_insert.word );
	word_to_insert.key = hash_table ( word_to_insert.word );
	word_to_insert.word = to_lowercase ( word_to_insert.word );

	if ( search_word ( word_to_insert.word ) > 0 )
	{
		cerr << "error: the word \"" << word_to_insert.word << "\" is already in the database.\n";
		cin.get();
		return;
	}

	insert_word( word_to_insert.word , word_to_insert.key );
}

void selected_deletion ()
{
	system ( "clear" );

	string word_to_delete;
	cout << "Enter the word to delete: ";
	getline ( cin >> ws , word_to_delete );
	word_to_delete = to_lowercase ( word_to_delete );

	if ( search_word ( word_to_delete ) < 0 )
	{
		cerr << "error: the word \"" << word_to_delete << "\" not found in the database";
		cin.get();
		return;
	}

	if ( delete_word( word_to_delete ) < 0 )
	{
		cerr << "error: unable to delete the word \"" << word_to_delete << "\"\n";
		cin.get();
		return;
	}

	cout << "The word \"" << word_to_delete << "\" deleted successfully.\n";
	cin.get();
	return;
}

void selected_updation ()
{
	system ( "clear" );

	string update, update_with;
	cout << "Enter the word you want to update: ";
	getline ( cin >> ws , update );
	update = to_lowercase ( update );

	if ( search_word ( update ) < 0 )
	{
		cerr << "error: the word \"" << update << "\" not found in the database";
		cin.get();
		return;
	}

	cout << "Enter the word you want to replace with: ";
	getline ( cin >> ws , update_with );
	update_with = to_lowercase ( update_with );

	update_word ( update , update_with );
	cerr << "The word \"" << update << "\" updated successfully.\n";
	cin.get();
	return;

}

void selected_searching ()
{
	system ( "clear" );

	string search;
	cout << "Enter the word you want to search: ";
	getline ( cin >> ws , search );
	search = to_lowercase ( search );

	int found_word = search_word ( search );

	if ( found_word > 0 )
	{
		if ( user_account[username].get_admin_prevliges() == true )
		{
			cout << "The word \"" << search << "\" found on line \"" << found_word << "\" of the database.\n";
			cin.get();
			return;
		}

		cout << "The word \"" << search << "\" found in the database.\n";
		cin.get();
		return;
	}

	cerr << "error: the word \"" << search << "\" not found in the database.\n";
	cin.get();
	return;

}

void selected_palindrome ()
{
	system ( "clear" );

	string palindrome;
	cout << "Enter the word to check its palindrome: ";
	cin >> palindrome;

	if ( palindrome_check ( palindrome ) )
	{
		cout << "\nThe word \"" << palindrome << "\" is a palindrome. \n";
		cin.get();
		cin.ignore();
		return;
	}

	cout << "\nThe word \"" << palindrome << "\" is not a palindrome. \n";
	cin.get();
	cin.ignore();
	return;
}

void selected_reversion ()
{
	system ( "clear" );

	string reverse;
	cout << "Enter the word to find its reverse: ";
	cin >> reverse;

	auto reversed = reverse_word ( reverse );
	cout << "\nThe reverse of the word \"" << reverse << "\" is: " << reversed << '\n';
	cin.get();
	cin.ignore();
	return;
}

void insert_word ( const string & the_word , const unsigned int hash_key )
{
	if ( !exists ( "./database/words.dat" ) )
	{
		database_existance();
	}

	if ( !is_empty ( "./database/words.dat" ) )
	{
		ifstream in_word ( "./database/words.dat" ); // in_word stands for input word

		if ( in_word.is_open () )
		{
			ifstream hash_compare_file ( "./database/words.dat" );
			ofstream temp_word ( "./database/temp_word.dat" );

			if ( hash_compare_file.is_open() && temp_word.is_open() )
			{
				string read_hash;
				string read_in_word;

				bool hash_exit = false;
				int hash_file_loop_index = -1;

				for ( int i = 0 ; getline ( hash_compare_file , read_hash ) ; i++ )
				{

					if ( stoul ( split_string ( read_hash , '|' , 0 ) ) >= hash_key )
					{
						hash_file_loop_index = i;
						hash_exit = true;
						break;
					}

					if ( i > 0 )
					{
						getline ( in_word , read_in_word );
						temp_word << read_in_word << '\n';
					}

				}

				if ( !hash_exit )
				{
					if ( hash_compare_file.eof() )
					{
						getline ( in_word , read_in_word );
						temp_word << read_in_word << '\n';
						temp_word << hash_key << " | " << the_word;
					}
				}

				if ( hash_exit )
				{
					if ( stoul ( split_string ( read_hash , '|' , 0 ) ) == hash_key )
					{
						getline ( in_word , read_in_word );
						temp_word << read_in_word << " , " << the_word << '\n';
					}

					else if ( stoul ( split_string ( read_hash , '|' , 0 ) ) > hash_key )
					{
						if ( hash_file_loop_index == 0 )
						{
							temp_word << hash_key << " | " << the_word << '\n';
						}

						else
						{
							getline ( in_word , read_in_word );
							temp_word << read_in_word << '\n';
							temp_word << hash_key << " | " << the_word << '\n';

						}

						for  ( int i = 0 ; !in_word.eof () ; i++ )
						{
							getline ( in_word , read_in_word );

							if ( i == 0 )
							{
								temp_word << read_in_word;
								continue;
							}

							temp_word << '\n' << read_in_word;
						}

					}
				}

				hash_compare_file.close ();

				in_word.close ();
				temp_word.close ();

				fs::remove ( "./database/words.dat" );
				fs::rename ( "./database/temp_word.dat" , "./database/words.dat" );
			}
		}

	}

	if ( is_empty( "./database/words.dat" ) )
	{
		ofstream in_word ( "./database/words.dat" );

		if ( in_word.is_open () )
		{
			in_word << hash_key << " | " << the_word;
			in_word.close ();
		}
	}

}

int delete_word ( const string & word_to_delete )
{
	ifstream in_word ( "./database/words.dat" ); // in_word stands for input word

	if ( in_word.is_open () )
	{
		ifstream hash_compare_file ( "./database/words.dat" );
		ofstream temp_word ( "./database/temp_word.dat" );

		if ( hash_compare_file.is_open() && temp_word.is_open() )
		{
			string read_hash;
			string read_in_word;

			bool hash_exit = false;
			int hash_file_loop_index = -1;
			const unsigned int hash_key = hash_table( word_to_delete );
			unsigned int read_hash_key = 0;

			for ( int i = 0 ; getline ( hash_compare_file , read_hash ) ; i++ )
			{
				read_hash_key = stoul ( split_string ( read_hash , '|' , 0 ) );
				if ( read_hash_key == hash_key )
				{
					hash_file_loop_index = i;
					hash_exit = true;
					break;
				}

				if ( i > 0 )
				{
					getline ( in_word , read_in_word );
					temp_word << read_in_word << '\n';
				}

			}

			if ( hash_exit )
			{
				if ( hash_file_loop_index == 0 )
				{
					getline ( in_word , read_in_word );
				}

				else
				{
					getline ( in_word , read_in_word );
					temp_word << read_in_word << '\n';

					getline ( in_word , read_in_word );
					vector<string> individual_words = split_string ( split_string ( read_in_word , '|' , 1 ) , ',' );

					for ( auto i : individual_words )
					{
						i = remove_boundary_spaces ( i );
					}

					if ( individual_words.size() > 1 && individual_words[0] != word_to_delete )
					{
						temp_word << hash_key << " | ";

						for ( int i = 0 ; i < individual_words.size() ; i++ )
						{
							if ( individual_words[i] == word_to_delete )
								continue;

							if ( i == ( individual_words.size() - 1 ) )
							{
								temp_word << individual_words[i] << '\n';
								continue;
							}

							temp_word << individual_words[i] << " , ";
						}
					}
				}

				for  ( int i = 0 ; !in_word.eof () ; i++ )
				{
					getline ( in_word , read_in_word );

					if ( i == 0 )
					{
						temp_word << read_in_word;
						continue;
					}

					temp_word << '\n' << read_in_word;
				}

			}

			hash_compare_file.close ();

			in_word.close ();
			temp_word.close ();

			fs::remove ( "./database/words.dat" );
			fs::rename ( "./database/temp_word.dat" , "./database/words.dat" );

			return 0;
		}
	}

	if ( is_empty( "./database/words.dat" ) )
	{
		return -3;
	}

	return -1;
}


int update_word ( const string & word_to_update , const string & updated_word )
{
	delete_word ( word_to_update );
	insert_word ( updated_word , hash_table (updated_word ) );
	return 0;
}

int search_word ( const string & word_to_find )
{
	if ( !exists ( "./database/words.dat" ) )
	{
		return -2; // -2 means database does not exist
	}

	if ( is_empty( "./database/words.dat" ) )
	{
		return -1; // -1 means the file is empty
	}

	if ( !is_empty ( "./database/words.dat" ) )
	{
		ifstream in_word ( "./database/words.dat" ); // in_word stands for input word

		if ( in_word.is_open () )
		{
			string read_in_word;

			bool hash_exit = false;
			int hash_file_loop_index = -1;
			const unsigned int hash_key = hash_table( word_to_find );
			unsigned int read_hash_key = 0;

			for ( int i = 0 ; getline ( in_word , read_in_word ) ; i++ )
			{
				read_hash_key = stoul ( split_string ( read_in_word , '|' , 0 ) );
				if ( read_hash_key >= hash_key )
				{
					hash_file_loop_index = i;
					hash_exit = true;
					break;
				}

			}

			if ( !hash_exit )
			{
				if ( in_word.eof() )
				{
					in_word.close();
					return -3; // -3 means the required word not found int the file
				}
			}

			if ( hash_exit )
			{
				if ( read_hash_key == hash_key )
				{
					string find_word = find_in_string ( split_string( read_in_word, '|' , 1 ) , word_to_find ) ;
					if ( find_word == word_to_find )
					{
						in_word.close();
						return ( hash_file_loop_index + 1 );
					}

					in_word.close();
					return -3; // word not found in file
				}

				else if ( read_hash_key > hash_key )
				{
					in_word.close();
					return -3; // -3 means the required word not found in the file
				}
			}

			in_word.close ();
		}

	}

	return -10; // the function somehow managed to execute uptill here
}

bool palindrome_check ( const string & check )
{
	string check_copy = to_lowercase ( check );
	string reversed = reverse_word ( check_copy );
	return ( check_copy == reversed );
}

string reverse_word ( const string & reverse )
{
	char_stack reverser;

	for ( auto i : reverse )
	{
		reverser.push ( i );
	}

	string reversed;
	while ( !reverser.empty() )
	{
		reversed.push_back ( reverser.top() );
		reverser.pop();
	}

	return reversed;
}

void print_database ( )
{
	cin.ignore();
	if ( exists ( "./database/words.dat" ) )
	{
		const int word_array_count = ( ( (int) ('z') ) - ( (int) ('a') ) );
		vector<string> word_array[word_array_count]= {{}};

		vector<string> read_from_file;
		string read;
		ifstream words_file ( "./database/words.dat" );

		if ( words_file.is_open() )
		{
			while ( getline ( words_file , read ) )
			{
				read_from_file = split_string ( split_string ( read , '|' , 1 ) , ',' );

				for ( auto i : read_from_file )
				{
					i = remove_boundary_spaces ( i );
					word_array [ ( (int) (i[0] ) ) - 97 ].push_back ( i );
				}
			}

			words_file.close();
		}

		for ( int i = 0 ; i < word_array_count ; i++ )
		{
			sort_strings ( word_array[i] );
		}

		system ( "clear" );

		cout << "\t\t\tPrinting Database\n";
		for ( int i = 0 ; i < word_array_count ; i++ )
		{
			if ( word_array[i].size() > 0 )
			{
				cout << ( (char) ( i + 97) ) << ": \n";
				for ( auto j : word_array[i] )
				{
					cout << '\t' << j << '\n';
				}
				cout << '\n';
			}
		}

		cout << "Press return key to continue... ";
		cin.get();
	}
}

void database_existance ( )
{
	string database_directory = "./database";

	directory_existence ( database_directory );

	if ( !exists ( database_directory + "/" + "words.dat" ) )
	{
		ofstream words_file ( "./database/words.dat" );

		if ( words_file.is_open() )
			words_file.close();

	}

}

unsigned int hash_table ( const string & word_to_hash )
{
	int length_of_word = word_to_hash.length();
	unsigned int hash_key = 0;

	for ( int i = 0 ; i < length_of_word ; i++ )
	{
		hash_key = hash_key + word_to_hash[i];
		hash_key = hash_key * word_to_hash[i];
	}

	return hash_key % UINT_MAX;
}

string to_lowercase ( const string & to_uncaptalize )
{
	string un_capitalized = to_uncaptalize;

	un_capitalized[0] = tolower( un_capitalized[0] );

	return un_capitalized;
}

string split_string( const string & string_to_split , const char symbol_to_split_at , const int position_to_return )
{
	vector <string> splitted_words;
	int distance_from_last_cut = 1;

	for (int i = 0 , j = 0 ; i < string_to_split.length() ; i++)
	{
		if (string_to_split[i] != symbol_to_split_at )
		{
			if (distance_from_last_cut == 1)
			{
				std::string new_word="\0";
				new_word[0] = string_to_split[i];
				splitted_words.push_back (new_word);
				distance_from_last_cut ++;
			}
			else
			{
				splitted_words[j] += string_to_split[i];
			}
		}
		else if (string_to_split [i] == symbol_to_split_at )
		{
			distance_from_last_cut = 1;
			j++;
		}
	}

	return splitted_words[position_to_return];
}

vector<string> split_string( const string & string_to_split , const char symbol_to_split_at )
{
	vector <string> splitted_words;
	int distance_from_last_cut = 1;

	for (int i = 0 , j = 0 ; i < string_to_split.length() ; i++)
	{
		if (string_to_split[i] != symbol_to_split_at )
		{
			if (distance_from_last_cut == 1)
			{
				std::string new_word;
				new_word += string_to_split[i];
				splitted_words.push_back (new_word);
				distance_from_last_cut ++;
			}
			else
			{
				splitted_words[j] += string_to_split[i];
			}
		}
		else if (string_to_split [i] == symbol_to_split_at )
		{
			distance_from_last_cut = 1;
			j++;
		}
	}

	return splitted_words;
}

string remove_symbols ( const string & source_string , const char symbol_to_remove )
{
	string string_without_symbol = source_string;

	for ( int i = 0 ; i < string_without_symbol.length() ; i++ )
	{
		if ( string_without_symbol[i] == symbol_to_remove )
		{
			string_without_symbol.erase ( i );
		}

	}

	return string_without_symbol;
}

string remove_boundary_spaces ( const string & source_string )
{
	string remove_space = source_string;

	while ( remove_space.front () == ' ' )
		remove_space.erase ( remove_space.begin() );

	while ( remove_space.back () == ' ' )
		remove_space.pop_back ();

	return remove_space;
}

string find_in_string ( const string & source , const string & word_to_find )
{
	const char to_split_at = ',';
	vector<string> splitted_words = split_string ( source , to_split_at );

	for ( int i = 0 ; i < splitted_words.size() ; i++ )
	{
		splitted_words[i] = remove_boundary_spaces ( splitted_words[i] );

		if ( splitted_words[i] == word_to_find )
			return splitted_words[i];

	}

	return {};
}

int number_of_words_in_string ( const string & source )
{
	const char to_split_at = ',';
	vector<string> splitted_words = split_string ( source , to_split_at );

	return splitted_words.size();
}

bool sort_strings ( vector<string> & to_sort )
{
	if ( to_sort.size() <= 0 )
	{
		return false;
	}

	string temp;
	for ( int i = 0 ; i < to_sort.size() - 1 ; i++ )
	{
		for ( int j = 0 ; j < to_sort.size() - 1 ; j++ )
		{
			if ( to_sort[i] > to_sort[j] )
			{
				temp = to_sort[i];
				to_sort[i] = to_sort[j];
				to_sort[j] = temp;
			}
		}
	}

	return true;
}

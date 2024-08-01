#include <vector>
#include <iostream>

#include "Client.hpp"

class Channel {

	public :

		Channel(){};
		~Channel(){};

		// Getters //

		std::string	get_channel_name(){ return this->_name; }

		// Setters //

		void	set_channel_name( std::string name ){ this->_name = name; }

		// Functions //

		void	add_new_client( Client client ){ this->_client_register.push_back(client); }
		void	print_client();
		bool	check_existing_client( int fd );

		// Clear Functions //

		void	client_clear( int fd );

	private :

	std::string	_name;

	std::vector<Client> _client_register;
};
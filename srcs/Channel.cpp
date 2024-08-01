#include "Channel.hpp"

// FUNCTIONS //

void	Channel::print_client() {

	std::cout << "Channel : " << get_channel_name() << std::endl;

	for (size_t i = 0; i < this->_client_register.size(); i++) {

		std::cout << "Client : " << this->_client_register[i].get_client_fd() << std::endl;
	}
}

bool	Channel::check_existing_client( int fd ) {

	for (size_t i = 0; i < _client_register.size(); i++) {

		if (_client_register[i].get_client_fd() == fd)
			return true;
	}
	return false;
}

// CLEAR FUNCTIONS //

void	Channel::client_clear( int fd ) {

	for (size_t i = 0; i < _client_register.size(); i++) {

		if (_client_register[i].get_client_fd() == fd)
			{_client_register.erase(_client_register.begin() + i); break;}
	}
}
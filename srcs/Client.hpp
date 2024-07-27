#include <iostream>

class Client {

	public :

		Client(){};
		~Client(){};

		// Getters //
		int			get_client_fd(){ return this->client_fd; };

		// Setters //
		void		set_client_fd( int fd ){ this->client_fd = fd; };
		void		set_client_ip_addr( std::string ip_addr ){ this->client_ip_addr = ip_addr; };

	private :

		int			client_fd;
		std::string client_ip_addr;
};
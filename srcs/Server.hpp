#include <unistd.h>
#include <string.h>

#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <poll.h>

#include "Client.hpp"

class Server {

	public :

		Server(){};
		~Server(){};

		// Getters //
		int				get_socket_fd(){ return this->_socket_fd; };
		int				get_port(){ return this->_port; };
		std::string		get_password(){ return this->_password; };

		// Setters //
		void			set_socket_fd( int fd ){ this->_socket_fd = fd; };
		void			set_port( int port ){ this->_port = port; };
		void			set_password( std::string password ){ this->_password = password; };

		// Fonctions //
		void			server_init( int port, std::string password );
		void			socket_creation();
		void			new_client_request();
		void			data_receiver( int fd );

		void			client_clear( int fd );
		void			close_socket_fd();
		static void		signal_handler( int signum );

	private :

		int							_port;
		int							_socket_fd;
		std::string					_password;
		std::vector<Client>			_client_register;
		std::vector<struct pollfd>	_pollfd_register;
		static bool					_signal;
};
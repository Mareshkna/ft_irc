#include "Server.hpp"

void	Server::server_init( int port, std::string password ) {

	set_port(port);
	set_password(password);
	socket_creation();
	std::cout << "Connected and waiting for requests..." << std::endl;

	while (_signal == false)
	{
		if((poll(&_pollfd_register[0], _pollfd_register.size(), -1) < 0) && _signal == false)
			throw(std::runtime_error("poll() faild"));

		for (size_t i = 0; i < _pollfd_register.size(); i++)
		{
			if (_pollfd_register[i].revents & POLLIN)
			{
				if (_pollfd_register[i].fd == get_socket_fd())
					new_client_request();
				else
					data_receiver(_pollfd_register[i].fd);
			}
		}
	}
	close_socket_fd();
}

void	Server::socket_creation() {

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(get_port());

	// Creating socket file descriptor
	set_socket_fd(socket(AF_INET, SOCK_STREAM, 0));
	if (get_socket_fd() < 0) {
		throw(std::runtime_error("failed to create socket"));
	}

	// Setting socket
	int opt = 1;
	if (setsockopt(get_socket_fd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		throw(std::runtime_error("failed to socket opt"));
	}
	if (fcntl(get_socket_fd(), F_SETFL, O_NONBLOCK) < 0) {
		throw(std::runtime_error("failed to fcntl"));
	}
	if (bind(get_socket_fd(), (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		throw(std::runtime_error("failed to bind"));
	}
	if (listen(get_socket_fd(), SOMAXCONN) < 0) {
		throw(std::runtime_error("failed to listen"));
	}

	struct pollfd pollfd;
	pollfd.fd = get_socket_fd();
	pollfd.events = POLLIN;
	pollfd.revents = 0;
	_pollfd_register.push_back(pollfd);
}

void Server::new_client_request() {

	Client client;
	struct sockaddr_in addr;
	struct pollfd pollfd;
	socklen_t addr_len = sizeof(addr);

	int connect_fd = accept(get_socket_fd(), (sockaddr *)&(addr), &addr_len);
	if (connect_fd == -1)
		{std::cout << "accept() failed" << std::endl; return;}

	if (fcntl(connect_fd, F_SETFL, O_NONBLOCK) == -1)
		{std::cout << "fcntl() failed" << std::endl; return;}

	pollfd.fd = connect_fd;
	pollfd.events = POLLIN;
	pollfd.revents = 0;

	client.set_client_fd(connect_fd);
	client.set_client_ip_addr(inet_ntoa((addr.sin_addr)));
	_client_register.push_back(client);
	_pollfd_register.push_back(pollfd);

	std::cout << "Client [" << connect_fd << "] Connected" << std::endl;
}

void Server::data_receiver( int fd ) {

	char buff[1024];
	memset(buff, 0, sizeof(buff));

	ssize_t bytes = recv(fd, buff, sizeof(buff) - 1 , 0);

	if (bytes <= 0) {

		std::cout << "Client <" << fd << "> Disconnected" << std::endl;
		client_clear(fd);
		close(fd);
	}

	else {

		buff[bytes] = '\0';
		std::cout << "Client [" << fd << "] Data : " << buff;
		// Parsing data here
	}
}

void	Server::client_clear( int fd ) {

	for (size_t i = 0; i < _pollfd_register.size(); i++) {

		if (_pollfd_register[i].fd == fd)
			{_pollfd_register.erase(_pollfd_register.begin() + i); break;}
	}

	for (size_t i = 0; i < _client_register.size(); i++) {

		if (_client_register[i].get_client_fd() == fd)
			{_client_register.erase(_client_register.begin() + i); break;}
	}
}

void	Server::close_socket_fd() {

	for(size_t i = 0; i < _client_register.size(); i++) {

		std::cout << "Client [" << _client_register[i].get_client_fd() << "] Disconnected" << std::endl;
		close(_client_register[i].get_client_fd());
	}

	if (get_socket_fd() != -1) {

		std::cout << "Server [" << get_socket_fd() << "] Disconnected" << std::endl;
		close(_socket_fd);
	}
}

bool Server::_signal = false;

void Server::signal_handler( int signum ) {

	(void)signum;
	std::cout << std::endl << "Signal Received !" << std::endl;
	_signal = true;
}
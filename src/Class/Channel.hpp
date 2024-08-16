#ifndef Channel_hpp
# define Channel_hpp

# include "Client.hpp"
# include "Server.hpp"
# include "Command.hpp"

class Server;
class Client;
class Command;

class Channel
{
    public:
		Channel(std::string topic, const Client& creator);
		~Channel();
        void    addClient(Client& client);
        void    delClient(Client& client);
	private :	
		Channel();
		Channel(const Channel& src);
		Channel& operator=(const Channel& src);
        std::vector<Client&> _clients;
        std::vector<Client&> _operators;
        std::string          _channel_topic;
        std::string          _channel_mode;
};


#endif
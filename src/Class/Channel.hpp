#ifndef Channel_hpp
# define Channel_hpp

# include "../irc.hpp"


class Server;
class Client;
class Command;

class Channel
{
    public:
		Channel();
		Channel(const std::string& topic, Client& creator);
		Channel(const Channel& src);
		Channel& operator=(const Channel& src);
		~Channel();
        void    addClient(Client& client);
        void    delClient(Client& client);
        void	delChannel();

	private :	
        std::vector<Client*> _clients;
        std::vector<Client*> _operators;
        std::string          _channel_topic;
        std::string          _channel_mode;
};


#endif
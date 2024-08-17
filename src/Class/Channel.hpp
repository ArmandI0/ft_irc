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
		Channel(const std::string& topic, Client& creator, Server* serv);
		Channel(const Channel& src);
		Channel& operator=(const Channel& src);
		~Channel();
		void					addClient(Client& client);
		void					delClient(Client& client);
		void					delChannel();
		std::map<int,Client*>	getOperators();
		std::map<int,Client*>	getClients();
		std::string				getChannelTopic();
		std::string				getChannelModes();
		bool					isModeOn(char mode);
		void					changeChannelMode(std::string& str);

	private :	
		std::map<int,Client*>	_clients;
		std::map<int,Client*>	_operators;
		std::string						_channel_topic;
		std::string						_channel_mode;
		Server*							_server;
};

#endif
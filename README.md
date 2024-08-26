# ft_irc

## Norme a respecter pour le serveur :
https://www.rfc-editor.org/rfc/rfc1459#section-1.1

## commandes

1ère salve:
USER
NICK
PASS
QUIT
PRIVMSG (only direct messages)
PING
PONG


2ème salve (channel):
JOIN
PART
PRIVMSG
MODE
TOPIC
INVITE
KICK


:DCC SEND input.csv 2130706433 41223 447
> @time=2024-08-26T13:28:38.618Z :nledent2!nledent2@nerd-9AE5B52D.unyc.it JOIN #home * :realname
:<nick>!<user> JOIN #channel * :<realname>
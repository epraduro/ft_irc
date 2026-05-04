<h1> FT_IRC </h1>
<p align="center"> 
  <img src="https://img.shields.io/badge/42-Project-black?style=for-the-badge&logo=42"> 
  <img src="https://img.shields.io/badge/Language-C++-blue?style=for-the-badge&logo=c%2B%2B"> 
  <img src="https://img.shields.io/badge/Protocol-IRC-purple?style=for-the-badge"> 
  <img src="https://img.shields.io/badge/Networking-TCP/IP-green?style=for-the-badge"> 
</p>

<h2> Description </h2>

ft_irc est une implémentation d’un serveur IRC (Internet Relay Chat) en C++, réalisée dans le cadre du cursus 42.

<p> IRC est un protocole de communication en temps réel permettant: </p>
•  discussions en groupe (channels) </br>
•  messages privés </br>
•  communication client ↔ serveur </br>

<p> Le projet consiste à recréer ce système from scratch en respectant les RFC officiels </p>

<h2> Objectifs </h2>
•  Implémenter un serveur réseau en C++ </br>
•  Gérer plusieurs connexions simultanées </br>
•  Comprendre les protocoles réseau (IRC, TCP/IP) </br>
•  Implémenter des commandes IRC </br>
•  Gérer des I/O non bloquants </br>

<h2> Fonctionnalités </h2>

•  Connexion de plusieurs clients simultanément </br>
•  Authentification (PASS, NICK, USER) </br>
•  Gestion des channels </br>
•  Messages publics et privés </br>
•  Gestion des opérateurs </br>

<h2> Fonctionnement </h2>
•  Le serveur écoute sur un port TCP </br>
•  Les clients se connectent via un client IRC </br>
•  Les messages sont routés : </br>
&nbsp;&nbsp;&nbsp;• vers un utilisateur </br>
&nbsp;&nbsp;&nbsp;• vers un channel </br>

Le protocole IRC est textuel et basé sur des commandes

<h2> Commandes IRC </h2>

<h4> Commandes de base </h4>

| <p align="center"> Commande </p> | <p align="center"> Description </p>|
| -------- | -------- |
|  <p align="center">`PASS`</p>    |  <p align="center">Authentification </p>    |
|  <p align="center">`NICK` </p>   |  <p align="center">Définir pseudo   </p>    |
|  <p align="center">`USER` </p>   |  <p align="center">Définir utilisateur </p> |
|  <p align="center">`JOIN` </p>   |  <p align="center">Rejoindre un channel</p> |
|  <p align="center">`PART` </p>   |  <p align="center">Quitter un channel</p>   |
|  <p align="center">`PRIVMSG`</p> |  <p align="center">Message privé     </p>   |

<h4> Commandes opérateur </h4>

Permissions opérateur:

| <p align="center"> Commande </p> | <p align="center"> Description   </p>           |
| -------- | ----------------------- |
| <p align="center"> `KICK` </p>   | <p align="center">Expulser un utilisateur </p> |
| <p align="center"> `INVITE`</p>  | <p align="center">Inviter un utilisateur </p>  |
| <p align="center"> `TOPIC` </p>  | <p align="center">Modifier le sujet    </p>    |
| <p align="center"> `MODE` </p>   | <p align="center">Modifier les règles  </p>    |

<h4> Mode de channel </h4>

| <p align="center"> Mode | <p align="center"> Description </p> |
| ---- | ------------------------------------------------- |
| <p align="center"> `+i` </p> | <p align="center"> Channel sur invitation uniquement   </p>           |
| <p align="center"> `+t` </p> | <p align="center"> Seuls les opérateurs peuvent modifier le topic  </p>|
| <p align="center"> `+k` </p> | <p align="center"> Mot de passe requis pour rejoindre </p>             |
| <p align="center"> `+o` </p> | <p align="center"> Donner le statut d’opérateur    </p>               |
| <p align="center"> `+l` </p> | <p align="center"> Limite de nombre d’utilisateurs   </p>             |

<h4> Exemple d'utlisation </h4>

    MODE #channel +i
    MODE #channel +k secret
    MODE #channel +o user42
    MODE #channel +l 10

<h2> Installation </h2>

    git clone https://github.com/epraduro/ft_irc.git
    cd ft_irc
    make

<h2> Lancer le server </h2>

    ./ircserv <port> <password>

Exemple :

    ./ircserv 6667 mypass

<h2> Connexion avec un client IRC </h2>

<h4> Tu peux utiliser : </h4>

netcat:

    nc 127.0.0.1 6667

ou un client IRC classique:

    /connect 127.0.0.1 6667
    /pass mypass

<h2> Technologies </h2>
•  C++98 </br>
•  Sockets (TCP/IP) </br>
•  poll() / select() </br>
•  Makefile </br>

<h2> Exemple d’utilisation </h2>

    NICK user1
    USER user1 0 * <Real_Name>
    JOIN #general
    PRIVMSG #general <text>

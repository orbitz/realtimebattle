/*
 * Network interface for RealTimeBattle clients
 * Copyright (C) 2005 Volker Stroebel <volker@planetpenguin.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "ClientInterface.h"

#include "ArenaController.h"
#include "ArenaRealTime.h"
#include "Various.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>

#include <sstream>

/*
 * The first client that registers itself to the server
 * automatically becomes the master client that is
 * allowed to control the core (start/end tournaments).
 * This is a dirty hack and should be removed with a real
 * authentification system in the future. 
 */
static bool has_master = false;


ClientConnection::ClientConnection(int sd):_socket(sd),
_initialized(false), _master(false)
{
    //we only need to set the socket nonblocking
    int opts = fcntl(_socket, F_GETFL);
    if (opts < 0) {
	cerr << "ClientConnection: Unable to get flags for stream -> " <<
	    strerror(errno) << endl;
    }

    opts = (opts | O_NONBLOCK);
    if (fcntl(_socket, F_SETFL, opts) < 0) {
	cerr << "ClientConnection: Unable to set flags for stream -> " <<
	    strerror(errno) << endl;
    }
}

/**
 * Sends exiting message to client and closes the socket
 */
ClientConnection::~ClientConnection()
{
    write(_socket, "Cexiting\n", 10);
    close(_socket);
}


/**
 * Check for messages from the client
 * Returns false if client connection is broken
 */
bool ClientConnection::loop()
{
    int rc;

    // read only once per 'tick' to minimize denial of service attacks
    if ((rc = read(_socket, _buffer,4096)) > 0) {
	char *pos = _buffer;
	for (int i = 0; i < 4096; ++i) {
	    // received last byte, but no valid line was constructed at this moment
	    // => wait until we get the next part of the message
	    if (i == rc) {
		_buffer[i]='\0';
		_line += pos;
		break;
	    // got a newline, now we can append the received data to the already received data in the previous ticks
	    } else if (_buffer[i] == '\n') {
		_buffer[i] = '\0';
		_line += pos;

		if (!parse_line(_line)) {
		    //client talks not our language
		    return false;
		}
		// now we clean the line
		_line.clear();
		pos = _buffer+i+1;
	    // string termination should only appear after a newline
	    } else if (_buffer[i] == '\0') {
		    // just ignore
		    pos= _buffer+i+1;
		    continue;
	    }
	}
	// we have received some data, so client is still alive
	return true;
    }

    if (rc == 0) {
	// EOF
	if (!_line.empty()) {
	    parse_line(_line);
	}
	return false;
    } else if (errno != EAGAIN) {
	cerr <<
	    "ClientConnection::Loop() -- Error receiving data from client -> "
	    << strerror(errno) << endl;
	return false;
    }
    return true;
}

/**
 * Send message to client
 */
int ClientConnection::send(const char *message, int len)
{
    return::send(_socket, message, len, 0);
}

/**
 * Parse line received from clients
 */
bool ClientConnection::parse_line(string & message)
{
    if (_initialized) {
	switch (_line[0]) {
	case 'C':
	    if (_master) {
		if (_line.length() > 4) {
		    parse_command(_line[1], message.substr(3));
		} else {
		    cerr << "Received Command with no argument from client"
			<< endl;
		}
	    }
	    break;
	default:
	    cerr << "Unknown message from client" << endl;
	}
    } else {
	//performing handshake with client if possible
	if (message == "WELCOME SERVER") {
	    if (!has_master) {
		write(_socket, "WELCOME MASTER\n",
		      16);
		cout << "register client as master" << endl;
		has_master = true;
		_master = true;
	    } else {
		write(_socket, "WELCOME OBSERVER\n", 18);
		cout << "register client as observer" << endl;
	    }
	    _initialized = true;
	} else {
	    cerr << "Cannot understand client: " << message << endl;
	    return false;
	}
    }
    return true;
}

/**
 * Parse command received from clients
 */
void ClientConnection::parse_command(const char command,const string& data)
{
    switch (command) {
    case 'N':
	cout << "Start new game: " << data << endl;
	the_arena_controller.start_realtime_arena();
	parse_tournament_file(data, (StartTournamentFunction)
			      ArenaRealTime::
			      start_tournament_from_tournament_file,
			      &realtime_arena);
	break;
    default:
	cerr << "Unknown command from client" << endl;
    }
}


auto_ptr < ClientInterface > ClientInterface::_instance(0);
int ClientInterface::port(32134);

void ClientInterface::set_port(int portnumber) {
	ClientInterface::port=portnumber;
}


ClientInterface::ClientInterface():_is_accepting(true)
{
    struct sockaddr_in servAddr;

    // create socket
    _sd = socket(AF_INET, SOCK_STREAM, 0);
    if (_sd < 0) {
	cerr << "cannot open socket: " << strerror(errno) << endl;
	return;
    }
    // set socket nonblocking
    {
	int opts;
	opts = fcntl(_sd, F_GETFL);
	if (opts < 0) {
	    cerr << "fcntl(F_GETFL): " << strerror(errno) << endl;
	}

	opts = (opts | O_NONBLOCK);
	if (fcntl(_sd, F_SETFL, opts) < 0) {
	    cerr << "fcntl(F_SETFL): " << strerror(errno) << endl;
	}
    }

    // bind server port
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(ClientInterface::port);

    if (bind(_sd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
	cerr << "cannot bind port: " << strerror(errno) << endl;
	return;
    }

    listen(_sd, 5);
}

/**
 * Deletes all registered ClientConnections
 * and closes network connection
 */
ClientInterface::~ClientInterface()
{
    list < ClientConnection * >::iterator it=_clients.begin();

    while (it!= _clients.end()) {
	delete(*it);
	it = _clients.erase(it);
    }

    close(_sd);
}

/**
 * Return pointer to the instance
 */
ClientInterface *ClientInterface::Instance() throw(bad_exception)
{
    ClientInterface *iface(ClientInterface::_instance.get());
    if (iface == 0) {
	ClientInterface::_instance.reset(iface = new ClientInterface());
    }
    return iface;
}

/**
 * Chechs for new connections and messages from the client
 */
void ClientInterface::loop()
{
    int newSd;

    struct sockaddr_in cliAddr;
    static size_t cliLen = sizeof(cliAddr);

    if (_is_accepting) {
#ifndef NDEBUG
	cout << "a" << flush;
#endif
	//check for new connections
	while ((newSd =
		accept(_sd, (struct sockaddr *) &cliAddr, &cliLen)) > 0) {
	    _clients.push_back(new ClientConnection(newSd));
	}
    } else {
#ifndef NDEBUG
	cout << "." << flush;
#endif
    }

    // check for client messages
    list < ClientConnection * >::iterator it(_clients.begin());
    while (it != _clients.end()) {
	if (!(*it)->loop()) {
	    cerr << "client disconnected" << endl;
	    delete(*it);
	    it = _clients.erase(it);
	}
	else
		++it;
    }
}

/**
 * Send messages to all connected clients
 */
int ClientInterface::send(const char *message, int len)
{
    list < ClientConnection * >::iterator it;
    for (it = _clients.begin(); it != _clients.end(); it++) {
	(*it)->send(message, len);
    }
}

/**
 * Set whether the server should listen to new connections
 */
void ClientInterface::set_accepting(bool state)
{
    _is_accepting = state;
}

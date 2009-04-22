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

#ifndef __CLIENT_INTERFACE_H_
#define __CLIENT_INTERFACE_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifndef NO_NETWORK

#include <memory>
#include <iostream>
#include <list>

using namespace std;

/**
 * A class that represents a connection to a client
 * and parses all commands that the client sends to the core
 */
class ClientConnection
{
public:
    ClientConnection(int sd);
    ~ClientConnection();

    bool loop();
    int send(const char* message, int len);
    
protected:
    int _socket;

    char _buffer[4097]; // this is the standard size of a piped connection

    bool _initialized;
    bool _master;

    string _line;

    bool parse_line(string &message);
    void parse_command(const char command,const string& data);
};

/**
 * A singleton that manages the network socket of the server
 * and stores a list of all connected clients
 */
class ClientInterface
{
public:
    ~ClientInterface();

    static ClientInterface* Instance() throw(bad_exception);

    void loop();
    int send(const char* message, int len);
    static void set_port(int port);

    void set_accepting(bool state=true);

protected:
    ClientInterface();

    int _sd;
    static int port;
    bool _is_accepting;


    list<ClientConnection*> _clients;
    static auto_ptr<ClientInterface> _instance;
};


#endif // NO_NETWORK

#endif // __CLIENT_INTERFACE_H_

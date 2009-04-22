/*
 * RealTimeBattle, a robot programming game
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
 
#ifndef BROADCAST_H
#define BROADCAST_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <memory>
#include <iostream>
#include <fstream>


using namespace std;

/**
 * The streambuf for the Broadcast class
 */
class BroadcastBuffer : public streambuf
{
public:
    BroadcastBuffer();
        
    bool set_logfile(const string& filename);
	void close_logfile();
        
protected:
    
#ifndef NO_NETWORK
    bool network_clients;
#endif
	ofstream logfile;

    int overflow(int c);
    streamsize xsputn(const char* s, streamsize num);
};

/**
 * An ostream for sending data to the client and writing the logfile
 */
class Broadcast : public ostream
{
public:
    static Broadcast* Instance() throw(bad_exception); 
    
    void set_logfile(const string& filename);
	bool is_active();
	void finalize();

protected:
    Broadcast();

    BroadcastBuffer buffer;

	bool _use_logfile;

    static auto_ptr<Broadcast> _instance;
};


#endif //BROADCAST_H

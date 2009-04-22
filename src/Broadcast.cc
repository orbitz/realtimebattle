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
 
#include "Broadcast.h"

#include <errno.h>
    
#ifndef NO_NETWORK    
    #include "ClientInterface.h"
#endif // NO_NETWORK
  
  
BroadcastBuffer::BroadcastBuffer()
#ifndef NO_NETWORK
 : network_clients(true)
#endif   
{    
}  
  
int
BroadcastBuffer::overflow(int c)
{
#ifndef NO_NETWORK
    if(network_clients){    
        if(c != EOF){ 
            char z = c;
            try{
                ClientInterface::Instance()->send(&z,1);
            }catch(const bad_exception &e){
                cerr<< e.what() << endl;
            }
        }
    }
#endif // NO_NETWORK
    if(logfile){
        logfile.put(c);
    } 
    
    return c;
}

streamsize
BroadcastBuffer::xsputn(const char* s, streamsize num)
{
#ifndef NO_NETWORK   
    if(network_clients){
        try{
            ClientInterface::Instance()->send(s,num);
        }catch(const bad_exception &e){
            cerr<< e.what() << endl;
        }
    }
#endif // NO_NETWORK
    
    if(logfile){
        logfile.write(s,num);
    }    

    //we always handle all chars
    return num;
}

/**
 * Set file used for logging
 */
bool
BroadcastBuffer::set_logfile(const string& filename)
{
	if(filename=="-" || filename=="STDOUT"){
		logfile.ios::rdbuf(cout.rdbuf());
	}else{
        logfile.open(filename.c_str(), ios::out);      
        if(!logfile){
            cerr << "Warning: unable to open logfile -> " << filename << endl;  
			return false;          
        }
    }
	return true;
}

/**
 * Close logfile if open
 */
void
BroadcastBuffer::close_logfile()
{
	// if logfile uses cout it shouldn't be open
	if(logfile.is_open()){
		logfile.close();
	}
}


auto_ptr<Broadcast> Broadcast::_instance(0);
 
Broadcast::Broadcast()
 : ostream(0),
   _use_logfile(false)
{
    rdbuf(&buffer);
}

Broadcast* Broadcast::Instance () throw(bad_exception)
{
    Broadcast* bcast(Broadcast::_instance.get());
    if(bcast==0){
        Broadcast::_instance.reset(bcast=new Broadcast());
    }
    return bcast;
}

/**
 * set name of the logfile
 */
void
Broadcast::set_logfile(const string& filename)
{
    _use_logfile = buffer.set_logfile(filename);
}

/**
 * returns whether broadcast is active
 */

bool
Broadcast::is_active()
{
#ifndef NO_NETWORK
	// in network games the stream is always active
	return true;
#else
	// the stream is active if the logfile is set
	return _use_logfile;
#endif
}

/**
 * close logfile if used
 */
void
Broadcast::finalize()
{
	if(_use_logfile){
		buffer.close_logfile();
		_use_logfile = false;	
	}
}

/**
 * @file EASEAMonitorServer.c
 * @author Pallamidessi Joseph
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
**/ 

#ifndef _AUDIOMONITORSERVER_HPP__
#define _AUDIOMONITORSERVER_HPP__

#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<sys/types.h>
#include<sys/time.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<cstring>
#include<sys/select.h>
#include<fcntl.h>
#include<signal.h>
#include<vector>
#include"EASEAClientData.hpp"
#include<AudioMonitorUtils.h>
#include<ClientMonitorParameter.h>

typedef int compositor_mode_t;
#define NOTIFY_MODE 1
#define METRO_MODE 1
/**
*  \class   EASEAMonitorServer 
*  \brief   Central server of the monitoring system.
*  \details TCP polling server, select() based.
*  
**/
class EASEAMonitorServer{
  
  public:
    /*Constructor/Destructor------------------------------------------------------*/ 
    /**
    * \brief    Constructor of EASEAMonitorServer.
    * \details  Create the listening socket on the specified port num.
    *
    *  @param  port Port on which the server will listened.
    *  @param  dbg  For enabling debug printing.
    **/
    EASEAMonitorServer(int port,int dbg=0,compositor_mode_t mode=NOTIFY_MODE);
    

    /**
    * \brief    Destructor of EASEAMonitorServer.
    * \details  Close its listening and client socket.
    *
    **/
    ~EASEAMonitorServer();
    

    /*Method----------------------------------------------------------------------*/
    /**
    * \brief    Start the server.
    * \details  To be called in a thread or a fork. 
    *
    **/
    void start();
    
    
      
    std::vector<EASEAClientData>* getVectorClient();
  protected:
    /*Signal handling function : not working*/
    void signalHandler();
    void sigIntEvent(int sig);
    
    
    /**
    * \brief    Create the FD_SET that will be monitored by select().
    * \details  Use socket present in the list_client.
    *
    **/
    void buildSocketList();
    
    
    /**
    * \brief    Create/handle a new connection.
    * \details  
    *
    **/
    virtual void newClient();
    
    
    /**
    * \brief    Handle the case of receiving something.
    * \details  Wrapper that either call recvFromClient() or newClient().
    *
    **/
    virtual void recvSomething();
    
    
    /**
    * \brief    Handle the case where a know client has sent something.
    * \details  
    *
    **/
    virtual void recvFromClient();
    
    /* Data-----------------------------------------------------------------------*/ 
    int debug;                      //Flag for debug printing 
    int port;                       //Listening port
    int max_select;                 //Needed for select()
    int servSockfd;                 //Listening socket 
	  struct sigaction terminaison;
    struct sockaddr_in my_addr;
    compositor_mode_t mMode;
    fd_set rdclient; 
    socklen_t addrlen;
    std::vector<EASEAClientData>* list_client; 

};

#endif

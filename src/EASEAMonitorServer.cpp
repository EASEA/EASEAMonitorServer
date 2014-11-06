#include "EASEAMonitorServer.hpp"

/*Constructor of EASEAMonitorServer, create the server socket and initialize the data
 * structure*/

EASEAMonitorServer::EASEAMonitorServer(int prt,int dbg,compositor_mode_t mode):
  debug(1),port(prt),mMode(mode){

    /* socket factory*/
    if((servSockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) == -1)
    {
      perror("socket");
      exit(EXIT_FAILURE);
    }


    /* init local addr structure and other params */
    my_addr.sin_family      = AF_INET;
    my_addr.sin_port        = htons(port);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addrlen                 = sizeof(struct sockaddr_in);

    /* bind addr structure with socket */
    if(bind(servSockfd,(struct sockaddr*)&my_addr,addrlen) == -1)
    {
      perror("bind");
      close(servSockfd);
      exit(EXIT_FAILURE);
    }

    /*Set servSockfd to be non-blocking*/
    fcntl(servSockfd,F_SETFL,O_NONBLOCK);

    /* set the socket in passive mode (only used for accept())
     * and set the list size for pending connection*/
    listen(servSockfd,SOMAXCONN);

    max_select=servSockfd;

    list_client=new std::vector<EASEAClientData>;
  }

/**
 * /brief    Destructor of EASEAMonitorServer
 **/
EASEAMonitorServer::~EASEAMonitorServer(){
  /*close every opened socket*/
  unsigned int i;

  for(i=0;i<list_client->size();i++){
    close(list_client->at(i).getSocket());
  }

}


void EASEAMonitorServer::signalHandler(){
  /*Signal handler in case of ^C, to close the sockets*/
  //terminaison.sa_handler=sigIntEvent;
  sigfillset(&terminaison.sa_mask);
  terminaison.sa_flags=0;

  sigaction(SIGINT,&terminaison,NULL);
}





void EASEAMonitorServer::sigIntEvent(int sig){
  unsigned int i;

  close(servSockfd);	

  for(i=0;i<list_client->size();i++){
    //send(list_client[i].getSocket(),"000/END",8,0);
    close(list_client->at(i).getSocket());
  }

  exit(0);
}


void EASEAMonitorServer::buildSocketList(){
  unsigned int i;

  FD_ZERO(&rdclient);
  FD_SET(servSockfd,&rdclient);

  for(i=0;i<list_client->size();i++){
    FD_SET(list_client->at(i).getSocket(),&rdclient);
  }

}

void EASEAMonitorServer::newClient(){
  int tmp_sockfd;
  EASEAClientData* new_client;

  tmp_sockfd = accept(servSockfd,(struct sockaddr*)&my_addr,&addrlen);

  /*selected need to know the highest numerical socket value*/
  if(tmp_sockfd>max_select){
    max_select=tmp_sockfd;
  }

  if (debug) {
    std::cout<<"nouveaux client"<<std::endl;
  }

  /*Adding the newly connected client to the list of client*/
  new_client = new EASEAClientData(tmp_sockfd);
  new_client->setIP(inet_ntoa(my_addr.sin_addr));
  new_client->setPort(ntohs(my_addr.sin_port));
  list_client->push_back(*new_client);	

}

void EASEAMonitorServer::recvSomething(){
  if(FD_ISSET(servSockfd,&rdclient))
    newClient();
  else
    recvFromClient();

}

/*FAIRE DES STRUCTS POUR L'ENVOI/RECEPTION*/
/* Case new data from known client*/
/* Check whose fd changed and received from them*/
void EASEAMonitorServer::recvFromClient(){
  char buf[1024];
  MonitorParameter* params;
  unsigned char typeOfParam;
  unsigned int i; 
  EASEAClientData* changedClient;
  memset(buf,'\0',1024); //reset buffer

  for(i=0;i<list_client->size();i++){
    if(list_client->at(i).getSocket()!=0){
      if(FD_ISSET(list_client->at(i).getSocket(),&rdclient)){

        changedClient=&list_client->at(i);

        if(recv(changedClient->getSocket(),buf,1024,0)!=0){
          typeOfParam=buf[0];

          switch (typeOfParam) {

            case SIMPLEDATA:
              params=new ClientMonitorParameter(NULL);
              params->deserialize(buf);
              break;

            default: 
              params=NULL;
          }

          if (!changedClient->toIgnore()) {
            changedClient->verifyReception(params);
            changedClient->verifySending(params);
            changedClient->addData(params);

            if (debug){
              std::cout<<"I have received something from "<<
                changedClient->getIP()<<":"<<changedClient->getPort()
                <<std::endl;
              EASEAClientRow last=changedClient->getLast();
              std::cout<<last.best()<<" "<<last.average()<<" "<<last.stdev()<<" "<<last.worst()<<std::endl;
            }
            
            if (mMode==NOTIFY_MODE) {
            }

          }
          else{
            list_client->at(i).setIgnoreFlag(false);
          }
        }
      }
    }
  }
}


/**
 * /brief    Start the server, which will never stop listening  
 **/
void EASEAMonitorServer::start(){


  debug=1; 

  while(1){

    buildSocketList();

    if((select(max_select+1,&rdclient,NULL,NULL,NULL))>=1){
      recvSomething();
    }
  }
}


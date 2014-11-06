#include "mainFrame.hpp"

int main(int argc, char* argv[]){
  int port=27800; //default port
  
  if (argc == 2) {
    port=atoi(argv[1]);
  }
  
  EASEAMonitorServer* serv=new EASEAMonitorServer(port,0);
  serv->start();
  
  return 0;
}

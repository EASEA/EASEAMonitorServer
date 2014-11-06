#include "EASEAClientData.hpp"


int EASEAClientData::mGlobalID=0;

EASEAClientData::EASEAClientData ():Note(){
  mID=mGlobalID;
  mGlobalID++;
}


EASEAClientData::EASEAClientData (int sock):
                Note(),nbData(0),ignoreFlag(true),hasSent(false),hasReceived(false),mLastEvalued(){
  clientSockfd=sock;
  mID=mGlobalID;
  mGlobalID++;
}


EASEAClientData::EASEAClientData (int sock,std::string ip,int port):
                Note(),nbData(0),ignoreFlag(true),mLastEvalued(){
  clientSockfd=sock;
  clPort=port;
  clIP=ip;
  mID=mGlobalID;
  mGlobalID++;
}


EASEAClientData::~EASEAClientData (){}


/*The packet received from EASEA nodes have a mask on their first byte, which indicate how
 * many and what data it contained*/
void EASEAClientData::processBuffer(char* buffer){
}


int EASEAClientData::getSocket(){
  return clientSockfd;
}


void EASEAClientData::addData(MonitorParameter* monitor){
  ClientMonitorParameter* tmp=static_cast<ClientMonitorParameter*>(monitor); 
  if(tmp->strType==SIMPLEDATA){
    this->best.push_back(tmp->best);
    this->worst.push_back(tmp->worst);
    this->stdev.push_back(tmp->stdev);
    this->average.push_back(tmp->average);
  }
  nbData++;
}


void EASEAClientData::print(){
  int i;

  printf(" BEST\t\tWORST\t\tSTDEV\t\tAVERAGE\n ");
  for (i = 0; i < nbData; i++) {
    printf(" %e\t\t%e\t\t%e\t\t%e\n ",best[i],worst[i],stdev[i],average[i]); 
  }
}


void EASEAClientData::setIP(std::string ip){
  clIP=ip;
}


void EASEAClientData::setPort(int port){
  clPort=port;
}


int EASEAClientData::getPort(){
  return clPort;
}


std::string EASEAClientData::getIP(){
  return clIP;
}


int EASEAClientData::getID(){      
  return mID;
}

std::vector<float>* EASEAClientData::getWorstVector(){
  return &worst;
}


std::vector<float>* EASEAClientData::getBestVector(){
  return &best;
}


std::vector<float>* EASEAClientData::getStDevVector(){
  return &stdev;
}


std::vector<float>* EASEAClientData::getAverageVector(){
  return &average;
}


EASEAClientRow EASEAClientData::getLast(){
  return EASEAClientRow(best.back(),worst.back(),average.back(),
                        stdev.back()); 
}


EASEAClientRow EASEAClientData::getFromIndex(int i){
  return EASEAClientRow(best[i],worst[i],average[i],
                        stdev[i]); 
}


bool EASEAClientData::toIgnore(){
  return ignoreFlag;
}


void EASEAClientData::setIgnoreFlag(bool value){
  ignoreFlag=value;
}


void EASEAClientData::verifyReception(MonitorParameter* params){
  if(params->isReception())
    hasReceived=true;
}


void EASEAClientData::verifySending(MonitorParameter* params){
  if(params->isSending())
    hasSent=true;
}


bool EASEAClientData::isAReception(){
  if(hasReceived){
    hasReceived=false;
    return true;
  }
  else{
    return false;
  }
}


bool EASEAClientData::isASending(){
  if(hasSent){
    hasSent=false;
    return true;
  }
  else{
    return false;
  }
}


int EASEAClientData::computeNote(){

  if(mQualityNotation)
    return computeVariationNote();
  else if (mQuantityNotation)
    return computeQuantifyNote();
  else
    return NOTE_1;
  
}


int EASEAClientData::computeVariationNote(){
  int variaBest=0;
  int variaWorst=0;
  int variaStdev=0;
  int variaAverage=0;
  int i;

  EASEAClientRow last=getFromIndex(nbData-1);
  EASEAClientRow previous=getFromIndex(nbData-2);
  
  if(nbData<2)
    return NOTE_1;

  for (i = 1; i <= mGenRange; i++) {
    /*Best variation*/
    if (previous.best()>last.best()) {
      variaBest=1;
    }

    /*Worst variation*/
    if (previous.worst()>last.worst()) {
      variaWorst=1;
    }

    /*Average variation*/
    if(previous.average()>last.average()){
      variaAverage=1;
    }
     
    /*Stdev variation*/
    if (previous.stdev()<last.stdev()) {
      variaStdev=2;
    }
    else if(previous.stdev()>last.stdev()){
      variaStdev=1;
    }
    
    if(nbData-mGenRange<0)
      break;

    last=previous;
    previous=getFromIndex(nbData-mGenRange);
  }
  
  //TODO: copy operator EASEAClientRow
  mLastEvalued=last;
  if(variaBest && variaAverage){
    return NOTE_1;
  }
  else if (variaAverage && !variaBest && variaWorst) {
    return NOTE_2;
  }
  else if (!variaAverage && !variaBest) {
    return NOTE_3;
  }
  else{
    return NOTE_4;
  } 

}


int EASEAClientData::computeQuantifyNote(){
  int variaBest=0;
  int variaWorst=0;
  int variaStdev=0;
  int variaAverage=0;
  int i;

  EASEAClientRow last=getFromIndex(nbData-1);
  EASEAClientRow previous=getFromIndex(nbData-2);

  if(nbData<2)
    return NOTE_1;

  for (i = 1; i <= mGenRange; i++) {
    /*Best variation*/
    if (percent(last.best(),previous.best())<mBestDiffRatio) {
      variaBest=1;
    }

    /*Worst variation*/
    if (percent(last.worst(),previous.worst())<mWorstDiffRatio) {
      variaWorst=1;
    }

    /*Average variation*/
    if (percent(last.average(),previous.average())<mAverageDiffRatio) {
      variaAverage=1;
    }
     
    /*Stdev variation*/
    if (percent(last.stdev(),previous.stdev())>mStdevDiffRatio) {
      variaStdev=2;
    }
    if (percent(last.stdev(),previous.stdev())<mStdevDiffRatio) {
      variaStdev=1;
    }

    if(nbData-mGenRange<0)
      break;

    last=previous;
    previous=getFromIndex(nbData-mGenRange);
  }
  
  //TODO: copy operator EASEAClientRow
  mLastEvalued=last;
  if(variaBest && variaAverage){
    return NOTE_1;
  }
  else if (variaAverage && !variaBest && variaWorst) {
    return NOTE_2;
  }
  else if (!variaAverage && !variaBest) {
    return NOTE_3;
  }
  else{
    return NOTE_4;
  } 

}


float EASEAClientData::percent(float numerator, float denominator){
  return 100-(numerator/denominator)*100;
}

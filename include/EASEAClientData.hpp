/**
 * @file EASEAClientData.c
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

#ifndef _EASEACLIENTDATA_HPP__
#define _EASEACLIENTDATA_HPP__

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <AudioMonitorUtils.h>
#include <ClientMonitorParameter.h>
#include <EASEAClientRow.hpp>
#include <Note.hpp>

/**
*  \class   EASEAClientData 
*  \brief    Represent an EASEA client and store its data. 
*  \details  Hold client data and socket. 
*  
**/
class EASEAClientData:public Note {
   
  public:
    
    /*Constructor/Destructor------------------------------------------------------*/ 
    /**
    * \brief    Constructor of EASEAClientData
    * \details  Empty constructor
    *
    **/
    EASEAClientData ();
    
    
    /**
    * \brief   Constructor of EASEAClientData
    * \details Store the client's socket.
    *
    * @param  sock A previously created socket representing an EASEA client
    **/
    EASEAClientData (int sock);
    
    /**
    * \brief   Constructor of EASEAClientData
    * \details Store the client's socket.
    *
    * @param  sock  A previously created socket representing an EASEA client
    * @param  ip    The IP adress of the client
    * @param  port  The port number of the client
    **/
    EASEAClientData (int sock,std::string ip,int port);
    
    
    /**
    * \brief    Destructor of EASEAClientData   
    * \details 
    *
    **/
    virtual ~EASEAClientData ();
    
    /*Method----------------------------------------------------------------------*/
    /**
    * \brief    Compute the difference in percent between two value using the
    *           denominator as the reference.
    *
    * @param  numerator   We want the difference in percent between this value and the
    *                     denominator, expressed by the denominator value.
    * @param  denominator See above.
    * @return percent     The calculated percentage can be positive or negative.
    **/
    float percent(float numerator, float denominator);


    /**
    * \brief    A more generic way to add data. 
    * \details  See specific implementation for details.
    *
    *  @param  buffer Adress to the data to process
    **/
    virtual void processBuffer(char* buffer);
    

    /**
    * \brief    Add data to the different vectors.
    * \details  TODO: Not very generic.
    *
    *  @param  best     Value to push back to the best's vector.
    *  @param  worst    Value to push back to the worst's vector.
    *  @param  stdev    Value to push back to the stdev's vector.
    *  @param  average  Value to push back to the average's vector.
    **/
    void addData(MonitorParameter* params);
    
    
    /**
    * \brief    Print the content of the different vectors.
    * \details  For debugging purpose, print on stdout.
    *
    **/
    void print();

   
   /**
    * \brief    Check if the received MonitorParameter based class notify a
    *           reception.
    * \details  Check the isReception flag on params and set the hasReceived 
    *           flag accordingly.
    *
    *  @param  params Last monitoring message parameter
    **/
   void verifyReception(MonitorParameter* params);
   
   
   /**
    * \brief    Check if the received MonitorParameter based class notify a
    *           sending.
    * \details  Check the isSending flag on params and set the hasSent flag
    *           accordingly.
    *
    *  @param  params Last monitoring message parameter
    **/
    void verifySending(MonitorParameter* params);

    
    /**
    * \brief    Compute a note based on the setting choosed by the users. 
    *           Wrapper for the note computation function.
    * \details  Call computeVariationNote if mQualityNotation is set to true.
    *           The user can changed the notation mode and its setting using 
    *           the GUI.
    *
    * @return  note The resulting note
    **/
    int computeNote(); 
    
    
    /**
    * \brief    Do the real calculation of the note by verifying variation, on 
    *           genRange generation value if possible.
    * \details  The genRange value is by default 2, but can be changed using 
    *           the GUI.
    *
    * @return  note the note
    **/
    int computeVariationNote();
    
    
    /**
    * \brief    Do the real calculation of the note by verifying variation of
    *           specified quantity, on genRange generation value if possible.
    * \details  The genRange value is by default 2, but can be changed using 
    *           the GUI.
    *
    * @return  note the note
    **/
    int computeQuantifyNote();
    /*Getter----------------------------------------------------------------------*/
    /**
    * \brief   Return the socket.
    * \details Getter.
    *
    * @return  socket This instance's socket.
    **/
    int getSocket();
    
    
    /**
    * \brief   Return the last data on each vector, in an EASEAClientRow.
    * \details 
    *
    * @return  row A row containing the last data on each vector.
    **/
    EASEAClientRow getLast(); 
    
    /**
    * \brief   Return the i-eme data on each vector, in an EASEAClientRow.
    * \details 
    *
    * @return  row  A row containing the  data at the specified index on each 
    *               vector.
    **/
    EASEAClientRow getFromIndex(int i); 
    
    /**
    * \brief    Return the value of ignoreFlag
    * \details  The select based server check this to know if the data are wanted.
    *
    * @return   ignoreFlag The current ignore flag.
    **/
    bool toIgnore();
    
    
    /**
    * \brief    Return the vector containing the worst notes.
    * \details  Getter.
    *
    * @return   vector A pointer to the vector.
    **/
    std::vector<float>* getWorstVector();
    
    
    /**
    * \brief    Return the vector containing the best notes.
    * \details  Getter.
    *
    * @return   vector A pointer to the vector.
    **/
    std::vector<float>* getBestVector();
    
    
    /**
    * \brief    Return the vector containing the standard deviation of the
    *           population.
    * \details  Getter.
    *
    * @return   vector A pointer to the vector.
    **/
    std::vector<float>* getStDevVector();
    
    
    /**
    * \brief    Return the vector containing the average notes of the population.
    * \details  Getter.
    *
    * @return   vector A pointer to the vector.
    **/
    std::vector<float>* getAverageVector();
    

    /**
    * \brief    Return the hasReceived flag.
    * \details  Getter.
    *
    * @return   bool hasReceived
    **/
    bool isAReception();
    
    
    /**
    * \brief    Return the hasSent flag.
    * \details  Getter.
    *
    * @return   bool hasSent.
    **/
    bool isASending();
    
    
    /**
    * \brief    Get the port number of the associated EASEA node.
    * \details  
    *
    * @return   int  The port number.
    **/
    int getPort();
    
    
    /**
    * \brief    Get the IP address of the associated EASEA node.
    * \details  
    *
    * @return   std::string A string representing the ip address 
    *           ("xxx.xxx.xxx.xxx").
    **/
    std::string getIP();
    
    /**
    * \brief    Get the ID of this EASEA client.
    *  \details  
    *
    * @return  mID the id.
    **/
    int getID();

    /*Setter----------------------------------------------------------------------*/
    /**
    * \brief    Set the ignore flag.
    * \details  Data will not be process/add if ignoreFlag is true. 
    *
    *  @param   value New value of the ignoreFlag.
    **/
    void setIgnoreFlag(bool value);
   

    /**
    * \brief    Set the ip address of the corresponding EASEA node (purely informal)  
    * \details 
    *
    *  @param   ip The string representing the ip address ("xxx.xxx.xxx.xxx")
    **/
    void setIP(std::string ip);
    
    
    /**
    * \brief    Set the port num of the corresponding EASEA node (purely informal)  
    * \details 
    *
    *  @param   port The port number
    **/
    void setPort(int port);
  private:
  
    /* Data-----------------------------------------------------------------------*/
    static int mGlobalID;
    int mID;
    int clientSockfd;
    int nbData;
    bool ignoreFlag;
    int clPort;
    bool hasSent;
    bool hasReceived;
    EASEAClientRow mLastEvalued;
    std::string clIP;
    std::vector<float> best;
    std::vector<float> average;
    std::vector<float> stdev;
    std::vector<float> worst;
};

#endif

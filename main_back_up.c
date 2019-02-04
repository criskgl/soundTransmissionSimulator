#include <stdio.h>
#include <stdlib.h> 
#include<time.h> 

void fillBuf (unsigned char * buf, FILE *ptrSrc){
    int byteCounter = 0;
    int valueCurrent = 0;
    unsigned char sampleBuf[1];
    while(fread(sampleBuf,sizeof(sampleBuf),1,ptrSrc) != 0){ //While there are bytes to transmit...    
        valueCurrent = sampleBuf[0];
        buf[byteCounter] = valueCurrent;
        byteCounter++;
    }
}

void printDataInFile(unsigned char * buf, int bufSize){
    int counterh = 1;
    int h = 0;
    printf("\n\tBYTES IN THE FILE:\n");
    printf("\t*****HEADER*****************************************************************\n");
    printf("\t");
    for(h = 0; h < 24; h++){
        printf("%u ", buf[h]);
        if(counterh % 4 == 0){
            printf("\n\t");
        }
        counterh ++;
    }
    printf("\n");
    printf("\t*****AUDIO SAMPLES**********************************************************\n");
    printf("\t");
    for(h = 24; h < 33; h++){
        printf("%u ", buf[h]);
    }
    printf(" .......... ");

    for(h = bufSize - 8; h < bufSize; h++){
        printf("%u ", buf[h]);
    }

    printf("\n");
}

void printPacketContents(unsigned char * packetBuf, int packetSize){
    int i = 0;
    for(int i = 0; i < packetSize; i++){
        printf("%i ", packetBuf[i]);
    }
    printf("\n");
}

void fillPacket (unsigned char * sourceBuf, int offsetSource, unsigned char * destBuf, int sizeOfDestBuf){
    int i = 0;
    int currentValue = 0; 
    for (int i = 0; i < sizeOfDestBuf; i++){
        currentValue = sourceBuf[offsetSource];
        destBuf[i] = currentValue;
        offsetSource++;
    }
}


int main()
{   
    //Print Header of program
    printf("\n");
    printf("\t***************************************************\n");
    printf("\t***           AUDIO STREAMING SIMULATION        ***\n");
    printf("\t***           FACTORS:                          ***\n");
    printf("\t***                                             ***\n");
    printf("\t***            >PACKET SIZE                     ***\n");
    printf("\t***            >LOSS PROBABILITY                ***\n");
    printf("\t***            >POLICY FOR UNAVAILABLE PACKETS  ***\n");
    printf("\t***                                             ***\n");
    printf("\t***            author:                          ***\n");
    printf("\t***            github:  @criskgl                ***\n");
    printf("\t***************************************************\n");

    unsigned char sampleBuf[1];
    unsigned char silence[1];
    int fileInputChoice;
    silence[0] = 0;
    FILE *ptrSrc;
    FILE *ptrDest;
    char *voiceOption1 = "audiosIn/poe.au";
    char *musicOption1 = "audiosIn/short.au";
    char *musicOption2 = "audiosIn/simple.au";
    char *musicOption3 = "audiosIn/pink_panther.au";

    printf("\nPlease enter file to transmit: \n");
    printf(" _____________________________\n");
    printf("|VOICE FILES                  |\n");
    printf("|_____________________________|\n");
    printf("|1: poe.au                    |\n");
    printf(" _____________________________|___\n");
    printf("|MUSIC FILES                      |\n");
    printf("|2: short.au   (short lenght)     |\n");
    printf("|3: simple.au (medium lenght)     |\n");
    printf("|4: pink_panther.au (long lenght) |\n");
    printf("|_________________________________|\n");
    printf(">>>Input(enter only integers): ");
    scanf("%d", &fileInputChoice);

    if(fileInputChoice == 1){
        ptrSrc = fopen(voiceOption1,"rb");//r for read
    }else if(fileInputChoice == 2){
        ptrSrc = fopen(musicOption1,"rb");//r for read
    }else if(fileInputChoice == 3){
        ptrSrc = fopen(musicOption2,"rb");//r for read
    }else if(fileInputChoice == 4){
        ptrSrc = fopen(musicOption3,"rb");//r for read
    }else{
        printf("\t\t*******************************************************\n");
        printf("\t\t*              Invalid File   Chosen                  *\n" );
        printf("\t\t* Program will terminate. Respect options next time.  *\n");
        printf("\t\t*******************************************************\n");
    }
    ptrDest = fopen("audiosOut/result.au","wb");//w for write
    
    //FIND SIZE OF FILE
    int sizeOfFile;
    fseek(ptrSrc, 0L, SEEK_END);
    sizeOfFile = ftell(ptrSrc);
    fseek(ptrSrc, 0L, SEEK_SET);


    /********************INITIAL PARAMETERS*********************/
    /*
        samples per packet. Because each sample is a byte 
        this whill mean number of bytes per packet
    */int packetSize = 0; 
        /*******************************************************/
        printf("\nPlease enter desired packet size \n");
        printf(" _____________________________\n");
        printf("|          PACKET SIZE        |\n");
        printf(" -----------------------------\n");
        printf(">>>Input(enter only integers): ");
        scanf("%d", &packetSize);
    /*
        this buffer will take as many bytes
        as packetSize indicates
    */unsigned char packetBuf[packetSize];
    printf("\n");
    /*
    modify "packetLossProbability" to 
    change probability of loosing each 
    of the samples
    */int packetLossProbability = 0;
        printf("\nPlease enter probability of lost packet size \n");
        printf(" _______________________________________\n");
        printf("|  LOSS PACKET PROBABILITY(%%)  |\n");
        printf(" ---------------------------------------\n");
        printf(">>>Input(enter only integers): ");
        scanf("%d", &packetLossProbability);

    int limitProb = 100 - packetLossProbability;
    /*
    policy when packet is lost
    during transmission
    */
    int lossPolicyChosen = 0;

        printf("\nPlease enter policy for lost packets: \n");
        printf(" _____________________________\n");
        printf("|POSSIBLE LOST PACKET POLICIES|\n");
        printf("|1: Put 0s in lost data       |\n");
        printf("|2: Replay Last Sample        |\n");
        printf("|3: Replay Entire Last Packet |\n");
        printf(" -----------------------------\n");
        printf(">>>Input(enter only integers): ");
        scanf("%d", &lossPolicyChosen);

    /********************INITIAL PARAMETERS SET*********************/
    
    
    printf ("\n\t>>>SIZE OF INPUT FILE IN BYTES: %i<<<\n", sizeOfFile);
    //FILL GENERAL BUFFER
    unsigned char fileDataBuf[sizeOfFile];
    fillBuf(fileDataBuf, ptrSrc);
    printDataInFile(fileDataBuf, sizeOfFile);
    
    


   if(lossPolicyChosen == 1){
        printf("\n#\n#\n#\n#\n#\n#\n#\n#\n#\n#\n#\n#\n#\n");
        printf("\t\t*********************************************\n");
        printf("\t\t*      PACKET SIZE: %i                       *\n", packetSize);
        printf("\t\t*      POLICY: Put 0s in lost data          *\n");
        printf("\t\t*      LOSS PROBABILITY: %i %%                *\n", packetLossProbability);
        printf("\t\t*********************************************\n");
        //First transmit HEADER WITHOUT LOSS
        //Just to clarify, if there was any lost in the header bytes,
        //The file would not be playable anymore at the 
        //recievers end
        //Header size is 24 bytes
        for(int c = 0; c < 24; c++){
            fread(sampleBuf,sizeof(sampleBuf),1,ptrSrc);
            fwrite(sampleBuf, sizeof(sampleBuf), 1, ptrDest);
        }

        int packetCounter = 0;
        //SET UP RANDOM
        int randomNum;
        srand(time(0));
        randomNum = rand() % 100 + 1;  
        printf("\n");
        //SEND AUDIO FILE PACKETS
        printf("\n\t********************************\n");
        printf("\t* STARTING PACKET TRANSMISSION *");
        printf("\n\t********************************\n");
        for(int offset = 24; offset < sizeOfFile; offset += packetSize){
            //FILL PACKET
            fillPacket(fileDataBuf, offset, packetBuf, packetSize);//packet starting at "offset" position of file
            printf("PACKET #%i: ", packetCounter);
            printPacketContents(packetBuf, packetSize);
            //PACKET ARRIVING TO RECIEVER
            fwrite(packetBuf, sizeof(packetBuf), 1, ptrDest);
            packetCounter++;
        } 
        
   }
   else if(lossPolicyChosen == 2){
        printf("\n#\n#\n#\n#\n#\n#\n#\n#\n#\n#\n#\n#\n#\n");
        printf("\t\t*********************************************\n");
        printf("\t\t*      PACKET SIZE: %i                       *\n", packetSize);
        printf("\t\t*      POLICY: Replay Last Sample           *\n");
        printf("\t\t*      LOSS PROBABILITY: %i %%                *\n", packetLossProbability);
        printf("\t\t*********************************************\n");
        
        printf("\n");
   }
   else if(lossPolicyChosen == 3){
        printf("\n#\n#\n#\n#\n#\n#\n#\n#\n#\n#\n#\n#\n#\n");
        printf("\t\t*********************************************\n");
        printf("\t\t*      PACKET SIZE: %i                       *\n", packetSize);
        printf("\t\t*      POLICY: Replay Entire Last Packet    *\n");
        printf("\t\t*      LOSS PROBABILITY: %i %%                *\n", packetLossProbability);
        printf("\t\t*********************************************\n");
        
        printf("\n");
   }else{
        printf("\n#\n#\n#\n#\n#\n#\n#\n#\n#\n#\n#\n#\n#\n");
        printf("\t\t*******************************************************\n");
        printf("\t\t*              Invalid Policy Chosen                  *\n" );
        printf("\t\t* Program will terminate. Respect options next time.  *\n");
        printf("\t\t*******************************************************\n");
        
        printf("\n");
        return(-1);

   }


    fclose(ptrSrc);
    fclose(ptrDest);
    return(0);
    } 



    //SEND AUDIO FILE PACKETS
    /*
    printf("\n********************************\n");
    printf("* STARTING PACKET TRANSMISSION *");
    printf("\n********************************\n");
    for(int offset = 24; offset < sizeOfFile; offset += packetSize){
        //FILL PACKET
        fillPacket(fileDataBuf, offset, packetBuf, packetSize);//packet starting at "offset" position of file
        printf("PACKET #%i: ", packetCounter);
        printPacketContents(packetBuf, packetSize);
        //PACKET ARRIVING TO RECIEVER
        fwrite(packetBuf, sizeof(packetBuf), 1, ptrDest);
        packetCounter++;
    } 
    */


    /*
    while(fread(buffer,sizeof(buffer),1,ptrSrc) != 0){// while not EOF...
        printf("%s", buffer); // prints a series of bytes
        fwrite(buffer, sizeof(buffer), 1, ptrDest);
    }
    */



    /*    
    while(fread(packetBuf,sizeof(packetBuf),packetSize,ptrSrc) != 0){ //While there are bytes to transmit...    
        if(randomNum > limitProb ){//Sample Loss
            valueCurrent = sampleBuf[0];
            fwrite(silence, sizeof(silence), 1, ptrDest);
        }else{//No Sample Loss
            unsigned int byteInFile = packetBuf[0];
            printf("%u ", byteInFile);
            byteInFile = packetBuf[1];
            printf("%u ", byteInFile);
            byteInFile = packetBuf[2];
            printf("%u ", byteInFile);
            byteInFile = packetBuf[3];
            printf("%u ", byteInFile);
            byteInFile = packetBuf[4];
            printf("%u ", byteInFile);
            byteInFile = packetBuf[5];
            printf("%u ", byteInFile);
            fwrite(packetBuf, sizeof(packetBuf), 1, ptrDest);
            valueCurrent = packetBuf[0];
        }
        randomNum = ((rand() + valueCurrent)) % 100 + 1;
    }
    */
        
/*
    while(fread(sampleBuf,sizeof(sampleBuf),1,ptrSrc) != 0){ //While there are bytes to transmit...    
        if(randomNum > limitProb ){//Sample Loss
            valueCurrent = sampleBuf[0];
            fwrite(silence, sizeof(silence), 1, ptrDest);
        }else{//No Sample Loss
            unsigned int byteInFile = sampleBuf[0];
            printf("%u ", byteInFile);
            fwrite(sampleBuf, sizeof(sampleBuf), 1, ptrDest);
            valueCurrent = sampleBuf[0];
        }
        randomNum = ((rand() + valueCurrent)) % 100 + 1;
    }
    */


    /*
    unsigned char myBuf[sizeOfFile];
    int byteCounter = 0;
    while(fread(sampleBuf,sizeof(sampleBuf),1,ptrSrc) != 0){ //While there are bytes to transmit...    
        valueCurrent = sampleBuf[0];
        myBuf[byteCounter] = valueCurrent;
        printf("%u ", myBuf[byteCounter]);
        byteCounter++;
    }
    */
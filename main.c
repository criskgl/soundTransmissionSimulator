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
    int h = 0;
    printf("\nBYTES IN THE FILE:\n");
    printf("*****HEADER*****************************************************************\n");
    for(h = 0; h < 24; h++){
        printf("%u ", buf[h]);
    }
    printf("\n");
    printf("*****AUDIO SAMPLES**********************************************************\n");

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
    printf("***************************************************\n");
    printf("*****         AUDIO STREAMING SIMULATION        ***\n");
    printf("*****         FACTORS:                          ***\n");
    printf("*****                                           ***\n");
    printf("*****          >PACKET SIZE                     ***\n");
    printf("*****          >LOSS PROBABILITY                ***\n");
    printf("*****          >POLICY FOR UNAVAILABLE PACKETS  ***\n");
    printf("***************************************************\n");

    unsigned char sampleBuf[1];
    unsigned char silence[1];
    silence[0] = 0;
    FILE *ptrSrc;
    FILE *ptrDest;
    ptrSrc = fopen("audiosIn/poe.au","rb");//r for read
    ptrDest = fopen("audiosOut/result.au","wb");//w for write
    int sizeOfFile;



    /********************INITIAL PARAME*********************/
    /*
        modify "packetLossProbability" to 
        change probability of loosing each 
        of the samples
    */int packetLossProbability = 0;
    int limitProb = 100 - packetLossProbability;
    /*
        samples per packet. Because each sample is a byte 
        this whill mean number of bytes per packet
    */int packetSize = 5; 
    /*
        this buffer will take as many bytes
        as packetSize indicates
    */unsigned char packetBuf[packetSize];
    /*******************************************************/
    
    
    
    /*
    while(fread(buffer,sizeof(buffer),1,ptrSrc) != 0){// while not EOF...
        printf("%s", buffer); // prints a series of bytes
        fwrite(buffer, sizeof(buffer), 1, ptrDest);
    }
    */
    
    //First transmit HEADER WITHOUT LOSS
    //Just to clarify, if there was any lost in the header bytes,
    //The file would not be playable anymore at the 
    //recievers end
    //Header size is 24 bytes
    for(int c = 0; c < 24; c++){
        fread(sampleBuf,sizeof(sampleBuf),1,ptrSrc);
        fwrite(sampleBuf, sizeof(sampleBuf), 1, ptrDest);
    }
    
    
    //FIND SIZE OF FILE
    fseek(ptrSrc, 0L, SEEK_END);
    sizeOfFile = ftell(ptrSrc);
    fseek(ptrSrc, 0L, SEEK_SET);

    printf ("\n>>>SIZE OF INPUT FILE IN BYTES: %i<<<\n", sizeOfFile);
    
    //FILL GENERAL BUFFER
    unsigned char fileDataBuf[sizeOfFile];
    fillBuf(fileDataBuf, ptrSrc);
    printDataInFile(fileDataBuf, sizeOfFile);
    int packetCounter = 0;
    //SET UP RANDOM
    int randomNum;
    srand(time(0));
    randomNum = rand() % 100 + 1;     

    //SEND AUDIO FILE PACKETS
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
    printf("\n");

    fclose(ptrSrc);
    fclose(ptrDest);
    
    return(0);
    } 
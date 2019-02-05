#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include <unistd.h>

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
    silence[0] = 0;
    FILE *ptrSrc;
    FILE *ptrDest;
    ptrSrc = fopen("audiosIn/pink_panther.au","rb");//r for read
    ptrDest = fopen("audiosOut/result.au","wb");//w for write
    int sizeOfFile;

    char *voiceOption1 = "audiosIn/poe.au";
    char *musicOption1 = "audiosIn/short.au";
    char *musicOption2 = "audiosIn/simple.au";
    char *musicOption3 = "audiosIn/pink_panther.au";
    int fileInputChoice;
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
        return(-1);
    }   


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
    /*modify "packetLossProbability" to 
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
    
    //HANDLE EACH OF THE CASES
    if(lossPolicyChosen == 1){
        printf("\n");
        printf("\t\t  #####\n");
        printf("\t\t  #   #\n");
        printf("\t\t  #   #\n");
        printf("\t\t  #   #\n");
        printf("\t\t  #   #\n");
        printf("\t\t  #   #\n");
        printf("\t\t  #   #\n");
        printf("\t\t  #   #\n");
        printf("\t\t###   ####\n");
        printf("\t\t ##   ##\n");
        printf("\t\t  #   #\n");
        printf("\t\t   # #\n");
        printf("\t\t    #\n");
        printf("\t\t*********************************************\n");
        printf("\t\t*      PACKET SIZE: %i                     *\n", packetSize);
        printf("\t\t*      POLICY: Put 0s in lost data          *\n");
        printf("\t\t*      LOSS PROBABILITY: %i %%                *\n", packetLossProbability);
        printf("\t\t*********************************************\n");
        //FIND SIZE OF FILE
        fseek(ptrSrc, 0L, SEEK_END);
        sizeOfFile = ftell(ptrSrc);
        fseek(ptrSrc, 0L, SEEK_SET);

        //First transmit HEADER WITHOUT LOSS
        //Just to clarify, if there was any lost in the header bytes,
        //The file would not be playable anymore at the 
        //recievers end
        //Header size is 24 bytes
        for(int c = 0; c < 24; c++){
            fread(sampleBuf,sizeof(sampleBuf),1,ptrSrc);
            fwrite(sampleBuf, sizeof(sampleBuf), 1, ptrDest);
        }

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

        unsigned char zerosPacket[packetSize];
        for(int i = 0; i < packetSize; i++){//fill zerosPacket with zeros
            zerosPacket[i] = 0;
        }
        unsigned char tempBuf[packetSize];//This buffer will safe last packet values
        int lastSampleInLastPacket = 0; //This will save last value of sample in last packet
        //SEND AUDIO FILE PACKETS
        printf("\n********************************\n");
        printf("* STARTING PACKET TRANSMISSION *");
        printf("\n********************************\n");
        sleep(1);
        for(int offset = 24; offset < sizeOfFile; offset += packetSize){
            //FILL PACKET
            fillPacket(fileDataBuf, offset, packetBuf, packetSize);//packet starting at "offset" position of file
            fillPacket(packetBuf, 0, tempBuf, packetSize);// save contents of current packet to restore if needed
            lastSampleInLastPacket = tempBuf[packetSize - 1];
            //printf("%i\n", lastSampleInLastPacket);
            
            //PACKET ARRIVING TO RECIEVER
            if(randomNum > limitProb){//PACKET LOSS
                printf("LOSS!!--> ");
                //we will send packet containing all zeros;
                fwrite(zerosPacket, sizeof(zerosPacket), 1, ptrDest);
                
                printf("PACKET #%i TRANSMITED: ", packetCounter);
                printPacketContents(zerosPacket, packetSize);
            }else{//NO LOSS
                printf("SUCCESS--> ");
                fwrite(packetBuf, sizeof(packetBuf), 1, ptrDest);
                printf("PACKET #%i TRANSMITED: ", packetCounter);
                printPacketContents(packetBuf, packetSize);
            }
            packetCounter++;
            randomNum = ((rand() + lastSampleInLastPacket)) % 100 + 1; 
        }

        printf("\n");

        fclose(ptrSrc);
        fclose(ptrDest);
    
        return(0);
    }
    if(lossPolicyChosen == 2){
        printf("\n");
        printf("\t\t  #####\n");
        printf("\t\t  #   #\n");
        printf("\t\t  #   #\n");
        printf("\t\t  #   #\n");
        printf("\t\t  #   #\n");
        printf("\t\t  #   #\n");
        printf("\t\t  #   #\n");
        printf("\t\t  #   #\n");
        printf("\t\t###   ####\n");
        printf("\t\t ##   ##\n");
        printf("\t\t  #   #\n");
        printf("\t\t   # #\n");
        printf("\t\t    #\n");        
        printf("\t\t*********************************************\n");
        printf("\t\t*      PACKET SIZE: %i                       *\n", packetSize);
        printf("\t\t*      POLICY: Replay Last Sample           *\n");
        printf("\t\t*      LOSS PROBABILITY: %i %%                *\n", packetLossProbability);
        printf("\t\t*********************************************\n");
        //FIND SIZE OF FILE
        fseek(ptrSrc, 0L, SEEK_END);
        sizeOfFile = ftell(ptrSrc);
        fseek(ptrSrc, 0L, SEEK_SET);

        //First transmit HEADER WITHOUT LOSS
        //Just to clarify, if there was any lost in the header bytes,
        //The file would not be playable anymore at the 
        //recievers end
        //Header size is 24 bytes
        for(int c = 0; c < 24; c++){
            fread(sampleBuf,sizeof(sampleBuf),1,ptrSrc);
            fwrite(sampleBuf, sizeof(sampleBuf), 1, ptrDest);
        }

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

        /*
        unsigned char zerosPacket[packetSize];
        for(int i = 0; i < packetSize; i++){//fill zerosPacket with zeros
            zerosPacket[i] = 0;
        }
        */
        unsigned char tempBuf[packetSize];//This buffer will safe last packet values
        int lastSampleInLastPacket = 0; //This will save last value of sample in last packet
        for(int i = 0; i < packetSize; i++){//fill last packet with zeros only to use if first packet containing 
                                            //samples is lost.
            tempBuf[i] = 0;
        }
        //SEND AUDIO FILE PACKETS
        printf("\n********************************\n");
        printf("* STARTING PACKET TRANSMISSION *");
        printf("\n********************************\n");
        sleep(1);
        for(int offset = 24; offset < sizeOfFile; offset += packetSize){
            //FILL PACKET
            fillPacket(fileDataBuf, offset, packetBuf, packetSize);//packet starting at "offset" position of file
            //fillPacket(packetBuf, 0, tempBuf, packetSize);// save contents of current packet to restore if needed
            lastSampleInLastPacket = tempBuf[packetSize - 1];
            //printf("%i\n", lastSampleInLastPacket);            
            //PACKET ARRIVING TO RECIEVER
            if(randomNum > limitProb){//PACKET LOSS
                printf("LOSS!!--> ");
                //we will send packet containing last packet contents since we've lost contents of the last packet;
                fwrite(tempBuf, sizeof(tempBuf), 1, ptrDest);
                printf("PACKET #%i TRANSMITED: ", packetCounter);
                printPacketContents(tempBuf, packetSize);

                //Update our tempBuf to be all zeros again
                for(int i = 0; i < packetSize; i++){
                tempBuf[i] = 0;
                }
            }else{//NO LOSS
                printf("SUCCESS--> ");
                fwrite(packetBuf, sizeof(packetBuf), 1, ptrDest);
                printf("PACKET #%i TRANSMITED: ", packetCounter);
                printPacketContents(packetBuf, packetSize);
                //Update our tempBuf to be whatever our last packet had
                fillPacket(packetBuf, 0, tempBuf, packetSize);// save contents of current packet to restore if needed
            }
            packetCounter++;
            randomNum = ((rand() + lastSampleInLastPacket)) % 100 + 1; 
        }

        printf("\n");

        fclose(ptrSrc);
        fclose(ptrDest);

        return(0);
    }
    if(lossPolicyChosen == 3){
        printf("Not Yet Implemented");
        printf("\n");
        printf("\t\t  #####\n");
        printf("\t\t  #   #\n");
        printf("\t\t  #   #\n");
        printf("\t\t  #   #\n");
        printf("\t\t  #   #\n");
        printf("\t\t  #   #\n");
        printf("\t\t  #   #\n");
        printf("\t\t  #   #\n");
        printf("\t\t###   ####\n");
        printf("\t\t ##   ##\n");
        printf("\t\t  #   #\n");
        printf("\t\t   # #\n");
        printf("\t\t    #\n");
        printf("\t\t*********************************************\n");
        printf("\t\t*      PACKET SIZE: %i                     *\n", packetSize);
        printf("\t\t*      POLICY: Replay Entire Last Packet     *\n");
        printf("\t\t*      LOSS PROBABILITY: %i %%                *\n", packetLossProbability);
        printf("\t\t*********************************************\n");
        //FIND SIZE OF FILE
        fseek(ptrSrc, 0L, SEEK_END);
        sizeOfFile = ftell(ptrSrc);
        fseek(ptrSrc, 0L, SEEK_SET);

        //First transmit HEADER WITHOUT LOSS
        //Just to clarify, if there was any lost in the header bytes,
        //The file would not be playable anymore at the 
        //recievers end
        //Header size is 24 bytes
        for(int c = 0; c < 24; c++){
            fread(sampleBuf,sizeof(sampleBuf),1,ptrSrc);
            fwrite(sampleBuf, sizeof(sampleBuf), 1, ptrDest);
        }

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

        unsigned char tempBuf[packetSize];//This buffer will safe last packet values   
        for(int i = 0; i < packetSize; i++){//Build packet for possible lost packets with 
            tempBuf[i] = 0;
        }   
        int lastSampleInLastPacket = 0; 

        //counter for lost packets
        int lostPacketCount = 0;
        //SEND AUDIO FILE PACKETS
        printf("\n********************************\n");
        printf("* STARTING PACKET TRANSMISSION *");
        printf("\n********************************\n");
        for(int offset = 24; offset < sizeOfFile; offset += packetSize){
            //FILL PACKET
            fillPacket(fileDataBuf, offset, packetBuf, packetSize);//packet starting at "offset" position of file
            //fillPacket(packetBuf, 0, tempBuf, packetSize);// save contents of current packet to restore if needed
            //sleep(1);
            //PACKET ARRIVING TO RECIEVER
            if(randomNum > limitProb){//PACKET LOSS
                printf("LOSS!!--> ");
                //we will send packet containing all zeros;
                fwrite(tempBuf, sizeof(tempBuf), 1, ptrDest);               
                printf("PACKET #%i TRANSMITED: ", packetCounter);
                printPacketContents(tempBuf, packetSize);
                lostPacketCount++;
            }else{//NO LOSS
                printf("SUCCESS--> ");
                fwrite(packetBuf, sizeof(packetBuf), 1, ptrDest);
                printf("PACKET #%i TRANSMITED: ", packetCounter);
                printPacketContents(packetBuf, packetSize);
                //Save last packet to use when losing other packets
                for(int j = 0; j < packetSize; j++){
                    tempBuf[j] = packetBuf[j]; 
                }
            }
            packetCounter++;
            //Generate new random
            lastSampleInLastPacket = packetBuf[packetSize - 1];
            randomNum = ((rand() + lastSampleInLastPacket)) % 100 + 1; 
        }
        printf("\n******************************\n");
        printf("* LOST PACKETS: %f %%  \n", (((float) (lostPacketCount * packetSize)/sizeOfFile)*100));
        printf("******************************");
        printf("\n");

        fclose(ptrSrc);
        fclose(ptrDest);
    
        return(0);
    }
} 

    //122 248 121 101 122
    //122 248 121 101 122
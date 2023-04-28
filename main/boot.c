#include <boot.h>

#define S_OK            0
#define SREC_TYPE       1
#define BYTE_COUNT      2
#define ADDRESS_OK      3
#define ADDRESS_BYTE    4 
#define DATA_OK         5
#define DATA_BYTE       6

volatile uint8_t checkField[7] = { 0, 0, 0, 0, 0, 0, 0 };
volatile uint8_t bytecount;
volatile uint32_t address;              //"S1" case 16bits-address 
uint8_t hexArr[2] = {0};       //used for computing the byte_count value
volatile uint32_t word = 0;
volatile uint32_t word_arr[4] = {0};
volatile uint8_t word_count = 0;
volatile uint8_t word_number = 0;
volatile uint8_t digit_arr[100] = {0};

#define MAX_SIZE (100u)

uint8_t QUEUE_ARR[MAX_SIZE] = {0};

//typedef void (*UART_Function_Pointer)(uint8_t Data);
//
//UART_Function_Pointer  UART_Call_Back = NULL;
//
// Initialized Queue Variable
CircurlarQueue_Types  Queue_Variable = {
  -1, -1, MAX_SIZE,QUEUE_ARR
};

void UART0_IRQHandler(void)
{
   uint8_t data = UART0->D;
   UART0_Transmit(data);
   uint8_t dec_data = hexToDex4bits(data);
   if(data == 'S')
   {     
        checkField[S_OK] = 1;
   }
   else if(checkField[S_OK] == 1) // data != 'S' and the previous data is 'S'
   {
        //0 1 2 7 8 9
        if(data == '2')
        {
            checkField[SREC_TYPE] = 1;
        }
        else if ( (data == '7') || (data == '8') || (data == '9') ) 
        {
            // end
            disableReceiveInterrupt();
        }
        checkField[S_OK] = 0;           //reset all checktype
    }
    else if( checkField[SREC_TYPE] == 1 )   // data != 'S' and we get "S2", get the ByteCount
    {
        /* Handle Byte Count Field */
        ++checkField[BYTE_COUNT];
        if(checkField[BYTE_COUNT] == 1)
        {
//            bytecount = (uint8)(data << 4); 
            hexArr[0] = data;   //get the first 4bits of address
        }       
        else
        {
            hexArr[1] = data;   //get thet second 4bits of address 
//            bytecount |= data;
            checkField[BYTE_COUNT] = 0; //we have already get all 2 4bits-digit of the bytecount, reset checkfield
            bytecount = hex_to_dec(hexArr);     //get the bytecount
            checkField[ADDRESS_OK] = 1;         //we finish computing the byte_count, turn on ADDRESS_OK flag
            word_number = (bytecount - 1 - 3) / 4; //bytecount - 1(byte checksum) - 3(byte address - because 'S2' type) / 4(bytes per word)
            checkField[SREC_TYPE] = 0;              //reset the checkfield of SREC_TYPE
        }
    }
    else if(checkField[ADDRESS_OK] == 1)   // if the byte_count is done computing and the ADDRESS_FLAG is turned on
    {
        /* Handle Address Field */
        /*
            It seems like we only consider the case "S1" for data, so S1 means that the address is 16bits
         */
      //   example:     S214[020000]08E8FF1F010402005F0402006B040200FE
        ++checkField[ADDRESS_BYTE];     //each time we get one 4bits-hexdigit of address, ++checkField[ADDRESS_BYTE]
        uint8_t shift = checkField[ADDRESS_BYTE];
        address |= (uint32_t)(dec_data << ((6 - shift) * 4));     //"6 -" is because we compute the "S2" case
        // 1111.1111.1111.1111 0x020000
        if (checkField[ADDRESS_BYTE] == 6)        //if we finish computing all 6 4bits-hexdigit of address
        {
            checkField[ADDRESS_BYTE] = 0; //reset checkField[ADDRESS_BYTE] for the use of next line 
            checkField[ADDRESS_OK] = 0;   //reset checkField[ADDRESS_OK] for the use of the next line
            checkField[DATA_OK]    = 1;   //turn on the DATA_OK flag to signal the computing databyte process        
            
        }
    }
    else if(checkField[DATA_OK] == 1)  // if DATA_OK is turn on then we compute the Data
    {
        /* Handle Data Field */
        ++checkField[DATA_BYTE];        //each time we receive a 4bits-hex-digit of data, ++checkField[DATA_BYTE] 
        //S1[13][0000]["08E8FF1F"110400006F0400007B040000][D7]
        
        CircurlarQueue_PushData(&Queue_Variable, data); //push 0 then 8 then E then 8 ...
        if(checkField[DATA_BYTE] == 8)    //we got 8 4bits-hex-digit 
        {   
            /* Push a Word to queue */
            uint8_t index = 0;
            uint32_t word = 0;
            /* loop 4 times: get 2 4bits-hex-digit, convert to decimal and add that to word in right order */
            for(index=0; index< 4; ++index)
            {
              hexArr[0] = CircurlarQueue_PopData(&Queue_Variable);
//              UART0_Transmit(hexArr[0]);
//              UART0_Transmit('_');
              hexArr[1] = CircurlarQueue_PopData(&Queue_Variable);
//              UART0_Transmit(hexArr[1]);
//              UART0_Transmit('_');
              word |= (uint32_t)(hex_to_dec(hexArr) << (index * 8));     //<-----------------------------
              //word will be in right order (1FFF8E08 for 0,8,E,8,F,F,1,F)
              //00.00.8976
            }
            checkField[DATA_BYTE] = 0; //reset checkField[data_type], start another section of computing a word  
            word_arr[word_count] = word;
            word = 0;                   //reset word 
            ++word_count;               //we will increase the word_count until it hits word_number in this line
        }        
        uint8_t temp_word_count = word_count;
        if(temp_word_count == word_number)
        {
            uint8_t index = 0;
            for(index=0; index<word_count; ++index)
            {
              if(index != 0)
              {
                  address += 4;
              }
              uint32_t destination = address;
              uint32_t value = word_arr[index];
//              if(index == 0)
//              {
//                Flash_EraseSector(destination);
//              }
              Flash_WriteLongWord(destination, word_arr[index]);
            }
            checkField[DATA_OK] = 0;    //when we finish flashing all words in one line turn off checkField[DATA_OK] flag
            word_count = 0;             //reset word_count
            checkField[DATA_BYTE] = 0;  //reset checkField[DATA_BYTE]
        }
    }
}
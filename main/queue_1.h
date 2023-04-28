#ifndef __queue_h_
#define	__queue_h_

#include "MKL46Z4.h"


#define MAX_SIZE_QUEUE	        (600u) //528u
#define TRUE			(1u)
#define FALSE			(0u)

/* define queue types using struct */
typedef struct
{
  int8_t Front, Rear;
  uint32_t Size;
  uint8_t *QueueArr;
} CircurlarQueue_Types;

/*Function Prototypes */
//Check full of Queue
uint8_t CircurlarQueue_IsFull(CircurlarQueue_Types *Queue);

//Check empty of Queue
uint8_t CircurlarQueue_IsEmpty(CircurlarQueue_Types *Queue);

//Handling push data in queue
void CircurlarQueue_PushData(CircurlarQueue_Types *Queue, uint8_t Input_Data);

//Handling peek data in queue
uint8_t CircurlarQueue_PopData(CircurlarQueue_Types *Queue);





#endif
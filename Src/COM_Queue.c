#include "COM_Queue.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

void COM_Buffer_Init(COM_Buffer *COM_Data)
{
	memset(COM_Data, 0, sizeof(COM_Buffer));
}

void COM_Enqueue(COM_Buffer *COM_Data, char ch)
{
	#if 0
	if(COM_Buffer_Full(COM_Data)==0)
	{
		//if(ch == '\n')
			COM_Data->flag = 1;

		if(COM_Data->tail < COM_BUFFER_MAX)
		{
			COM_Data->buffer[COM_Data->tail++] = ch;
		}
		else
		{
			COM_Data->tail = 0;
			COM_Data->buffer[COM_Data->tail] = ch;
		}
	}
	#endif
	if(COM_Buffer_Full(COM_Data) > 0)	//Full data Pushing
	{
		//printf("E->Full->D\r\n");
		COM_Dequeue(COM_Data);
	}
	// input Data
	COM_Data->flag = 1;

	if(COM_Data->tail < COM_BUFFER_MAX)
	{
		COM_Data->buffer[COM_Data->tail++] = ch;
	}
	else
	{
		COM_Data->tail = 0;
		COM_Data->buffer[COM_Data->tail++] = ch;
	}
}
char COM_Dequeue(COM_Buffer *COM_Data)
{
	if(COM_Buffer_Empty(COM_Data) == 0)
	{
		if(COM_Data->head < COM_BUFFER_MAX)
		{
			return COM_Data->buffer[COM_Data->head++];
		}
		else
		{
			COM_Data->head = 0;
			return COM_Data->buffer[COM_Data->head++];
		}
	}
	return -1;
}

char get_COM_Flag(COM_Buffer *COM_Data)
{
	return COM_Data->flag;
}

void COM_Flag_Reset(COM_Buffer *COM_Data)
{
	COM_Data->flag = 0;
}

int COM_Used_Buffer(COM_Buffer *COM_Data)
{
	if(COM_Data->tail > COM_Data->head)
		return COM_Data->tail - COM_Data->head;
	else if(COM_Data->tail < COM_Data->head)
		return COM_BUFFER_MAX - COM_Data->head + COM_Data->tail;
	return 0;
}

char COM_Buffer_Full(COM_Buffer *COM_Data)
{
	if((COM_Data->head == 0)&&(COM_Data->tail == COM_BUFFER_MAX - 1))
	{	//tail first and Head end so buffer full
		return 1;
	}
	else if(COM_Data->head - COM_Data->tail == 1)
	{
		return 2;
	}
	return 0;
}

char COM_Buffer_Empty(COM_Buffer *COM_Data)
{
	if(COM_Data->head == COM_Data->tail)
		return 1;

	return 0;
}
//char COM_Find_Words(COM_Buffer *COM_Data, Str_Unit *FindWords, Str_Unit *ResultData)
int COM_Find_Words(COM_Buffer *COM_Data, char *FindWords, int FindWordsLen, char **ResultData)
{
	int Temp_Index = 0;
	int Temp_Word_Len = FindWordsLen - 1;
	int ResultLen = 0;
	if(COM_Data->tail == COM_Data->head)
		return 0;
	else if((COM_Data->tail - COM_Data->head) > 0) // Queue Head First
	{					
		for(Temp_Index = COM_Data->head + Temp_Word_Len; Temp_Index < COM_Data->tail; Temp_Index++)
		{
			if(COM_Data->buffer[Temp_Index] == FindWords[Temp_Word_Len])
			{
				if(strncmp(COM_Data->buffer + Temp_Index - Temp_Word_Len, \
					       FindWords, Temp_Word_Len) == 0)
				{
					#if DEBUG_QUEUE
					printf("ResultData %ld \r\n", ResultData);
					#endif
					ResultLen = Temp_Index - COM_Data->head + 1;
					if(*ResultData == NULL)
					{
						#if 0	// all count lens 
						ResultLen = Temp_Index - COM_Data->head + 1;
						#endif
						*ResultData = (char *)calloc(ResultLen+1, sizeof(char));
					}
					#if DEBUG_QUEUE
					printf("Index %d, Head %d, Tail %d \r\n",Temp_Index, COM_Data->head, COM_Data->tail);
					#endif
					//printf(" ");
					memcpy(*ResultData, COM_Data->buffer + COM_Data->head, ResultLen);
					//ResultData[0][ResultLen] = 0;
					COM_Data->head = Temp_Index + 1;
					#if DEBUG_QUEUE
					printf("Head %d, Tail %d\r\n",COM_Data->head, COM_Data->tail); 
					#if DEBUG_QUEUE_RECV
					printf("Recv[%d][%s] \r\n", ResultLen, *ResultData);
					#endif
					printf("step1 \r\n");
					#endif
					return ResultLen;
				}
			}
		}
	}
	else	// Queue Tail First
	{
		if(COM_Data->head + Temp_Word_Len > COM_BUFFER_MAX)		//default end position of Temp_Index is front of the buffer.
		{
			Temp_Index = COM_Data->head + Temp_Word_Len - COM_BUFFER_MAX;
			for(Temp_Index; Temp_Index < COM_Data->tail; Temp_Index++)
			{
				if(COM_Data->buffer[Temp_Index] == FindWords[Temp_Word_Len])
				{
					if(Temp_Index - Temp_Word_Len >= 0)	//start position of Temp_Index is front of the buffer.
					{
						if(strncmp(COM_Data->buffer + Temp_Index - Temp_Word_Len, \
							       FindWords, Temp_Word_Len) == 0)
						{
							#if DEBUG_QUEUE
							printf("ResultData %ld \r\n", ResultData);
							#endif
							ResultLen = (COM_BUFFER_MAX - COM_Data->head) + Temp_Index + 1;
							if(*ResultData == NULL)
							{
								*ResultData = (char *)calloc(ResultLen+1, sizeof(char));
							}
							#if DEBUG_QUEUE
							printf("Index %d, Head %d, Tail %d \r\n",Temp_Index, COM_Data->head, COM_Data->tail);
							#endif
							memcpy(*ResultData, COM_Data->buffer + COM_Data->head, (COM_BUFFER_MAX - COM_Data->head));
							memcpy(*ResultData + (COM_BUFFER_MAX - COM_Data->head), COM_Data->buffer, Temp_Index);
							//*ResultData[ResultLen] = 0;
							COM_Data->head = Temp_Index + 1;
							#if DEBUG_QUEUE
							printf("Head %d, Tail %d\r\n",COM_Data->head, COM_Data->tail); 
							#if DEBUG_QUEUE_RECV
							printf("Recv[%d][%s] \r\n", ResultLen, *ResultData);
							#endif
							printf("step2 \r\n");
							#endif
							return ResultLen;
						}
					}
					else //start position of Temp_Index is rear of the buffer.
					{
						if((strncmp(COM_Data->buffer, FindWords + Temp_Word_Len - Temp_Index, Temp_Index)==0)\
							&&(strncmp(COM_Data->buffer + COM_BUFFER_MAX + Temp_Index - Temp_Word_Len  , FindWords, Temp_Word_Len - Temp_Index) == 0))
						{
							ResultLen = (COM_BUFFER_MAX - COM_Data->head) + Temp_Index + 1;
							if(*ResultData == NULL)
							{
								*ResultData = (char *)calloc(ResultLen+1, sizeof(char));
							}
							#if DEBUG_QUEUE
							printf("Index %d, Head %d, Tail %d \r\n",Temp_Index, COM_Data->head, COM_Data->tail);
							#endif
							memcpy(*ResultData, COM_Data->buffer + COM_Data->head, (COM_BUFFER_MAX - COM_Data->head));
							memcpy(*ResultData + (COM_BUFFER_MAX - COM_Data->head), COM_Data->buffer, Temp_Index);
							//ResultData[0][ResultLen] = 0;
							COM_Data->head = Temp_Index + 1;
							#if DEBUG_QUEUE
							printf("Head %d, Tail %d\r\n",COM_Data->head, COM_Data->tail); 
							#if DEBUG_QUEUE_RECV
							printf("Recv[%d][%s] \r\n", ResultLen, *ResultData);
							#endif
							printf("step3 \r\n");
							#endif
							return ResultLen;
						}
					}
				}
			}
		}
		else //default end position of Temp_Index is rear of the buffer.
		{
			Temp_Index = COM_Data->head + Temp_Word_Len;
			for(Temp_Index; Temp_Index < COM_BUFFER_MAX; Temp_Index++)	// start and end position is rear of the buffer.
			{
				if(COM_Data->buffer[Temp_Index] == FindWords[Temp_Word_Len])
				{
					if(strncmp(COM_Data->buffer + Temp_Index - Temp_Word_Len, \
						       FindWords, Temp_Word_Len) == 0)
					{
						#if DEBUG_QUEUE
						printf("ResultData %ld \r\n", ResultData);
						#endif
						ResultLen = Temp_Index - COM_Data->head +1;
						if(*ResultData == NULL)
						{
							*ResultData = (char *)calloc(ResultLen+1, sizeof(char));
						}
						#if DEBUG_QUEUE
						printf("Index %d, Head %d, Tail %d \r\n",Temp_Index, COM_Data->head, COM_Data->tail);
						#endif
						memcpy(*ResultData, COM_Data->buffer + COM_Data->head, ResultLen);
						//ResultData[0][ResultLen] = 0;
						COM_Data->head = Temp_Index + 1;
						#if DEBUG_QUEUE
						printf("Head %d, Tail %d\r\n",COM_Data->head, COM_Data->tail); 
						#if DEBUG_QUEUE_RECV
						printf("Recv[%d][%s] \r\n", ResultLen, *ResultData);
						#endif
						printf("step4 \r\n");
						#endif
						return ResultLen;
					}
				}
			}
			for(Temp_Index = 0; Temp_Index < COM_Data->tail; Temp_Index++) //end position is front of the buffer.
			{
				if(COM_Data->buffer[Temp_Index] == FindWords[Temp_Word_Len])
				{
					if(Temp_Index - Temp_Word_Len >= 0) //start position of Temp_Index is front of the buffer.
					{
						if(strncmp(COM_Data->buffer + Temp_Index - Temp_Word_Len, \
							       FindWords, Temp_Word_Len) == 0)
						{
							#if DEBUG_QUEUE
							printf("ResultData %ld \r\n", ResultData);
							#endif
							ResultLen = (COM_BUFFER_MAX - COM_Data->head) + Temp_Index + 1;
							if(*ResultData == NULL)
							{
								*ResultData = (char *)calloc(ResultLen+1, sizeof(char));
							}
							#if DEBUG_QUEUE
							printf("Index %d, Head %d, Tail %d \r\n",Temp_Index, COM_Data->head, COM_Data->tail);
							#endif
							memcpy(*ResultData, COM_Data->buffer + COM_Data->head, (COM_BUFFER_MAX - COM_Data->head));
							memcpy(*ResultData + (COM_BUFFER_MAX - COM_Data->head), COM_Data->buffer, Temp_Index);
							//ResultData[0][ResultLen] = 0;
							COM_Data->head = Temp_Index + 1;
							#if DEBUG_QUEUE
							printf("Head %d, Tail %d\r\n",COM_Data->head, COM_Data->tail);
							#if DEBUG_QUEUE_RECV
							printf("Recv[%d][%s] \r\n", ResultLen, *ResultData);
							#endif
							printf("step5 \r\n");
							#endif
							return ResultLen;
						}
					}
					else //start position of Temp_Index is rear of the buffer.
					{
						if((strncmp(COM_Data->buffer, FindWords + Temp_Word_Len - Temp_Index, Temp_Index)==0)\
							&&(strncmp(COM_Data->buffer + COM_BUFFER_MAX + Temp_Index - Temp_Word_Len  , FindWords, Temp_Word_Len - Temp_Index) == 0))
						{
							#if DEBUG_QUEUE
							printf("ResultData %ld \r\n", ResultData);
							#endif
							ResultLen = (COM_BUFFER_MAX - COM_Data->head) + Temp_Index + 1;
							if(*ResultData == NULL)
							{
								*ResultData = (char *)calloc(ResultLen+1, sizeof(char));
							}
							#if DEBUG_QUEUE
							printf("Index %d, Head %d, Tail %d \r\n",Temp_Index, COM_Data->head, COM_Data->tail);
							#endif
							memcpy(*ResultData, COM_Data->buffer + COM_Data->head, (COM_BUFFER_MAX - COM_Data->head));
							memcpy(*ResultData + (COM_BUFFER_MAX - COM_Data->head), COM_Data->buffer, Temp_Index);
					//ResultData[0][ResultLen] = 0;
							COM_Data->head = Temp_Index + 1;
							#if DEBUG_QUEUE
							printf("Head %d, Tail %d\r\n",COM_Data->head, COM_Data->tail); 
							#if DEBUG_QUEUE_RECV
							printf("Recv[%d][%s] \r\n", ResultLen, *ResultData);
							#endif
							printf("step6 \r\n");
							#endif
							return ResultLen;
						}
					}
				}
			}
		}
	}
	return 0;
}

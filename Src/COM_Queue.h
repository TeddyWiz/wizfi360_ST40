#ifndef __COM_QUEUE_H
#define __COM_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG_QUEUE 	0
#define DEBUG_QUEUE_RECV 1

#define COM_BUFFER_MAX 		2048

/**
 * @brief COM Buffer Struct
 * 
 */
typedef struct COM_Buffer_t
{
	char buffer[COM_BUFFER_MAX];
	char flag;
	int head;
	int tail;
}COM_Buffer;

/**
 * @brief Str Unit
 * 
 */
typedef struct Str_Unit_t
{
	char *StrData;
	int  StrLen;
}Str_Unit;

/**
 * @brief Queue Data Init
 * 
 * @param COM_Data COM Buffer 변수
 */
void COM_Buffer_Init(COM_Buffer *COM_Data);

/**
 * @brief char Data을 Queue에 입력한다.
 * 
 * @param COM_Data COM Buffer 변수
 * @param ch 입력할 Data
 */
void COM_Enqueue(COM_Buffer *COM_Data, char ch);

/**
 * @brief Queue에 있는 Data를 출력한다.
 * 
 * @param COM_Data COM Buffer 변수
 * @return 출력 Data
 */
char COM_Dequeue(COM_Buffer *COM_Data);

/**
 * @brief Queue flag를 초기화 한다.
 * 
 * @param COM_Data COM Buffer 변수
 */
void COM_Flag_Reset(COM_Buffer *COM_Data);

/**
 * @brief Queue의 Flag의 값을 반환한다.
 * 
 * @param COM_Data COM Buffer 변수
 * @return Flag
 */
char get_COM_Flag(COM_Buffer *COM_Data);

/**
 * @brief Queue에서 입력된 데이터 사이즈를 반환한다.
 * 
 * @param COM_Data COM Buffer 변수
 * @return Data Size
 */
int COM_Used_Buffer(COM_Buffer *COM_Data);

/**
 * @brief Queue의 데이터가 가득차 있으면 1이상의 값을 반환한다.
 * 
 * @param COM_Data COM Buffer 변수
 * @return 1 또는 2: Queue Full, 0: Queue not Full
 */
char COM_Buffer_Full(COM_Buffer *COM_Data);

/**
 * @brief Queue의 데이터가 없으면 1을 반환한다.
 * 
 * @param COM_Data COM Buffer 변수
 * @return 1: Queue Empty, 0: Queue Not Empty
 */
char COM_Buffer_Empty(COM_Buffer *COM_Data);

/**
 * @brief Queue에서 FindWords를 찾아서 있으면 FindWords를 포함한 받았던 모든 데이터를 ResultData에 메모리를 생성, 데이터를 넣어서 반환한다.
 * 
 * @param COM_Data COM Buffer 변수
 * @param FindWords 찾을 문자열
 * @param FindWordsLen 찾을 문자열 길이
 * @param ResultData 수신한 Data
 * @return 수신한 데이터 길이 
 */
int COM_Find_Words(COM_Buffer *COM_Data, char *FindWords, int FindWordsLen, char **ResultData);

#ifdef __cplusplus
}
#endif
#endif

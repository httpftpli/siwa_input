#ifndef __ALGORITHM__H__
#define __ALGORITHM__H__

#include "stdbool.h"
#include "type.h"




typedef struct tagfilo {
    unsigned long writeIndex;   /** < @brief 写下标  */
    unsigned long readIndex;    /** < @brief 写下标  */
	unsigned char sizeOfItem;
    unsigned long nItem;        /** < @brief item总个数 */
    bool full;
    bool empty;
	unsigned long nAvaItem;		/** < @brief 记录当前fifi中的item个数*/
    void *        buf;        /** < @brief  缓冲区指针 */
}RINGBUF;



typedef struct
{
	unsigned int count[2];
	unsigned int state[4];
	unsigned char buffer[64];
}MD5_CTX;


#define DIVUP(A,DIV) (((A)+(DIV)-1)/(DIV))

extern uint8 const UnMapTbl[];

extern void ringBufInit(RINGBUF *ringBuf, void * buf, unsigned int sizeOfItem,unsigned int nItem);
extern bool isRingBufFull(RINGBUF *ringBuf);
extern bool isRingBufEmpty(RINGBUF *ringBuf);
extern bool ringBufPush(RINGBUF *ringBuf, void *item);
extern bool ringBufPop(RINGBUF * ringBuf, void * item);
extern bool ringBufRead(RINGBUF * ringBuf, void ** item);


#endif










/**
 *  \file   algorithm.c
 *
 *  \brief
 *  \author  lfl
 *  \addtogroup Algorithm
 *  \# include "misc.h"
 *   @{
 *
 */


#include "algorithm.h"
#include "string.h"
#include "type.h"



void bubbleSortAscend_I32(int *buf, unsigned int nNum) {
    int j, k;
    int flag = nNum;
    unsigned int tmp;
    while (flag > 0) {
        k = flag;
        flag = 0;
        for (j = 1; j < k; j++) {
            if (buf[j - 1] > buf[j]) {
                tmp = buf[j - 1];
                buf[j - 1] = buf[j];
                buf[j] = tmp;
                flag = j;
            }
        }
    }
}

void bubbleSortAscend_U32(unsigned int *buf, unsigned int nNum) {
    int j, k;
    int flag = nNum;
    unsigned int tmp;
    while (flag > 0) {
        k = flag;
        flag = 0;
        for (j = 1; j < k; j++) {
            if (buf[j - 1] > buf[j]) {
                tmp = buf[j - 1];
                buf[j - 1] = buf[j];
                buf[j] = tmp;
                flag = j;
            }
        }
    }
}

void bubbleSortAscend_I16(short *buf, unsigned int nNum) {
    int j, k;
    int flag = nNum;
    unsigned int tmp;
    while (flag > 0) {
        k = flag;
        flag = 0;
        for (j = 1; j < k; j++) {
            if (buf[j - 1] > buf[j]) {
                tmp = buf[j - 1];
                buf[j - 1] = buf[j];
                buf[j] = tmp;
                flag = j;
            }
        }
    }
}

void bubbleSortAscend_U16(unsigned short *buf, unsigned int nNum) {
    int j, k;
    int flag = nNum;
    unsigned int tmp;
    while (flag > 0) {
        k = flag;
        flag = 0;
        for (j = 1; j < k; j++) {
            if (buf[j - 1] > buf[j]) {
                tmp = buf[j - 1];
                buf[j - 1] = buf[j];
                buf[j] = tmp;
                flag = j;
            }
        }
    }
}

void bubbleSortAscend_I8(char *buf, unsigned int nNum) {
    int j, k;
    int flag = nNum;
    unsigned int tmp;
    while (flag > 0) {
        k = flag;
        flag = 0;
        for (j = 1; j < k; j++) {
            if (buf[j - 1] > buf[j]) {
                tmp = buf[j - 1];
                buf[j - 1] = buf[j];
                buf[j] = tmp;
                flag = j;
            }
        }
    }
}

void bubbleSortAscend_U8(unsigned char *buf, unsigned int nNum) {
    int j, k;
    int flag = nNum;
    unsigned int tmp;
    while (flag > 0) {
        k = flag;
        flag = 0;
        for (j = 1; j < k; j++) {
            if (buf[j - 1] > buf[j]) {
                tmp = buf[j - 1];
                buf[j - 1] = buf[j];
                buf[j] = tmp;
                flag = j;
            }
        }
    }
}

void bubbleSortAscend(int *buf, unsigned int nNum) {
    int j, k;
    int flag = nNum;
    unsigned int tmp;
    while (flag > 0) {
        k = flag;
        flag = 0;
        for (j = 1; j < k; j++) {
            if (buf[j - 1] > buf[j]) {
                tmp = buf[j - 1];
                buf[j - 1] = buf[j];
                buf[j] = tmp;
                flag = j;
            }
        }
    }
}





int sum(int *buf, unsigned int nNum) {
    unsigned int sum = 0;
    for (int i = 0; i < nNum; i++) {
        sum += buf[i];
    }
    return sum;
}



void ringBufInit(RINGBUF *ringBuf, void *buf, unsigned int sizeOfItem, unsigned int nItem) {
    ringBuf->writeIndex = 0;
    ringBuf->readIndex = 0;
    ringBuf->sizeOfItem = sizeOfItem;
    ringBuf->nItem = nItem;
    ringBuf->buf = buf;
    ringBuf->empty = true;
    ringBuf->full = false;
}


BOOL isRingBufFull(RINGBUF *ringBuf) {
    return ringBuf->full;
}

BOOL isRingBufEmpty(RINGBUF *ringBuf) {
    return ringBuf->empty;
}


BOOL ringBufPush(RINGBUF *ringBuf, void *item) {

    if (isRingBufFull(ringBuf)) return 0;
    memcpy((unsigned char *)(ringBuf->buf) + ((ringBuf->writeIndex) * ringBuf->sizeOfItem)
           , item, ringBuf->sizeOfItem);
    //IntMasterIRQDisable();
    if (++ringBuf->writeIndex == ringBuf->nItem) ringBuf->writeIndex = 0;
    if (ringBuf->writeIndex==ringBuf->readIndex) {
        ringBuf->full = true;
    }
    ringBuf->empty = false;
    //IntMasterIRQEnable();
    return 1;
}



BOOL ringBufPop(RINGBUF *ringBuf, void *item) {
    if (isRingBufEmpty(ringBuf)) {
        return 0;
    }
    //IntMasterIRQDisable();
    memcpy(item, (unsigned char *)ringBuf->buf + ringBuf->readIndex * ringBuf->sizeOfItem, ringBuf->sizeOfItem);
    if (++ringBuf->readIndex == ringBuf->nItem) ringBuf->readIndex = 0;
    if (ringBuf->writeIndex==ringBuf->readIndex) {
        ringBuf->empty = true;
    }
    ringBuf->full = false;
    //IntMasterIRQEnable();
    return 1;
}



BOOL ringBufRead(RINGBUF *ringBuf, void **item) {
    if (isRingBufEmpty(ringBuf)) {
        return 0;
    }
    *item = (void *)((unsigned char *)ringBuf->buf + ringBuf->readIndex * ringBuf->sizeOfItem);
    return 1;
}


void ringBufClear(RINGBUF *ringBuf) {
    //IntMasterIRQDisable();
    ringBuf->readIndex = 0;
    ringBuf->writeIndex = 0;
    ringBuf->empty = true;
    ringBuf->full = false;
    //IntMasterIRQEnable();
}




//! @}

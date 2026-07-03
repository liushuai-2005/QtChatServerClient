#include "protocol.h"
#include "stdlib.h"
#include "string.h"
PDU *initPDU(uint uiMsgLen)
{
    //总长度=结构体的大小+柔性数组的长度
    uint uiTotalLen = sizeof(PDU) + uiMsgLen;
    //申请一块内存，用来存放PDU结构体
    PDU* pdu = (PDU*)malloc(uiTotalLen);
    //清空内存，全置为0
    //从pdu开始清，清成0，清uiTotalLen的范围
    memset(pdu,0,uiTotalLen);

    pdu->uiTotalLen = uiTotalLen;
    pdu->uiMsgLen = uiMsgLen;
    return pdu;
}

#ifndef __ECAT_TYPEDEF__
#define __ECAT_TYPEDEF__

typedef struct Ecat2Can_Pack
{
    __UINT32_TYPE__ StdId;
    __UINT8_TYPE__ ExtId;
    __UINT8_TYPE__ IDE;
    __UINT8_TYPE__ RTR; 
    __UINT8_TYPE__ DLC; 

    __UINT8_TYPE__ Data[8];

    bool LED[3];
}Ecat2Can_Pack;

#endif
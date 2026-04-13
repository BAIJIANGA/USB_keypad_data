#ifndef __KEY__
#define __KEY__

typedef unsigned char KEY_U8;
typedef unsigned short int KEY_U16;
typedef unsigned int KEY_U32;

typedef struct 
{
    KEY_U8* KEY_VALUE;//键值存储 按键低位在前 一个字节存储8个键值 调用KEY_SCAN之前需要先将键值写入
    KEY_U8* KEY_BUF;//消抖缓冲 一个按键一个字节
    KEY_U8* KEY_NOW_STATE;//按键当前状态 一个按键一个bit
    KEY_U8* KEY_OLD_STATE;//按键上一次状态 一个按键一个bit
    void (*Keyaction_HIGH)(KEY_U32 KEY_Index);//按键高电平回调函数
    void (*Keyaction_LOW)(KEY_U32 KEY_Index);//按键低电平回调函数
}KEY_HandleTypeDef;

void KEY_SCAN(KEY_HandleTypeDef* KEYx,KEY_U32 KEY_NUM);

#endif

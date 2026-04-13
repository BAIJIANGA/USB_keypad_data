#include "KEY.h"

/*******************************************************************************
 * 函数名：KEY_SCAN
 * 描述  ：按键扫描算法
 * 输入  KEY_HandleTypeDef	 KEYx句柄 KEY_NUM 按键个数
 * 输出  ：void
 * 调用  ：外部调用
 * 备注  ：建议放中断中 15ms扫描一次 扫描权重目前是4组电平变化即0X0f
 *******************************************************************************/
void KEY_SCAN(KEY_HandleTypeDef* KEYx,KEY_U32 KEY_NUM)
{

    KEY_U8 a = 0;
    KEY_U8 b = 0;

    for(a = 0;a < KEY_NUM/8;a++)
    {
        for(b = 0;b < 8;b++)
        {
            KEYx->KEY_BUF[a*8+b] = (KEYx->KEY_BUF[a*8+b]<<1)|((KEYx->KEY_VALUE[a]&(0x01<<b))>>b);
        }
    }

    for(b = 0;b<KEY_NUM%8;b++)
    {
         KEYx->KEY_BUF[a*8+b] = (KEYx->KEY_BUF[a*8+b]<<1)|((KEYx->KEY_VALUE[a]&(0x01<<b))>>b);
    }

    for(a = 0;a < KEY_NUM/8;a++)
    {
        for(b = 0;b < 8;b++)
        {
            if(((KEYx->KEY_BUF[(a<<3)+b])&0x0F) == 0x0F)
            {
                KEYx->KEY_NOW_STATE[a] |= (0x01<<b);
                if((KEYx->KEY_NOW_STATE[a]&(0x01<<b)) != (KEYx->KEY_OLD_STATE[a]&(0x01<<b)))
                {
                    KEYx->KEY_OLD_STATE[a] |= (0x01<<b);
                    KEYx->Keyaction_HIGH((a<<3)+b);
                }
            }
            else if(((KEYx->KEY_BUF[(a<<3)+b])|0xF0) == 0xF0)
            {
                KEYx->KEY_NOW_STATE[a] &= (~(0x01<<b));
                if((KEYx->KEY_NOW_STATE[a]&(0x01<<b)) != (KEYx->KEY_OLD_STATE[a]&(0x01<<b)))
                {
                    KEYx->KEY_OLD_STATE[a] &= (~(0x01<<b));
                    KEYx->Keyaction_LOW((a<<3)+b);
                }
            }
        }
    }

    for(b = 0;b<KEY_NUM%8;b++)
    {
        if(((KEYx->KEY_BUF[(a<<3)+b])&0x0F) == 0x0F)
        {
            KEYx->KEY_NOW_STATE[a] |= (0x01<<b);
            if((KEYx->KEY_NOW_STATE[a]&(0x01<<b)) != (KEYx->KEY_OLD_STATE[a]&(0x01<<b)))
            {
                KEYx->KEY_OLD_STATE[a] |= (0x01<<b);
                KEYx->Keyaction_HIGH((a<<3)+b);
            }
        }
        else if(((KEYx->KEY_BUF[(a<<3)+b])|0xF0) == 0xF0)
        {
            KEYx->KEY_NOW_STATE[a] &= (~(0x01<<b));
            if((KEYx->KEY_NOW_STATE[a]&(0x01<<b)) != (KEYx->KEY_OLD_STATE[a]&(0x01<<b)))
            {
                KEYx->KEY_OLD_STATE[a] &= (~(0x01<<b));
                KEYx->Keyaction_LOW((a<<3)+b);
            }
        }    
    }

}

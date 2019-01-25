/*
 * Copyright (c) 2018, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "sensirion_arch_config.h"
#include "sensirion_common.h"
#include "sensirion_i2c.h"
#include "stm32f10x_gpio.h"
#include "delay.h"

#define DELAY_USEC (I2C_CLOCK_PERIOD_USEC / 2)


/**
 * Initialize all hard- and software components that are needed to set the
 * SDA and SCL pins.
 */
void sensirion_init_pins()
{
	GPIO_InitTypeDef        GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_2|GPIO_Pin_3); 	//PB10,PB11 Êä³ö¸ß
//	GPIO_ResetBits(GPIOA,GPIO_Pin_2|GPIO_Pin_3); 	
}

void SDA_in()  
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
} 


void SDA_out()  
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	
  GPIO_StructInit(&GPIO_InitStructure);  
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;  
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
}

/**
 * Configure the SDA pin as an input. The pin should be either left floating
 * or pulled up to the supply voltage.
 */
void sensirion_SDA_in()
{
	GPIO_SetBits(GPIOA, GPIO_Pin_3);
}

/**
 * Configure the SDA pin as an output and drive it low. The pin must be pulled
 * to ground or set to logical false.
 */
void sensirion_SDA_out()
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_3);
}

/**
 * Read the value of the SDA pin.
 * @returns 0 if the pin is low and 1 otherwise.
 */
u8 sensirion_SDA_read()
{
	return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3);
}

/**
 * Configure the SCL pin as an input. The pin should be either left floating
 * or pulled up to the supply voltage.
 */
void sensirion_SCL_in()
{
	GPIO_SetBits(GPIOA, GPIO_Pin_2);
}

/**
 * Configure the SCL pin as an output and drive it low. The pin must be pulled
 * to ground or set to logical false.
 */
void sensirion_SCL_out()
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_2);  
}

/**
 * Sleep for a given number of microseconds. The function should delay the
 * execution for at least the given time, but may also sleep longer.
 *
 * @param useconds the sleep time in microseconds
 */
void sensirion_sleep_usec(u32 useconds)
{
//	useconds *= 10;
//	for(;useconds>0;useconds--);
	delay_us(useconds);
}

static s8 sensirion_i2c_write_byte(u8 data)
{
	s8 nack, i;
	SDA_out();
	for (i = 7; i >= 0; i--) {
			sensirion_SCL_out();
			if ((data >> i) & 0x01)
					sensirion_SDA_in();
			else
					sensirion_SDA_out();
			sensirion_sleep_usec(DELAY_USEC);
			sensirion_SCL_in();
			sensirion_sleep_usec(DELAY_USEC);
	}
	sensirion_SCL_out();
	sensirion_SDA_in();
	sensirion_sleep_usec(DELAY_USEC);
	SDA_in();
	sensirion_SCL_in();
	sensirion_sleep_usec(DELAY_USEC);
	nack = (sensirion_SDA_read() != 0);
	sensirion_SCL_out();

	return nack;
}

static u8 sensirion_i2c_read_byte(u8 ack)
{
	s8 i;
	u8 data = 0;
	sensirion_SDA_in();
	SDA_in();
	for (i = 7; i >= 0; i--) {
			sensirion_sleep_usec(DELAY_USEC);
			sensirion_SCL_in();
			sensirion_sleep_usec(DELAY_USEC);
			data |= (sensirion_SDA_read() != 0) << i;
			sensirion_SCL_out();
	}
	SDA_out();
	sensirion_sleep_usec(DELAY_USEC);
	if (ack)
			sensirion_SDA_out();
	else
			sensirion_SDA_in();
	sensirion_sleep_usec(DELAY_USEC);
	sensirion_SCL_in();
	sensirion_sleep_usec(DELAY_USEC);
	sensirion_SCL_out();
	sensirion_SDA_in();

	return data;
}

static void sensirion_i2c_start(void)
{
	SDA_out();
	sensirion_SDA_out();
	sensirion_sleep_usec(DELAY_USEC);
	sensirion_SCL_out();
	sensirion_sleep_usec(DELAY_USEC);
}

static void sensirion_i2c_stop(void)
{
	SDA_out();  
	sensirion_SDA_out();
	sensirion_sleep_usec(DELAY_USEC);
	sensirion_SCL_in();
	sensirion_sleep_usec(DELAY_USEC);
	sensirion_SDA_in();
	sensirion_sleep_usec(DELAY_USEC);
}

s8 sensirion_i2c_write(u8 address, const u8* data, u16 count)
{
    s8 ret;
    u16 i;
    sensirion_i2c_start();
    ret = sensirion_i2c_write_byte(address << 1);
    if (ret != STATUS_OK) {
        sensirion_i2c_stop();
        return ret;
    }
    for (i = 0; i < count; i++) {
        ret = sensirion_i2c_write_byte(data[i]);
        if (ret != STATUS_OK) {
            sensirion_i2c_stop();
            break;
        }
    }
    sensirion_i2c_stop();
    return ret;
}

s8 sensirion_i2c_read(u8 address, u8* data, u16 count)
{
    s8 ret;
    u16 i;
    sensirion_i2c_start();
    ret = sensirion_i2c_write_byte((address << 1) | 1);
    if (ret != STATUS_OK) {
        sensirion_i2c_stop();
        return ret;
    }
    for (i = 0; i < count; i++)
        data[i] = sensirion_i2c_read_byte(1);

    sensirion_i2c_stop();
    return ret;
}

void sensirion_i2c_init()
{
    sensirion_init_pins();
}

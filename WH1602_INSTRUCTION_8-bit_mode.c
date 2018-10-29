//Displey_WEH001602
//
//распиновка подключения дисплея к AT90USB1287:
//1  - VSS - Ground
//2  - VDD - +5V
//3  - NC  - PB4 - Light Displey
//4  - RS  - PB5 - L:Command; H:Data
//5  - R/W - PB6 - L:Write;   H:Read
//6  - E   - PB7 - Enabled
//7  - DB0 - PD0 - DATA
//8  - DB1 - PD1 - DATA
//9  - DB2 - PD2 - DATA
//10 - DB3 - PD3 - DATA
//11 - DB4 - PD4 - DATA
//12 - DB5 - PD5 - DATA
//13 - DB6 - PD6 - DATA
//14 - DB7 - PD7 - DATA
//15 - NC  - No connect
//16 - NC  - No connect
//
//значащие биты:
//IDaddress     Increment/Decrement Bit
//				=1 Increment Bit, =0 Decrement Bit
//Shift			Shift Entire Display Control Bit
//				=0 Shift Function disable
//Dbit			Sets Entire Display
//				=1 Sets Entire Display on, =0 Sets Entire Display Off
//Cbit			Sets Cursor
//				=1 Sets Cursor on, =0 Sets Cursor off
//Bbit			Sets Blinking of Cursor Position Character
//				=1 Sets Blinking on, =0 Sets Blinking off
//SCbit			Display Shift/Cursor Move
//				=1 Display Shift, =0 Cursor Move
//RLbit			Shift Right/Left
//				=1 Shift to the Right, =0 Shift to the Left
//GCbit			Graphic/Character Mode Selection
//				=1 Graphic Mode is selected, =0 Character Mode is selected
//PWRbit		Internal DCDC On/Off Control
//				=1 DCDC on, =0 DCDC off
//DLbit			Sets Interface Data Length
//				=1 for an 8-Bit Data Transfer, =0 for an 4-Bit Data Transfer
//Nbit			Number of Lines Displayed
//				=1 2-Line Display, =0 1-Line Display
//Fbit			Character Pattern Mode
//				=1 5 x 10 dots, =0 5 x 8 dots
//FT1bit		Sets Font Table
//FT0bit		Sets Font Table
//				FT1   FT0    Font Table
//				 0     0     the ENGLISH_JAPANESE CHARACTER FONT TABLE
//				 0     1     the WESTERN EUROPEAN CHARACTER FONT TABLE
//				 1     0     the ENGLISH_RUSSIAN CHARACTER FONT TABLE
//				 1     1     N/A
//BusyFlag		Busy Flag
//				=1 Internal Operating in Progress
//				=0 No Internal Operation is being executed, next instruction can be accepted

#define F_CPU 16000000UL 
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#define WH1602_NC_DDR     DDRB
#define WH1602_NC_PORT    PORTB
#define WH1602_NC_PIN     PINB
#define WH1602_NC_LEG     4

#define WH1602_RS_DDR     DDRB
#define WH1602_RS_PORT    PORTB
#define WH1602_RS_PIN     PINB
#define WH1602_RS_LEG     5

#define WH1602_RW_DDR     DDRB
#define WH1602_RW_PORT    PORTB 
#define WH1602_RW_PIN     PINB
#define WH1602_RW_LEG     6

#define WH1602_E_DDR      DDRB
#define WH1602_E_PORT     PORTB
#define WH1602_E_PIN      PINB
#define WH1602_E_LEG      7

#define WH1602_DB7_DDR    DDRD
#define WH1602_DB7_PORT   PORTD
#define WH1602_DB7_PIN    PIND
#define WH1602_DB7_LEG    7

#define WH1602_DB6_DDR    DDRD
#define WH1602_DB6_PORT   PORTD
#define WH1602_DB6_PIN    PIND
#define WH1602_DB6_LEG    6

#define WH1602_DB5_DDR    DDRD
#define WH1602_DB5_PORT   PORTD
#define WH1602_DB5_PIN    PIND
#define WH1602_DB5_LEG    5

#define WH1602_DB4_DDR    DDRD
#define WH1602_DB4_PORT   PORTD
#define WH1602_DB4_PIN    PIND
#define WH1602_DB4_LEG    4

#define WH1602_DB3_DDR    DDRD
#define WH1602_DB3_PORT   PORTD
#define WH1602_DB3_PIN    PIND
#define WH1602_DB3_LEG    3

#define WH1602_DB2_DDR    DDRD
#define WH1602_DB2_PORT   PORTD
#define WH1602_DB2_PIN    PIND
#define WH1602_DB2_LEG    2

#define WH1602_DB1_DDR    DDRD
#define WH1602_DB1_PORT   PORTD
#define WH1602_DB1_PIN    PIND
#define WH1602_DB1_LEG    1

#define WH1602_DB0_DDR    DDRD
#define WH1602_DB0_PORT   PORTD
#define WH1602_DB0_PIN    PIND
#define WH1602_DB0_LEG    0

#define STATE_E_DELAY     2

#define BIT_7_MASK        0x80
#define BIT_6_MASK        0x40
#define BIT_5_MASK        0x20
#define BIT_4_MASK        0x10
#define BIT_3_MASK        0x08
#define BIT_2_MASK        0x04
#define BIT_1_MASK        0x02
#define BIT_0_MASK        0x01

typedef enum bool	{
	FALSE,
	TRUE
} bool;

unsigned char address=0x00;
unsigned char data=0x00;

//*************ФУНКЦИИ НЕ ЯВЛЯЮЩИЕСЯ ИНСТРУКЦИЯМИ**********************
//настройка портов подключенных к шинам INSTRUCTION и DATA дисплея на выход
void DDR_Init_DataWrite()	{
	WH1602_NC_DDR|=_BV(WH1602_NC_LEG);
	WH1602_RS_DDR|=_BV(WH1602_RS_LEG);
	WH1602_RW_DDR|=_BV(WH1602_RW_LEG);
	WH1602_E_DDR|=_BV(WH1602_E_LEG);
	WH1602_DB7_DDR|=_BV(WH1602_DB7_LEG);
	WH1602_DB6_DDR|=_BV(WH1602_DB6_LEG);
	WH1602_DB5_DDR|=_BV(WH1602_DB5_LEG);
	WH1602_DB4_DDR|=_BV(WH1602_DB4_LEG);
	WH1602_DB3_DDR|=_BV(WH1602_DB3_LEG);
	WH1602_DB2_DDR|=_BV(WH1602_DB2_LEG);
	WH1602_DB1_DDR|=_BV(WH1602_DB1_LEG);
	WH1602_DB0_DDR|=_BV(WH1602_DB0_LEG);
}
//настройка портов подключенных к шине DATA дисплея на вход
void DDR_Init_DataRead()	{
	WH1602_DB7_DDR&=~_BV(WH1602_DB7_LEG);
	WH1602_DB7_PORT|=_BV(WH1602_DB7_LEG);
	WH1602_DB6_DDR&=~_BV(WH1602_DB7_LEG);
	WH1602_DB6_PORT|=_BV(WH1602_DB7_LEG);
	WH1602_DB5_DDR&=~_BV(WH1602_DB7_LEG);
	WH1602_DB5_PORT|=_BV(WH1602_DB7_LEG);
	WH1602_DB4_DDR&=~_BV(WH1602_DB7_LEG);
	WH1602_DB4_PORT|=_BV(WH1602_DB7_LEG);
	WH1602_DB3_DDR&=~_BV(WH1602_DB7_LEG);
	WH1602_DB3_PORT|=_BV(WH1602_DB7_LEG);
	WH1602_DB2_DDR&=~_BV(WH1602_DB7_LEG);
	WH1602_DB2_PORT|=_BV(WH1602_DB7_LEG);
	WH1602_DB1_DDR&=~_BV(WH1602_DB7_LEG);
	WH1602_DB1_PORT|=_BV(WH1602_DB7_LEG);
	WH1602_DB0_DDR&=~_BV(WH1602_DB7_LEG);
	WH1602_DB0_PORT|=_BV(WH1602_DB7_LEG);
}
//чтение состояния портов подключенных к шине DATA
char PIN_Use_DataRead()	{
	data=0x00;
	if (WH1602_DB7_PIN&_BV(WH1602_DB7_LEG))
		data|=BIT_7_MASK;
	if (WH1602_DB6_PIN&_BV(WH1602_DB6_LEG))
		data|=BIT_6_MASK;
	if (WH1602_DB5_PIN&_BV(WH1602_DB5_LEG))
		data|=BIT_5_MASK;
	if (WH1602_DB4_PIN&_BV(WH1602_DB4_LEG))
		data|=BIT_4_MASK;
	if (WH1602_DB3_PIN&_BV(WH1602_DB3_LEG))
		data|=BIT_3_MASK;
	if (WH1602_DB2_PIN&_BV(WH1602_DB2_LEG))
		data|=BIT_2_MASK;
	if (WH1602_DB1_PIN&_BV(WH1602_DB1_LEG))
		data|=BIT_1_MASK;
	if (WH1602_DB0_PIN&_BV(WH1602_DB0_LEG))
		data|=BIT_0_MASK;
	
	return(data);
}
//команда начала обмена данными
void State_E_DataReadWrite()	{
	WH1602_E_PORT|=_BV(WH1602_E_LEG);
	_delay_us(STATE_E_DELAY);
	WH1602_E_PORT&=~_BV(WH1602_E_LEG);
}
//*********************************************************************

//*************************INSTRUCTIONS********************************
//очистка всех значений DDRAM дисплея и перемещение позиции курсора в начальное состояние
void WH1602_ClearDisplay()	{
	WH1602_RS_PORT&=~_BV(WH1602_RS_LEG);
	WH1602_RW_PORT&=~_BV(WH1602_RW_LEG);
	WH1602_DB7_PORT&=~_BV(WH1602_DB7_LEG);
	WH1602_DB6_PORT&=~_BV(WH1602_DB6_LEG);
	WH1602_DB5_PORT&=~_BV(WH1602_DB5_LEG);
	WH1602_DB4_PORT&=~_BV(WH1602_DB4_LEG);
	WH1602_DB3_PORT&=~_BV(WH1602_DB3_LEG);
	WH1602_DB2_PORT&=~_BV(WH1602_DB2_LEG);
	WH1602_DB1_PORT&=~_BV(WH1602_DB1_LEG);
	WH1602_DB0_PORT|=_BV(WH1602_DB0_LEG);
	
	State_E_DataReadWrite();
}
//установка адреса DDRAM в 0 и перемещение позиции курсора в начальное состояние
void WH1602_ReturnHome()	{
	WH1602_RS_PORT&=~_BV(WH1602_RS_LEG);
	WH1602_RW_PORT&=~_BV(WH1602_RW_LEG);
	WH1602_DB7_PORT&=~_BV(WH1602_DB7_LEG);
	WH1602_DB6_PORT&=~_BV(WH1602_DB6_LEG);
	WH1602_DB5_PORT&=~_BV(WH1602_DB5_LEG);
	WH1602_DB4_PORT&=~_BV(WH1602_DB4_LEG);
	WH1602_DB3_PORT&=~_BV(WH1602_DB3_LEG);
	WH1602_DB2_PORT&=~_BV(WH1602_DB2_LEG);
	WH1602_DB1_PORT|=_BV(WH1602_DB1_LEG);
	WH1602_DB0_PORT&=~_BV(WH1602_DB0_LEG);
	
	State_E_DataReadWrite();
}
//установка режима ввода
void WH1602_EntryModeSet(bool IDaddress,bool Shift)	{
	WH1602_RS_PORT&=~_BV(WH1602_RS_LEG);
	WH1602_RW_PORT&=~_BV(WH1602_RW_LEG);
	WH1602_DB7_PORT&=~_BV(WH1602_DB7_LEG);
	WH1602_DB6_PORT&=~_BV(WH1602_DB6_LEG);
	WH1602_DB5_PORT&=~_BV(WH1602_DB5_LEG);
	WH1602_DB4_PORT&=~_BV(WH1602_DB4_LEG);
	WH1602_DB3_PORT&=~_BV(WH1602_DB3_LEG);
	WH1602_DB2_PORT|=_BV(WH1602_DB2_LEG);
	if(IDaddress==TRUE)
		WH1602_DB1_PORT|=_BV(WH1602_DB1_LEG);
	else
		WH1602_DB1_PORT&=~_BV(WH1602_DB1_LEG);
	if(Shift==TRUE)
		WH1602_DB0_PORT|=_BV(WH1602_DB0_LEG);
	else
		WH1602_DB0_PORT&=~_BV(WH1602_DB0_LEG);
	
	State_E_DataReadWrite();
}
//включение/выключение дисплея
void WH1602_DisplayOnOff(bool Dbit,bool Cbit,bool Bbit)	{
	WH1602_RS_PORT&=~_BV(WH1602_RS_LEG);
	WH1602_RW_PORT&=~_BV(WH1602_RW_LEG);
	WH1602_DB7_PORT&=~_BV(WH1602_DB7_LEG);
	WH1602_DB6_PORT&=~_BV(WH1602_DB6_LEG);
	WH1602_DB5_PORT&=~_BV(WH1602_DB5_LEG);
	WH1602_DB4_PORT&=~_BV(WH1602_DB4_LEG);
	WH1602_DB3_PORT&=~_BV(WH1602_DB3_LEG);
	if(Dbit==TRUE)
		WH1602_DB2_PORT|=_BV(WH1602_DB2_LEG);
	else
		WH1602_DB2_PORT&=~_BV(WH1602_DB2_LEG);
	if(Cbit==TRUE)
		WH1602_DB1_PORT|=_BV(WH1602_DB1_LEG);
	else
		WH1602_DB1_PORT&=~_BV(WH1602_DB1_LEG);
	if(Bbit==TRUE)
		WH1602_DB0_PORT|=_BV(WH1602_DB0_LEG);
	else
		WH1602_DB0_PORT&=~_BV(WH1602_DB0_LEG);
	
	State_E_DataReadWrite();	
}
//сдвиг курсора или дисплея влево или вправо без записи данных
void WH1602_CursorDisplayShift(bool SCbit,bool RLbit)	{
	WH1602_RS_PORT&=~_BV(WH1602_RS_LEG);
	WH1602_RW_PORT&=~_BV(WH1602_RW_LEG);
	WH1602_DB7_PORT&=~_BV(WH1602_DB7_LEG);
	WH1602_DB6_PORT&=~_BV(WH1602_DB6_LEG);
	WH1602_DB5_PORT&=~_BV(WH1602_DB5_LEG);
	WH1602_DB4_PORT|=_BV(WH1602_DB4_LEG);
	if(SCbit==TRUE)
		WH1602_DB3_PORT|=_BV(WH1602_DB3_LEG);
	else	
		WH1602_DB3_PORT&=~_BV(WH1602_DB3_LEG);
	if(RLbit==TRUE)
		WH1602_DB2_PORT|=_BV(WH1602_DB2_LEG);
	else
		WH1602_DB2_PORT&=~_BV(WH1602_DB2_LEG);
	WH1602_DB1_PORT&=~_BV(WH1602_DB1_LEG);
	WH1602_DB0_PORT&=~_BV(WH1602_DB0_LEG);
	
	State_E_DataReadWrite();
}
//выбор графического/символьного режима, внутренний dc-dc контроль
void WH1602_ModePwr(bool GCbit,bool PWRbit)	{
	WH1602_RS_PORT&=~_BV(WH1602_RS_LEG);
	WH1602_RW_PORT&=~_BV(WH1602_RW_LEG);
	WH1602_DB7_PORT&=~_BV(WH1602_DB7_LEG);
	WH1602_DB6_PORT&=~_BV(WH1602_DB6_LEG);
	WH1602_DB5_PORT&=~_BV(WH1602_DB5_LEG);
	WH1602_DB4_PORT|=_BV(WH1602_DB4_LEG);
	if(GCbit==TRUE)
		WH1602_DB3_PORT|=_BV(WH1602_DB3_LEG);
	else	
		WH1602_DB3_PORT&=~_BV(WH1602_DB3_LEG);
	if(PWRbit==TRUE)
		WH1602_DB2_PORT|=_BV(WH1602_DB2_LEG);
	else
		WH1602_DB2_PORT&=~_BV(WH1602_DB2_LEG);
	WH1602_DB1_PORT|=_BV(WH1602_DB1_LEG);
	WH1602_DB0_PORT|=_BV(WH1602_DB0_LEG);
	
	State_E_DataReadWrite();
}
//установка функций дисплея
void WH1602_FunctionSet(bool DLbit,bool Nbit,bool Fbit,bool FT1bit,bool FT0bit)	{
	WH1602_RS_PORT&=~_BV(WH1602_RS_LEG);
	WH1602_RW_PORT&=~_BV(WH1602_RW_LEG);
	WH1602_DB7_PORT&=~_BV(WH1602_DB7_LEG);
	WH1602_DB6_PORT&=~_BV(WH1602_DB6_LEG);
	WH1602_DB5_PORT|=_BV(WH1602_DB5_LEG);
	if(DLbit==TRUE)
		WH1602_DB4_PORT|=_BV(WH1602_DB4_LEG);
	else	
		WH1602_DB4_PORT&=~_BV(WH1602_DB4_LEG);
	if(Nbit==TRUE)
		WH1602_DB3_PORT|=_BV(WH1602_DB3_LEG);
	else	
		WH1602_DB3_PORT&=~_BV(WH1602_DB3_LEG);
	if(Fbit==TRUE)
		WH1602_DB2_PORT|=_BV(WH1602_DB2_LEG);
	else
		WH1602_DB2_PORT&=~_BV(WH1602_DB2_LEG);
	if(FT1bit==TRUE)
		WH1602_DB1_PORT|=_BV(WH1602_DB1_LEG);
	else
		WH1602_DB1_PORT&=~_BV(WH1602_DB1_LEG);
	if(FT0bit==TRUE)
		WH1602_DB0_PORT|=_BV(WH1602_DB0_LEG);
	else
		WH1602_DB0_PORT&=~_BV(WH1602_DB0_LEG);
	
	State_E_DataReadWrite();
}
//запись адреса CGRAM в счетчик адресов
void WH1602_SetCGRAMAddress(address)	{
	WH1602_RS_PORT&=~_BV(WH1602_RS_LEG);
	WH1602_RW_PORT&=~_BV(WH1602_RW_LEG);
	WH1602_DB7_PORT&=~_BV(WH1602_DB7_LEG);
	WH1602_DB6_PORT|=_BV(WH1602_DB6_LEG);
	if(address&BIT_5_MASK)
		WH1602_DB5_PORT|=_BV(WH1602_DB5_LEG);
	else	
		WH1602_DB5_PORT&=~_BV(WH1602_DB5_LEG);
	if(address&BIT_4_MASK)
		WH1602_DB4_PORT|=_BV(WH1602_DB4_LEG);
	else	
		WH1602_DB4_PORT&=~_BV(WH1602_DB4_LEG);
	if(address&BIT_3_MASK)
		WH1602_DB3_PORT|=_BV(WH1602_DB3_LEG);
	else	
		WH1602_DB3_PORT&=~_BV(WH1602_DB3_LEG);
	if(address&BIT_2_MASK)
		WH1602_DB2_PORT|=_BV(WH1602_DB2_LEG);
	else
		WH1602_DB2_PORT&=~_BV(WH1602_DB2_LEG);
	if(address&BIT_1_MASK)
		WH1602_DB1_PORT|=_BV(WH1602_DB1_LEG);
	else
		WH1602_DB1_PORT&=~_BV(WH1602_DB1_LEG);
	if(address&BIT_0_MASK)
		WH1602_DB0_PORT|=_BV(WH1602_DB0_LEG);
	else
		WH1602_DB0_PORT&=~_BV(WH1602_DB0_LEG);
	
	State_E_DataReadWrite();	
}
//запись адреса DDRAM в счетчик адресов
void WH1602_SetDDRAMAddress(address)	{
	WH1602_RS_PORT&=~_BV(WH1602_RS_LEG);
	WH1602_RW_PORT&=~_BV(WH1602_RW_LEG);
	WH1602_DB7_PORT|=_BV(WH1602_DB7_LEG);
	if(address&BIT_6_MASK)
		WH1602_DB6_PORT|=_BV(WH1602_DB6_LEG);
	else	
		WH1602_DB6_PORT&=~_BV(WH1602_DB6_LEG);	
	if(address&BIT_5_MASK)
		WH1602_DB5_PORT|=_BV(WH1602_DB5_LEG);
	else	
		WH1602_DB5_PORT&=~_BV(WH1602_DB5_LEG);
	if(address&BIT_4_MASK)
		WH1602_DB4_PORT|=_BV(WH1602_DB4_LEG);
	else	
		WH1602_DB4_PORT&=~_BV(WH1602_DB4_LEG);
	if(address&BIT_3_MASK)
		WH1602_DB3_PORT|=_BV(WH1602_DB3_LEG);
	else	
		WH1602_DB3_PORT&=~_BV(WH1602_DB3_LEG);
	if(address&BIT_2_MASK)
		WH1602_DB2_PORT|=_BV(WH1602_DB2_LEG);
	else
		WH1602_DB2_PORT&=~_BV(WH1602_DB2_LEG);
	if(address&BIT_1_MASK)
		WH1602_DB1_PORT|=_BV(WH1602_DB1_LEG);
	else
		WH1602_DB1_PORT&=~_BV(WH1602_DB1_LEG);
	if(address&BIT_0_MASK)
		WH1602_DB0_PORT|=_BV(WH1602_DB0_LEG);
	else
		WH1602_DB0_PORT&=~_BV(WH1602_DB0_LEG);
	
	State_E_DataReadWrite();
}
//чтение флага BusyFlag с циклом ожидания его обнуления
void WH1602_ReadBusyFlag()	{
	//устанавливаем команду чтения BusyFlag
	WH1602_RS_PORT&=~_BV(WH1602_RS_LEG);
	WH1602_RW_PORT|=_BV(WH1602_RW_LEG);
	//настраиваем ножку чтения BusyFlag на вход и делаем подтяжку к питанию
	WH1602_DB7_DDR&=~_BV(WH1602_DB7_LEG);
	WH1602_DB7_PORT|=_BV(WH1602_DB7_LEG);
	//продолжаем считывать состояние BusyFlag пока он не сбросится
	bool BusyFlag;
	while(BusyFlag!=FALSE)
	{
		//устанавливем высокий уровень команды обмена данными
		WH1602_E_PORT|=_BV(WH1602_E_LEG);
		_delay_us(STATE_E_DELAY);	
		//читаем состояние BusyFlag
		if (WH1602_DB7_PIN&_BV(WH1602_DB7_LEG))
			BusyFlag=TRUE;
		else
			BusyFlag=FALSE;
		//устанавливем низкий уровень команды обмена данными
		WH1602_E_PORT&=~_BV(WH1602_E_LEG);
		_delay_us(STATE_E_DELAY);
	}
	//настраиваем ножку чтения BusyFlag на выход
	WH1602_DB7_DDR|=_BV(WH1602_DB7_LEG);	
}
//считывание значения счетчика адреса
char WH1602_ReadAddress()	{
	WH1602_RS_PORT&=~_BV(WH1602_RS_LEG);
	WH1602_RW_PORT|=_BV(WH1602_RW_LEG);
	
	DDR_Init_DataRead();
	
	WH1602_E_PORT|=_BV(WH1602_E_LEG);
	_delay_us(STATE_E_DELAY);
	
	PIN_Use_DataRead();
	
	WH1602_E_PORT&=~_BV(WH1602_E_LEG);
	
	bool BusyFlag;
	if (BIT_7_MASK&data)
		BusyFlag=TRUE;
	else
		BusyFlag=FALSE;
	address=~BIT_7_MASK&data;
	
	DDR_Init_DataWrite();
	
	return(BusyFlag, address);
}
//запись 8-битных значений данных в CGRAM или DDRAM
void WH1602_WriteData(data)	{
	WH1602_RS_PORT|=_BV(WH1602_RS_LEG);
	WH1602_RW_PORT&=~_BV(WH1602_RW_LEG);
	if(data&BIT_7_MASK)
		WH1602_DB7_PORT|=_BV(WH1602_DB7_LEG);
	else	
		WH1602_DB7_PORT&=~_BV(WH1602_DB7_LEG);	
	if(data&BIT_6_MASK)
		WH1602_DB6_PORT|=_BV(WH1602_DB6_LEG);
	else	
		WH1602_DB6_PORT&=~_BV(WH1602_DB6_LEG);	
	if(data&BIT_5_MASK)
		WH1602_DB5_PORT|=_BV(WH1602_DB5_LEG);
	else	
		WH1602_DB5_PORT&=~_BV(WH1602_DB5_LEG);
	if(data&BIT_4_MASK)
		WH1602_DB4_PORT|=_BV(WH1602_DB4_LEG);
	else	
		WH1602_DB4_PORT&=~_BV(WH1602_DB4_LEG);
	if(data&BIT_3_MASK)
		WH1602_DB3_PORT|=_BV(WH1602_DB3_LEG);
	else	
		WH1602_DB3_PORT&=~_BV(WH1602_DB3_LEG);
	if(data&BIT_2_MASK)
		WH1602_DB2_PORT|=_BV(WH1602_DB2_LEG);
	else
		WH1602_DB2_PORT&=~_BV(WH1602_DB2_LEG);
	if(data&BIT_1_MASK)
		WH1602_DB1_PORT|=_BV(WH1602_DB1_LEG);
	else
		WH1602_DB1_PORT&=~_BV(WH1602_DB1_LEG);
	if(data&BIT_0_MASK)
		WH1602_DB0_PORT|=_BV(WH1602_DB0_LEG);
	else
		WH1602_DB0_PORT&=~_BV(WH1602_DB0_LEG);
	
	State_E_DataReadWrite();
}
//чтение 8-битных значений данных из CGRAM или DDRAM
char WH1602_ReadData()	{
	WH1602_RS_PORT|=_BV(WH1602_RS_LEG);
	WH1602_RW_PORT|=_BV(WH1602_RW_LEG);
	
	DDR_Init_DataRead();
	
	WH1602_E_PORT|=_BV(WH1602_E_LEG);
	_delay_us(STATE_E_DELAY);
	
	PIN_Use_DataRead();
	
	WH1602_E_PORT&=~_BV(WH1602_E_LEG);
	
	DDR_Init_DataWrite();
	
	return(data);
}
//*********************************************************************


/*
//функция инициализации дисплея в 8-битном режиме
void LCD_WH1602_Init(void)	{
	//настройка портов МК подключенных к LCD
	LCD_INSTRUCTION_DDR|=INSTRUCTION_BUS;
	LCD_INSTRUCTION_PORT=0x00;
	LCD_DATA_DDR|=DATA_BUS;
	LCD_DATA_PORT=0x00;
	//задержка для стабилизации питания
	_delay_ms(100);
	//функция установки
	LCD_W1602_FunctionSet(DL,N,F,FT1,FT0);
	//проверка Busy Flag
	LCD_W1602_BusyFlag();
	//включение дисплея
	LCD_W1602_DisplayONOff();
	//проверка Busy Flag
	LCD_W1602_BusyFlag();
	//очистка дисплея
	LCD_W1602_DisplayClear();
	//проверка Busy Flag
	LCD_W1602_BusyFlag();
	//выбор режима ввода
	LCD_W1602_EntryModeSet();
	//проверка Busy Flag
	LCD_W1602_BusyFlag();
}
*/


void main (void)	{
	DDRA=0x00;
	PORTA=0x00;
	DDRB=0x00;
	PORTB=0x00;
	DDRC=0x00;
	PORTC=0x00;
	DDRD=0x00;
	PORTD=0x00;
	DDRE=0x00;
	PORTE=0x00;
	DDRF=0x00;
	PORTF=0x00;

	while (1)	{
/*
DDRE=0xff;
PORTE=0b00001111;
_delay_ms(1000);
PORTE&=~(1<<3);
_delay_ms(1000);
PORTE&=~(_BV(2));
_delay_ms(1000);
PORTE|=_BV(PE2);
_delay_ms(1000);
PORTE&=~((1<<0)|(_BV(1))|(_BV(PE2)));
_delay_ms(1000);
*/
	}
}

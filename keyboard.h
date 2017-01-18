#ifndef __JPS_KEYBOARD_H
#define __JPS_KEYBOARD_H

#define PS2_DATA_PORT	0x60
#define PS2_COMM_PORT	0x64

#define PS2_CMD				0xD1
#define CMD_SCANCODE_SET_GET	0xF0
#define PRM_SCANCODE_GET		0x00
#define PRM_SCANCODE_SET_SC1	0x01
#define PRM_SCANCODE_SET_SC2	0x02
#define PRM_SCANCODE_SET_SC3	0x03
#define CMD_LED_CTRL			0xED
#define LED_SCR_LCK			0x01
#define LED_NUM_LCK			0x02
#define LED_CAPS_LCK			0x04

#define PS2_ACK			0xFA
#define PS2_RSD			0xFE

#define STATUS_BIT_SHIFT 0x1
#define STATUS_BIT_CTRL 0x2

int isShift();
int isCtrl();

void parseKey(short int key);
void parseKeyOff(short int key);

char scancodeToAscii(unsigned char sc);

void init_keyboard();
#endif
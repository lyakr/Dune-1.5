/*
*  display.c:
* ȭ�鿡 ���� ������ ���
* ��, Ŀ��, �ý��� �޽���, ����â, �ڿ� ���� ���
* io.c�� �ִ� �Լ����� �����
*/

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "common.h"
#define MAX_SANDWORMS 2

#ifndef INPUT_STATE_H
#define INPUT_STATE_H

typedef struct {
	KEY last_key;
	int last_key_time;
} INPUT_STATE;

extern INPUT_STATE input_state;

#endif

// ǥ���� ���� ����. ���� ���� �״µ�, ���ⲯ �߰��ϰų� �����ϱ�
#define COLOR_DEFAULT	15
#define COLOR_CURSOR	112
#define COLOR_RESOURCE  112
#define COLOR_SYSTEM_MESSAGE 14
#define BLUE 1
#define RED 4
#define YELLOW 6
#define ORANGE 14 
#define GRAY 8
#define BLACK 0
#define WHITE 7
// ������ �ڿ�, ��, Ŀ���� ǥ��
// ������ ȭ�鿡 ǥ���� ����� ���⿡ �߰��ϱ�
#ifndef KEY_DEFINITIONS_H
#define KEY_DEFINITIONS_H
#define MAX_OBJECTS 100  // �ִ� ������Ʈ ��
// Ű �Է� �� ����
typedef int KEY; // Ű �ڷ��� ����

#define k_none   0
#define k_up     1
#define k_down   2
#define k_left   3
#define k_right  4
#define k_quit   5
#define k_undef  -1


#endif // KEY_DEFINITIONS_H

#define DOUBLE_CLICK_THRESHOLD 300 // ����Ŭ�� ���� �ð�(ms)

void deselect_object(void);


void display(
	RESOURCE resource,
	char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH],
	CURSOR cursor
);
#endif



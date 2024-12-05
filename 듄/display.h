/*
*  display.c:
* ȭ�鿡 ���� ������ ���
* ��, Ŀ��, �ý��� �޽���, ����â, �ڿ� ���� ���
* io.c�� �ִ� �Լ����� �����
*/

#ifndef _DISPLAY_H_
#define _DISPLAY_H_
#include "common.h"

// ǥ���� ���� ����. ���� ���� �״µ�, ���ⲯ �߰��ϰų� �����ϱ�
#define COLOR_DEFAULT	15
#define COLOR_CURSOR	112
#define COLOR_RESOURCE  112

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

// Ű �Է� �� ����
#define KEY_UP    72   // ȭ��ǥ ����
#define KEY_DOWN  80   // ȭ��ǥ �Ʒ���
#define KEY_LEFT  75   // ȭ��ǥ ����
#define KEY_RIGHT 77   // ȭ��ǥ ������
#define KEY_SPACE 32   // �����̽���
#define KEY_ESC   27   // ESC Ű

#endif // KEY_DEFINITIONS_H

void deselect_object(void);

#define TICK 10
#define DOUBLE_CLICK_THRESHOLD 300 // ms
#define SINGLE_CLICK_MOVE_DISTANCE 1
#define DOUBLE_CLICK_MOVE_DISTANCE 4

void display(
	RESOURCE resource,
	char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH],
	CURSOR cursor
);

#endif

/*
*  display.c:
* 화면에 게임 정보를 출력
* 맵, 커서, 시스템 메시지, 정보창, 자원 상태 등등
* io.c에 있는 함수들을 사용함
*/

#ifndef _DISPLAY_H_
#define _DISPLAY_H_
#include "common.h"

// 표시할 색상 정의. 대충 맞춰 뒀는데, 취향껏 추가하거나 변경하기
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
// 지금은 자원, 맵, 커서만 표시
// 앞으로 화면에 표시할 내용들 여기에 추가하기
#ifndef KEY_DEFINITIONS_H
#define KEY_DEFINITIONS_H

// 키 입력 값 정의
#define KEY_UP    72   // 화살표 위쪽
#define KEY_DOWN  80   // 화살표 아래쪽
#define KEY_LEFT  75   // 화살표 왼쪽
#define KEY_RIGHT 77   // 화살표 오른쪽
#define KEY_SPACE 32   // 스페이스바
#define KEY_ESC   27   // ESC 키

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

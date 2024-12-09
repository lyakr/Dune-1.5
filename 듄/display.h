/*
*  display.c:
* 화면에 게임 정보를 출력
* 맵, 커서, 시스템 메시지, 정보창, 자원 상태 등등
* io.c에 있는 함수들을 사용함
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

// 표시할 색상 정의. 대충 맞춰 뒀는데, 취향껏 추가하거나 변경하기
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
// 지금은 자원, 맵, 커서만 표시
// 앞으로 화면에 표시할 내용들 여기에 추가하기
#ifndef KEY_DEFINITIONS_H
#define KEY_DEFINITIONS_H
#define MAX_OBJECTS 100  // 최대 오브젝트 수
// 키 입력 값 정의
typedef int KEY; // 키 자료형 정의

#define k_none   0
#define k_up     1
#define k_down   2
#define k_left   3
#define k_right  4
#define k_quit   5
#define k_undef  -1


#endif // KEY_DEFINITIONS_H

#define DOUBLE_CLICK_THRESHOLD 300 // 더블클릭 감지 시간(ms)

void deselect_object(void);


void display(
	RESOURCE resource,
	char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH],
	CURSOR cursor
);
#endif



/*
*  display.c:
* 화면에 게임 정보를 출력
* 맵, 커서, 시스템 메시지, 정보창, 자원 상태 등등
* io.c에 있는 함수들을 사용함
*/

#include "display.h"
#include "io.h"

// 출력할 내용들의 좌상단(topleft) 좌표
const POSITION resource_pos = { 0, 0 };
const POSITION map_pos = { 1, 0 };

const POSITION status_window_pos = { 0, MAP_WIDTH + 2 };
const POSITION command_window_pos = { MAP_HEIGHT / 2 + 1, MAP_WIDTH + 2 };
const POSITION system_message_pos = { MAP_HEIGHT + 1, 0 };

char backbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char frontbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };

void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]);
void display_resource(RESOURCE resource);
void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void display_cursor(CURSOR cursor);
void draw_dividers(void);
void display_status_window(void);
void display_command_window(void);
void display_system_message(void);

void display(
	RESOURCE resource,
	char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH],
	CURSOR cursor)
{
	display_resource(resource);
	display_map(map);
	display_cursor(cursor);
	display_resource(resource);
	display_map(map);
	display_cursor(cursor);
	draw_dividers();
	display_status_window();
	display_command_window();
	display_system_message();
}

void display_resource(RESOURCE resource) {
	set_color(COLOR_RESOURCE);
	gotoxy(resource_pos);
	printf("spice = %d/%d, population=%d/%d\n",
		resource.spice, resource.spice_max,
		resource.population, resource.population_max
	);
}

// subfunction of draw_map()
void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]) {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			for (int k = 0; k < N_LAYER; k++) {
				if (src[k][i][j] >= 0) {
					dest[i][j] = src[k][i][j];
				}
			}
		}
	}
}

void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	project(map, backbuf);

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (frontbuf[i][j] != backbuf[i][j]) {
				POSITION pos = { i, j };
				printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT);
			}
			frontbuf[i][j] = backbuf[i][j];
		}
	}
}

// frontbuf[][]에서 커서 위치의 문자를 색만 바꿔서 그대로 다시 출력
void display_cursor(CURSOR cursor) {
	POSITION prev = cursor.previous;
	POSITION curr = cursor.current;

	char ch = frontbuf[prev.row][prev.column];
	printc(padd(map_pos, prev), ch, COLOR_DEFAULT);

	ch = frontbuf[curr.row][curr.column];
	printc(padd(map_pos, curr), ch, COLOR_CURSOR);
}
// 구분선 그리기
void draw_dividers(void) {
	set_color(COLOR_DEFAULT);

	// 맵 오른쪽에 수직 구분선
	for (int i = 0; i <= MAP_HEIGHT; i++) {
		gotoxy((POSITION) { i, MAP_WIDTH });
		printf("|");
	}
	// 시스템 메시지 위쪽에 수평 구분선
	for (int j = 0; j <= MAP_WIDTH + 20; j++) {
		gotoxy((POSITION) { MAP_HEIGHT, j });
		printf("-");
	}
}

// 상태창 출력
void display_status_window(void) {
	set_color(COLOR_DEFAULT);
	gotoxy(status_window_pos);
	printf("=== Status Window ===");
}

// 명령창 출력
void display_command_window(void) {
	set_color(COLOR_DEFAULT);
	gotoxy(command_window_pos);
	printf("=== Command Window ===");
}

// 시스템 메시지 출력
void display_system_message(void) {
	set_color(COLOR_DEFAULT);
	gotoxy(system_message_pos);
	printf("=== System Messages ===");
}


// 오브젝트의 색상을 반환하는 함수
int get_object_color(char obj) {
	switch (obj) {
	case 'B': return 9;   // 본부 (파란색)
	case 'H': return 14;  // 하베스터 (노란색)
	case 'W': return 6;   // 샌드웜 (황토색)
	case 'S': return 12;  // 스파이스 (빨간색)
	case 'P': return 8;   // 장판 (회색)
	case 'R': return 7;   // 바위 (밝은 회색)
	default: return COLOR_DEFAULT; // 기본 색상
	}
}

// 초기 상태를 설정하는 함수
void init_game_state(RESOURCE* resource, char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	// 2. 맵 초기화
	for (int layer = 0; layer < N_LAYER; layer++) {
		for (int i = 0; i < MAP_HEIGHT; i++) {
			for (int j = 0; j < MAP_WIDTH; j++) {
				map[layer][i][j] = -1; // 빈 칸으로 초기화
			}
		}
	}

	// Layer 0: 지형 및 건물 배치
	map[0][MAP_HEIGHT - 4][1] = 'B'; map[0][MAP_HEIGHT - 4][2] = 'B'; // 아트레이디스 본부
	map[0][MAP_HEIGHT - 3][1] = 'B'; map[0][MAP_HEIGHT - 3][2] = 'B';
	map[0][1][MAP_WIDTH - 3] = 'B'; map[0][1][MAP_WIDTH - 4] = 'B'; // 하코넨 본부
	map[0][2][MAP_WIDTH - 3] = 'B'; map[0][2][MAP_WIDTH - 4] = 'B';

	map[0][MAP_HEIGHT - 6][1] = 'P'; map[0][MAP_HEIGHT - 6][2] = 'P'; // 아트레이디스 장판
	map[0][MAP_HEIGHT - 5][1] = 'P'; map[0][MAP_HEIGHT - 5][2] = 'P';
	map[0][3][MAP_WIDTH - 5] = 'P'; map[0][3][MAP_WIDTH - 6] = 'P'; // 하코넨 장판
	map[0][4][MAP_WIDTH - 5] = 'P'; map[0][4][MAP_WIDTH - 6] = 'P';

	map[0][5][5] = 'S'; // 스파이스 매장지
	map[0][6][7] = 'S';
	map[0][10][10] = 'S';

	map[0][8][20] = 'R'; map[0][9][20] = 'R'; // 바위
	map[0][8][21] = 'R'; map[0][9][21] = 'R';

	// Layer 1: 유닛 배치
	map[1][MAP_HEIGHT - 5][3] = 'H'; // 하베스터 (아트레이디스)
	map[1][3][MAP_WIDTH - 6] = 'H';  // 하베스터 (하코넨)

	map[1][7][7] = 'W';  // 샌드웜
	map[1][12][15] = 'W';
}

// 맵의 모든 오브젝트를 색상과 함께 출력하는 함수
void display_colored_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			char obj = map[0][i][j]; // Layer 0에서 지형 확인
			if (obj == -1) obj = map[1][i][j]; // Layer 1 확인
			if (obj >= 0) {
				set_color(get_object_color(obj));
				printf("%c", obj);
			}
			else {
				set_color(COLOR_DEFAULT);
				printf(" ");
			}
		}
		printf("\n");
	}
	set_color(COLOR_DEFAULT); // 기본 색상으로 복원
}

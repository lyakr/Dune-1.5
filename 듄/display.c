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

void set_color_by_char(char object) {
	switch (object) {
	case 'B': set_color(BLUE); break;   // 본진 (플레이어)
	case 'W': set_color(YELLOW); break; // 샌드웜
	case 'H': set_color(RED); break;    // 하코넨 (AI 유닛)
	case 'S': set_color(ORANGE); break; // 스파이스 매장지
	case 'P': set_color(BLACK); break;  // 장판
	case 'R': set_color(GRAY); break;   // 바위
	case ' ': set_color(GRAY); break;   // 기본 지형
	default: set_color(WHITE); break;   // 기타
	}
}

void initialize_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	// Layer 0 (Terrain Layer)
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			map[0][i][j] = ' '; // 기본 지형(빈 공간)
		}
	}

	// 본부 배치
	map[0][MAP_HEIGHT - 3][1] = 'B'; // 플레이어 본부
	map[0][MAP_HEIGHT - 3][2] = 'B';
	map[0][MAP_HEIGHT - 2][1] = 'B';
	map[0][MAP_HEIGHT - 2][2] = 'B';

	map[0][1][MAP_WIDTH - 3] = 'B'; // 적 본부
	map[0][1][MAP_WIDTH - 4] = 'B';
	map[0][2][MAP_WIDTH - 3] = 'B';
	map[0][2][MAP_WIDTH - 4] = 'B';

	// 장판 배치
	map[0][MAP_HEIGHT - 4][1] = 'P'; // 플레이어 장판
	map[0][MAP_HEIGHT - 4][2] = 'P';

	map[0][3][MAP_WIDTH - 3] = 'P'; // 적 장판
	map[0][3][MAP_WIDTH - 4] = 'P';

	// 스파이스 배치
	map[0][MAP_HEIGHT / 2][1] = 'S';
	map[0][MAP_HEIGHT / 2][MAP_WIDTH - 2] = 'S';

	// 바위 배치
	map[0][MAP_HEIGHT / 3][MAP_WIDTH / 3] = 'R';
	map[0][2 * MAP_HEIGHT / 3][2 * MAP_WIDTH / 3] = 'R';
	map[0][MAP_HEIGHT / 2][MAP_WIDTH / 2] = 'R';

	// Layer 1 (Unit Layer)
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			map[1][i][j] = -1; // 초기화
		}
	}

	// 하베스터 배치
	map[1][MAP_HEIGHT - 3][1] = 'H'; // 플레이어 하베스터
	map[1][2][MAP_WIDTH - 3] = 'H'; // 적 하베스터

	// 샌드웜 배치
	map[1][MAP_HEIGHT / 3][MAP_WIDTH / 3] = 'W';
	map[1][2 * MAP_HEIGHT / 3][2 * MAP_WIDTH / 3] = 'W';
}




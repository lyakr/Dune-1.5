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



static char selected_object = -1; // 현재 선택된 오브젝트
void select_object(CURSOR cursor, char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	char new_selected_char = map[1][cursor.current.row][cursor.current.column]; // 상위 레이어 확인
	if (new_selected_char == -1) { // 유닛 없으면 하위 레이어 확인
		new_selected_char = map[0][cursor.current.row][cursor.current.column];
	}

	// 선택된 오브젝트가 변경된 경우만 갱신
	if (selected_object != new_selected_char) {
		deselect_object(); // 이전 선택 제거
		selected_object = new_selected_char; // 새로운 선택 업데이트

		// 상태창에 새로운 오브젝트 정보 표시
		gotoxy(status_window_pos);
		set_color(COLOR_DEFAULT);
		if (selected_object == ' ') {
			printf("=== 사막 지형 ===");
		}
		else if (selected_object == 'B') {
			printf("=== 본진 (Base) ===\n");
		}
		else if (selected_object == 'P') {
			printf("=== 장판 (Plate) ===\n");
		}
		else if (selected_object == 'S') {
			printf("=== 스파이스 (Spice) ===\n");
		}
		else if (selected_object == 'R') {
			printf("=== 바위 (Rock) ===\n");
		}
		else if (selected_object == 'H') {
			printf("=== 하베스터 (Harvester) ===\n");
		}
		else if (selected_object == 'W') {
			printf("=== 샌드웜 (Sandworm) ===\n");
		}
		else {
			printf("=== 알 수 없는 오브젝트 ===\n");
		}
	}
}
void deselect_object(void) {
	if (selected_object != -1) {
		gotoxy(status_window_pos);
		set_color(COLOR_DEFAULT);
		printf("                    "); // 상태창 초기화
		gotoxy(status_window_pos);
		printf("=== 상태창 비움 ===\n");
		selected_object = -1; // 선택 해제
	}
}


void move_cursor_double_click(CURSOR* cursor, char key) {
	POSITION new_pos = cursor->current;

	switch (key) {
	case KEY_UP:
		new_pos.row = (cursor->current.row - 4 >= 0) ? cursor->current.row - 4 : 0;
		break;
	case KEY_DOWN:
		new_pos.row = (cursor->current.row + 4 < MAP_HEIGHT) ? cursor->current.row + 4 : MAP_HEIGHT - 1;
		break;
	case KEY_LEFT:
		new_pos.column = (cursor->current.column - 4 >= 0) ? cursor->current.column - 4 : 0;
		break;
	case KEY_RIGHT:
		new_pos.column = (cursor->current.column + 4 < MAP_WIDTH) ? cursor->current.column + 4 : MAP_WIDTH - 1;
		break;
	default:
		return; // 다른 키는 무시
	}

	cursor->previous = cursor->current;
	cursor->current = new_pos;
	display_cursor(*cursor);
}

void handle_space_key(CURSOR* cursor, char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	select_object(*cursor, map); // 현재 커서 위치의 오브젝트 선택
}

void handle_esc_key(void) {
	deselect_object(); // 선택된 오브젝트 해제 및 상태창 비우기
}



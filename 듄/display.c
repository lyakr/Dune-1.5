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

KEY last_key = k_none;      // 마지막으로 입력된 키
DWORD last_key_time = 0;    // 마지막 입력 시간

int is_double_click(KEY key) {
	DWORD current_time = GetTickCount();

	// 동일 키이고, 시간 차이가 임계값 이내라면 더블클릭으로 간주
	if (key == last_key && (current_time - last_key_time) <= DOUBLE_CLICK_THRESHOLD) {
		return 1; // 더블클릭 감지
	}

	// 상태 업데이트
	last_key = key;
	last_key_time = current_time;

	return 0; // 단일 클릭
}

void cursor_move_with_key(KEY key) {
	int distance = is_double_click(key) ? DOUBLE_CLICK_MOVE_DISTANCE : SINGLE_CLICK_MOVE_DISTANCE;
	cursor_move(ktod(key), distance);
}

#define SPICE_GENERATION_CHANCE 10  // 10% 확률
#define SPICE_MAX 9                // 최대 매장량
#define MOVE_PERIOD 2500           // 샌드웜 이동 주기 (ms)

typedef struct {
	POSITION pos;
	int next_move_time;
} SANDWORM;

void move_sandworm(SANDWORM* worm, char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], int sys_clock);
void eat_unit(SANDWORM* worm, char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void generate_spice(SANDWORM* worm, char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
POSITION find_closest_unit(SANDWORM* worm, char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);

void move_sandworm(SANDWORM* worm, char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], int sys_clock) {
	if (sys_clock < worm->next_move_time) return; // 이동 시간이 아니라면 반환

	worm->next_move_time = sys_clock + MOVE_PERIOD; // 다음 이동 시간 갱신

	POSITION target = find_closest_unit(worm, map);
	if (target.row == -1 && target.column == -1) return; // 이동할 대상 없음

	// 방향 계산
	int dr = target.row - worm->pos.row;
	int dc = target.column - worm->pos.column;
	POSITION next = worm->pos;

	if (abs(dr) > abs(dc)) {
		next.row += (dr > 0) ? 1 : -1;
	}
	else {
		next.column += (dc > 0) ? 1 : -1;
	}

	// 맵에서 이전 위치 제거
	map[1][worm->pos.row][worm->pos.column] = -1;

	// 새 위치로 이동
	worm->pos = next;
	map[1][next.row][next.column] = 'W';

	// 유닛 잡아먹기
	eat_unit(worm, map);

	// 확률적으로 스파이스 생성
	generate_spice(worm, map);
}

void eat_unit(SANDWORM* worm, char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	char target = map[1][worm->pos.row][worm->pos.column];
	if (target != -1 && target != 'W') { // 유닛 존재 && 샌드웜이 아님
		map[1][worm->pos.row][worm->pos.column] = -1; // 유닛 제거
		// 시스템 메시지 출력
		set_color(COLOR_SYSTEM_MESSAGE);
		printf("A unit was devoured by the sandworm at (%d, %d)!\n",
			worm->pos.row, worm->pos.column);
	}
}

void generate_spice(SANDWORM* worm, char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	if (rand() % 100 < SPICE_GENERATION_CHANCE) { // 일정 확률로 생성
		int spice_amount = 1 + rand() % SPICE_MAX; // 1~SPICE_MAX 사이 값
		map[0][worm->pos.row][worm->pos.column] = 'S';
		// 스파이스 매장량을 표현 (1~9)
		printf("A new spice field (%d) appeared at (%d, %d)!\n",
			spice_amount, worm->pos.row, worm->pos.column);
	}
}

POSITION find_closest_unit(SANDWORM* worm, char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	POSITION closest = { -1, -1 };
	int min_distance = MAP_HEIGHT * MAP_WIDTH;

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			char target = map[1][i][j];
			if (target != -1 && target != 'W') { // 유닛 존재 && 샌드웜 제외
				int distance = abs(worm->pos.row - i) + abs(worm->pos.column - j);
				if (distance < min_distance) {
					min_distance = distance;
					closest.row = i;
					closest.column = j;
				}
			}
		}
	}

	return closest;
}
//
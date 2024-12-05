/*
*  display.c:
* ȭ�鿡 ���� ������ ���
* ��, Ŀ��, �ý��� �޽���, ����â, �ڿ� ���� ���
* io.c�� �ִ� �Լ����� �����
*/

#include "display.h"
#include "io.h"

// ����� ������� �»��(topleft) ��ǥ
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

// frontbuf[][]���� Ŀ�� ��ġ�� ���ڸ� ���� �ٲ㼭 �״�� �ٽ� ���
void display_cursor(CURSOR cursor) {
	POSITION prev = cursor.previous;
	POSITION curr = cursor.current;

	char ch = frontbuf[prev.row][prev.column];
	printc(padd(map_pos, prev), ch, COLOR_DEFAULT);

	ch = frontbuf[curr.row][curr.column];
	printc(padd(map_pos, curr), ch, COLOR_CURSOR);
}
// ���м� �׸���
void draw_dividers(void) {
	set_color(COLOR_DEFAULT);

	// �� �����ʿ� ���� ���м�
	for (int i = 0; i <= MAP_HEIGHT; i++) {
		gotoxy((POSITION) { i, MAP_WIDTH });
		printf("|");
	}
	// �ý��� �޽��� ���ʿ� ���� ���м�
	for (int j = 0; j <= MAP_WIDTH + 20; j++) {
		gotoxy((POSITION) { MAP_HEIGHT, j });
		printf("-");
	}
}

// ����â ���
void display_status_window(void) {
	set_color(COLOR_DEFAULT);
	gotoxy(status_window_pos);
	printf("=== Status Window ===");
}

// ���â ���
void display_command_window(void) {
	set_color(COLOR_DEFAULT);
	gotoxy(command_window_pos);
	printf("=== Command Window ===");
}

// �ý��� �޽��� ���
void display_system_message(void) {
	set_color(COLOR_DEFAULT);
	gotoxy(system_message_pos);
	printf("=== System Messages ===");
}

void set_color_by_char(char object) {
	switch (object) {
	case 'B': set_color(BLUE); break;   // ���� (�÷��̾�)
	case 'W': set_color(YELLOW); break; // �����
	case 'H': set_color(RED); break;    // ���ڳ� (AI ����)
	case 'S': set_color(ORANGE); break; // �����̽� ������
	case 'P': set_color(BLACK); break;  // ����
	case 'R': set_color(GRAY); break;   // ����
	case ' ': set_color(GRAY); break;   // �⺻ ����
	default: set_color(WHITE); break;   // ��Ÿ
	}
}

void initialize_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	// Layer 0 (Terrain Layer)
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			map[0][i][j] = ' '; // �⺻ ����(�� ����)
		}
	}

	// ���� ��ġ
	map[0][MAP_HEIGHT - 3][1] = 'B'; // �÷��̾� ����
	map[0][MAP_HEIGHT - 3][2] = 'B';
	map[0][MAP_HEIGHT - 2][1] = 'B';
	map[0][MAP_HEIGHT - 2][2] = 'B';

	map[0][1][MAP_WIDTH - 3] = 'B'; // �� ����
	map[0][1][MAP_WIDTH - 4] = 'B';
	map[0][2][MAP_WIDTH - 3] = 'B';
	map[0][2][MAP_WIDTH - 4] = 'B';

	// ���� ��ġ
	map[0][MAP_HEIGHT - 4][1] = 'P'; // �÷��̾� ����
	map[0][MAP_HEIGHT - 4][2] = 'P';

	map[0][3][MAP_WIDTH - 3] = 'P'; // �� ����
	map[0][3][MAP_WIDTH - 4] = 'P';

	// �����̽� ��ġ
	map[0][MAP_HEIGHT / 2][1] = 'S';
	map[0][MAP_HEIGHT / 2][MAP_WIDTH - 2] = 'S';

	// ���� ��ġ
	map[0][MAP_HEIGHT / 3][MAP_WIDTH / 3] = 'R';
	map[0][2 * MAP_HEIGHT / 3][2 * MAP_WIDTH / 3] = 'R';
	map[0][MAP_HEIGHT / 2][MAP_WIDTH / 2] = 'R';

	// Layer 1 (Unit Layer)
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			map[1][i][j] = -1; // �ʱ�ȭ
		}
	}

	// �Ϻ����� ��ġ
	map[1][MAP_HEIGHT - 3][1] = 'H'; // �÷��̾� �Ϻ�����
	map[1][2][MAP_WIDTH - 3] = 'H'; // �� �Ϻ�����

	// ����� ��ġ
	map[1][MAP_HEIGHT / 3][MAP_WIDTH / 3] = 'W';
	map[1][2 * MAP_HEIGHT / 3][2 * MAP_WIDTH / 3] = 'W';
}

KEY last_key = k_none;      // ���������� �Էµ� Ű
DWORD last_key_time = 0;    // ������ �Է� �ð�

int is_double_click(KEY key) {
	DWORD current_time = GetTickCount();

	// ���� Ű�̰�, �ð� ���̰� �Ӱ谪 �̳���� ����Ŭ������ ����
	if (key == last_key && (current_time - last_key_time) <= DOUBLE_CLICK_THRESHOLD) {
		return 1; // ����Ŭ�� ����
	}

	// ���� ������Ʈ
	last_key = key;
	last_key_time = current_time;

	return 0; // ���� Ŭ��
}

void cursor_move_with_key(KEY key) {
	int distance = is_double_click(key) ? DOUBLE_CLICK_MOVE_DISTANCE : SINGLE_CLICK_MOVE_DISTANCE;
	cursor_move(ktod(key), distance);
}

#define SPICE_GENERATION_CHANCE 10  // 10% Ȯ��
#define SPICE_MAX 9                // �ִ� ���差
#define MOVE_PERIOD 2500           // ����� �̵� �ֱ� (ms)

typedef struct {
	POSITION pos;
	int next_move_time;
} SANDWORM;

void move_sandworm(SANDWORM* worm, char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], int sys_clock);
void eat_unit(SANDWORM* worm, char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void generate_spice(SANDWORM* worm, char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
POSITION find_closest_unit(SANDWORM* worm, char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);

void move_sandworm(SANDWORM* worm, char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], int sys_clock) {
	if (sys_clock < worm->next_move_time) return; // �̵� �ð��� �ƴ϶�� ��ȯ

	worm->next_move_time = sys_clock + MOVE_PERIOD; // ���� �̵� �ð� ����

	POSITION target = find_closest_unit(worm, map);
	if (target.row == -1 && target.column == -1) return; // �̵��� ��� ����

	// ���� ���
	int dr = target.row - worm->pos.row;
	int dc = target.column - worm->pos.column;
	POSITION next = worm->pos;

	if (abs(dr) > abs(dc)) {
		next.row += (dr > 0) ? 1 : -1;
	}
	else {
		next.column += (dc > 0) ? 1 : -1;
	}

	// �ʿ��� ���� ��ġ ����
	map[1][worm->pos.row][worm->pos.column] = -1;

	// �� ��ġ�� �̵�
	worm->pos = next;
	map[1][next.row][next.column] = 'W';

	// ���� ��ƸԱ�
	eat_unit(worm, map);

	// Ȯ�������� �����̽� ����
	generate_spice(worm, map);
}

void eat_unit(SANDWORM* worm, char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	char target = map[1][worm->pos.row][worm->pos.column];
	if (target != -1 && target != 'W') { // ���� ���� && ������� �ƴ�
		map[1][worm->pos.row][worm->pos.column] = -1; // ���� ����
		// �ý��� �޽��� ���
		set_color(COLOR_SYSTEM_MESSAGE);
		printf("A unit was devoured by the sandworm at (%d, %d)!\n",
			worm->pos.row, worm->pos.column);
	}
}

void generate_spice(SANDWORM* worm, char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	if (rand() % 100 < SPICE_GENERATION_CHANCE) { // ���� Ȯ���� ����
		int spice_amount = 1 + rand() % SPICE_MAX; // 1~SPICE_MAX ���� ��
		map[0][worm->pos.row][worm->pos.column] = 'S';
		// �����̽� ���差�� ǥ�� (1~9)
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
			if (target != -1 && target != 'W') { // ���� ���� && ����� ����
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
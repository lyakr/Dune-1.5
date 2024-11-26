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


// ������Ʈ�� ������ ��ȯ�ϴ� �Լ�
int get_object_color(char obj) {
	switch (obj) {
	case 'B': return 9;   // ���� (�Ķ���)
	case 'H': return 14;  // �Ϻ����� (�����)
	case 'W': return 6;   // ����� (Ȳ���)
	case 'S': return 12;  // �����̽� (������)
	case 'P': return 8;   // ���� (ȸ��)
	case 'R': return 7;   // ���� (���� ȸ��)
	default: return COLOR_DEFAULT; // �⺻ ����
	}
}

// �ʱ� ���¸� �����ϴ� �Լ�
void init_game_state(RESOURCE* resource, char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	// 2. �� �ʱ�ȭ
	for (int layer = 0; layer < N_LAYER; layer++) {
		for (int i = 0; i < MAP_HEIGHT; i++) {
			for (int j = 0; j < MAP_WIDTH; j++) {
				map[layer][i][j] = -1; // �� ĭ���� �ʱ�ȭ
			}
		}
	}

	// Layer 0: ���� �� �ǹ� ��ġ
	map[0][MAP_HEIGHT - 4][1] = 'B'; map[0][MAP_HEIGHT - 4][2] = 'B'; // ��Ʈ���̵� ����
	map[0][MAP_HEIGHT - 3][1] = 'B'; map[0][MAP_HEIGHT - 3][2] = 'B';
	map[0][1][MAP_WIDTH - 3] = 'B'; map[0][1][MAP_WIDTH - 4] = 'B'; // ���ڳ� ����
	map[0][2][MAP_WIDTH - 3] = 'B'; map[0][2][MAP_WIDTH - 4] = 'B';

	map[0][MAP_HEIGHT - 6][1] = 'P'; map[0][MAP_HEIGHT - 6][2] = 'P'; // ��Ʈ���̵� ����
	map[0][MAP_HEIGHT - 5][1] = 'P'; map[0][MAP_HEIGHT - 5][2] = 'P';
	map[0][3][MAP_WIDTH - 5] = 'P'; map[0][3][MAP_WIDTH - 6] = 'P'; // ���ڳ� ����
	map[0][4][MAP_WIDTH - 5] = 'P'; map[0][4][MAP_WIDTH - 6] = 'P';

	map[0][5][5] = 'S'; // �����̽� ������
	map[0][6][7] = 'S';
	map[0][10][10] = 'S';

	map[0][8][20] = 'R'; map[0][9][20] = 'R'; // ����
	map[0][8][21] = 'R'; map[0][9][21] = 'R';

	// Layer 1: ���� ��ġ
	map[1][MAP_HEIGHT - 5][3] = 'H'; // �Ϻ����� (��Ʈ���̵�)
	map[1][3][MAP_WIDTH - 6] = 'H';  // �Ϻ����� (���ڳ�)

	map[1][7][7] = 'W';  // �����
	map[1][12][15] = 'W';
}

// ���� ��� ������Ʈ�� ����� �Բ� ����ϴ� �Լ�
void display_colored_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			char obj = map[0][i][j]; // Layer 0���� ���� Ȯ��
			if (obj == -1) obj = map[1][i][j]; // Layer 1 Ȯ��
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
	set_color(COLOR_DEFAULT); // �⺻ �������� ����
}

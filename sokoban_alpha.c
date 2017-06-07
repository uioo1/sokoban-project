#include <stdio.h>
#include <stdlib.h>
#include <termio.h>

int i = -1, j, k, stage, dl_x, dl_y;
char get, game_act;
int map_current[5][30][30];
int pl_x, pl_y;
int map_file[5][30][30] = {};
char name[10];
char ch = 0;

void getname();
void getmap();
void showmap();
void showcommand();
void showname();
void pl_move();

int getch(void) {
	int ch;

	struct termios buf;
	struct termios save;

	tcgetattr(0, &save);
	buf = save;

	buf.c_lflag &= ~(ICANON | ECHO);
	buf.c_cc[VMIN] = 1;
	buf.c_cc[VTIME] = 0;

	tcsetattr(0, TCSAFLUSH, &buf);

	ch = getchar();
	tcsetattr(0, TCSAFLUSH, &save);


	return ch;
}

int main(void) {
	getname();
	getmap();

	while (1) {
		system("clear");
		showname();
		showmap();
		showcommand();
		get = getch();
		pl_move();
	}
	return 0;
}

void getname(){	//이름을 입력받는 함수
	system("clear");
	printf("Start....\n");
	printf("input name : ");
	scanf("%s", name);
}

void getmap(){	//map.txt에서 맵 불러와서 저장
	FILE *file = fopen("map.txt", "r");

	while (ch != 'e') {
		for (k = 0; k < 30; k++) {
			fscanf(file, "%c", &ch);
			if (ch == 'm') {
				i++;
				j = 0;
				k = -1;
				continue;
			}
			if (ch == 'a' || ch == 'p') {
				k--;
				continue;
			}
			if (ch == 'e') {
				break;
			}
			if (ch == '\n') {
				j++;
				k = -1;
				continue;
			}
			else {
				switch (ch) {
				case ' ':	//공백일 경우 0으로 저장
					map_file[i][j][k] = 0;
					break;
				case '#':	//벽일 경우 1로 저장
					map_file[i][j][k] = 1;
					break;
				case '$':	//박스일 경우 2로 저장
					map_file[i][j][k] = 2;
					break;
				case 'O':	//박스 놓는곳일 경우 4로 저장
					map_file[i][j][k] = 4;
					break;
				case '@':	//플레이어일 경우 5로 저장
					map_file[i][j][k] = 5;
					break;
				default:
					break;
				}
			}
		}
	}

	for (i = 0; i<5; i++) {	//  원본 파일 그대로 복사
		for (j = 0; j<30; j++) {
			for (k = 0; k<30; k++) {
				map_current[i][j][k] = map_file[i][j][k];
			}
		}
	}

	for (i = 0; i<5; i++) {
		for (j = 1; j<30; j++) {
			end_cnt = 0;
			for (k = 0; k<30; k++) {
				if (map_file[i][j][k] == 0) {
					end_cnt++;
				}
			}
			if (end_cnt == 30) {
				map_end[i][0] = j;
				break;
			}
		}
	}
	fclose(file);
}

void showname(){
	printf("    Hello ");
	for (i = 0; i < 20; i++) {
		printf("%c", name[i]);
	}
	printf("\n");
}

void showmap(){
	for (j = 0; j < map_end[stage][0]; j++) {
		for (k = 0; k < 30; k++) {
			switch (map_current[stage][j][k]) {
				case 0:	//0일 경우 공백으로 표시
					printf(" ");
					break;
				case 1:	//1일 경우 벽으로 표시
					printf("#");
					break;
				case 2:	//2일 경우 박스로 표시
					printf("$");
					break;
				case 4:	//4일 경우 박스놓는 곳으로 표시
					printf("O");
					break;
				case 5:	//플레이어일 경우 5로 표시
					printf("@");
					pl_x = k;
					pl_y = j;
					break;
				default:
					break;
			}
		}
		printf("\n");
	}
}

void showcommand(){
	printf("(Command) %c", game_act);
}

void pl_move() {
	switch (get) {
	case 'l':
		dl_x = 1, dl_y = 0;
		if (map_file[stage][pl_y][pl_x] == 4) {	//있던 자리가 박스 놓는 위치일 경우
			if (map_current[stage][pl_y + dl_y][pl_x + dl_x] == 2) {	//오른쪽이 박스일 경우
				if (map_current[stage][pl_y + 2 * dl_y][pl_x + 2 * dl_x] != 1 && map_current[stage][pl_y + 2 * dl_y][pl_x + 2 * dl_x] != 2) {//건너편이 박스랑 벽이 아닐 경우
					map_current[stage][pl_y + dl_y][pl_x + dl_x] = 5;	//플레이어 오른쪽 이동
					map_current[stage][pl_y + 2 * dl_y][pl_x + 2 * dl_x] = 2; //건너편은 박스
					map_current[stage][pl_y][pl_x] = 4;	//있던 자리는 박스 놓는 위치로 바꿈
				}
				else {	//건너편이 박스나 벽일 경우
					break;
				}
			}
			else if (map_current[stage][pl_y + dl_y][pl_x + dl_x] == 1) {	//오른쪽이 벽일 경우
				break;
			}
			else if (map_current[stage][pl_y + dl_y][pl_x + dl_x] == 0 || map_current[stage][pl_y + dl_y][pl_x + dl_x] == 4) {	//오른쪽이 공백이거나 박스 놓는곳일 경우에
				map_current[stage][pl_y + dl_y][pl_x + dl_x] = 5;	//플레이어 오른쪽 이동
				map_current[stage][pl_y][pl_x] = 4;	//있던 자리는 박스 놓는 위치로 바꿈
			}
		}
		else {	//있던 자리가 공백일 경우
			if (map_current[stage][pl_y + dl_y][pl_x + dl_x] == 2) {	//오른쪽이 박스일 경우
				if (map_current[stage][pl_y + 2 * dl_y][pl_x + 2 * dl_x] != 1 && map_current[stage][pl_y + 2 * dl_y][pl_x + 2 * dl_x] != 2) {//건너편이 박스랑 벽이 아닐 경우
					map_current[stage][pl_y + dl_y][pl_x + dl_x] = 5;	//플레이어 오른쪽 이동
					map_current[stage][pl_y + 2 * dl_y][pl_x + 2 * dl_x] = 2; //건너편 박스
					map_current[stage][pl_y][pl_x] = 0;	//있던 자리는 공백
				}
				else {	//건너편이 박스나 벽일 경우
					break;
				}
			}
			else if (map_current[stage][pl_y + dl_y][pl_x + dl_x] == 1) {	//오른쪽이 벽일 경우
				break;
			}
			else if (map_current[stage][pl_y + dl_y][pl_x + dl_x] == 0 || map_current[stage][pl_y + dl_y][pl_x + dl_x] == 4) {	//오른쪽이 공백이거나 박스 놓는곳일 경우에
				map_current[stage][pl_y + dl_y][pl_x + dl_x] = 5;	//플레이어 오른쪽 이동
				map_current[stage][pl_y][pl_x] = 0;	//있던 자리는 박스 놓는 공백
			}
		}
		break;
	case 'h':
		dl_x = -1, dl_y = 0;
		if (map_file[stage][pl_y][pl_x] == 4) {	//있던 자리가 박스 놓는 위치일 경우
			if (map_current[stage][pl_y + dl_y][pl_x + dl_x] == 2) {	//왼쪽이 박스일 경우
				if (map_current[stage][pl_y + 2 * dl_y][pl_x + 2 * dl_x] != 1 && map_current[stage][pl_y + 2 * dl_y][pl_x + 2 * dl_x] != 2) {//건너편이 박스랑 벽이 아닐 경우
					map_current[stage][pl_y + dl_y][pl_x + dl_x] = 5;	//플레이어 왼쪽 이동
					map_current[stage][pl_y + 2 * dl_y][pl_x + 2 * dl_x] = 2; //건너편 박스
					map_current[stage][pl_y][pl_x] = 4;	//있던 자리는 박스 놓는 위치로 바꿈
				}
				else {	//건너편이 박스나 벽일 경우
					break;
				}
			}
			else if (map_current[stage][pl_y + dl_y][pl_x + dl_x] == 1) {	//왼쪽이 벽일 경우
				break;
			}
			else if (map_current[stage][pl_y + dl_y][pl_x + dl_x] == 0 || map_current[stage][pl_y + dl_y][pl_x + dl_x] == 4) {	//왼쪽이 공백이거나 박스 놓는곳일 경우에
				map_current[stage][pl_y + dl_y][pl_x + dl_x] = 5;	//플레이어 왼쪽 이동
				map_current[stage][pl_y][pl_x] = 4;	//있던 자리는 박스 놓는 위치로 바꿈
			}
		}
		else {	//있던 자리가 공백일 경우
			if (map_current[stage][pl_y + dl_y][pl_x + dl_x] == 2) {	//왼쪽이 박스일 경우
				if (map_current[stage][pl_y + 2 * dl_y][pl_x + 2 * dl_x] != 1 && map_current[stage][pl_y + 2 * dl_y][pl_x + 2 * dl_x] != 2) {	//건너편이 박스랑 벽이 아닐 경우
					map_current[stage][pl_y + dl_y][pl_x + dl_x] = 5;	//플레이어 왼쪽 이동
					map_current[stage][pl_y + 2 * dl_y][pl_x + 2 * dl_x] = 2; //건너편 박스
					map_current[stage][pl_y][pl_x] = 0;	//있던 자리는 공백
				}
				else {	//건너편이 박스나 벽일 경우
					break;
				}
			}
			else if (map_current[stage][pl_y + dl_y][pl_x + dl_x] == 1) {	//오른쪽이 벽일 경우
				break;
			}
			else if (map_current[stage][pl_y + dl_y][pl_x + dl_x] == 0 || map_current[stage][pl_y + dl_y][pl_x + dl_x] == 4) {	//건너편이 박스랑 벽이 아닐 경우
				map_current[stage][pl_y + dl_y][pl_x + dl_x] = 5;	//플레이어 왼쪽 이동
				map_current[stage][pl_y][pl_x] = 0;	//있던 자리는 공백
			}
		}
		break;
	case 'j':
		dl_x = 0, dl_y = 1;
		if (map_file[stage][pl_y][pl_x] == 4) {	//있던 자리가 박스 놓는 위치일 경우
			if (map_current[stage][pl_y + dl_y][pl_x + dl_x] == 2) {	//아래쪽이 박스일 경우
				if (map_current[stage][pl_y + 2 * dl_y][pl_x + 2 * dl_x] != 1 && map_current[stage][pl_y + 2 * dl_y][pl_x + 2 * dl_x] != 2) {//건너편이 박스랑 벽이 아닐 경우
					map_current[stage][pl_y + dl_y][pl_x + dl_x] = 5;	//플레이어 아래쪽 이동
					map_current[stage][pl_y + 2 * dl_y][pl_x + 2 * dl_x] = 2; //건너편 박스
					map_current[stage][pl_y][pl_x] = 4;	//있던 자리는 박스 놓는 위치로 바꿈
				}
				else {	//건너편이 박스나 벽일 경우
					break;
				}
			}
			else if (map_current[stage][pl_y + dl_y][pl_x + dl_x] == 1) {	//오른쪽이 벽일 경우
				break;
			}
			else if (map_current[stage][pl_y + dl_y][pl_x + dl_x] == 0 || map_current[stage][pl_y + dl_y][pl_x + dl_x] == 4) {	//아래쪽이 공백이거나 박스 놓는곳일 경우에
				map_current[stage][pl_y + dl_y][pl_x + dl_x] = 5;	//플레이어 아래쪽 이동
				map_current[stage][pl_y][pl_x] = 4;	//있던 자리는 박스 놓는 위치로 바꿈
			}
		}
		else {	//있던 자리가 공백일 경우
			if (map_current[stage][pl_y + dl_y][pl_x + dl_x] == 2) {	//아래쪽이 박스일 경우
				if (map_current[stage][pl_y + 2 * dl_y][pl_x + 2 * dl_x] != 1 && map_current[stage][pl_y + 2 * dl_y][pl_x + 2 * dl_x] != 2) {//건너편이 박스랑 벽이 아닐 경우
					map_current[stage][pl_y + dl_y][pl_x + dl_x] = 5;	//플레이어 아래쪽 이동
					map_current[stage][pl_y + 2 * dl_y][pl_x + 2 * dl_x] = 2; //건너편 박스
					map_current[stage][pl_y][pl_x] = 0;	//있던 자리는 공백
				}
				else {	//건너편이 박스나 벽일 경우
					break;
				}
			}
			else if (map_current[stage][pl_y + dl_y][pl_x + dl_x] == 1) {	//아래쪽이 벽일 경우
				break;
			}
			else if (map_current[stage][pl_y + dl_y][pl_x + dl_x] == 0 || map_current[stage][pl_y + dl_y][pl_x + dl_x] == 4) {	//아래쪽이 공백이거나 박스 놓는곳일 경우에
				map_current[stage][pl_y + dl_y][pl_x + dl_x] = 5;	//플레이어 아래쪽 이동
				map_current[stage][pl_y][pl_x] = 0;	//있던 자리는 공백
			}
		}
		break;
	case 'k':
		dl_x = 0, dl_y = -1;
		if (map_file[stage][pl_y][pl_x] == 4) {	//있던 자리가 박스 놓는 위치일 경우
			if (map_current[stage][pl_y + dl_y][pl_x + dl_x] == 2) {	//위쪽이 박스일 경우
				if (map_current[stage][pl_y + 2 * dl_y][pl_x + 2 * dl_x] != 1 && map_current[stage][pl_y + 2 * dl_y][pl_x + 2 * dl_x] != 2) {//건너편이 박스랑 벽이 아닐 경우
					map_current[stage][pl_y + dl_y][pl_x + dl_x] = 5;	//플레이어 위쪽 이동
					map_current[stage][pl_y + 2 * dl_y][pl_x + 2 * dl_x] = 2; //건너편 박스
					map_current[stage][pl_y][pl_x] = 4;	//있던 자리는 박스 놓는 위치로 바꿈
				}
				else {
					break;
				}
			}
			else if (map_current[stage][pl_y + dl_y][pl_x + dl_x] == 1) {	//오른쪽이 벽일 경우
				break;
			}
			else if (map_current[stage][pl_y + dl_y][pl_x + dl_x] == 0 || map_current[stage][pl_y + dl_y][pl_x + dl_x] == 4) {	//위쪽이 공백이거나 박스 놓는곳일 경우에
				map_current[stage][pl_y + dl_y][pl_x + dl_x] = 5;	//플레이어 위쪽 이동
				map_current[stage][pl_y][pl_x] = 4;	//있던 자리는 박스 놓는 위치로 바꿈
			}
		}
		else {	//있던 자리가 공백일 경우
			if (map_current[stage][pl_y + dl_y][pl_x + dl_x] == 2) {	//위쪽이 박스일 경우
				if (map_current[stage][pl_y + 2 * dl_y][pl_x + 2 * dl_x] != 1 && map_current[stage][pl_y + 2 * dl_y][pl_x + 2 * dl_x] != 2) {	//건너편이 박스랑 벽이 아닐 경우
					map_current[stage][pl_y + dl_y][pl_x + dl_x] = 5;	//플레이어 위쪽 이동
					map_current[stage][pl_y + 2 * dl_y][pl_x + 2 * dl_x] = 2; //건너편 박스
					map_current[stage][pl_y][pl_x] = 0;
				}
				else {	//건너편이 박스나 벽이 경우
					break;
				}
			}
			else if (map_current[stage][pl_y + dl_y][pl_x + dl_x] == 1) {	//위쪽이 벽일 경우
				break;
			}
			else if (map_current[stage][pl_y + dl_y][pl_x + dl_x] == 0 || map_current[stage][pl_y + dl_y][pl_x + dl_x] == 4) {	//위쪽이 공백이거나 박스 놓는곳일 경우에
				map_current[stage][pl_y + dl_y][pl_x + dl_x] = 5;	//플레이어 위쪽 이동
				map_current[stage][pl_y][pl_x] = 0;	//있던 자리는 공백
			}
		}
		break;
	default:
		break;
	}
}

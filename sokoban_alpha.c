#include <stdio.h>
#include <stdlib.h>
#include <termio.h>
#include <time.h>

int i = -1, j, k, stage, dl_x, dl_y, enter_judge, save_count, game_exit,save_stage;
char get, game_act, get_enter;
int map_current[5][30][30];
int pl_x, pl_y,start_time[5],finish_time[5],diff_time[5],time_num=0,clear_num;
int map_file[5][30][30] = {};
char name[10];
char ch = 0;

void getname();
void getmap();
void showmap();
void showcommand();
void showname();
void pl_move();
void other_act();
void time_set();
void clear();

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

void time_set() {
	if (time_num == 0) {
		start_time[stage] = clock();
		time_num = 1;
	}
}

void getkey() {
	enter_judge = 0;
	printf("%c", get); 
	get_enter = getch();
	if (get_enter == '\n')
		enter_judge = 1;
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
void other_act(){
	switch (get) {
	case 'r':	//현재 맵 처음부터 다시 시작
		getkey();		
		if (enter_judge == 1) {
			for (j = 0; j < 30; j++) {
				for (k = 0; k < 30; k++) {
					map_current[stage][j][k] = map_file[stage][j][k];
				}
			}
		}
		game_act = 'r';
		break;
	case 'n':	//첫번째 맵부터 다시 시작
		getkey();		
		if (enter_judge == 1) {
			stage = 0;
			for (i = 0; i < 5; i++) {
				for (j = 0; j < 30; j++) {
					for (k = 0; k < 30; k++) {
						map_current[i][j][k] = map_file[i][j][k];
					}
				}
			}
		}
		game_act = 'n';
		break;
	case 'e':	//게임 종료, 종료하기전에 저장해야 함
		getkey();		
		if (enter_judge == 1) {
			if (save_count == 1) {
				game_exit = 1;
			}
			else {
				printf("\n종료하기 전에 저장해야 합니다");
			}
		}
		}
		game_act = 'e';
		break;
	case 's': 	//현재 상태를 파일에 저장
		getkey();		
		if (enter_judge == 1) {
			FILE *f = fopen("sokoban.txt", "w");
			fprintf(f, "%d", stage);
			int j, k;
			for (j = 0; j < 30; j++) {
				for (k = 0; k < 30; k++) {
					fprintf(f, "%d", map_current[stage][j][k]);
				}
			}
			save_count = 1;
			fclose(f);
		}
	}

		}
		game_act = 's';
		break;
	case 'd':	//명령 내용 보여주기
		getkey();		
		if (enter_judge == 1) {
			
		}
		game_act = 'd';
		break;
	case 'f': 	//저장된 내용을 불러오기
		getkey();		
		if (enter_judge == 1) {
			FILE *f = fopen("sokoban.txt", "r");
			fscanf(f, "%c", &save_stage);
			for (j = 0; j < 30; j++) {
				for (k = 0; k < 30; k++) {
					fscanf(f, "%c", &ch);
					map_current[save_stage][j][k] = ch - 48;
				}
			}
			stage = save_stage - 48;
			fclose(f);
			game_act = 'f';
				}
			}
		}
		break;
	case 't':	//게임 순위 보여주기
		getkey();		
		if (enter_judge == 1) {
			
		}
		game_act = 't';
		break;
	case 'u':	//예전 상태로 돌아가기, 최대 5번까지
		getkey();		
		if (enter_judge == 1) {
			
		}
		game_act = 'u';
		break;
	default:
		break;
	}
}
void clear(){	//게임을 클리어 했는지 판단하는 함수
	clear_num = 0;
	for (j = 0; j < 30; j++) {	//박스 놓는 곳이 있는지 판단하기
		for (k = 0; k < 30; k++) {
			if (map_current[stage][j][k] == 4) {
				clear_num++;
			}
		}
	}
	if (clear_num == 0 && map_file[stage][pl_y + dl_y][pl_x + dl_x] != 4) {	//박스 놓는 곳이 없고 플레이어 위치가 박스 놓는 곳이 아니라면 클리어
		finish_time[stage] = clock();
		diff_time[stage] = (finish_time[stage] - start_time[stage]) * 2.5 / CLOCKS_PER_SEC;
		stage++;
		time_num = 0;
		getch();

		if (stage == 5) {
			game_exit = 1;
		}
	}
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define ROW_MAX 26
#define ROW_MIN 5
#define COLUMN_MAX 26
#define MINE_CELL 50
#define CHECK_CELL 150
#define UNKOWN_CELL 100


int table_size, num_of_mine;
int mine_matrix[ROW_MAX + 2][COLUMN_MAX + 2], mine_matrix_answer[ROW_MAX + 2][COLUMN_MAX + 2];
char row[ROW_MAX + 1] = "abcdefghijklmnopqrstuvwxyz";

int clear_check() {
  int i ,j;
    for(i = 1; i < table_size + 1; i++) {
      for(j = 1; j < table_size + 1; j++) {
	if (mine_matrix_answer[i][j] < MINE_CELL) {
	  if (mine_matrix[i][j] == UNKOWN_CELL || mine_matrix[i][j] == CHECK_CELL)
	    return 1;
	}
    }
  }
    return 0;
}

void display_answer() {
  int i, j;
  char str[3];

  putchar(' ');
  for(i = 0; i < table_size; i++) { putchar(' '); putchar(row[i]);}
  puts("");

  for(i = 1; i < table_size + 1; i++) {
    sprintf(str, "%d", i - 1);
    printf("%s ", str);
    for(j = 1; j < table_size + 1; j++) {
      if (mine_matrix_answer[j][i] >= MINE_CELL)
	printf("x ");
      else
	printf("%d ", mine_matrix_answer[j][i]);
    }
    puts("");
  }
}

void display_mine_matrix() {
  int i, j;
  char str[3];

  putchar(' ');
  for(i = 0; i < table_size; i++) { putchar(' '); putchar(row[i]);}
  puts("");

  for(i = 1; i < table_size + 1; i++) {
    sprintf(str, "%d", i - 1);
    printf("%s ", str);
    for(j = 1; j < table_size + 1; j++) {
      if (mine_matrix[j][i] == UNKOWN_CELL)
	printf("? ");
      else if (mine_matrix[j][i] == CHECK_CELL)
	printf("x ");
      else
	printf("%d ", mine_matrix[j][i]);
    }
    puts("");
  }
}

void game_start() {
  int i, j, x, y, in_count;
  char c, open_or_check, in_char[4], *e;

  while(1){
    printf("選択する列•行を入力して下さい:\n");

    in_count = 0;
    while ((c = getchar()) != EOF) {
      if (c == '\n') break;
      if ( in_count < 3) {
	in_char[in_count] = c;
	in_count += 1;
      }
    }

    if(in_count < 2) { printf("正しく入力して下さい\n"); continue; }
    else { in_char[in_count] = '\0'; }

    // x座標を特定
    for(i = 0; i < 27; i++) { if ( in_char[0] == row[i]) { x = (i + 1); } }
    // y座標を特定
    y = ( strtol(&in_char[1], &e, 10) + 1);
    // 座標位置をチェック
    if(x > table_size || y > table_size) { printf("枠外です\n"); continue; }
    // 指定座標がまだ開かれていないかをチェック
    if(mine_matrix[x][y] != UNKOWN_CELL && mine_matrix[x][y] != CHECK_CELL) {
      printf("その座標はすでに開かれています\n");
      continue;
    }

    printf("開くならoを、地雷チェックはxを入力して下さい:\n");
    open_or_check = '\0';
    while ((c = getchar()) != EOF) {
      if (c == '\n') break;
      else if (c == 'o') {open_or_check = 'o';}
      else if (c == 'x') {open_or_check = 'x';}
    }

    switch(open_or_check) {
    case 'o':
      // 指定座標に地雷なし && 指定座標が地雷チェックされてない
      if( mine_matrix_answer[x][y] < MINE_CELL && mine_matrix[x][y] != CHECK_CELL) {
        mine_matrix[x][y] = mine_matrix_answer[x][y];
        display_mine_matrix();
      // 指定座標が地雷チェックされている
      } else if (mine_matrix[x][y] == CHECK_CELL) {
        printf("地雷チェックされているので開けません\n");
        display_mine_matrix();
        continue;
      }
      // 指定座標に地雷あり
      else {
        printf("Game Over!\n");
	display_answer();
	exit(0);
      }
      break;

    case 'x':
      if( mine_matrix[x][y] == CHECK_CELL ) {
        printf("チェック解除\n");
        mine_matrix[x][y] = UNKOWN_CELL;
      } else {
        mine_matrix[x][y] = CHECK_CELL;
      }
      display_mine_matrix();
      break;

    default:
        printf("不正な入力です\n");
        continue;
	break;
    }

    if(clear_check() == 0) {
      printf("Game Clear!\n");
      display_mine_matrix();
      exit(0);
    }

    //選んだ座標
    /* printf("x:%d y:%d\n", x, y); */
    /* printf("mine_matrix[%d][%d] = %d\n", x, y, mine_matrix_answer[x][y]); */
  }
}

int get_number_from_stdin() {
  char c, *e, field[3];
  int count, n;

  count = 0;
  while ((c = getchar()) != EOF) {
    if (c == '\n') break;
    if (!isdigit(c)) { printf("10進数で入力して下さい\n"); exit(0);}
    if ( count < 2) {
      field[count] = c;
      count += 1;
    }
  }
  field[count] = '\0';

  n = strtol(field, &e, 10);

  return n;
}
void initialize() {
  int i, j, x, y;

  printf("n × n のフィールドを作成します\n");
  printf("[%d <= n <= %d]\n", ROW_MIN, ROW_MAX);
  printf("nを入力して下さい：");
  table_size = get_number_from_stdin();
  if ( table_size < ROW_MIN || table_size > ROW_MAX ) {
    printf("%d <= n <= %d", ROW_MIN, ROW_MAX);
    printf("で入力してください\n");
    exit(0);
  }


  printf("地雷の数を入力して下さい\n");
  printf("[5 <= 地雷の数 <= %d]", table_size);
  num_of_mine = get_number_from_stdin();
  if ( table_size < ROW_MIN || table_size > ROW_MAX ) {
    printf("5 <= 地雷の数 <= %d", table_size);
    printf("で入力してください\n");
    exit(0);
  }


  for(i = 0; i < table_size + 2; i++) {
    for(j = 0; j < table_size + 2; j++) {
      mine_matrix[i][j] = UNKOWN_CELL;
      mine_matrix_answer[i][j] = 0;
    }
  }

  srand((unsigned)time(NULL));
  for(i = 0; i < num_of_mine; i++){
    x = rand() % table_size;
    y = rand() % table_size;
    // 地雷が重複したら選び直し
    while(mine_matrix_answer[x + 1][y + 1] >= MINE_CELL) { x = rand() % table_size; y = rand() % table_size; }
    // 地雷セット
    mine_matrix_answer[x + 1][y + 1] = MINE_CELL;
    // 地雷の位置から8近傍に対して+1
    // 上3つ
    mine_matrix_answer[x][y] += 1;
    mine_matrix_answer[x][y + 1] += 1;
    mine_matrix_answer[x][y + 2] += 1;
    //下3つ
    mine_matrix_answer[x + 2][y] += 1;
    mine_matrix_answer[x + 2][y + 1] += 1;
    mine_matrix_answer[x + 2][y + 2] += 1;
    //横2つ
    mine_matrix_answer[x + 1][y] += 1;
    mine_matrix_answer[x + 1][y + 2] += 1;
  }

}


int main(void) {
  initialize();
  // display_answer();
  display_mine_matrix();
  game_start();
}

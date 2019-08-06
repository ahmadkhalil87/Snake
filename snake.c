#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "support.h"
#define WIDTH 50
#define HEIGHT 25

// Autoren: khalila, siebert2

enum object{Snake = '*', Fruit = 'F', Empty = ' '};
typedef enum object obj;
struct positions {
  int x;
  int y;
};
typedef struct positions position;
struct snake{
  position head;
  struct snake* tail;
};
typedef struct snake* p_snake;
p_snake my_snake;
obj field[HEIGHT][WIDTH];
position my_fruit;

void resetField(p_snake snk);
p_snake resetSnake(int n);
void printField();
void move(p_snake s, char d);
void putSnakeOnField(p_snake snk);
position putFruitOnField();
void start(p_snake s);

int main(){
  support_init();
  my_snake = resetSnake(3);
  resetField(my_snake);
  start(my_snake);
  return 0;
}

void resetField(p_snake snk){
  for (int i=0; i<HEIGHT;i++){
    for (int j=0; j<WIDTH;j++){
      if (i==my_fruit.x && j==my_fruit.y){
        field[i][j] = Fruit;
        continue;
      }
      field[i][j] = Empty;
    }
  }
  putSnakeOnField(snk);
}

p_snake resetSnake(int n){
  position pos;
  pos.x = (int) HEIGHT/2;
  pos.y = (int) WIDTH/2;
  p_snake snk = malloc(sizeof(struct snake));
  snk->head = pos;
  p_snake p = snk;
  int i = 1;
  while (i < n){
    p->tail=malloc(sizeof(struct snake));
    p->tail->head=snk->head;
    p=p->tail;
    i++;
  }
  return snk;
}

void printField(){
  for (int i=-1; i<=HEIGHT;i++){
    for (int j=-1; j<=WIDTH;j++){
      if (i==-1 || i==HEIGHT){
	printf("-");
      }
      else{
	if (j==-1 || j==WIDTH){
	  printf("|");
	}
	else{
	  printf("%c",field[i][j]);
	}
      }
    }
    printf("\n");
  }
}

void putSnakeOnField(p_snake snk){
  p_snake p = snk;
  while (p != NULL){
    field[p->head.x][p->head.y] = Snake;
    p = p->tail;
  }
}

position putFruitOnField(){
  position pos;
  int x = time(NULL) % HEIGHT;
  int y = time(NULL) % WIDTH;
  if (field[x][y]!=Snake){
    pos.x = x;
    pos.y = y;
    field[x][y] = Fruit;
    return pos;
  }
  else{
    for (int i=0; i<HEIGHT;i++){
      for (int j=0; j<WIDTH;j++){
        if (field[i][j] == Empty) {
          field[i][j] = Fruit;
          pos.x = i;
          pos.y = j;
          return pos;
        }
      }
    }
  }//
}

void move(p_snake s, char d){
  position p = s->head;
  switch(d){
  case 'd':{
    if (p.y==WIDTH-1) p.y=0;
    else p.y++;
    break;
  }
  case 'a':{
    if (p.y==0) p.y=WIDTH-1;
    else p.y--;
    break;
  }
  case 'w':{
    if (p.x==0) p.x=HEIGHT-1;
    else p.x--;
    break;
  }
  case 's':{
    if (p.x==HEIGHT-1) p.x=0;
    else p.x++;
    break;
  }
  }
  if (field[p.x][p.y]==Snake){
      resetField(s);
      printf("\nGAME OVER");
      exit(0);
  }
  if (field[p.x][p.y]==Fruit){
    p_snake ps = s;
    p_snake pt = s->tail;
    position tmp;
    while (ps!=NULL && pt!=NULL && pt->head.x>=0){//
      tmp = pt->head;
      pt->head = ps->head;
      ps->head = tmp;
      ps = pt;
      pt = pt->tail;
    }//
    p_snake new = malloc(sizeof(struct snake));
    new->head = s->head;
    new->tail = s->tail;
    s->head = p;
    s->tail = new;
    my_fruit = putFruitOnField();
  }
  else{
    p_snake ps = s;
    p_snake pt = s->tail;
    position tmp;
    while (ps!=NULL && pt!=NULL && pt->head.x>=0){
      tmp = pt->head;
      pt->head = ps->head;
      ps->head = tmp;
      ps = pt;
      pt = pt->tail;
    }
    s->head = p;
  }
  support_clear();
  resetField(s);
  printField();
}

void start(p_snake s){
  my_fruit = putFruitOnField();
  printField();
  char a = 'a';
  char n;
  while (a!='x'){
    n = (char) support_readkey(1000);
    if (n == 'a' || n == 'w' || n == 's' || n == 'd' || n == 'x') a = n;
    move(s, a);
  }
  printf("Exit");
}

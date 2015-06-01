#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

// Constantes
const int WIDTH_SCREEN = 100, HEIGHT_SCREEN = 32;

// Getch() without ENTER
#include <unistd.h>   //_getch*/
#include <termios.h>  //_getch*/
char getch(){
  
    char buf=0;
    struct termios old={0};
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
    //printf("%c\n",buf);
    return buf;
}
// Fim funções Getch() withou ENTER


enum direcao{ UP, DOWN, LEFT, RIGHT};

struct junta
{	
	int x, y;
	struct junta *ant, *prox;
}*cabeca = NULL, *cauda = NULL;


void adicionar_junta(int x, int y);
void remover_junta();
void movetoxy(int x, int y);
void resize_console_window(const int width, const int height);
void update();
void detectar_tecla();

int sentido = 0;
int sair = 0;
int x = 1, y = 1;

int main()
{
	resize_console_window(WIDTH_SCREEN, HEIGHT_SCREEN); // Redimensiono a janela
	system("clear"); // Limpo a tela	
	
	do
	{
		update();
	}while(sair != 1);
	
	movetoxy(30, 1);

	struct junta *temp = cabeca;
	while(temp != NULL)
	{
		printf("\n%d, %d", temp->x, temp->y);
		temp = temp->prox;
	}


	return 0;
}

void update()
{
	//system("clear");
	detectar_tecla();
	movetoxy(x, y);
	printf(">");
	movetoxy(x, y);
}

void detectar_tecla()
{

	if (getch() == '\033') { // if the first value is esc
	    getch(); // skip the [
	    switch(getch()) { // the real value
	        case 'A':
	            // code for arrow up
	        	//printf("\nUp");
	        	y--;
	            break;
	        case 'B':
	            // code for arrow down
	        	//printf("\nDown");
	        	y++;
	            break;
	        case 'C':
	            // code for arrow right
	        	//printf("\nRight");
	        	x++;
	            break;
	        case 'D':
	            // code for arrow left
	        	//printf("\nLeft");
	        	x--;
	            break;
	    }
	}

	if(x < 1)
		x = 1;
	if(x > WIDTH_SCREEN)
		x = WIDTH_SCREEN;
	if(y < 1)
		y = 1;
	if(y > HEIGHT_SCREEN)
		y = HEIGHT_SCREEN;
}

void adicionar_junta(int x, int y)
{
	struct junta *dado = (struct junta*) malloc(sizeof(struct junta));
	dado->x = x;
	dado->y = y;
	dado->prox = NULL;

	if(cabeca == NULL) // Vazio
	{
		dado->ant = NULL;
		cabeca = dado;
		cauda = dado;
	}
	else
	{
		dado->ant = cauda;
		cauda->prox = dado;
		cauda = dado;
	}
}

void remover_junta()
{

	if(cabeca == NULL)
	{
		printf("\nNao foi possivel remover uma junta. Pois esta vazio!");
	}
	else
	{
		if(cabeca != cauda)
		{
			struct junta *temp = cauda; // Adiciono uma referência pra liberar a memória depois
			cauda->ant->prox = NULL;
			cauda = cauda->ant;
			free(temp);
		}
		else // So tem uma junta
		{
			free(cabeca);
			cabeca = NULL;
			cauda = NULL;
		}
	}
}

void movetoxy(int x, int y){
     printf("%c[%d;%df",0x1B,y,x);
}

void resize_console_window(int width, int height)
{
    printf("\e[8;%d;%d;t", height, width);
}
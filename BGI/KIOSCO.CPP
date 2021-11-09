#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdarg.h> //para gprintf
#include <dos.h>
#include <iostream.h>
#include <time.h>
#include <alloc.h>
#include <ctype.h>
#include <bios.h>
#include <string.h>
#include "cargaf.h"


#define MENSAJE1 "\nBETY: memoria insuficiente o archivo no encontrado!"
#define LLAMA "\nLLAMA A \"ARREGLATUTI\" (tu querido hijo) que por unos cuantos US$ lo arregla ...jijiji!!!"
#define ESC 283
#define F2 15360
#define CLR 3592
#define UP 18432
#define DW 20480
#define UNO_TEC 561
#define UNO_PAD 20224
#define DOS_TEC 818
#define DOS_PAD 20480
#define TRES_TEC 1075
#define TRES_PAD 20736
#define CLR 3592
#define ARRIBA 18432
#define ABAJO 20480


/*             funciones                                            */
void DrawBorder(void);
void Initialize(void);
void starts(void);
void Texto(void);
int menu(void);
void presentacion(void);
void cambios(void);
void buscar(void);
void reloj(void);
int gprintf(int *, int *, char *, ... );
void optarcolor(int , int);
void plantilla(void);
void llen_plantilla(int st, struct articulo *lst, int n);
void borrar_blk(int x1, int y1, int x2, int y2);
void ordenar(struct articulo *lst, int n);
int busqueda(struct articulo *lst, const int n, char *cs, int *jin);
wind_busc(char *cs);

int GraphDriver;             /* The Graphics device driver           */
int GraphMode;               /* The Graphics mode value              */
int ErrorCode;
int MaxColors;
struct tm *tblock;



void main(void)
{
int opcion;
int salir;

Initialize();
presentacion();
delay(3000);
while(salir != 1)
	  {
	  menu();
	  fflush(stdin);
	  while (bioskey(1) == 0);
	  opcion = bioskey(0);
	  switch(opcion)
			  {
			  case UNO_TEC : cambios(); break;
			  case UNO_PAD : cambios(); break;
			  case DOS_TEC : buscar(); break;
			  case DOS_PAD : buscar(); break;
			  case TRES_TEC : salir = 1; break;
			  case TRES_PAD : salir = 1; break;
			  case ESC : salir = 1; break;
			  }
	  }
closegraph();
}

void Initialize(void)
{

GraphDriver = DETECT;                 /* Request auto-detection       */
initgraph( &GraphDriver, &GraphMode, "" );
ErrorCode = graphresult();            /* Read result of initialization*/
if( ErrorCode != grOk )
  {              /* Error occured during init    */
  printf(" Graphics System Error: %s\n", grapherrormsg( ErrorCode ) );
  exit( 1 );
  }
MaxColors = getmaxcolor();
}

void presentacion(void)
{
cleardevice();
DrawBorder();
starts();
Texto();
}

void DrawBorder(void)
{
setcolor( MaxColors );            /* Set current color to white   */
setlinestyle( SOLID_LINE, 0, NORM_WIDTH );
rectangle( 0, 0, getmaxx(), getmaxy());
}

void starts()
{
int i;
struct viewporttype viewinfo;

getviewsettings( &viewinfo );
for ( i=0 ; i<1000; ++i )
	 putpixel(random(viewinfo.right), random(viewinfo.bottom), random(MaxColors+1));
}

void Texto()
{
struct viewporttype infovista;
int midx, midy, locy;
char k[]={"KioscO"};
char b[]={"Bety"};

midx = (infovista.right)/2;
midy = (infovista.bottom)/2;
optarcolor(4, 1);
settextstyle(TRIPLEX_FONT, HORIZ_DIR,100);
settextjustify(CENTER_TEXT, CENTER_TEXT);
locy = midy-50;
gprintf(&midx, &locy, "%s", k);
optarcolor(13, 1);
settextstyle(GOTHIC_FONT,HORIZ_DIR,80);
gprintf(&midx, &locy, "%s", b);
}

int gprintf( int *xloc, int *yloc, char *fmt, ... )
{
  va_list  argptr;                      /* Argument list pointer        */
  char str[140];                        /* Buffer to build sting into   */
  int cnt;                              /* Result of SPRINTF for return */

  va_start( argptr, fmt );              /* Initialize va_ functions     */

  cnt = vsprintf( str, fmt, argptr );   /* prints string to buffer      */
  outtextxy( *xloc, *yloc, str );       /* Send string in graphics mode */
  *yloc += textheight( "H" ) + 2;       /* Advance to next line         */

  va_end( argptr );                     /* Close va_ functions          */

  return( cnt );                        /* Return the conversion count  */

}

int menu(void)
{
int x, y;

x = 10;
y = 4;
cleardevice();
DrawBorder();
optarcolor(7, 1);
settextstyle(BOLD_FONT, HORIZ_DIR, 2);
settextjustify(LEFT_TEXT, TOP_TEXT);
gprintf(&x, &y, "1- Agregar o quitar articulos");
gprintf(&x, &y, "2- Abrir pantalla de busqueda");
gprintf(&x, &y, "3- Salir");

return 0;
}

void optarcolor(int colvga, int colherc)
{
if(MaxColors == 15)
  setcolor(colvga);
else
  setcolor(colherc);
}

void cambios(void)
{

}

void buscar()
{
int j, jant, i, n, salir = 1, stp, c, m;
struct articulo *lst;
char cs[N];

for(i = 0; i < N; i++)
	cs[i] = '\0';

lst = carga(&n);

if(!lst)
	{
	closegraph();
	cout << MENSAJE1;
	cout << LLAMA;
	exit(1);
	}

ordenar(lst, n);

jant = 0;
j = 0;
i = 0;

plantilla();
wind_busc(cs);
llen_plantilla(j, lst, n);
while(salir)
	  {
	  while (bioskey(1) == 0);
	  c = bioskey(0);

	  switch(c)
			  {
			  case ESC :
								salir = 0; break;
			  case F2 :
								sound(1000);
								delay(1000);
								nosound();
								break;
			  case ABAJO :
								if(j < n-1)
								  {
								  ++j;
								  llen_plantilla(j, lst, n);
								  }
								break;
			  case ARRIBA :
								if(j > 0)
								  {
								  --j;
								  llen_plantilla(j, lst, n);
								  }
								break;
			  case CLR :
								if(i > 0)
								  {
								  cs[i-1] = '\0';
								  --i;
								  wind_busc(cs);
								  j = 0;
								  busqueda(lst, n, cs, &j);
								  jant = j;
								  llen_plantilla(j, lst, n);
								  }
								break;
			  default :
								if(i < N)
								  {
								  j = jant;
								  cs[i] = c;
								  m = busqueda(lst, n, cs, &j);
								  if(m == 0)
									 {
									 cs[i] = '\0';
									 --i;
									 }
								  wind_busc(cs);
								  llen_plantilla(j, lst, n);
								  jant = j;
								  ++i;
								  }
			  }
	  }
free(lst);
}








void reloj(void)
{
	time_t timer;

	/* gets time of day */
	timer = time(NULL);

	/* converts date/time to a structure */
	tblock = localtime(&timer);
}

void plantilla(void)
{
int secx, secy, x, y, i;


secx = getmaxx()/20;
secy = getmaxy()/20;

cleardevice();
DrawBorder();
setlinestyle(4,0xFFFF,1);
optarcolor(15, 1);
line(0, 2*secy, getmaxx(), 2*secy);
line(0, 4*secy, getmaxx(), 4*secy);
line(18*secx, 0, 18*secx, 2*secy);
line(4*secx, 2*secy, 4*secx, getmaxy());//L1
line(7*secx, 2*secy, 7*secx, getmaxy());//L2
line(13*secx, 2*secy, 13*secx, getmaxy());//L3
line(15*secx, 2*secy, 15*secx, getmaxy());//L4
line(17*secx, 2*secy, 17*secx, getmaxy());//L5
optarcolor(15, 1);
settextstyle(2, HORIZ_DIR, 0);
setusercharsize(1, 1, 1, 1);
settextjustify(CENTER_TEXT, CENTER_TEXT);
outtextxy(2*secx, 3*secy, "MARCA");
outtextxy((11*secx)/2, 3*secy, "CODIGO");
outtextxy((20*secx)/2, 3*secy, "DESCRIPCION");
outtextxy(14*secx, (5*secy)/2, "PRECIO");
outtextxy(14*secx, (7*secy)/2, "AL PUBL.");
outtextxy(16*secx, (5*secy)/2, "PRECIO");
outtextxy(16*secx, (7*secy)/2, "X MAY.");
outtextxy((37*secx)/2, 3*secy, "FECHA DE ACT.");
reloj();
x = 19*secx; y = secy;
gprintf(&x , &y, "%d/%d/%d", tblock->tm_mday, (tblock->tm_mon+1), (tblock->tm_year+1900));
}

wind_busc(char *cs)
{
int secx, secy, x, y;

settextjustify(LEFT_TEXT, CENTER_TEXT);
secx = getmaxx()/20;
secy = getmaxy()/20;

x = 1*secx; y = secy;
borrar_blk(0+1, 0+1, 10*secx,(2*secy)-1);
gprintf(&x, &y, "Ingrese articulo a buscar: %s", cs);
settextjustify(CENTER_TEXT, CENTER_TEXT);
}

void llen_plantilla(int st, struct articulo *lst, int n)
{
int secx, secy, x, y, i;
float posy;


secx = getmaxx()/20;
secy = getmaxy()/20;

borrar_blk(0+1, (4*secy)+1, (4*secx)-1, getmaxy()-1);
borrar_blk((4*secx)+1, (4*secy)+1, (7*secx)-1, getmaxy()-1);
borrar_blk((7*secx)+1, (4*secy)+1, (13*secx)-1, getmaxy()-1);
borrar_blk((13*secx)+1, (4*secy)+1, (15*secx)-1, getmaxy()-1);
borrar_blk((15*secx)+1, (4*secy)+1, (17*secx)-1, getmaxy()-1);
borrar_blk((17*secx)+1, (4*secy)+1, getmaxx()-1, getmaxy()-1);

posy = (float) 4.25*secy;
i = st;
for(i; i < (st+33) && i < n; i++)
	{
	x = 2*secx; y = posy;
	gprintf(&x, &y, "%s", (lst+i)->marca) ;
	x = (11*secx)/2; y = posy;
	gprintf(&x, &y, "%s", (lst+i)->codigo);
	x = (20*secx)/2; y = posy;
	gprintf(&x, &y, "%s", (lst+i)->descr);
	x = 14*secx; y = posy;
	gprintf(&x, &y, "$ %.2f", (lst+i)->precio);
	x = 16*secx; y = posy;
	gprintf(&x, &y, "$ %.2f", (lst+i)->preciom);
	x = (37*secx)/2; y = posy;
	gprintf(&x, &y, "%d/%d/%d", (lst+i)->tc.tm_mday, (lst+i)->tc.tm_mon+1, (lst+i)->tc.tm_year+1900);
	posy = (float) posy + (secy*1.25)/2;
	}

}

void borrar_blk(int x1, int y1, int x2, int y2)
{
int secx, secy;
unsigned int size;
void *blk;

secx = getmaxx()/20;
secy = getmaxy()/20;
size = imagesize(x1, y1, x2, y2);
blk = malloc(size);
getimage(x1, y1, x2, y2, blk);
putimage(x1, y1, blk, XOR_PUT);
free(blk);
}


void ordenar(struct articulo *lst, int n)
{
int i, j;
struct articulo aux;

for(j = 0; j < n; j++)
	for(i = 0; i < n-1; i++)
		if ((strcmp( (lst+i)->marca , (lst+i+1)->marca)) >= 0)
			{
			aux = *(lst+i);
			*(lst+i) = *(lst+i+1);
			*(lst+i+1) = aux;
			}
}

int busqueda(struct articulo *lst, const int n, char *cs, int *jin)
{
int i, j, result = 0;
char *cad;
cad = (char *) malloc((n*N)* sizeof(char));

if(cs[0] == '\0')
  *jin = 0;
else
  {
  for(j = *jin; j<n; j++)
	  for(i = 0; cs[i] != '\0'; i++)
		  {
		  *(cad+(j*N)+i) = (lst+j)->marca[i];
		  *(cad+(j*N)+i+1)='\0';
		  }
  for(j = *jin; j<n; j++)
		if(!( strcmp(cs, (cad+(j*N))) ))
		  {
		  *jin = j;
		  j = n;
		  result = 1;
		  }
  }
free(cad);
return result;
}




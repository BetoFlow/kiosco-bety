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


#define MENSAJE1 "\nBETY: memoria insuficiente o archivo no encontrado!"
#define LLAMA "\nLLAMA A ARREGLATUTI (tu querido hijo) que por unos cuantos U$S lo arregla ...jijiji!!!"
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
struct articulo *carga(int *);
int contart(FILE *);
int gprintf(int *, int *, char *, ... );
void optarcolor(int , int);
void toma_cad(FILE *, char *, char);
void plantilla(int, struct articulo *, int);

int GraphDriver;             /* The Graphics device driver           */
int GraphMode;               /* The Graphics mode value              */
int ErrorCode;
int MaxColors;
struct tm *tblock;

#define items 9/*Cantidad de datos por articulo (camabiar si se agregan mas!!!)*/
struct articulo {int nro; char marca[25]; char codigo[14]; float precio, preciom; char descr[25]; struct tm tc;};


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
int x, y, i, n;
struct articulo *lst;
x = 10;
y = 4;

lst = carga(&n);

if(!lst)
	{
	closegraph();
	cout << MENSAJE1;
	cout << LLAMA;
	exit(1);
	}

cleardevice();
DrawBorder();
optarcolor(15, 1);
settextstyle(6, HORIZ_DIR, 0);
setusercharsize(3, 6, 3, 6);
settextjustify(LEFT_TEXT, TOP_TEXT);
for(i = 0; i < n; i++)
	gprintf(&x, &y, "%d %c %s %s %.2f %.2f %d/%d/%d\n", (lst+i)->nro, (lst+i)->marca[0], (lst+i)->codigo, (lst+i)->descr, (lst+i)->precio, (lst+i)->preciom, lst[i].tc.tm_mday, lst[i].tc.tm_mon+1, lst[i].tc.tm_year+1900);
getch();

free(lst);
}

void buscar()
{
int x, y, k, i, n, salir, flag, secx, stp;
struct articulo *lst;
char cs[26], c;

lst = carga(&n);

if(!lst)
	{
	closegraph();
	cout << MENSAJE1;
	cout << LLAMA;
	exit(1);
	}

closegraph();
for(i = 0; i < n; i++ )
printf("|%s|\n", (lst+i)->marca);
getch();
Initialize();

plantilla(0, lst, n);
getch();


free(lst);
}


struct articulo *carga(int *ni)
{
int i, n;
FILE *pf;
float *f;
struct articulo *list;
char ent;
int nro[10];

pf = fopen("libro1.txt", "rt");
n = contart(pf);
list = (struct articulo *) malloc (n * sizeof(struct articulo));
f = (float *) malloc (n * sizeof(float));
if(list)
  {
  for(i = 0; i < n; i++)
	  {
	  toma_cad(pf, (list+i)->marca, '\t');
	  toma_cad(pf, (list+i)->codigo, '\t');
	  toma_cad(pf, (list+i)->descr, '\t');
	  fscanf(pf, "%f", (f+i));
	  (list+i)->precio = *(f+i);
	  fscanf(pf, "%f", (f+i));
	  (list+i)->preciom = *(f+i);
	  fscanf(pf, "%d", &list[i].nro);
	  fscanf(pf, "%d", &list[i].tc.tm_mday);
	  fscanf(pf, "%d", &list[i].tc.tm_mon);
	  list[i].tc.tm_mon = list[i].tc.tm_mon - 1; //para poder luego usarlo en las funciones time
	  fscanf(pf, "%d", &list[i].tc.tm_year);
	  list[i].tc.tm_year = list[i].tc.tm_year - 1900;
	  fscanf(pf, "%c", &ent); //para que lea el enter del final y no lo meta en el proximo "marca"
	  }
  }
rewind(pf);
fclose(pf);
*ni = n;
return (list);
}

int contart(FILE *pfx)
{
char c;
int i = 0;

while ((c = fgetc (pfx)) != EOF)
		if (c == '\t')
			i ++;
i = i/(items-1);
rewind(pfx);
return i;
}

void toma_cad(FILE *pf1, char *cadena, char stp)
{
char s;
int j;
j = 0;
while((s = fgetc(pf1)) != stp)
	  {
	  *(cadena+j) = s;
	  j++;
	  }
*(cadena+j) = '\0';
}

void reloj(void)
{
	time_t timer;

	/* gets time of day */
	timer = time(NULL);

	/* converts date/time to a structure */
	tblock = localtime(&timer);
}

void plantilla(int st, struct articulo *lst, int n)
{
int secx, secy, x, y, i, posy;

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
outtextxy(4*secx, secy, "Ingrese articulo a buscar:");
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


//settextjustify(LEFT_TEXT, CENTER_TEXT);
posy = 5*secy;
for(i = st; i < n; i++)
	{
	x = 2*secx; y = posy;
	gprintf(&x, &y, "%s", (lst+i)->marca);
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
	posy = posy + (secy)/2;
	}

}

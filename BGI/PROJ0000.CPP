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

#define N 30
#define CD 15
#define PLUS 100
#define MENSAJE1 "\nBETY: memoria insuficiente o archivo no encontrado!"
#define LLAMA "\nLLAMA A \"ARREGLATUTI\" (tu querido hijo) que por unos cuantos US$ lo arregla ...jijiji!!!"
#define ESC 0x11B
#define F1 0x3b00
#define F2 0x3C00
#define F3 0x3d00
#define F4 0x3e00
#define F5 0x3f00
#define F6 0x4000
#define CLR 0xE08
#define UP 0x4800
#define DW 0x5000
#define UNO_TEC 0x231
#define UNO_PAD 0x4F00
#define DOS_TEC 0x332
#define DOS_PAD 20480
#define TRES_TEC 0x433
#define TRES_PAD 20736
#define CLR 0xE08



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
void plantilla(void);
void llen_plantilla(int st, struct articulo *lst, int n);
void borrar_blk(int x1, int y1, int x2, int y2);
void ordenar(int, struct articulo *lst, int n);
int busqueda(int, struct articulo *lst, const int n, char *cs, int *jin);
void wind_busc(char *cs);
void minuscula(struct articulo *lst, int n);
void leer(char *cad, int cant);

int GraphDriver;             /* The Graphics device driver           */
int GraphMode;               /* The Graphics mode value              */
int ErrorCode;
int MaxColors;
struct tm *tblock;

#define items 9/*Cantidad de datos por articulo (cambiar si se agregan mas!!!)*/
struct articulo {int h; char marca[N]; char codigo[CD]; float precio, preciom; char descr[N]; struct tm tc; char nom_may[N];};


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
char by[]={"By A. R. Miguens"};

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
locy = midy+180;
settextstyle(SIMPLEX_FONT,HORIZ_DIR,1);
optarcolor(WHITE, 1);
gprintf(&midx, &locy, "%s", by);
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
gprintf(&x, &y, "1- Agregar articulos");
gprintf(&x, &y, "2- Abrir pantalla de busqueda");
gprintf(&x, &y, "3- Salir al sistema operativo");

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
int i, cant, datos, c;
int salir = 1;
struct articulo prod[PLUS];
struct text_info ti;
FILE *pf;
char saux[6];
gettextinfo(&ti);
closegraph();
clrscr();
i = 0;
cant = 0;
datos = 0;
while(salir && (i < PLUS))
	  {
	  gotoxy(ti.winleft, ti.winbottom-1);
	  textcolor(BLACK);
	  textbackground(WHITE);
	  cputs( "[F1]-MARCA [F2]-CODIGO [F3]-DECRIPCION [F4]-PRECIO [F5]-PRECIO X M. [F6]-MAYOR.");
	  textcolor(WHITE);
	  textbackground(BLACK);
	  while (bioskey(1) == 0);
	  c = bioskey(0);

	  switch(c)
			  {
			  case ESC :
							 salir = 0; break;
			  case F1 :
							 gotoxy(1, 1);
							 cputs("\nF1-Ingrese marca del producto: ");
							 fflush(stdin);
							 gets(prod[i].marca);
							 datos = datos | 1 ;
							 break;

			  case F2 :
							 gotoxy(1, 2);
							 cputs("\nF2-Ingrese el codigo: ");
							 fflush(stdin);
							 gets(prod[i].codigo);
							 datos = datos | 2 ;
							 break;

			  case F3 :
							 gotoxy(1, 3);
							 cputs("\nF3-Ingrese una descripcion: ");
							 fflush(stdin);
							 gets(prod[i].descr);
							 datos = datos | 4 ;
							 break;

			  case F4 :
							 gotoxy(1, 4);
							 cputs("\nF4-Ingrese precio al publico: ");
							 fflush(stdin);
							 gets(saux);
							 if(saux == "\n\0")
								 prod[i].precio = 0;
							 else
								 prod[i].precio = atof(saux);
							 datos = datos | 8 ;
							 break;

			  case F5 :
							 gotoxy(1, 5);
							 cputs("\nF5-Ingrese precio x mayor: ");
							 fflush(stdin);
							 gets(saux);
							 if(saux == "\n\0")
								 prod[i].preciom = 0;
							 else
								 prod[i].preciom = atof(saux);
							 datos = datos | 16 ;
							 break;

			  case F6 :
							 gotoxy(1, 6);
							 cputs("\nF6-Ingrese nombre del mayorista: ");
							 fflush(stdin);
							 gets(prod[i].nom_may);
							 datos = datos | 32 ;
							 break;
			  }
	  if(datos == 63)
		 {
		 gotoxy(ti.winleft, ti.winbottom-10);
		 clreol();
		 textcolor(LIGHTGRAY);
		 cputs("El articulo \"");
		 textcolor(WHITE);
		 cputs(prod[i].marca);
		 textcolor(LIGHTGRAY);
		 cputs("\" ya tiene todos los datos necesarios.\n\r");
		 cputs("Desea cambiar algo?\n\r");
		 textcolor(WHITE+BLINK);
		 cputs("(PRESIONE [c] PARA CAMBIOS O CUALQUIER OTRA TECLA PARA CONTINUAR)");
		 textcolor(LIGHTGRAY);
		 while (bioskey(1) == 0);
		 c = bioskey(0);
		 if(((c & 0xff) != 'c') && ((c & 0xff) != 'C'))
			  {
			  reloj();
			  prod[i].tc.tm_mday = tblock->tm_mday;
			  prod[i].tc.tm_mon = tblock->tm_mon+1;
			  prod[i].tc.tm_year = tblock->tm_year+1900;
			  cant = ++i;
			  datos = 0;
			  clrscr();
			  }
		 else
			  {
           gotoxy(ti.winleft, ti.winbottom-10);
			  clreol();
			  gotoxy(ti.winleft, ti.winbottom-9);
			  clreol();
			  gotoxy(ti.winleft, ti.winbottom-8);
			  clreol();
           }
		 }
	  }
pf = fopen("libro1.txt", "a");
for(i = 0; i < (cant); i++)
	fprintf(pf, "%s\t%s\t%s\t%f\t%f\t%d\t%d\t%d\t%s\n", prod[i].marca, prod[i].codigo, prod[i].descr, prod[i].precio, prod[i].preciom, prod[i].tc.tm_mday, prod[i].tc.tm_mon, prod[i].tc.tm_year, prod[i].nom_may);
rewind(pf);
fclose(pf);
Initialize();
}

void buscar()
{
int j, i, n, salir, stp, c, m, modo_busc, modo_ant;
struct articulo *lst;
char cs[N];

lst = carga(&n);
minuscula(lst, n);
for(i = 0; i < N; i++)
	cs[i] = '\0';
modo_ant = 0;
modo_busc = 0;
salir = 1;
j = 0;
i = 0;
plantilla();
wind_busc(cs);
ordenar(modo_busc, lst, n);
llen_plantilla(j, lst, n);
while(salir)
	  {
	  while (bioskey(1) == 0);
	  c = bioskey(0);

	  switch(c)
			  {
			  case ESC :
								salir = 0; break;
			  case F1 :
								modo_ant = modo_busc = 0;
								ordenar(modo_busc, lst, n);
								j = 0;
								llen_plantilla(j, lst, n);
								for(i = 0; i < N; i++)
									cs[i] = '\0';
								wind_busc(cs);
								i = 0;
								break;
			  case F2 :
								modo_ant = modo_busc = 1;
								ordenar(modo_busc, lst, n);
								j = 0;
								llen_plantilla(j, lst, n);
								for(i = 0; i < N; i++)
									cs[i] = '\0';
								wind_busc(cs);
								i = 0;
								break;
			  case F3 :
								modo_ant = modo_busc = 2;
								ordenar(modo_busc, lst, n);
								j = 0;
								llen_plantilla(j, lst, n);
								for(i = 0; i < N; i++)
									cs[i] = '\0';
								wind_busc(cs);
								i = 0;
								break;

			  case F4 :
								ordenar(modo_busc, lst, n);
								j = 0;
								llen_plantilla(j, lst, n);
								for(i = 0; i < N; i++)
									cs[i] = '\0';
								wind_busc(cs);
								i = 0;
								break;
			  case DW :
								if(j < n-1)
								  {
								  ++j;
								  llen_plantilla(j, lst, n);
								  }
								break;
			  case UP :
								if(j > 0)
								  {
								  --j;
								  llen_plantilla(j, lst, n);
								  }
								break;
			  default :
								if((i == 0) && isdigit(c & 0xff))
								  {
								  if(modo_busc != 1)
									 modo_ant = modo_busc;
								  modo_busc = 1;
								  ordenar(modo_busc, lst, n);
								  j = 0;
								  llen_plantilla(j, lst, n);
								  for(i = 0; i < N; i++)
									  cs[i] = '\0';
								  wind_busc(cs);
								  i = 0;
								  }
								else if((i == 0) && isalnum(c & 0xff))
										 {
										 modo_busc = modo_ant;
										 ordenar(modo_busc, lst, n);
										 j = 0;
										 i = 0;
										 ordenar(modo_busc, lst, n);
										 llen_plantilla(j, lst, n);
										 cs[i] = '\0';
										 wind_busc(cs);
										 }
								if((c == CLR) && (i > 0))
								  {
								  --i;
								  cs[i] = '\0';
								  wind_busc(cs);
								  busqueda(modo_busc, lst, n, cs, &j);
								  llen_plantilla(j, lst, n);
								  }

								else if(i < N)
										{
										cs[i] = c;
										wind_busc(cs);
										if(!busqueda(modo_busc, lst, n, cs, &j))
										  {
										  cs[i] = '\0';
										  wind_busc(cs);
										  }
										else {
											  llen_plantilla(j, lst, n);
											  ++i;
											  }
										}
			  }
	  }
free(lst);
}


struct articulo *carga(int *ni)
{
int i, n;
FILE *pf;
float *f;
struct articulo *list;
char ent;

pf = fopen("libro1.txt", "r");
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
	  fscanf(pf, "%d", &list[i].tc.tm_mday);
	  fscanf(pf, "%d", &list[i].tc.tm_mon);
	  list[i].tc.tm_mon = list[i].tc.tm_mon - 1; //para poder luego usarlo en las funciones time
	  fscanf(pf, "%d", &list[i].tc.tm_year);
	  list[i].tc.tm_year = list[i].tc.tm_year - 1900;
	  fscanf(pf, "%c", &ent);
	  toma_cad(pf, (list+i)->nom_may, '\n');
	  //fscanf(pf, "%c", &ent); //para que lea el enter del final y no lo meta en el proximo "marca"
	  }
  }
else
  {
  closegraph();
  cout << MENSAJE1;
  cout << LLAMA;
  exit(1);
  }

rewind(pf);
fclose(pf);
free(f);
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

void wind_busc(char *cs)
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


void ordenar(int modo, struct articulo *lst, int n)
{
int i, j;
struct articulo aux;


for(j = 0; j < n; j++)
	for(i = 0; i < n-1; i++)
		switch(modo)
			 {
			 case 0:
					 if((strcmp( (lst+i)->marca , (lst+i+1)->marca)) >= 0)
						{
						aux = *(lst+i);
						*(lst+i) = *(lst+i+1);
						*(lst+i+1) = aux;
						}
						break;
			 case 1:
					 if((strcmp( (lst+i)->codigo , (lst+i+1)->codigo)) >= 0)
						{
						aux = *(lst+i);
						*(lst+i) = *(lst+i+1);
						*(lst+i+1) = aux;
						}
						break;
			 case 2:
					if((strcmp( (lst+i)->descr , (lst+i+1)->descr)) >= 0)
						{
						aux = *(lst+i);
						*(lst+i) = *(lst+i+1);
						*(lst+i+1) = aux;
						}
						break;
			 }

}

int busqueda(int modo, struct articulo *lst, const int n, char *cs, int *jin)
{
int i, j, result = 0;
char *cad;
cad = (char *) malloc((n*N)* sizeof(char));

if(cs[0] == '\0')
  *jin = 0;
else
  {
  for(j = 0; j<n; j++)
	  for(i = 0; cs[i] != '\0'; i++)
		  {
		  switch(modo)
			{
			case 0:
					*(cad+(j*N)+i) = (lst+j)->marca[i];
					break;
			case 1:
					*(cad+(j*N)+i) = (lst+j)->codigo[i];
					break;
			case 2:
					*(cad+(j*N)+i) = (lst+j)->descr[i];
					break;
			}
		  *(cad+(j*N)+i+1)='\0';
		  }
  for(j = 0; j<n; j++)
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

void minuscula(struct articulo *lst, int n)
{
int j;

for(j = 0; j < n; j++)
	{
	strcpy((lst+j)->marca, strlwr((lst+j)->marca));
	strcpy((lst+j)->descr, strlwr((lst+j)->descr));
	strcpy((lst+j)->nom_may, strlwr((lst+j)->nom_may));
	}
}



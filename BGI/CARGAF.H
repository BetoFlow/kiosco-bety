#define N 30
#define CD 15
#define items 9/*Cantidad de datos por articulo (cambiar si se agregan mas!!!)*/
struct articulo {int nro; char marca[N]; char codigo[CD]; float precio, preciom; char descr[N]; struct tm tc;};

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



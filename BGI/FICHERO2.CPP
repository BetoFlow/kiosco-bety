#include <stdio.h>
#include <iostream.h>

#define n 1


struct producto{char marca[25]; char codigo[12]; float precio;};

void main()
{
int i, j;
struct producto prod[n], produ[n];
FILE *pf;
char s;
for(i = 0; i < n; i++)
	{
	cout << "marca?\n";
	gets(prod[i].marca);
	cout << "codigo?\n";
	cin >> prod[i].codigo;
	cout << "precio?\n";
	cin >> prod[i].precio;
	}

pf = fopen("d:\\producto.txt", "wb+");
for(i = 0; i < n; i++)
	fprintf(pf, "%s,%s,%f\n", prod[i].marca, prod[i].codigo, prod[i].precio);
rewind(pf);

for(i = 0; i < n; i++)
	{
	j = 0;
	while((s = fgetc(pf)) != ',')
		  {
		  produ[i].marca[j] = s;
		  j++;
		  }
	produ[i].marca[j] = '\0';
	}
for(i = 0; i < n; i++)
	cout << produ[i].marca <<'\n';
rewind(pf);
cout << pf << '\n';
fclose(pf);
}
#include <stdio.h>
#include <iostream.h>

#define n 1


struct producto{char marca[25]; char codigo[12]; float precio;};

void main()
{
int i;
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

pf = fopen("d:\\producto.txt", "w+");
for(i = 0; i < n; i++)
	fprintf(pf, "%s,%s,%f\n", prod[i].marca, prod[i].codigo, prod[i].precio);
rewind(pf);
for(i = 0; i < n; i++)
	fscanf(pf,"%s%s%f\n", &produ[i].marca, &produ[i].codigo, &produ[i].precio);
for(i = 0; i < n; i++)
	cout << produ[i].marca << ' ' << produ[i].codigo << ' ' << produ[i].precio << '\n';
rewind(pf);
while((s = fgetc(pf)) != ' ')
		cout << s;
cout << pf << '\n';
rewind(pf);
cout << pf << '\n';
fclose(pf);
}
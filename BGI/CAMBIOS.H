void cambios(void)
{
int i, cant, datos;
int parar;
struct articulo prod[PLUS];
FILE *pf;

closegraph();
clrscr();
parar = 0;
i = 0;
datos = 0;
while(salir)
	  {
	  while (bioskey(1) == 0);
	  c = bioskey(0);

	  switch(c)
			  {
			  case ESC :
							 salir = 0; break;
			  case F1 :
							 cout << "\n-Ingrese marca del producto: "
							 fflush(stdin);
							 gets(prod[i].marca);
							 datos ++;
							 break;

			  case F2 :
							 cout << "\nIngrese el codigo: ";
							 fflush(stdin);
							 gets(prod[i].codigo);

							 break;

			  case F3 :
							 cout << "\ndescripcion?\n";
							 fflush(stdin);
							 gets(prod[i].descr);
							 break;

			  case F4 :
							 cout << "\nprecio al publico?\n";
							 fflush(stdin);
							 cin >> prod[i].precio;
							 break;

			  case F5 :
							 cout << "precio x mayor?\n";
							 fflush(stdin);
							 cin >> prod[i].preciom;
							 break;

			  case F6 :
							 cout << "mayorista?\n";
							 fflush(stdin);
							 gets(prod[i].nom_may);
							 break;
			  }
	  if(datos == 6)
		 {
		 cant = ++i;
		 datos = 0;
		 }

pf = fopen("libro1.txt", "a");
for(i = 0; i < (cant+1); i++)
	fprintf(pf, "%s\t%s\t%s\t%f\t%f\t%d\t%d\t%d\t%s\n", prod[i].marca, prod[i].codigo, prod[i].descr, prod[i].precio, prod[i].preciom, prod[i].tc.tm_mday, prod[i].tc.tm_mon, prod[i].tc.tm_year, prod[i].nom_may);
rewind(pf);
fclose(pf);
Initialize();
void llen_plantilla(int st, struct articulo *lst, int n)
{
int secx, secy, x, y, i, posy;


secx = getmaxx()/20;
secy = getmaxy()/20;

borrar_blk(0+1, (4*secy)+1, (4*secx)-1, getmaxy()-1);
borrar_blk((4*secx)+1, (4*secy)+1, (7*secx)-1, getmaxy()-1);
borrar_blk((7*secx)+1, (4*secy)+1, (13*secx)-1, getmaxy()-1);
borrar_blk((13*secx)+1, (4*secy)+1, (15*secx)-1, getmaxy()-1);
borrar_blk((15*secx)+1, (4*secy)+1, (17*secx)-1, getmaxy()-1);
borrar_blk((17*secx)+1, (4*secy)+1, getmaxx()-1, getmaxy()-1);

posy = 5*secy;
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
	posy = posy + (secy)/2;
	}

}

#include <math.h>
double f(double x, double y){
	return exp(x)*(x*cos(y)-y*sin(y));
}
#include "./pde_2.c"


double **U, **Unew;//, **Ub;
int main()
{
	double sta = 1.0/128.0;
	int sa = 129, i;//, sb = 129/*,j*/;
	FILE *outs;
	init(&U,sa,sa);
	init(&Unew,sa,sa);
	//init(&Ub,sb,sb);
	
	init_cond(&U, sa, sa, sta,sta);
	
	memcpy(&Unew,&U,sizeof(Unew));
	for(i = 0; i<4000; i++)
	{
		iteration(&U,&Unew, sa,sa, sta,sta);
		memcpy(&U,&Unew,sizeof(Unew));
	}
	
	outs = fopen("../output/out1.dat", "w");
	output_line(U, sa,sa, sta,sta, outs);
	fclose(outs);
	
	outs = fopen("../output/field.dat", "w");
	output_field(U, sa,sa, sta,sta, outs);
	fclose(outs);
	
	system("./Plot_field");
	
	return 0;
}



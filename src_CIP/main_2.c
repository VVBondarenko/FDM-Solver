#include <math.h>
double f(double x, double y){
	return exp(x)*(x*cos(y)-y*sin(y));
}
#include "./pde_2.c"


double **U, **Unew;//, **Ub;
int main()
{
	double sta = 1.0/128.;//, stb = 1./512.;
	int sa = 129, i;//, sb = 513/*,j*/;
	FILE *outs;
	init(&U,sa,sa);
	init(&Unew,sa,sa);
	//init(&Ub,sb,sb);
	
	init_cond(&U, sa, sa, sta,sta);
	
	memcpy(&Unew,&U,sizeof(Unew));
	for(i = 0; i<7000; i++)
	{
		iteration(&U,&Unew, sa,sa, sta,sta);
		memcpy(&U,&Unew,sizeof(Unew));
	}
	printf("step a done\n");

	outs = fopen("../output/out3.dat", "w");
	output_grid(U, sa,sa, sta,sta, outs);
	fclose(outs);
	printf("pot. output \n");


	outs = fopen("../output/press.dat", "w");
	output_field_color(U, sa,sa, sta,sta, outs);
	fclose(outs);
	printf("pressure output\n");

	
	outs = fopen("../output/field.dat", "w");
	output_field(U, sa,sa, sta,sta, outs);
	fclose(outs);
	printf("field output\n");
	
	
	system("./lines_of_level");
	system("./Plot_field");
	system("./Plot_col");	
	system("./Plot");
	
	return 0;
}



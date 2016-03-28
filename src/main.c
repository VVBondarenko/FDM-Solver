#include <math.h>
double f(double x, double y){
	return exp(x)*(x*cos(y)-y*sin(y));
}
#include "./pde.c"


double **U, **Unew, **Ub;
int main()
{
	double /* **U, **Unew,err=0.0,*/ sta = 1.0/8.0, stb=1.0/128.0;
	int sa = 9, sb = 129, i/*,j*/;
	FILE *outs;
	init(&U,sa,sa);
	init(&Unew,sa,sa);
	init(&Ub,sb,sb);
	
	init_cond(&U, sa, sa, sta,sta);
	
	memcpy(&Unew,&U,sizeof(Unew));
	for(i = 0; i<1000 /*&& (err>0.001 || err == 0.0)*/; i++)
	{
		//err = 0.0;
		iteration(&U,&Unew, sa,sa, sta,sta);
		memcpy(&U,&Unew,sizeof(Unew));
	}
	
	outs = fopen("../output/out1.dat", "w");
	output_line(U, sa,sa, sta,sta, outs);
	fclose(outs);
	
	//init_cond(&Ub, sb, sb, stb,stb);
	interpol(&Ub, sb, sb, stb, stb, 	&U, sa-1, sa-1, sta, sta);
	init_cond(&Ub, sb, sb, stb, stb);
	
	outs = fopen("../output/out2.dat", "w");
	output_line(Ub, sb,sb, stb,stb, outs);
	fclose(outs);
	
	freeArr(&Unew, sa);
	//output_line(U, sa,sa, sta,sta);
	//output_line(Ub, sb,sb, stb,stb);
	//freeArr(&U, sa);
	init(&Unew,sb,sb);
	init_cond(&Unew, sb, sb, stb, stb);
	memcpy(&Unew,&Ub,sizeof(Ub));
	//err = 0.0;
	for(i = 0; i<1000 /*&& (err>0.0005 || err == 0.0)*/ ; i++)
	{
		iteration(&Ub,&Unew, sb,sb, stb,stb);
		memcpy(&Ub,&Unew,sizeof(Unew));
	}
	
	outs = fopen("../output/out3.dat", "w");
	output_line(Ub, sb,sb, stb,stb, outs);
	fclose(outs);
	
	
	outs = fopen("../output/out3_mstk.dat", "w");
	opt_mstk_line(Ub, sb,sb, stb,stb, outs);
	fclose(outs);
	//output_line(U, size,size, step,step);

	//printf("%f %f %f\n", step*5, step*5, err);
	return 0;
}



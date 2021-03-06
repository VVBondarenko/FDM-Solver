#include <math.h>
double f(double x, double y) {
    return exp(x)*(x*cos(y)-y*sin(y));
}
#include "./pde.c"


//double **U, **Unew, **Ux, **Uy;//, **Ub;
double err_control(double **A, double **B, int nx, int ny)
{
    int i, j;
    double temp;
    for (i = 1; i < nx-1; i++)
    {
        for (j = 1; j < ny-1; j++)
        {
            temp = fmax(temp, fabs(A[i][j]-B[i][j]));
        }
    }
    return temp;
}

int main()
{
    double  **U, **Unew,**Ux, **Uy,err=0.0, sta = 1.0/64.0;//, stb=1.0/128.0;
    int sa = 65, /*sb = 129,*/ i,j,k;
    FILE *outs;
    init(&U,sa,sa);
    init(&Unew,sa,sa);
    init(&Ux,sa,sa);
    init(&Uy,sa,sa);

    init_cond(&U, sa, sa, sta,sta);

    memcpy(&Unew,&U,sizeof(Unew));
    for(i = 0; i<1000 /*&& (err>0.0002 || err == 0.)*/; i++)
    {
        iteration(&U,&Unew, sa,sa, sta,sta);
        memcpy(&U,&Unew,sizeof(Unew));
    }

    outs = fopen("../output/exact.dat", "w");
	for (i = 0; i < sa; i++)
	{
		for (j = 0; j < sa; j++)
		{
			fprintf(outs, "%f %f %f\n", (double)i*sta, (double)j*sta, f((double)i*sta, (double)j*sta));
		}
		
	}
    fclose(outs);

    outs = fopen("../output/out1.dat", "w");
    output_line(U, sa,sa, sta,sta, outs);
    fclose(outs);
    outs = fopen("../output/out1_mstk.dat", "w");
    opt_mstk_line(U, sa,sa, sta,sta, outs);
    fclose(outs);

    if(1)
    {
        //calculatin' partial du/dx and du/dy
        for (i = 1; i < sa-1; i++)
        {
            for (j = 1; j < sa-1; j++)
            {
                Ux[i][j] = (U[i+1][j]-U[i-1][j])/sta*0.5;
                Uy[i][j] = (U[i][j+1]-U[i][j-1])/sta*0.5;
            }
        }

        for (i = 0; i < sa; i++)
        {
            //Ux[0][i]	= (U[1][i]-U[0][i])/sta;
            Uy[0][i]	= (f(0.,(double)i*sta+0.0005)-f(0.,(double)i*sta-0.000005))/0.00001;//(U[i][1]-U[i][0])/sta;
            Ux[0][i]	= (f(0.+0.0005,(double)i*sta)-f(0.-0.000005,(double)i*sta))/0.00001;//(U[i][1]-U[i][0])/sta;

            //Ux[sa-1][i]	= (U[sa-1][j]-U[sa-2][j])/sta;
            Uy[sa-1][i]	= (f(1.,(double)i*sta+0.0005)-f(1.,(double)i*sta-0.000005))/0.00001;
            Ux[sa-1][i]	= (f(1.+0.0005,(double)i*sta)-f(1.-0.000005,(double)i*sta))/0.00001;

            Ux[i][0]	= (f((double)i*sta+0.0005,0.)-f((double)i*sta-0.000005,0.))/0.00001;
            Uy[i][0]	= (f((double)i*sta,0.+0.0005)-f((double)i*sta,0.-0.000005))/0.00001;
            //Uy[i][0]	= (U[i][1]-U[i][0])/sta;

            Ux[i][sa-1]	= (f((double)i*sta+0.0005,1.)-f((double)i*sta-0.000005,1.))/0.00001;
            Uy[i][sa-1]	= (f((double)i*sta,1.+0.0005)-f((double)i*sta,1.-0.000005))/0.00001;
            //Uy[i][sa-1]	= (U[i][sa-1]-U[i][sa-2])/sta;
        }


        for (k = 0; k < 1000; k++)
        {
            for(i=1; i<sa-1; i++)
            {
                for(j=1; j<sa-1; j++)
                {
                    Unew[i][j] = 0.25*(U[i+1][j]+U[i-1][j]+U[i][j+1]+U[i][j-1])
                                 +0.125*sta*(Ux[i+1][j]-Ux[i-1][j]+Uy[i][j+1]-Uy[i][j-1]);
                }
            }

            for (i = 1; i < sa-1; i++)
            {
                for (j = 1; j < sa-1; j++)
                {
                    Ux[i][j] = (U[i+1][j]-U[i-1][j])/sta*0.5;
                    Uy[i][j] = (U[i][j+1]-U[i][j-1])/sta*0.5;
                    //Ux[i][j] = (8.*(U[i+1][j]-U[i-1][j])+U[i-2][j]-U[i+2][j])/sta/12.;
                    //Uy[i][j] = (8.*(U[i][j+1]-U[i][j-1])+U[i][j-2]-U[i][j+2])/sta/12.;
                }
            }
            //for (i = 0; i < sa; i++)
            //{
            //Ux[0][i]	= (U[1][i]-U[0][i])/sta;
            //Ux[sa-1][i]	= (U[sa-1][j]-U[sa-2][j])/sta;
            //Uy[i][0]	= (U[i][1]-U[i][0])/sta;
            //Uy[i][sa-1]	= (U[i][sa-1]-U[i][sa-2])/sta;
            //}
            memcpy(&U,&Unew,sizeof(Unew));

        }

        outs = fopen("../output/out2.dat", "w");
        output_line(U, sa,sa, sta,sta, outs);
        fclose(outs);
        outs = fopen("../output/out2_mstk.dat", "w");
        opt_mstk_line(U, sa,sa, sta,sta, outs);
        fclose(outs);
    }
    ////init_cond(&Ub, sb, sb, stb,stb);
    //interpol(&Ub, sb, sb, stb, stb, 	&U, sa-1, sa-1, sta, sta);
    //init_cond(&Ub, sb, sb, stb, stb);
    //outs = fopen("../output/out1_mstk.dat", "w");
    //opt_mstk_line(U, sa,sa, sta,sta, outs);
    //fclose(outs);

    //outs = fopen("../output/out2.dat", "w");
    //output_line(Ub, sb,sb, stb,stb, outs);
    //fclose(outs);

    //freeArr(&Unew, sa);

    //init(&Unew,sb,sb);
    //init_cond(&Unew, sb, sb, stb, stb);
    //memcpy(&Unew,&Ub,sizeof(Ub));
    ////err = 0.0;
    //for(i = 0; i<5000 /*&& (err>0.0005 || err == 0.0)*/ ; i++)
    //{
    //iteration(&Ub,&Unew, sb,sb, stb,stb);
    //memcpy(&Ub,&Unew,sizeof(Unew));
    //}

    //outs = fopen("../output/out3.dat", "w");
    //output_line(Ub, sb,sb, stb,stb, outs);
    //fclose(outs);


    //outs = fopen("../output/out3_mstk.dat", "w");
    //opt_mstk_line(Ub, sb,sb, stb,stb, outs);
    //fclose(outs);
    i = system("../bin/plot_1.py & ../bin/plot_mstk_1.py & ../bin/plot_exact.py & ../bin/plot_mstk_2.py &");
    return 0;
}



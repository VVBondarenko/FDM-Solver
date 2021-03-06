#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


void init	(double ***array, int sizeX, int sizeY)
{
	int i,j;
	*array = malloc(sizeX * sizeof(double*));
	for(i=0; i<sizeX; i++)
		(*array)[i] = malloc(sizeY * sizeof(double));
		
	for(i = 0; i < sizeX; i++)
		for(j = 0; j < sizeY; j++)
			(*array)[i][j] = 0;
}

void freeArr	(double ***array, int sizeX)
{
	int i;
	for(i=0; i<sizeX; i++)
		free((void *)(*array)[i]);
	free((void *)(*array));
}

/*void init_cond	(double ***array, int sizeX, int sizeY, double xh, double yh)
{
	int i;
	for(i = 0; i < sizeX; i++)
	{
		(*array)[i][0] = exp(xh*i);
		(*array)[i][sizeY-1] = exp(xh*i + yh*(sizeY-1));
	}

	for(i = 0; i < sizeY; i++)
	{
		(*array)[0][i] = exp(yh*i);
		(*array)[sizeX-1][i] = exp(xh*i + xh*(sizeX-1));
	}
}*/

void init_cond	(double ***array, int sizeX, int sizeY, double xh, double yh)
{
	int i;
	for(i = 0; i < sizeX; i++)
	{
		(*array)[i][0] = f(xh*i,0.0);
		(*array)[i][sizeY-1] = f(xh*i, yh*(sizeY-1));
	}

	for(i = 0; i < sizeY; i++)
	{
		(*array)[0][i] = f(0.0,yh*i);
		(*array)[sizeX-1][i] = f(xh*(sizeX-1), yh*i);
	}
}

void iteration	(double ***array, double ***Narray, int sizeX, int sizeY, double xh, double yh/*, double *err*/)
{
	int i,j;
	double kx = xh*xh, ky = yh*yh/*, err = 0.*/;
	
	for(i=1; i<sizeX-1; i++)
		for(j=1; j<sizeY-1; j++)
		{
			(*Narray)[i][j] = 0.5 / (kx+ky) * (ky*( (*array)[i+1][j]+ (*array)[i-1][j])+kx*( (*array)[i][j+1]+ (*array)[i][j-1]) /*- kx*ky*2.0*exp(xh*i+yh*j)*/);
			//err =  fabs((*array)[i][j]-(*Narray)[i][j]);
		}
	//return err;
}






//interpolating from values in B to A
void interpol	(double ***arr_a, int saX, int saY, double xha, double yha, 	double ***arr_b, int sbX, int sbY, double xhb, double yhb)
{
	int i,j,m,n, kx=saX/sbX, ky=saY/sbY;
	double xrat = xha/xhb, yrat = yha/yhb;
	for(i = 0; i < sbX; i++){
	 for(j = 0; j < sbY; j++){
	  for(m = 0; m < kx; m++){
	   for(n = 0; n < ky; n++){
	     if(m >= n )(*arr_a)[i*kx+m][j*ky+n] = xrat *((*arr_b)[i+1][j] - (*arr_b)[i][j])* m + yrat *((*arr_b)[i+1][j+1]-(*arr_b)[i+1][j]) * n + (*arr_b)[i][j];
	     else 		(*arr_a)[i*kx+m][j*ky+n] = xrat *((*arr_b)[i+1][j+1] - (*arr_b)[i][j+1])* m + yrat *((*arr_b)[i][j+1]-(*arr_b)[i][j]) * n + (*arr_b)[i][j];
	   }
	  }
	 }
	}
	//init_cond(arr_a, saX, saY, xha, yha);
}

void output_grid(double **array, int sizeX, int sizeY, double xh, double yh)
{
	int i, j;
	for(i=0;i<sizeX;i++)
	{
		for(j=0;j<sizeY;j++)
		{
			printf("%f %f %f \n", xh*i, yh*j, array[i][j]);
		}
	}
}


void output_line(double **array, int sizeX, int sizeY, double xh, double yh, FILE *opt)
{
	int m=0,n=0, i;
	
	
	for(i=0;i<sizeX*sizeY; i++)
	{
		fprintf(opt,"%f %f %f\n", xh*m, yh*n, array[m][n]);
		fflush(stdout);
		if(m<sizeX && n%2 == 0)
			m++;
		if(m==sizeX)
			n++;
		if(m==0)
			n++;
		if(m>0 && n%2 == 1)
			m--;
	}
}
void opt_mstk_line(double **array, int sizeX, int sizeY, double xh, double yh, FILE *opt)
{
	int m=0,n=0, i;
	
	
	for(i=0;i<sizeX*sizeY; i++)
	{
		fprintf(opt,"%f %f %f\n", xh*m, yh*n, exp(xh*m)*(xh*m*cos(yh*n)-yh*n*sin(yh*n))-array[m][n]);
		fflush(stdout);
		if(m<sizeX && n%2 == 0)
			m++;
		if(m==sizeX)
			n++;
		if(m==0)
			n++;
		if(m>0 && n%2 == 1)
			m--;
	}
	
}


































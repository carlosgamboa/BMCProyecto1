/*
 * Algoritmo Genera las secuencia cromosomica con la distancia de los genes.
 * Si van a probarlo con una matrix mas grande solo se debe modificar las variables y constantes. 
 * En este caso SIZE,nodos no se necesita modificar otros. Recuerden llenar la matriz con los valores de prueba
 * (-1 significa espacio Nulo)
 * @garias
 * */
#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#include<stdio.h>
#define SIZE 255
#define GHOST 999

int last,first;
double maximo;
//int nodos =4;

/*esta matrix se deja asi de inicio
 * Esta matriz es solo una forma de almacenar la cadena de path
 * El row indica la cantidad de path. Si solo hay un path se escribe
 * Horizontal en el row 0 y asi ..
 * */
int path[SIZE][SIZE];
double cost[SIZE][SIZE];
/*esta es para probar los pesos*/	   
double matrix[SIZE][SIZE];

int init_allMatrix();
int min(int,int);
void printMatrix_double(double m[SIZE][SIZE],int nodos);
void printMatrix_int(int p[SIZE][SIZE],int nodos);
int findFirst(double m[SIZE][SIZE],int nodos);
int findNext(double m[SIZE][SIZE], int row,int nodos);
double findMinRow(double m[SIZE][SIZE],int r,int nodos);
void remainRow(double m[SIZE][SIZE],int r,int nodos,double min);
void makeZeroRows(double m[SIZE][SIZE],int nodos);
double findMinRow(double m[SIZE][SIZE],int r,int nodos);
void makeZeroRows(double m[SIZE][SIZE],int nodos);
void makePath(double values[SIZE][SIZE],double c[SIZE][SIZE],int p[SIZE][SIZE],double m[SIZE][SIZE],int row_path,int T_step,int nodos);
void deleteFistC(double m[SIZE][SIZE], int c,int nodos);
int havePaths(double m[SIZE][SIZE],int nodos);
void getMutltipleMaps(double values[SIZE][SIZE],double c[SIZE][SIZE],double m[SIZE][SIZE],int nodos);
void fill_Matrix(double values[SIZE][SIZE],int nodos);
void create_all_maps(double values[SIZE][SIZE],int nodos);
int hasMore(double values[SIZE][SIZE],double m[SIZE][SIZE],int nodos);


int init_allMatrix()
{
 int i,j;
 for(i=0;i<SIZE;i++)
  for(j=0;j<SIZE;j++)
  {
	  path[i][j]=-1;
	  matrix[i][j]=-1;
	  cost[i][j]=-1;
  }
 return 0;
}

void fill_Matrix(double values[SIZE][SIZE],int nodos)
{
  int c,r;
 for(r=0;r<SIZE;r++)
 {
   for(c=0;c<SIZE-1;c++)
   {
	   matrix[r][c]=values[r][c];
   }
 }
}

/*
 * Busca el peso mayor entre dos genes
 * Inicializa 
 * */
int findFirst(double m[SIZE][SIZE],int nodos)
{
	int c,r;
	
	for(r=0;r<nodos;r++)
	{
		for(c=0;c<nodos;c++)
		{
			if(m[r][c]!=-1)
			{
				if(m[r][c]>maximo)
				{
					maximo=m[r][c];
					first=r;
					last=c;
				}
			}
		}
	}
	return 1;	
}


/*
 * Busca el nodo mas sercano al gen
 * Elimina la columna donde se encuentra para que no se vuelva a evaluar.
 * */
int findNext(double m[SIZE][SIZE], int row,int nodos)
{
	int c,i,next;
	next = -1;
	for(c=0;c<nodos;c++)
	{
		if (m[row][c]==0)
		{	
			next = c;
			for(i=0; i<nodos; i++)
				m[i][next]=-1;//elimino la columna
			
			return next;
		}
	}
	
	return -1;
}

/*
 * Busca el peso menos en el row siempre y cuando no sea -1
 * Si todo el row esta en null porque no existe un peso 
 * devuelve un valor GHOST indica final de un Path 
 * */
double findMinRow(double m[SIZE][SIZE],int r,int nodos)
{
	int c;
	double min=GHOST;
	for(c=0;c<nodos;c++)
	{
		if (m[r][c]!=-1)
		{	
			if (m[r][c] < min)// -1 es un espacio Null
				min=m[r][c];
		}
	}
	return min;
}

/*
 * Resta el peso menor al row para crear valor Zero.
 * si es un espacio con valor -1 (nulo) no se calcula
 * */
void remainRow(double m[SIZE][SIZE],int r,int nodos,double min)
{
	int c;
	for(c=0;c<nodos;c++)
	{	
		if (m[r][c]!=-1)
			m[r][c] = m[r][c]-min;
	} 
	
}

/*
 * Es para llenar una columna con nulos -1
 * De esta forma la desavilitamos ese nodo para no usarlo en el calulo
 * de un nuevo Path
 * */
void makeZeroRows(double m[SIZE][SIZE],int nodos)
{ 
	int row,c;
	double min_r;
	for(row=0;row<nodos;row++)
	{
		
		 //1. Busca el peso menor 
		 //2. Si no existe ni un solo peso devuelve GHOST
		min_r = findMinRow(m,row,nodos);
		//3. Si Min_r es 0 marca el proximo nodo en la ruta
		if (min_r>0)
		{
			//4. Si el min_r>0 tenemos que crear un ZERO
			remainRow(m,row,nodos,min_r);
		} 
	}
		
}
/*
 * Arma la ruta. row_path es el gen actual que esta buscando su proximo gen
 * mT es el numero de la interaccion de la matriz o del paso, tambien
 * uno de los indices de la matrix path para ir escribiendo el path
 * */
void makePath(double values[SIZE][SIZE],double c[SIZE][SIZE],int p[SIZE][SIZE],double m[SIZE][SIZE],int row_path,int T_step,int nodos)
{
	int next;
	double min_r;
	int mT=0;
	p[T_step][mT]=row_path;
	printf("inicia Path [%d][%d] =%d\n",T_step,mT,row_path);
	while(1==1)
	{	
		
		printf("\n");
		printf("Valores: de [%d ---> %d]\n",first,last);
		printMatrix_double(m,nodos);
			
		//busca el proximo
		next=findNext(m,row_path,nodos);
		
		if (next==-1)
		{
			//if (next==-1) no encontro el proximo
		    //busco el menor de la fila y lo resto
			min_r = findMinRow(m,row_path,nodos);
			if (min_r!=GHOST)
			{	
				remainRow(m,row_path,nodos,min_r);
				makeZeroRows(m,nodos);
			}	
			else
				break;
		}
		else
		{
			printf("next %d \n",next);
			
			c[T_step][mT]=values[row_path][next];
			mT++;
			p[T_step][mT]=next;
			
			row_path = next;
			
			printf("\n");
			printf("--------------MATRIX DE PATHS------------\n");
			printMatrix_int(p,nodos);
		
		}
	}	
}


/*
 * Se Utiliza desde el comienzo para eliminar de la matriz el nodo first 
 * para no volver a pasar en el path
 * */
void deleteFistC(double m[SIZE][SIZE], int c,int nodos)
{
	int i;
	 for(i=0; i<nodos; i++)
		m[i][c]=-1;//elimino la columna init
	
}

int hasMore(double values[SIZE][SIZE],double m[SIZE][SIZE],int nodos)
{
	int r,c;
	int has_more=0;
	for (r=0;r<nodos;r++)
	{
		for(c=0;c<nodos;c++)
		{
			if (m[r][c]!=-1)
			{
				m[r][c]=values[r][c];
				has_more=1;
			}
		}
	}
	
	return has_more;
}
 

void getMutltipleMaps(double values[SIZE][SIZE],double c[SIZE][SIZE],double m[SIZE][SIZE],int nodos)
{
	int T_step = 0;
	int is_check=0;
	
	while (T_step<nodos)
	{
		is_check=hasMore(values,m,nodos);
		if (is_check==1)
			maximo=-1;
		else
			break;
			
		findFirst(m,nodos);
		printf("1.[%d ---> %d]---=%f\n",first,last,maximo);
		deleteFistC(m,first,nodos);
		
		printf("2.Borra la columna\n");
		printMatrix_double(m,nodos);
		
		
		printf("3.make zeros\n");
		makeZeroRows(m,nodos);
		
		printf("4.path\n");
		makePath(values,c,path,m,first,T_step,nodos);
		T_step++;
	}
	
}

//cuento si me faltan genes de meter en el path. Si no hay libres se termina
void printMatrix_double(double m[SIZE][SIZE],int nodos)
{
	int i,j;
	 printf("MATRIX:\n");
	for(i=0;i<nodos;i++)
	{
	  printf("%d : ",i);	
      for(j=0;j<nodos;j++)
      {
		printf("%f     ", m[i][j] );
	  }	 
	  printf("\n");
	}  
	 printf("\n");
}

void printMatrix_int(int p[SIZE][SIZE],int nodos)
{
	int i,j;
	 printf("\n");
	for(i=0;i<nodos;i++)
	{
	  printf("%d : ",i);	
      for(j=0;j<nodos;j++)
      {
		printf("%d     ", p[i][j] );
	  }	 
	  printf("\n");
	}  
	 printf("\n");
}

void create_all_maps(double values[SIZE][SIZE],int nodos)
{
	init_allMatrix();
	fill_Matrix(values,nodos);
	
	//print
	printMatrix_double(matrix,nodos);
	
	getMutltipleMaps(values,cost,matrix,nodos);
	
   printf("\n");
   printf("--------------MATRIX DE PATHS------------\n");
   printMatrix_int(path,nodos);
}


#endif


 


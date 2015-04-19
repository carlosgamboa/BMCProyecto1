/*
 * Algoritmo Genera las secuencia cromosomica con la distancia de los genes.
 * Si van a probarlo con una matrix mas grande solo se debe modificar las variables y constantes. 
 * En este caso SIZE,numGenes no se necesita modificar otros. Recuerden llenar la matriz con los valores de prueba
 * (-1 significa espacio Nulo)
 * @garias
 * */
#include<stdio.h>
#define SIZE 8
#define GHOST 999

int last,first;
int numGenes = 8;

/*esta matrix se deja asi de inicio
 * Esta matriz es solo una forma de almacenar la cadena de path
 * El row indica la cantidad de path. Si solo hay un path se escribe
 * Horizontal en el row 0 y asi ..
 * */
int path[SIZE][SIZE]= { 
	   {-1,-1,-1,-1,-1,-1,-1,-1},
	   {-1,-1,-1,-1,-1,-1,-1,-1}, 
	   {-1,-1,-1,-1,-1,-1,-1,-1}, 
	   {1,-1,-1,-1,-1,-1,-1,-1},
	   {-1,-1,-1,-1,-1,-1,-1,-1},
	   {-1,-1,-1,-1,-1,-1,-1,-1},
	   {-1,-1,-1,-1,-1,-1,-1,-1},
	   {-1,-1,-1,-1,-1,-1,-1,-1},
	   };
	   
/*esta es para probar los pesos*/	   
int matrix[SIZE][SIZE]= { 
	   {-1 ,15 ,2  ,13 ,5  ,5  ,3  ,23},
	   {-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1}, 
	   {2  ,13 ,-1 ,11 ,3  ,3  ,1  ,18}, 
	   {13 ,2  ,11 ,-1 ,14 ,12 ,10 ,10},
	   {-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1},
	   {5  ,10 ,3  ,8  ,6  ,-1 ,4  ,18},
	   {3  ,14 ,1  ,12 ,5  ,5  ,-1 ,22},
	   {-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1},
	   };

int init_allMatrix();
int min(int,int);
void printMatrix(int m[SIZE][SIZE]);
int findFirst(int m[SIZE][SIZE]);
int findNext(int m[SIZE][SIZE], int row,int nodos);
int findMinRow(int m[SIZE][SIZE],int r,int nodos);
void remainRow(int m[SIZE][SIZE],int r,int nodos,int min);
void makeZeroRows(int m[SIZE][SIZE],int nodos);
int findMinRow(int m[SIZE][SIZE],int r,int nodos);
void makeZeroRows(int m[SIZE][SIZE],int nodos);
void makePath(int p[SIZE][SIZE],int m[SIZE][SIZE],int row_path, int mT,int nodos);
void deleteFistC(int m[SIZE][SIZE], int c,int nodos);
int havePaths(int m[SIZE][SIZE],int nodos);
void getMutltipleMaps(int m[SIZE][SIZE],int nodos);

int init_allMatrix()
{
 int i,j;
 for(i=0;i<SIZE;i++)
  for(j=0;j<SIZE;j++)
  {
	  path[i][j]=-1;
	  matrix[i][j]=-1;
  }
 return 0;
}

/*
 * Busca el peso mayor entre dos genes
 * Inicializa 
 * */
int findFirst(int m[SIZE][SIZE])
{
	int c,r;
	int first =0;
	int maximo=m[0][0];
	
	for(r=0;r<numGenes;r++)
		for(c=0;c<numGenes;c++)
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
	return first;	
}

/*
 * Busca el nodo mas sercano al gen
 * Elimina la columna donde se encuentra para que no se vuelva a evaluar.
 * */
int findNext(int m[SIZE][SIZE], int row,int nodos)
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
int findMinRow(int m[SIZE][SIZE],int r,int nodos)
{
	int c,min;
	min=GHOST;
	for(c=0;c<nodos;c++)
	{
		if ((m[r][c]!=-1)&&(m[r][c]<min))// -1 es un espacio Null
			min=m[r][c];
	}
	return min;
}

/*
 * Resta el peso menor al row para crear valor Zero.
 * si es un espacio con valor -1 (nulo) no se calcula
 * */
void remainRow(int m[SIZE][SIZE],int r,int nodos,int min)
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
void makeZeroRows(int m[SIZE][SIZE],int nodos)
{ 
	int row,c,min_r;
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
void makePath(int p[SIZE][SIZE],int m[SIZE][SIZE],int row_path,int T_step,int nodos)
{
	int next,min_r;
	int mT=0;
	p[T_step][mT]=row_path;
	while(1==1)
	{
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
				makeZeroRows(matrix,numGenes);
			}	
			else
				break;
		}
		else
		{
			printf("next %d \n",next);
			mT++;
			p[T_step][mT]=next;
			row_path = next;
		}
	}	
}

/*
 * Solo hay paths cuando hay un valor en la matrix
 * Cuando termina los mapas la matrix resultante solo tiene valores nulos
 * */
int havePaths(int m[SIZE][SIZE],int nodos)
{
	int r,c;
	for(r=0;r<nodos;r++)
	{
		for(c=0;c<nodos;c++)
		{
			if (m[r][c]!=-1)
				return 1;
		}
	}
	return 0;
}

/*
 * Se Utiliza desde el comienzo para eliminar de la matriz el nodo first 
 * para no volver a pasar en el path
 * */
void deleteFistC(int m[SIZE][SIZE], int c,int nodos)
{
	int i;
	 for(i=0; i<nodos; i++)
		m[i][c]=-1;//elimino la columna init
	
}
 

void getMutltipleMaps(int m[SIZE][SIZE],int nodos)
{
	int T_step = 0;
	
	while ((havePaths)&&(T_step<5))
	{
		findFirst(matrix);
		deleteFistC(matrix,first,numGenes);
		
		makeZeroRows(matrix,numGenes);
		printf("First: %d ---> last: %d\n",first,last);
		
		makePath(path,matrix,first,T_step,numGenes);
		T_step++;
	}
	
}

//cuento si me faltan genes de meter en el path. Si no hay libres se termina

void printMatrix(int m[SIZE][SIZE])
{
	int i,j;
	printf("----------------------------------------\n");
	printf("    0      1       2       3       4\n");
	for(i=0;i<SIZE;i++)
	{
	  printf("%d  ",i);	
      for(j=0;j<SIZE;j++)
      {
		printf("%d     ", m[i][j] );
	  }	 
	  printf("\n");
	}  
	printf("----------------------------------------\n");
}


int main ()
{
	
   //init_allMatrix();
    
	getMutltipleMaps(matrix,numGenes);
    
	printf("\n");
	printf("--------------MATRIX DE PATHS------------\n");
	printMatrix(path);
    
   return 0;
}
 


#include<stdio.h>
int path[100][100];
int matrix[100][100];

int init_allMatrix();
int min(int,int);

int init_allMatrix()
{
 int i,j;
 for(i=0;i<=100;i++)
 {
  for(j=0;j<=100;j++)
  {
	  path[i][j]=999;
	  matrix[i][j]=999;
  }
 }
 return 0;
}

void floyd(int path[100][100],int matrix[100][100],int nodes)
{
	int i,j,k,value;
	
	for(k=1;k<=nodes;k++)
	{
		for(i=1;i<=nodes;i++)
		{
			for(j=1;j<=nodes;j++)
			{
				if(i==j)
				  matrix[i][j]=-1;
				else
				{
				  value = matrix[i][j];
				  matrix[i][j]=min(matrix[i][j],matrix[i][k]+matrix[k][j]);
				  if (value != matrix[i][j])
					  path[i][j]=k;
				}
			}	  
		}	  
	}
}

int min(int a,int b)
{
 if(a<b)
  return(a);
 else
  return(b);
}


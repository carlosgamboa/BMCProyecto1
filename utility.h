#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

char* filename = "genes.txt";
/*-------------------STRING CHAT FUNCTIONS--------------------------------------------*/
/* concat two strings and return the char* result concated*/
char* concat(char *s1, char *s2)
{
	char *result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the zero-terminator
	//in real code you would check for errors in malloc here
	strcpy(result, s1);
	strcat(result, s2);
	return result;
}

/*convert a number into string.*/
void tostring(char str[], int num)
{
	int i, rem, len = 0, n;
	n = num;
	
	while (n != 0)
	{
		len++;
		n /= 10;
	}
	
	for (i = 0; i < len; i++)
	{
		rem = num % 10;
		num = num / 10;
		str[len - (i + 1)] = rem + '0';
	}
	if (len == 0 && num == 0)
	{
		str[0] = '0';
		str[1] = '\0';
	}
	else
	{
		str[len] = '\0';
	}
}
/*-------------------NAME GENES FUNCTIONS---------------------------------------------*/

/*Create all the Gene name G1 G2 G3 ... Gnumber*/
char* getGenName(int number)
{
	int size = 2;
	char str_num[10];
	char *str_name = (char *)malloc(sizeof(char)*size); 
	*(str_name+0) = 'G'; 
	*(str_name+1) = '\0';  
	tostring(str_num, number);
	char* gen_name = concat(str_name, str_num);
	return gen_name;  
}

/*-------------------FILE FUNCTIONS---------------------------------------------*/

/*Open a new new file and a file with same name already exists, 
 * the existing file’s contents are overwritte*/
int fopen_w(char * filename)
{
	FILE *fp = fopen(filename, "w");
	if (fp == NULL)
	{
		puts("error open a new file");
		exit(0);
	}
	else
	{
		//fputs("GeeksforGeeks", fp);
		puts("new file created");
		fclose(fp);
	}
	return 0;
}

/* Create the possibles pars into a file with space of porcents*/
int fwrite_pares(char * filename, int num)
{
	int i;
	int j;
	char *str_gen;
	char *str_gen2;
	char *str_par;
	
	FILE *fp = fopen(filename, "a");
	if (fp == NULL)
	{
		puts("error open a new file");
		exit(0);
	}
	else
	{
		fprintf(fp, "%d\n", num);
		
		for (i=0; i<num; i++)
		{
			str_gen = getGenName(i);
			for (j=0; j<num; j++)
			{
				if (j!=i)
				{
					str_gen2 = getGenName(j);
					str_par = concat(str_gen,"-");
					str_par = concat(str_par,str_gen2);
					fprintf(fp, "%s=0.0\n", str_par);
				}
			}
			printf("Gen name: %s\n", str_gen);
		}
		
		fclose(fp);
	}
	
    return 0;
}

/*Refile the table with the genes pars*/
int fcreate_table(char * filename, int numero)
{
	fopen_w(filename);
	fwrite_pares(filename,numero);

	return 0;
}

//Pro1.cpp
#include "Pro1.h"

//数组输入
int Input(int Arr[], int &N)
{
	printf("输入整型数组元素个数：");
	scanf_s("%d", &N);
	for (int i = 0; i < N; i++)
	{
		printf("请输入第%d个：", i + 1);
		scanf_s("%d", &Arr[i]);
	}
	return 0;
}//Input

//平均值
int Average(int Arr[], int N)
{
	int sum = 0;
	if (N == 0)
	{
		printf("数组为空!\n");
		return 0;
	}
	for (int i = 0; i < N; i++)
		sum += Arr[i];
	return sum / N;
}//Average


//数组排序
void Sort(int Arr[], int N)
{
	int tmp;
	for (int i = 0; i < N - 1; i++)
		for (int j = 0; j < N - 1 - i; j++)
			if (Arr[j] > Arr[j + 1])
			{
				tmp = Arr[j];
				Arr[j] = Arr[j + 1];
				Arr[j + 1] = tmp;
			}
}//Sort

//文件写入
int WriteFile(int Arr[], int N)
{
	//printf("输入文件路径!\n");
	//scanf_s("%s", FP);
	int i;
	FILE *fp = NULL;
	char FP[] = "D:/TTF/test.txt";
	fp = fopen(FP, "w");
	if (fp == NULL)
	{
		printf("File cannot open!\n");
		return ERROR;
	}
	else
	{	
		fprintf(fp, "排序结果：");
		Sort(Arr, N);
		for (i = 0; i < N - 1; i++)
		{
			fprintf(fp, "%d", Arr[i]);
			fprintf(fp, "%s", ",");
		}
		fprintf(fp, "%d", Arr[i]);
		fprintf(fp, "%s", "\n-------------------------\n");
		fprintf(fp, "%s", "改组数据的平均值：");
		fprintf(fp, "%d", Average(Arr, N));
		fprintf(fp, "%s", "\n-------------------------");
		fclose(fp);		
		return OK;
	}
}//WriteFile

//测试
void TPro1()
{
	int N = 0, Arr[MAXLENGTH];
	Input(Arr, N);
	WriteFile(Arr, N);
}//TPro1
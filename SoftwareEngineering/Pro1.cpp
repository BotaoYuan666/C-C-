//Pro1.cpp
#include "Pro1.h"

//��������
int Input(int Arr[], int &N)
{
	printf("������������Ԫ�ظ�����");
	scanf_s("%d", &N);
	for (int i = 0; i < N; i++)
	{
		printf("�������%d����", i + 1);
		scanf_s("%d", &Arr[i]);
	}
	return 0;
}//Input

//ƽ��ֵ
int Average(int Arr[], int N)
{
	int sum = 0;
	if (N == 0)
	{
		printf("����Ϊ��!\n");
		return 0;
	}
	for (int i = 0; i < N; i++)
		sum += Arr[i];
	return sum / N;
}//Average


//��������
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

//�ļ�д��
int WriteFile(int Arr[], int N)
{
	//printf("�����ļ�·��!\n");
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
		fprintf(fp, "��������");
		Sort(Arr, N);
		for (i = 0; i < N - 1; i++)
		{
			fprintf(fp, "%d", Arr[i]);
			fprintf(fp, "%s", ",");
		}
		fprintf(fp, "%d", Arr[i]);
		fprintf(fp, "%s", "\n-------------------------\n");
		fprintf(fp, "%s", "�������ݵ�ƽ��ֵ��");
		fprintf(fp, "%d", Average(Arr, N));
		fprintf(fp, "%s", "\n-------------------------");
		fclose(fp);		
		return OK;
	}
}//WriteFile

//����
void TPro1()
{
	int N = 0, Arr[MAXLENGTH];
	Input(Arr, N);
	WriteFile(Arr, N);
}//TPro1
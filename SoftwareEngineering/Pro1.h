//Pro1.h
#include <stdio.h>
#include <stdlib.h>

#pragma warning(disable:4996)
#define MAXLENGTH 100
#define ERROR 0
#define OK 1

//�������ͺ���
int Input(int Arr[], int &N);

//ƽ��ֵ
int Average(int Arr[], int N);

//��������
void Sort(int Arr[], int N);

//д���ļ�
int WriteFile(int Arr[], int N);

//���Ժ���
void TPro1();
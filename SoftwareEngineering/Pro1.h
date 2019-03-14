//Pro1.h
#include <stdio.h>
#include <stdlib.h>

#pragma warning(disable:4996)
#define MAXLENGTH 100
#define ERROR 0
#define OK 1

//输入整型函数
int Input(int Arr[], int &N);

//平均值
int Average(int Arr[], int N);

//数组排序
void Sort(int Arr[], int N);

//写入文件
int WriteFile(int Arr[], int N);

//测试函数
void TPro1();
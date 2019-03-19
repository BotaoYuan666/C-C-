//Pro2.h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma warning(disable:4996)

#define M 10
#define N 300

//打开文件
void OpenFile(char SourceFilePath[N], char buf[M][N], int &n);

//统计记录条数
int CalcHead(char buf[M][N], int n);

//文件中出现域名总数
int CalcDomain(char buf[M][N], int n);

//记录最长
int CalcLongRecordByte(char buf[M][N], int n);

//找出每个域名在所有记录中域内容最大的记录编号
int CalcTheLongDomainContent(char buf[M][N], int n, char *domain[11], int K1[], int K3[], char CH[11][N]);

//文件写入
int WriteFile(char DestinationFilePath[N], char buf[M][N], int n);

//重新排序写入文件
int ReorderWriteFiles(char SourceFilePath[N], char buf[M][N], int n);

//主子串匹配
int Index(char Sub[N], char T[N]);

//测试函数
int TPro2(char SourceFilePath[N],char DestinationFilePath[N],int YON);

//注释行：121
//代码行：240
//总共行：361
//Pro2.h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma warning(disable:4996)

#define M 10
#define N 300

//���ļ�
void OpenFile(char SourceFilePath[N], char buf[M][N], int &n);

//ͳ�Ƽ�¼����
int CalcHead(char buf[M][N], int n);

//�ļ��г�����������
int CalcDomain(char buf[M][N], int n);

//��¼�
int CalcLongRecordByte(char buf[M][N], int n);

//�ҳ�ÿ�����������м�¼�����������ļ�¼���
int CalcTheLongDomainContent(char buf[M][N], int n, char *domain[11], int K1[], int K3[], char CH[11][N]);

//�ļ�д��
int WriteFile(char DestinationFilePath[N], char buf[M][N], int n);

//��������д���ļ�
int ReorderWriteFiles(char SourceFilePath[N], char buf[M][N], int n);

//���Ӵ�ƥ��
int Index(char Sub[N], char T[N]);

//���Ժ���
int TPro2(char SourceFilePath[N],char DestinationFilePath[N],int YON);

//ע���У�121
//�����У�240
//�ܹ��У�361
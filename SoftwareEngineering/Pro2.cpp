//Pro2.cpp

#include "Pro2.h"

/*****************************************************
*	������  ��TPro2                                  *
*   ����    �����Ժ���				                 *
*	���������SourceFilePath[N]     ��Դ�ļ�·��     *
*             DestinationFilePath[N]��Ŀ���ļ�·��   *
*             YON                   ���Ƿ��޸�Դ�ļ� *
*	�����������                                     *
*	����ֵ  ����                                     *
*****************************************************/
int TPro2(char SourceFilePath[N], char DestinationFilePath[N], int YON)
{
	int n = 0;
	char buf[M][N];
	OpenFile(SourceFilePath, buf, n);
	WriteFile(DestinationFilePath, buf, n);
	if(YON) ReorderWriteFiles(SourceFilePath, buf, n);
	printf("The Pro2 is OK!\n");
	return 0;
}//TPro2

/*************************************************
*	������  ��OpenFile                           *
*   ����    ���ļ����ݶ��뺯��  	             *
*	���������SourceFilePath[N]     ��Դ�ļ�·�� *
*             buf[][]����λ�ַ������飨Uninit��  *
*             n      ������n��Uninit��           *
*	���������buf[][]����λ�ַ������飨Init��    *
*             n      ������n��Init��             *
*	����ֵ  ����                                 *
*************************************************/
void OpenFile(char SourceFilePath[N],char buf[M][N],int &n)
{
	FILE *fp;
	fp = fopen(SourceFilePath, "r");
	if (!fp)
		printf("���ļ�������!\n");
	else
	{
		while (fgets(buf[n++], N, fp) != NULL);
	}
	fclose(fp);
	n -= 1;
}//OpenFile

/*************************************************
*	������  ��CalcHead                           *
*   ����    ��ͳ�Ƽ�¼���� 	                     *
*	���������buf[][]����λ�ַ������飨Init��    *
*             n      ������n��Init��             *
*	�����������                                 *
*	����ֵ  ��num����¼����                      *
*************************************************/
int CalcHead(char buf[M][N],int n)
{
	/*
	i  ����¼ѭ������
	pos����λ<HEAD>��������λ��
	num����¼��¼����
	*/
	int i = 0, pos, num = 0;
	char Sub[] = "<HEAD>";
	if (!buf)
		printf("����Ϊ��\n");
	while (i < n)
	{
		pos = Index(Sub, buf[i]);
		if (pos != -1)
			num += 1;
		i += 1;
	}
	return num;
}//CalcHead

/*************************************************
*	������  ��CalcDomain                         *
*   ����    ���ļ��г�����������                 *
*	���������buf[][]����λ�ַ������飨Init��    *
*             n      ������n��Init��             *
*	�����������                                 *
*	����ֵ  ��num����������                      *
*************************************************/
int CalcDomain(char buf[M][N], int n)
{
	/*
	i  ����¼ѭ������
	pos����λ<XXX>xxx<XXX>�������г��ָ���
	num����¼��������
	*/
	int i = 0, pos, num = 0;
	if (!buf)
		printf("����Ϊ��\n");
	char Sub[] = "><";
	while (i < n)
	{
		pos = Index(Sub, buf[i]);
		if (pos != -1)
			num = num + pos - 1;  //1������<HEAD>xxx<HEAD>
		i += 1;
	}
	return num;
}//CalcDomain

/*************************************************
*	������  ��CalcLongRecordByte                 *
*   ����    �������ļ���¼������Ǹ���¼       *
*	���������buf[][]����λ�ַ������飨Init��    *
*             n      ������n��Init��             *
*	�����������                                 *
*	����ֵ  ��RB�����¼���                   *
*************************************************/
int CalcLongRecordByte(char buf[M][N], int n)
{
	int RB = 0, j, KK;
	for (int i = 0; i < n; i++)
	{
		KK = 0;
		j = KK;
		while(buf[i][j++] != '\0')
			KK++;
		if (RB < KK)
			RB = KK - 1;
	}
	return RB;
}//CalcLongRecordByte

/*****************************************************************
*	������  ��CalcTheLongDomainContent                           *
*   ����    ���ҳ�ÿ�����������м�¼�����������ļ�¼���       *
*	���������buf[][]  ����λ�ַ������飨Init��                  *
*             n        ������n��Init��                           *
*             *domain[]��������Uninit��                          *
*			  K1[]     ��ÿ����������ȣ�Uninit��              *
			  K3[]     ��ÿ�����������λ�ڵļ�¼��ţ�Uninit��*
			  CH[11][N]��ÿ��������Ӧ����ݣ�Uninit��          *
*	���������*domain[]��������Init��                            *
			  K1[]     ��ÿ����������ȣ�Init��                *
			  K3[]     ��ÿ�����������λ�ڵļ�¼��ţ�Init��  *
			  CH[11][N]��ÿ��������Ӧ����ݣ�Init��            *
*	����ֵ  ��0                                                  *
*****************************************************************/
int CalcTheLongDomainContent(char buf[M][N], int n, char *domain[11], int K1[], int K3[], char CH[11][N])
{
	//K2����¼������ÿ����¼�����ݳ���
	//K4�������CH����ʹ��
	//P1������ÿ��������ÿ����¼�е�λ��
	//P2������ÿ�����������м�¼����������Ǹ���¼��λ��
	int K2, P1[2], P2[2];
	//"ATI","AUTH","CLASS1","CLASS2","FREQ","ID","ISSUE","TITLE","PUB","PUBC","RR"
	char *domain1[] = { (char*)"<ATI>",(char*)"<AUTH>",(char*)"<CLASS1>",(char*)"<CLASS2>",(char*)"<FREQ>",\
		              (char*)"<ID>",(char*)"<ISSUE>",(char*)"<TITLE>",(char*)"<PUB>",(char*)"<PUBC>",(char*)"<RR>" };
	char *domain2[] = { (char*)"</ATI>",(char*)"</AUTH>",(char*)"</CLASS1>",(char*)"</CLASS2>",(char*)"</FREQ>",\
				      (char*)"</ID>",(char*)"</ISSUE>",(char*)"</TITLE>",(char*)"</PUB>",(char*)"</PUBC>",(char*)"</RR>" };
	for (int j = 0; j < 11; j++)
	{
		K1[j] = 0;
		domain[j] = domain1[j];
		for (int i = 0; i < n; i++)
		{
			P1[0] = Index(domain1[j], buf[i]);
			P1[1] = Index(domain2[j], buf[i]);
			K2 =  P1[1] - P1[0] - strlen(domain1[j]);
			if (K1[j] < K2)
			{
				K1[j] = K2;
				K3[j] = i + 1;
				P2[0] = P1[0];
				P2[1] = P1[1];
			}
		}
		for (int k = P2[0] + strlen(domain1[j]), K4 = 0; k < P2[1]; k++,K4++)
		{
			CH[j][K4] = buf[K3[j] - 1][k];
			CH[j][K4 + 1] = '\0';
		}
	}
	return 0;
}//CalcTheLongDomainContent

/****************************************************
*	������  ��WriteFile                             *
*   ����    ������д���ļ�                          *
*	���������SourceFilePath[N]��Դ�ļ�·����Init�� *
*             buf[][]����λ�ַ������飨Init��       *
*             n      ������n��Init��                *
*	�����������                                    *
*	����ֵ  ��0 or 1                                *
****************************************************/
int WriteFile(char DestinationFilePath[N], char buf[M][N], int n)
{
	//K1��ÿ����������ȣ�Uninit��
	//K2��ÿ�����������λ�ڵļ�¼��ţ�Uninit��
	//CH[][]   ��ÿ��������Ӧ����ݣ�Uninit��
	//FP[]     ���ļ�·��
	//*domain[]��������Uninit��
	int K1[11], K2[11];
	char CH[11][N], *domain[11];
	FILE *fp = fopen(DestinationFilePath, "w");
	if (!fp)
	{
		printf("File cannot open!\n");
		return 0;
	}
	else
	{
		fprintf(fp, "��¼������%d\n", CalcHead(buf, n));
		fprintf(fp, "�����������Ϊ��%d\n", CalcDomain(buf, n));
		fprintf(fp, "���¼��ռ�ֽ���Ϊ��%d\n", CalcLongRecordByte(buf, n));
		fprintf(fp, "\n\n\n");
		CalcTheLongDomainContent(buf, n, domain, K1, K2, CH);
		for (int i = 0; i < 11; i++)
		{
			fprintf(fp, "����Ϊ��%s\n", domain[i]);
			fprintf(fp, "�����Ϊ��%d\n", K1[i]);
			fprintf(fp, "������¼��Ϊ��%d\n", K2[i]);
			fprintf(fp, "������Ϊ��%s\n", CH[i]);
			fprintf(fp, "\n");
		}
	}
	fclose(fp);
	return 1;
}//WriteFile

/****************************************************
*	������  ��ReorderWriteFiles                     *
*   ����    ����������д���ļ�                      *
*	���������SourceFilePath[N]��Դ�ļ�·����Init�� *
*             buf[][]����λ�ַ������飨Init��       *
*             n      ������n��Init��                *
*	�����������                                    *
*	����ֵ  ��0                                     *
****************************************************/
int ReorderWriteFiles(char SourceFilePath[N], char buf[M][N], int n)
{
	//P   ������ÿ��һ��������ÿ����¼�е�λ��
	//CH[]��ÿ��������Ӧÿ����¼���ݣ�Uninit��
	//FP[]���ļ�·��
	//K   �������CH����ʹ��
	int P[2];
	char CH[N];
	FILE *fp = fopen(SourceFilePath, "w");
	if (!fp)
	{
		printf("File cannot open!\n");
		return 0;
	}
	char *domain1[] = { (char*)"<ATI>",(char*)"<AUTH>",(char*)"<CLASS1>",(char*)"<CLASS2>",(char*)"<FREQ>",\
					  (char*)"<ID>",(char*)"<ISSUE>",(char*)"<TITLE>",(char*)"<PUB>",(char*)"<PUBC>",(char*)"<RR>" };
	char *domain2[] = { (char*)"</ATI>",(char*)"</AUTH>",(char*)"</CLASS1>",(char*)"</CLASS2>",(char*)"</FREQ>",\
					  (char*)"</ID>",(char*)"</ISSUE>",(char*)"</TITLE>",(char*)"</PUB>",(char*)"</PUBC>",(char*)"</RR>" };
	for (int i = 0; i < n; i++)
	{
		fprintf(fp, "<HEAD>");
		for (int j = 0; j < 11; j++)
		{
			P[0] = Index(domain1[j], buf[i]);
			P[1] = Index(domain2[j], buf[i]);
			for (int k = P[0] + strlen(domain1[j]), K = 0; k < P[1]; k++,K++)
			{
				CH[K] = buf[i][k];
				CH[K + 1] = '\0';
			}
			fprintf(fp, "%s", domain1[j]);
			fprintf(fp, "%s", CH);
			fprintf(fp, "%s", domain2[j]);
			CH[0] = '\0';
		}
		fprintf(fp, "</HEAD>\n");
	}
	fclose(fp);
	return 1;
}//ReorderWriteFiles

/*************************************************
*	������  ��Index                              *
*   ����    ���Ӵ�ƥ��                           *
*	���������buf[][]����λ�ַ������飨Init��    *
*             n      ������n��Init��             *
*	�����������                                 *
*	����ֵ  ��pos���Ӵ�λ��������λ��            *
*             K  �������г����Ӵ��Ĵ���          *
*************************************************/
int Index(char Sub[N], char T[N])
{
	if(!(Sub&&T))
		printf("����Ϊ��\n");
	//i��j�����ڵ���
	//ii  ������iԭֵ
	//K   ������
	//pos ����λ
	int i = 0, j = i, ii, K = 0, pos;
	while (T[i])
	{
		j = 0;
		ii = i;
		while (Sub[j])
		{
			if (Sub[j++] == T[i++])
			{
				if (Sub[j] == '\0')
				{
					K += 1;
					pos = i - strlen(Sub);
				}
			}
			else break;
		}
		i = ii + 1;
	}
	//KΪ1���ص����Ӵ�λ��������λ��
	//K��Ϊ1���ص��������г����Ӵ��Ĵ���
	if(K == 1)
		return pos;
	else return K;
}//Index
//Pro2.cpp

#include "Pro2.h"

/*****************************************************
*	函数名  ：TPro2                                  *
*   功能    ：测试函数				                 *
*	输入参数：SourceFilePath[N]     ：源文件路径     *
*             DestinationFilePath[N]：目标文件路径   *
*             YON                   ：是否修改源文件 *
*	输出参数：无                                     *
*	返回值  ：无                                     *
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
*	函数名  ：OpenFile                           *
*   功能    ：文件内容读入函数  	             *
*	输入参数：SourceFilePath[N]     ：源文件路径 *
*             buf[][]：二位字符串数组（Uninit）  *
*             n      ：整型n（Uninit）           *
*	输出参数：buf[][]：二位字符串数组（Init）    *
*             n      ：整型n（Init）             *
*	返回值  ：无                                 *
*************************************************/
void OpenFile(char SourceFilePath[N],char buf[M][N],int &n)
{
	FILE *fp;
	fp = fopen(SourceFilePath, "r");
	if (!fp)
		printf("此文件不存在!\n");
	else
	{
		while (fgets(buf[n++], N, fp) != NULL);
	}
	fclose(fp);
	n -= 1;
}//OpenFile

/*************************************************
*	函数名  ：CalcHead                           *
*   功能    ：统计记录条数 	                     *
*	输入参数：buf[][]：二位字符串数组（Init）    *
*             n      ：整型n（Init）             *
*	输出参数：无                                 *
*	返回值  ：num：记录条数                      *
*************************************************/
int CalcHead(char buf[M][N],int n)
{
	/*
	i  ：记录循环次数
	pos：定位<HEAD>在主串中位置
	num：记录记录条数
	*/
	int i = 0, pos, num = 0;
	char Sub[] = "<HEAD>";
	if (!buf)
		printf("数组为空\n");
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
*	函数名  ：CalcDomain                         *
*   功能    ：文件中出现域名总数                 *
*	输入参数：buf[][]：二位字符串数组（Init）    *
*             n      ：整型n（Init）             *
*	输出参数：无                                 *
*	返回值  ：num：域名个数                      *
*************************************************/
int CalcDomain(char buf[M][N], int n)
{
	/*
	i  ：记录循环次数
	pos：定位<XXX>xxx<XXX>在主串中出现个数
	num：记录域名个数
	*/
	int i = 0, pos, num = 0;
	if (!buf)
		printf("数组为空\n");
	char Sub[] = "><";
	while (i < n)
	{
		pos = Index(Sub, buf[i]);
		if (pos != -1)
			num = num + pos - 1;  //1：代表<HEAD>xxx<HEAD>
		i += 1;
	}
	return num;
}//CalcDomain

/*************************************************
*	函数名  ：CalcLongRecordByte                 *
*   功能    ：计算文件记录中最长的那个记录       *
*	输入参数：buf[][]：二位字符串数组（Init）    *
*             n      ：整型n（Init）             *
*	输出参数：无                                 *
*	返回值  ：RB：最长纪录编号                   *
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
*	函数名  ：CalcTheLongDomainContent                           *
*   功能    ：找出每个域名在所有记录中域内容最大的记录编号       *
*	输入参数：buf[][]  ：二位字符串数组（Init）                  *
*             n        ：整型n（Init）                           *
*             *domain[]：域名（Uninit）                          *
*			  K1[]     ：每个域名最长长度（Uninit）              *
			  K3[]     ：每个域名最长内容位于的记录编号（Uninit）*
			  CH[11][N]：每个域名对应最长内容（Uninit）          *
*	输出参数：*domain[]：域名（Init）                            *
			  K1[]     ：每个域名最长长度（Init）                *
			  K3[]     ：每个域名最长内容位于的记录编号（Init）  *
			  CH[11][N]：每个域名对应最长内容（Init）            *
*	返回值  ：0                                                  *
*****************************************************************/
int CalcTheLongDomainContent(char buf[M][N], int n, char *domain[11], int K1[], int K3[], char CH[11][N])
{
	//K2：记录域名在每个记录中内容长度
	//K4：负责给CH迭代使用
	//P1：查找每个域名在每条记录中的位置
	//P2：保存每个域名在所有记录中内容最长的那个记录的位置
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
*	函数名  ：WriteFile                             *
*   功能    ：内容写入文件                          *
*	输入参数：SourceFilePath[N]：源文件路径（Init） *
*             buf[][]：二位字符串数组（Init）       *
*             n      ：整型n（Init）                *
*	输出参数：无                                    *
*	返回值  ：0 or 1                                *
****************************************************/
int WriteFile(char DestinationFilePath[N], char buf[M][N], int n)
{
	//K1：每个域名最长长度（Uninit）
	//K2：每个域名最长内容位于的记录编号（Uninit）
	//CH[][]   ：每个域名对应最长内容（Uninit）
	//FP[]     ：文件路径
	//*domain[]：域名（Uninit）
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
		fprintf(fp, "记录总数：%d\n", CalcHead(buf, n));
		fprintf(fp, "出现域的总数为：%d\n", CalcDomain(buf, n));
		fprintf(fp, "最长纪录所占字节数为：%d\n", CalcLongRecordByte(buf, n));
		fprintf(fp, "\n\n\n");
		CalcTheLongDomainContent(buf, n, domain, K1, K2, CH);
		for (int i = 0; i < 11; i++)
		{
			fprintf(fp, "域名为：%s\n", domain[i]);
			fprintf(fp, "最长长度为：%d\n", K1[i]);
			fprintf(fp, "所处记录号为：%d\n", K2[i]);
			fprintf(fp, "域内容为：%s\n", CH[i]);
			fprintf(fp, "\n");
		}
	}
	fclose(fp);
	return 1;
}//WriteFile

/****************************************************
*	函数名  ：ReorderWriteFiles                     *
*   功能    ：重新排序写入文件                      *
*	输入参数：SourceFilePath[N]：源文件路径（Init） *
*             buf[][]：二位字符串数组（Init）       *
*             n      ：整型n（Init）                *
*	输出参数：无                                    *
*	返回值  ：0                                     *
****************************************************/
int ReorderWriteFiles(char SourceFilePath[N], char buf[M][N], int n)
{
	//P   ：查找每个一对域名在每条记录中的位置
	//CH[]：每个域名对应每条记录内容（Uninit）
	//FP[]：文件路径
	//K   ：负责给CH迭代使用
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
*	函数名  ：Index                              *
*   功能    ：子串匹配                           *
*	输入参数：buf[][]：二位字符串数组（Init）    *
*             n      ：整型n（Init）             *
*	输出参数：无                                 *
*	返回值  ：pos：子串位于主串的位置            *
*             K  ：主串中出现子串的次数          *
*************************************************/
int Index(char Sub[N], char T[N])
{
	if(!(Sub&&T))
		printf("数组为空\n");
	//i，j：用于迭代
	//ii  ：备份i原值
	//K   ：计数
	//pos ：定位
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
	//K为1返回的是子串位于主串的位置
	//K不为1返回的是主串中出现子串的次数
	if(K == 1)
		return pos;
	else return K;
}//Index
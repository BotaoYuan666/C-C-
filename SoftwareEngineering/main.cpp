//main.cpp

//#include "Pro1.h"
#include "Pro2.h"

int main(int argc, char *argv[])
{	
	int YON = 0;
	printf("The command line has %d arguments :\n", argc - 1);
	if (argc < 3 || argc>3)
	{
		printf("����������ٻ����\n");
	}
	else
	{
		printf("�Ƿ���������д��Դ�ļ�\n");
		printf("0��No(Ĭ��)\n1��Yes\n");
		scanf("%d", &YON);
		TPro2(argv[1],argv[2],YON);
	}
	printf("The main is OK!");
	return 0;
}
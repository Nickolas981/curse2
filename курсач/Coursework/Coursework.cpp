
#include "stdafx.h"
#include "Coursework.h"
#include "cFile.h"


#define MAX_LOADSTRING 100
#define _CRT_SECURE_NO_WARNINGS
HINSTANCE hInst;                               
CHAR szTitle[MAX_LOADSTRING];                 
CHAR szWindowClass[MAX_LOADSTRING];           

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

char* selected = nullptr;
//std::string selectPath;
char selectedPath[255];
//typedef std::multimap<std::string, std::string> Map;
std::string course(std::string , std::string);
Map files;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_COURSEWORK, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_COURSEWORK));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_COURSEWORK));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCE(IDC_COURSEWORK);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; 

   HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	if (uMsg == BFFM_INITIALIZED)
	{
		if (selected != nullptr)
			delete selected;

		selected = new char[strlen((char*)lpData) + 1];
		strcpy(selected, ((char*)lpData));

		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
	}

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	static OPENFILENAME ofn;
	static BROWSEINFO brInfo;

	static std::vector<HANDLE> cppFiles;
	static std::vector<HANDLE> hFiles;
	static std::vector<HANDLE> rcFiles;
	
    switch (message)
    {
	case WM_CREATE:
		ofn.hwndOwner = hWnd;
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.nMaxFile = sizeof(selected);
		ofn.lpstrFile = selected;

		brInfo.hwndOwner = hWnd;
		brInfo.pszDisplayName = selected;
		brInfo.lpszTitle = "Виберіть директорію для початку роботи";
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
			case IDM_OPEN:
			{
				LPITEMIDLIST iil = SHBrowseForFolder(&brInfo);
				if (iil != nullptr) {
					SHGetPathFromIDList(iil, selectedPath);
					std::string fPath(selectedPath);
					
					HANDLE hFind = INVALID_HANDLE_VALUE;
					WIN32_FIND_DATA findFiles;
					cFile file =  cFile(fPath);
					MessageBox(hWnd, file.retStr().c_str(), "Files", MB_OK);
					//strcat(selectedPath, "\\*.cpp");
					//hFind = FindFirstFile(selectedPath, &findFiles);
					
					//do {
					//	if (!(findFiles.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
					//	{
					//		
					//		std::string filename = findFiles.cFileName;
					//		//course(fPath + filename);
					//		if (filename[0]  != -52)
					//			MessageBox(hWnd, filename.c_str(), "Files", MB_OK);
					//			MessageBox(hWnd, course(fPath , filename).c_str(), "result", MB_OK);
					//	}
					//} while (FindNextFile(hFind, &findFiles) != 0);
				}
			}
			break;
			case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

std::string course(std::string filePath, std::string fileName)
{
	int i, j;
	char buff[255];
	
	std::ifstream fin(filePath + "\\" + fileName);
	fin.getline(buff, 50);
	std::string res;
	while (std::strstr(buff, "#include"))
	{
		i = j = -1;
		while (buff[++i] != '<');
		while (buff[i + (++j)] != '>');
		//files.insert(std::pair<std::string, std::vector<std::string>>)
		//files.insert(Map::value_type(fileName, (std::string(buff)).substr(i,j + 1)));
		res += (std::string(buff)).substr(i, j + 1);
		fin.getline(buff, 50);
	}
	
	return (res);
}
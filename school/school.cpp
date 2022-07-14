// school.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

//#pragma warning(disable:4996)
#include "framework.h"
#include "school.h"
#include "resource.h"
#include <ctime>
#include <fstream>
#include <string.h>
#include <ctime>
#include <array>
#include <iostream>
#include <iomanip>

using namespace std;

constexpr auto MAX_LOADSTRING = 100;

time_t timer;
struct tm* t;
// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
wchar_t guest_name_in[1000][100];               //방문객 이름
wchar_t guest_name_out[1000][100];				//나간 방문객
wchar_t out_guest[100];							//임시 방문객변수
wchar_t in_guest[100];							
wstring guest_intime[100];						//시간 변수
wstring guest_outtime[100];
int guest_count_in = 0;                            //방문객 인원수
int guest_count_out = 0;
int gcountin;					//for문용
int gcountout;					
int check_inlist = 0;
wchar_t* widecstr;				//wstring to wchar_t용
size_t tmp = 0;
size_t csize;
int flagin = 0;
int flagout = 0;

std::wstring currentDateTime()
{
	time_t rawtime;
	struct tm timeinfo;
	wchar_t buffer[20];
	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);
	wcsftime(buffer, 20, L"%Y-%m-%d %H:%M:%S", &timeinfo);
	return buffer;
}

std::wstring concatenate(wchar_t* list, wstring time) {
	wchar_t guest_name[1000];
	wcsncpy_s(guest_name, list, sizeof(list)+1);
	wcscat_s(guest_name, 10, L"------");
	const wchar_t* wcs = time.c_str();
	wcscat_s(guest_name, 100, wcs);
	return guest_name;
}


// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_SCHOOL, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SCHOOL));

	MSG msg;

	// 기본 메시지 루프입니다:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SCHOOL));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_SCHOOL);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}


BOOL CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hList1 = GetDlgItem(hDlg, IDC_LIST1);
	HWND hList2 = GetDlgItem(hDlg, IDC_LIST2);
	switch (uMsg)
	{
	case WM_INITDIALOG:
		for (int i = 0; i < guest_count_in; i++) {
			widecstr = _wcsdup(concatenate(guest_name_in[i], guest_intime[i]).c_str());	//입장 기록
			SendMessage(hList1, LB_ADDSTRING, 0, (WPARAM)widecstr);
			if (wcsncmp(guest_name_out[i], L"", sizeof(guest_name_out[i]))!=0) {
				widecstr = _wcsdup(concatenate(guest_name_out[i], guest_outtime[i]).c_str());
				SendMessage(hList2, LB_ADDSTRING, 0, (WPARAM)widecstr);
			}
		}
		return TRUE;
	case WM_COMMAND:
		
		switch (wParam)
		{
		case IDCANCEL:
			EndDialog(hDlg, 0);
			SendMessage(hList1, LB_RESETCONTENT, 0, 0);	//리스트박스 초기화
			SendMessage(hList1, LB_RESETCONTENT, 0, 0);
			hList1 = NULL;
			hList2 = NULL;
			break;
		}
		return TRUE;
	}
	return FALSE;
}


//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hEdit;
	int check;
	switch (message)
	{
	case WM_CREATE:
	{
		hEdit = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, 375, 200, 250, 30, hWnd, (HMENU)1, hInst, NULL);		//입력 창 생성
		CreateWindow(L"button", L"들어가기", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 375, 250, 120, 30, hWnd, (HMENU)2, hInst, NULL);			//버튼 생성
		CreateWindow(L"button", L"나가기", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 505, 250, 120, 30, hWnd, (HMENU)3, hInst, NULL);
		CreateWindow(L"button", L"출입기록 확인", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 10, 120, 30, hWnd, (HMENU)4, hInst, NULL);
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case 2:																	//들어가기 버튼 눌렀을 때
			GetWindowText(hEdit, in_guest, 100);					//이름 배열에 추가
			if (_tcscmp(L"", in_guest) == 0) {
				MessageBox(hWnd, L"이름을 입력해주세요.", L"경고", MB_OK);
				break;
			}
			flagin = 0;
			flagout = 0;
			if (guest_count_out == 0)
				gcountout = guest_count_out + 1;
			else
				gcountout = guest_count_out;
			if (guest_count_in == 0)
				gcountin = guest_count_in + 1;
			else
				gcountin = guest_count_in;
			for (int i = 0; i < gcountin; i++) {								//배열에서 이름 찾기
				for (int j = 0; j < gcountout; j++) {
					if ((_tcscmp(guest_name_in[i], in_guest) == 0)) {	//이름 있을 경우 퇴장 처리
						flagin++;
					}
					if ((_tcscmp(guest_name_out[i], in_guest) == 0)) {	//이름 있을 경우 퇴장 처리
						flagout++;
					}
				}
			}
			if (flagin == flagout) {
				wcsncpy_s(guest_name_in[guest_count_in], in_guest, sizeof(in_guest) + 1);
				guest_intime[guest_count_in] = currentDateTime();
				guest_count_in++;
				MessageBox(hWnd, L"입장 처리되었습니다.", L"확인", MB_OK);
				SetWindowText(hEdit, L"");
			}
			else {
				MessageBox(hWnd, L"이미 입장하셨습니다.", L"경고", MB_OK);
			}
			break;
		case 3:
			check = MessageBox(hWnd, L"퇴장하시겠습니까?", L"확인", MB_YESNO);		//퇴장 버튼 눌렀을 떄
			if (check == IDYES)
			{
				GetWindowText(hEdit, out_guest, 100);
				for (int i = 0; i < guest_count_in; i++) {								//배열에서 이름 찾기
					if (_tcscmp(guest_name_in[i], out_guest) == 0) {					//이름 있을 경우 퇴장 처리
						wcsncpy_s(guest_name_out[guest_count_out], out_guest, sizeof(out_guest)+1);
						guest_outtime[guest_count_out] = currentDateTime();
						MessageBox(hWnd, L"퇴장 처리되었습니다.", L"확인", MB_OK);
						check_inlist = 1;
						SetWindowText(hEdit, L"");
						guest_count_out++;
						break;
					}
				}
				if (check_inlist != 1) {			//이름 없으면 경고
					MessageBox(hWnd, L"이름이 등록되어 있지 않습니다.", L"경고", MB_OK);
				}
			}
			break;
		case 4:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_SCHOOL_DIALOG), hWnd,(DLGPROC)DialogProc);
		}
		break;
	case WM_GETMINMAXINFO:
	{
		LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;			//창 크기 고정
		lpMMI->ptMinTrackSize.x = 1000;
		lpMMI->ptMinTrackSize.y = 500;
		lpMMI->ptMaxTrackSize.x = 1000;
		lpMMI->ptMaxTrackSize.y = 500;
		return 0;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
	}
}

// 정보 대화 상자의 메시지 처리기입니다.
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
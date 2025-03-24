#include "ConsoleRenderer.h"

#include <windows.h>

#include "DebugUtility.h"

static HANDLE hConsoleHandle;      // 초기 화면 콘솔의 핸들

static int nScreenWidth = 0; // 콘솔창의 너비
static int nScreenHeight = 0; // 콘솔창의 높이
static int nScreenBufferSize = 0; // 콘솔창의 스크린버퍼 크기
static int nScreenBufferIndex = 0; // 콘솔창이 사용할 스크린버퍼의 인덱스
static HANDLE hScreenBuffer[2]; // 콘솔창이 사용할 스크린버퍼의 핸들

void ScreenInit()
{
    // 현재 화면크기에 맞는 화면 콘솔스크린버퍼 2개를 만든다.    
    hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    hScreenBuffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    hScreenBuffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

    // 기본 콘솔,생성된 콘솔스크린 모두 커서 안보이게 설정
    CONSOLE_CURSOR_INFO cursorInfo = { 0, };
    cursorInfo.bVisible = FALSE;
    cursorInfo.dwSize = 1; // 커서의 크기를 결정 (1~100 사이만 가능)
    SetConsoleCursorInfo(hConsoleHandle, &cursorInfo);
    SetConsoleCursorInfo(hScreenBuffer[0], &cursorInfo);
    SetConsoleCursorInfo(hScreenBuffer[1], &cursorInfo);

    //기본 콘솔의 화면 크기 정보를 얻는다.
    CONSOLE_SCREEN_BUFFER_INFO Info; // 초기 화면 콘솔의 화면 정보
    GetConsoleScreenBufferInfo(hConsoleHandle, &Info);

    nScreenHeight = Info.srWindow.Bottom - Info.srWindow.Top + 1;
    nScreenWidth = Info.srWindow.Right - Info.srWindow.Left + 1;
    nScreenBufferSize = nScreenWidth * nScreenHeight;
}

void ScreenFlipping()
{
    // 실제 콘솔이 사용할 스크린버퍼의 Handle을 설정하여 화면에 보여준다.
    SetConsoleActiveScreenBuffer(hScreenBuffer[nScreenBufferIndex]);
    // 다음에 사용할 스크린 버퍼의 인덱스를 증가시켜 준비한다.
    nScreenBufferIndex++;
    nScreenBufferIndex = nScreenBufferIndex % 2;  // 0,1,0,1,0,1,0,1....
}

void ScreenClear()
{
    COORD Coor = { 0, 0 };
    DWORD dw;
    FillConsoleOutputCharacter(hScreenBuffer[nScreenBufferIndex], ' ', nScreenBufferSize, Coor, &dw);
}

void ScreenRelease()
{
    CloseHandle(hScreenBuffer[0]);
    CloseHandle(hScreenBuffer[1]);
}

BOOL ScreenDrawChar(int x, int y, wchar_t ch, WORD attr)
{
    COORD	cdPos = { x, y };
    BOOL	bRval = FALSE;
    DWORD	dwCharsWritten;

    bRval = FillConsoleOutputCharacterW(hScreenBuffer[nScreenBufferIndex], ch, 1, cdPos, &dwCharsWritten);
    if (bRval == FALSE)
    {
        DebugLog("Error, FillConsoleOutputCharacter()\n");
    }

    bRval = FillConsoleOutputAttribute(hScreenBuffer[nScreenBufferIndex], attr, 1, cdPos, &dwCharsWritten);
    if (bRval == FALSE)
    {
        DebugLog("Error, FillConsoleOutputAttribute()\n");
    }

    return bRval;
}

BOOL ScreenDrawString(int x, int y, const wchar_t* pStr, WORD attr)
{
    COORD	cdPos = { x, y };
    BOOL	bRval = FALSE;
    DWORD	dwCharsWritten;

    DWORD nNumberOfBytesToWrite = (DWORD)wcslen(pStr);
    //특정 위치에 문자열을 출력한다.
    WriteConsoleOutputCharacterW(hScreenBuffer[nScreenBufferIndex], pStr, nNumberOfBytesToWrite, cdPos, &dwCharsWritten);
    bRval = FillConsoleOutputAttribute(hScreenBuffer[nScreenBufferIndex], attr, nNumberOfBytesToWrite, cdPos, &dwCharsWritten);
    if (bRval == FALSE)
    {
        DebugLog("Error, FillConsoleOutputAttribute()\n");
    }

    return bRval;
}

BOOL ScreenSetAttributes(WORD attr)
{
    COORD	cdPos = { 0, 0 };
    BOOL	bRval = FALSE;
    DWORD	dwCharsWritten;
    //	int x,y;	

    bRval = FillConsoleOutputAttribute(hScreenBuffer[nScreenBufferIndex], attr, nScreenBufferSize, cdPos, &dwCharsWritten);
    if (bRval == FALSE)
    {
        DebugLog("Error, FillConsoleOutputCharacter()\n");
        return bRval;
    }

    return bRval;
}

int ScreenWidth()
{
    return nScreenWidth;
}
int ScreenHeight()
{
    return nScreenHeight;
}
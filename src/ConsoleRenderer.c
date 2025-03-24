#include "ConsoleRenderer.h"

#include <windows.h>

#include "DebugUtility.h"

static HANDLE hConsoleHandle;      // �ʱ� ȭ�� �ܼ��� �ڵ�

static int nScreenWidth = 0; // �ܼ�â�� �ʺ�
static int nScreenHeight = 0; // �ܼ�â�� ����
static int nScreenBufferSize = 0; // �ܼ�â�� ��ũ������ ũ��
static int nScreenBufferIndex = 0; // �ܼ�â�� ����� ��ũ�������� �ε���
static HANDLE hScreenBuffer[2]; // �ܼ�â�� ����� ��ũ�������� �ڵ�

void ScreenInit()
{
    // ���� ȭ��ũ�⿡ �´� ȭ�� �ֽܼ�ũ������ 2���� �����.    
    hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    hScreenBuffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    hScreenBuffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

    // �⺻ �ܼ�,������ �ֽܼ�ũ�� ��� Ŀ�� �Ⱥ��̰� ����
    CONSOLE_CURSOR_INFO cursorInfo = { 0, };
    cursorInfo.bVisible = FALSE;
    cursorInfo.dwSize = 1; // Ŀ���� ũ�⸦ ���� (1~100 ���̸� ����)
    SetConsoleCursorInfo(hConsoleHandle, &cursorInfo);
    SetConsoleCursorInfo(hScreenBuffer[0], &cursorInfo);
    SetConsoleCursorInfo(hScreenBuffer[1], &cursorInfo);

    //�⺻ �ܼ��� ȭ�� ũ�� ������ ��´�.
    CONSOLE_SCREEN_BUFFER_INFO Info; // �ʱ� ȭ�� �ܼ��� ȭ�� ����
    GetConsoleScreenBufferInfo(hConsoleHandle, &Info);

    nScreenHeight = Info.srWindow.Bottom - Info.srWindow.Top + 1;
    nScreenWidth = Info.srWindow.Right - Info.srWindow.Left + 1;
    nScreenBufferSize = nScreenWidth * nScreenHeight;
}

void ScreenFlipping()
{
    // ���� �ܼ��� ����� ��ũ�������� Handle�� �����Ͽ� ȭ�鿡 �����ش�.
    SetConsoleActiveScreenBuffer(hScreenBuffer[nScreenBufferIndex]);
    // ������ ����� ��ũ�� ������ �ε����� �������� �غ��Ѵ�.
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
    //Ư�� ��ġ�� ���ڿ��� ����Ѵ�.
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
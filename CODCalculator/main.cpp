#include <windows.h>
#include <commctrl.h>
#include <cstdio>
#include <string>
#include <cmath>
#include "resource.h"

#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "Comctl32.lib")

// 对话框过程函数
INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

// 计算函数
double CalculateCOD(double initialMass, double initialCOD, double addedCOD, double targetCOD);

// 验证输入
bool ValidateInput(HWND hDlg, double& initialMass, double& initialCOD, double& addedCOD, double& targetCOD);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // 初始化通用控件
    INITCOMMONCONTROLSEX icc;
    icc.dwSize = sizeof(icc);
    icc.dwICC = ICC_STANDARD_CLASSES;
    InitCommonControlsEx(&icc);

    // 创建对话框
    DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_MAIN_DIALOG), NULL, DialogProc, 0);
    return 0;
}

INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HICON hIcon = NULL;  // 使用静态变量确保只初始化一次

    switch (uMsg)
    {
    case WM_INITDIALOG:
        // 加载图标（如果尚未加载）
        if (!hIcon) {
            hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(1));
        }
        if (hIcon) {
            SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        }

        // 设置默认焦点
        SetFocus(GetDlgItem(hDlg, IDC_INITIAL_MASS));
        return TRUE;

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        int wmEvent = HIWORD(wParam);

        // 处理按钮点击
        if (wmEvent == BN_CLICKED)
        {
            if (wmId == IDC_CALCULATE)
            {
                double initialMass, initialCOD, addedCOD, targetCOD;

                if (ValidateInput(hDlg, initialMass, initialCOD, addedCOD, targetCOD))
                {
                    double result = CalculateCOD(initialMass, initialCOD, addedCOD, targetCOD);

                    // 显示结果
                    wchar_t buffer[50];
                    swprintf_s(buffer, L"%.2f", result);
                    SetDlgItemText(hDlg, IDC_RESULT, buffer);
                }
            }
            else if (wmId == IDC_CLEAR)
            {
                // 清除所有输入
                SetDlgItemText(hDlg, IDC_INITIAL_MASS, L"");
                SetDlgItemText(hDlg, IDC_INITIAL_COD, L"");
                SetDlgItemText(hDlg, IDC_ADDED_COD, L"");
                SetDlgItemText(hDlg, IDC_TARGET_COD, L"");
                SetDlgItemText(hDlg, IDC_RESULT, L"");
                SetFocus(GetDlgItem(hDlg, IDC_INITIAL_MASS));
            }
        }
        return TRUE;
    }

    case WM_CLOSE:
        EndDialog(hDlg, 0);
        return TRUE;

    case WM_DESTROY:
        // 清理图标资源
        if (hIcon) {
            DestroyIcon(hIcon);
            hIcon = NULL;
        }
        return TRUE;
    }
    return FALSE;
}

double CalculateCOD(double initialMass, double initialCOD, double addedCOD, double targetCOD)
{
    // 公式: f = (a * (e - b)) / (d - e)
    return (initialMass * (targetCOD - initialCOD)) / (addedCOD - targetCOD);
}

bool ValidateInput(HWND hDlg, double& initialMass, double& initialCOD, double& addedCOD, double& targetCOD)
{
    wchar_t buffer[256];
    bool valid = true;

    // 获取输入值
    if (GetDlgItemText(hDlg, IDC_INITIAL_MASS, buffer, 256) == 0)
    {
        MessageBox(hDlg, L"请输入初始质量", L"输入错误", MB_ICONWARNING);
        return false;
    }
    initialMass = _wtof(buffer);

    if (GetDlgItemText(hDlg, IDC_INITIAL_COD, buffer, 256) == 0)
    {
        MessageBox(hDlg, L"请输入初始COD", L"输入错误", MB_ICONWARNING);
        return false;
    }
    initialCOD = _wtof(buffer);

    if (GetDlgItemText(hDlg, IDC_ADDED_COD, buffer, 256) == 0)
    {
        MessageBox(hDlg, L"请输入加入液体COD", L"输入错误", MB_ICONWARNING);
        return false;
    }
    addedCOD = _wtof(buffer);

    if (GetDlgItemText(hDlg, IDC_TARGET_COD, buffer, 256) == 0)
    {
        MessageBox(hDlg, L"请输入目标COD", L"输入错误", MB_ICONWARNING);
        return false;
    }
    targetCOD = _wtof(buffer);

    // 验证数值有效性
    if (initialMass <= 0)
    {
        MessageBox(hDlg, L"初始质量必须大于0", L"输入错误", MB_ICONERROR);
        valid = false;
    }
    if (fabs(addedCOD - targetCOD) < 0.0001)
    {
        MessageBox(hDlg, L"加入液体COD不能等于目标COD", L"计算错误", MB_ICONERROR);
        valid = false;
    }

    return valid;
}
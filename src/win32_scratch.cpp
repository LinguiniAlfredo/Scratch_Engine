#include "windows.h"

static bool Running;
static BITMAPINFO BitmapInfo;
static void *BitmapMemory;
static HBITMAP BitmapHandle;
static HDC BitmapDeviceContext;

static void ResizeDIBSection(int width, int height)
{
    if (BitmapHandle) {
        DeleteObject(BitmapHandle);
    }
    if (!BitmapDeviceContext) {
        BitmapDeviceContext = CreateCompatibleDC(0);
    }

    BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
    BitmapInfo.bmiHeader.biWidth = width;
    BitmapInfo.bmiHeader.biHeight = height;
    BitmapInfo.bmiHeader.biPlanes = 1;
    BitmapInfo.bmiHeader.biBitCount = 32;
    BitmapInfo.bmiHeader.biCompression = BI_RGB;

    BitmapHandle = CreateDIBSection(
        BitmapDeviceContext, 
        &BitmapInfo,
        DIB_RGB_COLORS, 
        &BitmapMemory,
        0, 0);
}

static void UpdateWindow(HDC DeviceContext, int x, int y, int width, int height)
{
    StretchDIBits(
            DeviceContext, 
            x, y, width, height,
            x, y, width, height,
            BitmapMemory,
            &BitmapInfo,
            DIB_RGB_COLORS,
            SRCCOPY
    );
}

LRESULT MainWindowCallback(HWND Window, 
                           UINT Message, 
                           WPARAM wParam, 
                           LPARAM lParam)
{
    LRESULT Result = 0;
    switch(Message) {
        case WM_SIZE:
        {
            RECT ClientRect;
            GetClientRect(Window, &ClientRect);
            int w = ClientRect.right - ClientRect.left;
            int h = ClientRect.bottom - ClientRect.top;
            ResizeDIBSection(w, h);
        }break;

        case WM_DESTROY:
        {
            Running = false;
        }break;

        case WM_CLOSE:
        {
            Running = false;
        }break;

        case WM_ACTIVATEAPP:
        {
            OutputDebugStringA("WM_ActivateApp\n");
        }break;

        case WM_PAINT:
        {
            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(Window, &Paint);
            
            int x = Paint.rcPaint.left;
            int y = Paint.rcPaint.top;
            int width = Paint.rcPaint.right - Paint.rcPaint.left;
            int height = Paint.rcPaint.bottom - Paint.rcPaint.top;
            UpdateWindow(DeviceContext, x, y, width, height);

            EndPaint(Window, &Paint);
        } break;

        default:
        {
            // OutputDebugStringA("default\n");
            Result = DefWindowProc(Window, Message, wParam, lParam);
        }break;
    }
    return Result;
}

int WinMain(HINSTANCE Instance, 
            HINSTANCE PrevInstance, 
            LPSTR CommandLine, 
            int ShowCode)
{
    WNDCLASSA WindowClass = {};

    WindowClass.style = CS_OWNDC|CS_VREDRAW|CS_HREDRAW;
    WindowClass.lpfnWndProc = MainWindowCallback;
    WindowClass.hInstance = Instance;
    // WindowClass.hIcon = ;
    WindowClass.lpszClassName = "Scratch_Engine_Class";

    if(RegisterClassA(&WindowClass)) {
        HWND WindowHandle = CreateWindowExA(
            0,
            WindowClass.lpszClassName,
            "Scratch_Engine",
            WS_OVERLAPPEDWINDOW|WS_VISIBLE,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            0,
            0,
            Instance,
            0
        );
        if (WindowHandle) {
            Running = true;
            MSG Message;
            while(Running) {
                BOOL MessageResult = GetMessage(&Message, WindowHandle, 0, 0);
                if (MessageResult > 0) {
                    TranslateMessage(&Message);
                    DispatchMessage(&Message);
                } else {
                    break;
                }
            }
        } else {
            //TODO - error
        }
    } else {
        // TODO - error
    }

    return(0);
}

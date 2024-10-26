#include "windows.h"

static bool Running;

LRESULT MainWindowCallback(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam)
{
    LRESULT Result = 0;
    switch(Message) {
        case WM_SIZE:
            OutputDebugStringA("WM_Size\n");
            break;
        case WM_DESTROY:
	    Running = false;
            break;
        case WM_CLOSE:
	    Running = false;
            break;
        case WM_ACTIVATEAPP:
            OutputDebugStringA("WM_ActivateApp\n");
            break;
        case WM_PAINT:
        {
            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(Window, &Paint);
            
            int X = Paint.rcPaint.left;
            int Y = Paint.rcPaint.top;
            int Width = Paint.rcPaint.right - Paint.rcPaint.left;
            int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
            PatBlt(DeviceContext, X, Y, Width, Height, BLACKNESS); 

            EndPaint(Window, &Paint);
        } break;
        default:
            // OutputDebugStringA("default\n");
            Result = DefWindowProc(Window, Message, wParam, lParam);
            break;
    }
    return Result;
}

int WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CommandLine, int ShowCode)
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

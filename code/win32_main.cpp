 /* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Robert Wells $
   $Notice: (C) Copyright 2016 by Gandhi Games. All Rights Reserved. $
   ======================================================================== */

#include <windows.h>

#define internal static
#define local_persist static
#define global_variable static

//TODO(robert): Global for now. Make sure it is clear what can be touched. Global variables no not do that.
global_variable bool Running;

// Device Independent Bitmap.
internal void
Win32_ResizeDIBSection(int Width, int Height)
{
    
}

internal void
Win32_UpdateWindow(HWND Window, int X, int Y, int Width, int Height)
{
}

LRESULT CALLBACK
Win32_MainWindowCallback (HWND   Window,
                    UINT   Message,
                    WPARAM WParam,
                    LPARAM LParam)
{
    LRESULT Result = 0;
 
    switch(Message)
    {
        // Resize
        case WM_SIZE:
        {
            RECT ClientRect;
            GetClientRect(Window, &ClientRect);
            int Width = ClientRect.right - ClientRect.left;
            int Height = ClientRect.bottom - ClientRect.top;
            Win32_ResizeDIBSection(Width, Height);
            break;
        }
        // Deletes window
        case WM_DESTROY:
        {
            //TODO(robert): Handle as error and re-create window?
            Running = false;
            break;
        }

        // Close
        case WM_CLOSE:
        {
            //TODO(robert): Handle with msg to user.
            Running = false;
            break;
        }

        // Click in window
        case WM_ACTIVATEAPP:
        {
            OutputDebugStringA("WM_ACTIVATEAPP\n");
            break;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(Window, &Paint);
            int X = Paint.rcPaint.left;
            int Y = Paint.rcPaint.top;
            LONG Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
            LONG Width = Paint.rcPaint.right - Paint.rcPaint.left;
            Win32_UpdateWindow(Window, X, Y, Width, Height);
            EndPaint(Window, &Paint);
        }
        
        default:
        {
            Result = DefWindowProc(Window, Message, WParam, LParam);
            break;
        };
    }

    return Result;
    
}


int CALLBACK
WinMain(HINSTANCE Instance,
        HINSTANCE PrevInstance,
        LPSTR     CommandLine,
        int       ShowCode)
{
    WNDCLASS WindowClass = {};

    //TODO(robert): Check if HREDRAW/VREDRAW/OWNDC still matter.
    WindowClass.style = CS_OWNDC|CS_HREDRAW|CS_VREDRAW;
    WindowClass.lpfnWndProc =  Win32_MainWindowCallback;
    WindowClass.hInstance = Instance;
    //WindowClass.hIcon;
    WindowClass.lpszClassName = "C_engineWindowClass";

    int success = RegisterClass(&WindowClass);

    if(success)
    {
        HWND windowHandle = CreateWindowEx(
            0,
            WindowClass.lpszClassName,
            "C_engineWindowClass",
            WS_OVERLAPPEDWINDOW|WS_VISIBLE,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            0,
            0,
            Instance,
            0);

        if(windowHandle)
        {
            Running = true;
            while(Running)
            {
                MSG Message;
                BOOL MsgResult = GetMessage(&Message, 0, 0, 0);

                if(MsgResult > 0)
                {
                    TranslateMessage(&Message);
                    DispatchMessage(&Message);
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            //TODO(robert): Handle getting window handle failure.
            OutputDebugStringA("Failed to get window handle");
        }
    }
    else
    {
        //TODO(robert): Handle window registration failure.
        OutputDebugStringA("Failed to register window");
    }
    
    return(0); 
}

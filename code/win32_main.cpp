 /* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Robert Wells $
   $Notice: (C) Copyright 2016 by Gandhi Games. All Rights Reserved. $
   ======================================================================== */

#include <windows.h>

LRESULT CALLBACK
MainWindowCallback (HWND   window,
                    UINT   message,
                    WPARAM wParam,
                    LPARAM lParam)
{
    LRESULT result = 0;
 
    switch(message)
    {
        // Resize
        case WM_SIZE:
        {
            OutputDebugStringA("WM_SIZE\n");
            break;
        }
        // Deletes window
        case WM_DESTROY:
        {
            OutputDebugStringA("WM_DESTROY\n");
            break;
        }

        // Close
        case WM_CLOSE:
        {
            OutputDebugStringA("WM_CLOSE\n");
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
            PAINTSTRUCT paint;
            HDC deviceContext = BeginPaint(window, &paint);
            int x = paint.rcPaint.left;
            int y = paint.rcPaint.top;
            LONG height = paint.rcPaint.bottom - paint.rcPaint.top;
            LONG width = paint.rcPaint.right - paint.rcPaint.left;
            static DWORD operation = WHITENESS;  
            PatBlt(deviceContext, x, y, width, height, operation);


            if(operation == BLACKNESS)
            {
                operation = WHITENESS;
            }
            else
            {
                operation = BLACKNESS;
            }
              
            EndPaint(window, &paint);
        }
        
        default:
        {
            //   OutputDebugStringA("default\n");
            result = DefWindowProc(window, message, wParam, lParam);
            break;
        };
    }

    return result;
    
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
    WindowClass.lpfnWndProc =  MainWindowCallback;
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
            MSG message;

            for(;;)
            {
                BOOL msgResult = GetMessage(&message, 0, 0, 0);

                if(msgResult > 0)
                {
                    TranslateMessage(&message);

                    DispatchMessage(&message);
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

/* ==========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Mitko Stojanov $
   $Notice: (C) Copyright 2024 by Mitko Stojanov, Inc. All Rights Reserved. $
   ========================================================================== */

#include <windows.h>
#include <stdint.h>
#include <functional>
#include <iostream>
#include <fstream>
#include <string>
#include <SDL.h>

#define internal static
#define local_persist static
#define global_variable static

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef int32 bool32;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

struct win32_offscreen_buffer
{
    BITMAPINFO Info;
    void *Memory;
    int Width;
    int Height;
    int Pitch;
    int BytesPerPixel = 4;
};

struct win32_window_dimension
{
    int Width;
    int Height;
};

global_variable bool32 Running;
global_variable win32_offscreen_buffer GlobalBackbuffer;
global_variable SDL_GameController* controller;

internal void
LoadControllerMappings(const std::string& mappingFile)
{
    std::ifstream file(mappingFile);

    if (!file.is_open())
    {
        std::cerr << "Failed to open controller mapping file: " << mappingFile << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (SDL_GameControllerAddMapping(line.c_str()) == -1)
        {
            std::cerr << "Failed to add mapping: " << line << " - " << SDL_GetError() << std::endl;
        }
        else
        {
            std::cout << "Added mapping: " << line << std::endl;
        }
    }

    file.close();
}

internal void
Win32InitDSound(void)
{
    HMODULE DSoundLibrary = LoadLibraryA("dsound.dll");

    if(DSoundLibrary)
    {
        direct_sound_create *DirectSoundCreate = (direct_sound_create *)GetProcAddress(DSoundLibrary, "DirectSoundCreate");

        DIRECTSOUND *DirectSound;
        if(DirectSoundCreate && SUCCEEDED(DirectSoundCreate(0, &DirectSound, 0)))
        {
        }
        else
        {

        }
    }
    else
    {

    }
}

internal void
OpenController(int joystick_index)
{
    if (SDL_IsGameController(joystick_index))
    {
        controller = SDL_GameControllerOpen(joystick_index);
        if (controller)
        {
            std::cout << "Opened controller at index " << joystick_index << ": " << SDL_GameControllerName(controller) << std::endl;
        }
        else
        {
            std::cerr << "Could not open game controller at index " << joystick_index << ": " << SDL_GetError() << std::endl;
        }
    }
}

internal void
CloseController()
{
    if (controller)
    {
        std::cout << "Closing controller " << std::endl;
        SDL_GameControllerClose(controller);
        controller = nullptr;
    }
}

win32_window_dimension
Win32GetWindowDimension(HWND Window)
{
    win32_window_dimension Result;

    RECT ClientRect;
    GetClientRect(Window, &ClientRect);
    Result.Width = ClientRect.right - ClientRect.left;
    Result.Height = ClientRect.bottom - ClientRect.top;

    return(Result);
}

internal void
Win32RenderWeirdGradient(win32_offscreen_buffer *Buffer, int BlueOffset, int GreenOffset)
{
    uint8 *Row = (uint8 *)Buffer->Memory;
    int changed = 2;
    for(int Y = 0; Y < Buffer->Height; ++Y)
    {
        uint32 *Pixel = (uint32 *)Row;
        for(int X = 0; X < Buffer->Width; ++X)
        {
            changed++;
            uint8 Blue = (X + BlueOffset);
            uint8 Green = (Y + GreenOffset+changed);

            *Pixel++ = ((Green << 8) | Blue);
        }
        changed = Y+2;
        Row += Buffer->Pitch;
    }

}
internal void
Win32ResizeDIBSection(win32_offscreen_buffer *Buffer, int Width, int Height)
{
    // TODO(mite): bulletproof this.

    if(Buffer->Memory)
    {
        VirtualFree(Buffer->Memory, 0, MEM_RELEASE);
    }

    Buffer->Width = Width;
    Buffer->Height = Height;
    Buffer->BytesPerPixel = 4;

    Buffer->Info.bmiHeader.biSize = sizeof(Buffer->Info.bmiHeader);
    Buffer->Info.bmiHeader.biWidth = Buffer->Width;
    Buffer->Info.bmiHeader.biHeight = -Buffer->Height;
    Buffer->Info.bmiHeader.biPlanes = 1;
    Buffer->Info.bmiHeader.biBitCount = 32;
    Buffer->Info.bmiHeader.biCompression = BI_RGB;

    int BitmapMemorySize = (Buffer->Width*Buffer->Height)*Buffer->BytesPerPixel;
    Buffer->Memory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);

    Buffer->Pitch = Buffer->Width*Buffer->BytesPerPixel;
}

internal void
Win32DisplayBufferInWindow(win32_offscreen_buffer *Buffer,
                           HDC DeviceContext,
                           int WindowWidth, int WindowHeight)
{
    StretchDIBits(DeviceContext,
                  0, 0, WindowWidth, WindowHeight,
                  0, 0, Buffer->Width, Buffer->Height,
                  Buffer->Memory,
                  &Buffer->Info,
                  DIB_RGB_COLORS, SRCCOPY);

}

LRESULT CALLBACK
Win32MainWindowCallback(HWND Window,
  UINT Message,
  WPARAM WParam,
  LPARAM LParam
)
{
    LRESULT Result = 0;

    switch(Message)
    {
        case WM_SIZE:
        {
        } break;

        case WM_DESTROY:
        {
            Running = false;
            // TODO(mite): handle this error - reopen window
            OutputDebugStringA("WM_DESTROY\n");
        } break;

        case WM_CLOSE:
        {
            Running = false;
            // TODO(mite): handle with a message for the user
            OutputDebugStringA("WM_CLOSE\n");

        } break;

        case WM_ACTIVATEAPP:
        {
            OutputDebugStringA("WM_ACTIVATEAPP\n");
        } break;

        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            uint32 VKCode = WParam;
            bool32 WasDown = ((LParam & (1 << 30)) != 0);
            bool32 IsDown = ((LParam & (1 << 31)) == 0);
            if(WasDown != IsDown)
            {
                if(VKCode == 'W')
                {

                }
                else if(VKCode == 'A')
                {
                }
                else if(VKCode == 'S')
                {
                }
                else if(VKCode == 'D')
                {
                }
                else if(VKCode == 'Q')
                {
                }
                else if(VKCode == 'E')
                {
                }
                else if(VKCode == VK_UP)
                {
                }
                else if(VKCode == VK_DOWN)
                {
                }
                else if(VKCode == VK_LEFT)
                {
                }
                else if(VKCode == VK_RIGHT)
                {
                }
                else if(VKCode == VK_ESCAPE)
                {
                }
                else if(VKCode == VK_SPACE)
                {
                }
            }

            bool32 AltKeyWasDown = ((LParam & (1 << 29)) != 0);
            if(VKCode == VK_F4 && AltKeyWasDown)
            {
                Running = false;
            }
        } break;

        case WM_PAINT:
        {
            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(Window, &Paint);
            int X = Paint.rcPaint.left;
            int Y = Paint.rcPaint.right;
            int Width = Paint.rcPaint.right - Paint.rcPaint.left;
            int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;

            win32_window_dimension Dimension = Win32GetWindowDimension(Window);

            Win32DisplayBufferInWindow(&GlobalBackbuffer,
                                       DeviceContext,
                                       Dimension.Width, Dimension.Height);
            EndPaint(Window, &Paint);
        } break;

        default:
        {
//            OutputDebugStringA("default\n");
            Result = DefWindowProc(Window, Message, WParam, LParam);
        } break;
    }

    return(Result);
}

int CALLBACK
WinMain(HINSTANCE Instance,
    HINSTANCE PrevInstance,
    LPSTR CommandLine,
    int ShowCode)
{

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    LoadControllerMappings("controller_mappings.txt");

    WNDCLASS WindowClass = {};

    Win32ResizeDIBSection(&GlobalBackbuffer, 1280, 720);

    WindowClass.style = CS_HREDRAW|CS_VREDRAW;
    WindowClass.lpfnWndProc = Win32MainWindowCallback;
    WindowClass.hInstance = Instance;
//    WindowClass.hIcon = ;
    WindowClass.lpszClassName = "ManInTheBoxWindowClass";

    if(RegisterClassA(&WindowClass))
    {
        HWND Window =
            CreateWindowEx(
                0,
                WindowClass.lpszClassName,
                "Man in the boX",
                WS_OVERLAPPEDWINDOW|WS_VISIBLE,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                0,
                0,
                Instance,
                0);
        if(Window)
        {
            Running = true;
            int XOffset = 0;
            int YOffset = 0;

            HDC DeviceContext = GetDC(Window);

            Win32InitDSound();

            while(Running)
            {
                MSG Message;

                while(PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
                {
                    if(Message.message == WM_QUIT)
                    {
                        Running = false;
                    }

                    TranslateMessage(&Message);
                    DispatchMessageA(&Message);
                }

                if(!controller)
                {
                    int numJoysticks = SDL_NumJoysticks();
                    for (int i = 0; i < numJoysticks; ++i)
                    {
                        if (SDL_IsGameController(i))
                        {
                            OpenController(i);
                            if (controller)
                            {
                                break;
                            }
                        }
                    }
                }
                else
                {
                    // Handle SDL Events
                    SDL_Event event;
                    while (SDL_PollEvent(&event))
                    {

                        if(event.type == SDL_CONTROLLERDEVICEADDED)
                        {
                            int joystick_index = event.cdevice.which;
                            std::cout << "Controller added at index " << joystick_index << std::endl;
                            OpenController(joystick_index);
                            break;
                        }

                        if(event.type == SDL_CONTROLLERDEVICEREMOVED)
                        {
                            int instance_id = event.cdevice.which;
                            std::cout << "Controller removed" << std::endl;
                            CloseController();
                            break;
                        }

                        if (event.type == SDL_CONTROLLERBUTTONDOWN)
                        {
                            if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP)
                            {
                                YOffset += 20;
                                std::cout << "D-pad Up pressed." << std::endl;
                            }
                            else if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
                            {
                                YOffset -= 20;
                                std::cout << "D-pad Down pressed." << std::endl;
                            }
                        }
                    }
                }

                Win32RenderWeirdGradient(&GlobalBackbuffer, XOffset, YOffset);

                win32_window_dimension Dimension = Win32GetWindowDimension(Window);

                Win32DisplayBufferInWindow(&GlobalBackbuffer, DeviceContext, Dimension.Width, Dimension.Height);

                ++XOffset;
                //YOffset += 2;

            }

            if (controller)
            {
                CloseController();
            }
        }
        else
        {
            // TODO(mite): Logging
        }

    }
    else
    {
        // TODO(mite): Logging
    }
    SDL_Quit();
    return(0);
}

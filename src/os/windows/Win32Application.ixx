/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module;
#include "Libraries.h"
#include "Win32Libraries.h"
export module app.win32;

import app;

export class Win32Application {
public:
    static int run(
        const std::shared_ptr<Application>& app,
        UINT width, UINT height,
        const std::wstring& name,
        HINSTANCE hInstance,
        int nCmdShow);
    static HWND getHwnd() { return hwnd; }
    static auto& getApp() { return app; }

private:
    static constexpr auto ID_VULKAN{1001};
    static constexpr auto ID_DIRECTX{1002};

    static HWND hwnd;
    static std::shared_ptr<Application> app;

    static bool dirExists(const std::string& dirName_in);
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK SelectorWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static vireo::Backend backendSelectorDialog(HINSTANCE hInstance, const std::wstring& title);
};

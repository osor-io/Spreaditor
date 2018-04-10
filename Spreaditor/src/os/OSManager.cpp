#include "OSManager.h"
#include "../window/WindowManager.h"
#include "Windows.h"

OSManager::OSManager()
{
}


OSManager::~OSManager()
{
}

void OSManager::start_up() {

    assert(WindowManager::get().get_window_handle());

}

void OSManager::shut_down() {}

std::string OSManager::user_open_file(const char* filters) {

    auto dialog_data = OPENFILENAME{};
    char filename_buffer[MAX_OS_FILENAME_SIZE];
    auto& window_handle = WindowManager::get().get_window_handle();
    auto file_handle = HANDLE{};

    ZeroMemory(&dialog_data, sizeof(dialog_data));
    dialog_data.lStructSize = sizeof(dialog_data);
    dialog_data.hwndOwner = window_handle;
    dialog_data.lpstrFile = filename_buffer;
    dialog_data.lpstrFile[0] = '\0';
    dialog_data.nMaxFile = MAX_OS_FILENAME_SIZE;
    dialog_data.lpstrFilter = filters;
    dialog_data.nFilterIndex = 1;
    dialog_data.lpstrFileTitle = NULL;
    dialog_data.nMaxFileTitle = 0;
    dialog_data.lpstrInitialDir = NULL;
    dialog_data.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    auto return_string = std::string{};

    if (GetOpenFileName(&dialog_data) == TRUE) {
        return_string.append(filename_buffer);
    }

    return std::move(return_string);
}

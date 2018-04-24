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

	//// Executable Path
	{

		HMODULE hModule = GetModuleHandleW(NULL);
		WCHAR path[MAX_PATH];

		GetModuleFileNameW(hModule, path, MAX_PATH);

		char cpath[260];
		char DefChar = ' ';
		WideCharToMultiByte(CP_ACP, 0, path, -1, cpath, 260, &DefChar, NULL);

		m_executable_path = std::string(cpath);

		m_executable_path = m_executable_path.substr(0, m_executable_path.find_last_of("\\/")+1);

	}

}

void OSManager::shut_down() {}

std::string OSManager::user_open_file(const char* filters) {

	auto dialog_data = OPENFILENAME{};
	char filename_buffer[MAX_OS_FILENAME_SIZE];
	auto window_handle = WindowManager::get().get_window_handle();
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

std::vector<std::string> OSManager::user_open_files(const char * filters){
	auto filenames = std::vector<std::string>();
	
	constexpr auto bufer_size = MAX_OS_FILENAME_SIZE * 256;

	auto dialog_data = OPENFILENAME{};
	char filename_buffer[bufer_size];
	auto window_handle = WindowManager::get().get_window_handle();
	auto file_handle = HANDLE{};

	ZeroMemory(&dialog_data, sizeof(dialog_data));
	dialog_data.lStructSize = sizeof(dialog_data);
	dialog_data.hwndOwner = window_handle;
	dialog_data.lpstrFile = filename_buffer;
	dialog_data.lpstrFile[0] = '\0';
	dialog_data.nMaxFile = bufer_size;
	dialog_data.lpstrFilter = filters;
	dialog_data.nFilterIndex = 1;
	dialog_data.lpstrFileTitle = NULL;
	dialog_data.nMaxFileTitle = 0;
	dialog_data.lpstrInitialDir = NULL;
	dialog_data.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER;

	assert(GetOpenFileName(&dialog_data) == TRUE);

	char* buffer = filename_buffer;
	std::string directory = buffer;
	buffer += (directory.length() + 1);
	while (*buffer) {
		std::string filename = buffer;
		buffer += (filename.length() + 1);
		filenames.push_back(std::move(filename));
	}

	return filenames;
}

std::string OSManager::user_save_file(const char* filters) {

	auto dialog_data = OPENFILENAME{};
	char filename_buffer[MAX_OS_FILENAME_SIZE];
	auto window_handle = WindowManager::get().get_window_handle();
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
	dialog_data.Flags = OFN_PATHMUSTEXIST;

	auto return_string = std::string{};

	if (GetSaveFileName(&dialog_data) == TRUE) {
		return_string.append(filename_buffer);
	}

	return std::move(return_string);
}

std::string OSManager::executable_path() {
	return m_executable_path;
}

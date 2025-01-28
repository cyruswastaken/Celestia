#pragma once
#include <windows.h>
#include <tlhelp32.h>
#include <cstdint>
#include <string_view>

class Memory
{
private:
	std::uintptr_t processId = 0;
	void* processHandle = nullptr;
public:

	Memory(const std::string_view processName)noexcept
	{
		::PROCESSENTRY32 entry = { };
		entry.dwSize = sizeof(::PROCESSENTRY32);

		const auto processSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		while (::Process32Next(processSnapshot, &entry))
		{
			if (!processName.compare(entry.szExeFile))
			{
				processId = entry.th32ProcessID;
				processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId); //"PROCESS_ALL_ACCESS" makes this cheat detected for the games having anticheat.
				break;
			}
		}
		//close the handle
		if (processSnapshot)
		{
			::CloseHandle(processSnapshot);
		}
	}
	~Memory()
	{
		if (processHandle)
		{
			::CloseHandle(processHandle);
		}
	}

	const std::uintptr_t GetModuleAddress(const std::string_view moduleName)const noexcept
	{
		::MODULEENTRY32 entry = { };
		entry.dwSize = sizeof(::MODULEENTRY32);

		const auto processSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);

		std::uintptr_t moduleBaseAddress = 0;
		while (::Module32Next(processSnapshot, &entry))
		{
			if (!moduleName.compare(entry.szModule))
			{
				moduleBaseAddress = reinterpret_cast<std::uintptr_t>(entry.modBaseAddr);
			}
		}
		if (processSnapshot)
			::CloseHandle(processSnapshot);
		return moduleBaseAddress;
	}


	template <typename T>
	constexpr const T Read(const std::uintptr_t& address) const noexcept
	{
		T value = {};
		::ReadProcessMemory(processHandle, reinterpret_cast<const void*>(address), &value, sizeof(T), NULL);
		return value;
	}

	template <typename T>
	constexpr void Write(const std::uintptr_t& address, const T& value) const noexcept
	{
		::WriteProcessMemory(processHandle, reinterpret_cast<void*>(address), &value, sizeof(T), NULL);
	}
};






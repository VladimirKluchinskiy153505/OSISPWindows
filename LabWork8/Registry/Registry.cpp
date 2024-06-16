#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>
#include <fstream>

void ExportRegistryKey(HKEY hKey, const std::wstring& keyName, const std::wstring& outputPath) {
    // Open the registry key
    HKEY hSubKey;
    if (RegOpenKeyEx(hKey, keyName.c_str(), 0, KEY_READ, &hSubKey) != ERROR_SUCCESS) {
        std::wcerr << L"Error opening registry key: " << keyName << std::endl;
        return;
    }

    // Create or open the output file
    std::wofstream outFile(outputPath);
    if (!outFile.is_open()) {
        std::wcerr << L"Error opening output file: " << outputPath << std::endl;
        RegCloseKey(hSubKey);
        return;
    }

    // Export the registry key to the output file
    outFile << L"Windows Registry Editor Version 5.00" << std::endl;
    outFile << std::endl;
    outFile << L"[" << keyName << L"]" << std::endl;

    const DWORD bufferSize = 1024;
    WCHAR valueName[bufferSize];
    DWORD valueNameSize = bufferSize;
    DWORD valueType;
    BYTE valueData[bufferSize];
    DWORD valueDataSize = bufferSize;

    for (DWORD i = 0; RegEnumValue(hSubKey, i, valueName, &valueNameSize, nullptr, &valueType, valueData, &valueDataSize) == ERROR_SUCCESS; ++i) {
        outFile << L"\"" << valueName << L"\"=";

        switch (valueType) {
        case REG_SZ:
            outFile << L"\"" << reinterpret_cast<WCHAR*>(valueData) << L"\"";
            break;
        case REG_DWORD:
            outFile << L"dword:" << *reinterpret_cast<DWORD*>(valueData);
            break;
            // Add additional cases for other value types if needed

        case REG_QWORD:
            outFile << L"qword:" << *reinterpret_cast<unsigned long long int*>(valueData);
            break;
            // Add additional cases for other value types if needed
        default:
            outFile <<valueData;
            break;
        }

        outFile << std::endl;

        // Reset the buffer sizes for the next iteration
        valueNameSize = bufferSize;
        valueDataSize = bufferSize;
    }

    // Close the registry key and the output file
    RegCloseKey(hSubKey);
    outFile.close();
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <HKEY_ROOT> <KEY_NAME> <OUTPUT_FILE>" << std::endl;
        return 1;
    }

    // Parse command line arguments
    HKEY hRootKey;
    if (std::string(argv[1]) == "HKEY_CLASSES_ROOT") {
        hRootKey = HKEY_CLASSES_ROOT;
    }
    else if (std::string(argv[1]) == "HKEY_CURRENT_USER") {
        hRootKey = HKEY_CURRENT_USER;
    }
    else if (std::string(argv[1]) == "HKEY_LOCAL_MACHINE") {
        hRootKey = HKEY_LOCAL_MACHINE;
    }
    else if (std::string(argv[1]) == "HKEY_USERS") {
        hRootKey = HKEY_USERS;
    }
    else if (std::string(argv[1]) == "HKEY_CURRENT_CONFIG") {
        hRootKey = HKEY_CURRENT_CONFIG;
    }
    else {
        std::cerr << "Invalid root key specified. Use HKEY_LOCAL_MACHINE or HKEY_CURRENT_USER." << std::endl;
        return 1;
    }

    std::wstring keyName = L""; // Convert char* to wstring
    keyName.assign(argv[2], argv[2] + strlen(argv[2]));

    std::wstring outputPath = L""; // Convert char* to wstring
    outputPath.assign(argv[3], argv[3] + strlen(argv[3]));

    // Export the registry key
    ExportRegistryKey(hRootKey, keyName, outputPath);

    std::wcout << L"Registry key exported successfully to: " << outputPath << std::endl;

    return 0;
}
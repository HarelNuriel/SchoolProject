#pragma once
#include<string>

#ifdef AES128_EXPORTS
#define AES128_API __declspec(dllexport)
#else
#define AES128_API __declspec(dllimport)
#endif


extern "C" __declspec(dllexport) void AES_Encrypt(const std::string fileContent, const std::string inputKey);

extern "C" __declspec(dllexport) void AES_Decrypt(const std::string fileContent, const std::string inputKey);

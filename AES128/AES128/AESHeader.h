#pragma once
#include<string>

#ifdef AES128_EXPORTS
#define AES128_API __declspec(dllexport)
#else
#define AES128_API __declspec(dllimport)
#endif


extern "C" AES128_API void AES_Encrypt(const std::string & fileContent, const std::string & inputKey);

extern "C" AES128_API void AES_Decrypt(const std::string & fileContent, const std::string & inputKey);

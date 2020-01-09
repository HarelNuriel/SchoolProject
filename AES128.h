#pragma once

#include <iostream>
#include <string.h>

class AES128
{
public:
	void AES_Decrypt(const std::string& encryptedFileContent, const std::string& inputKey);
	void AES_Encrypt(const std::string& encryptedFileContent, const std::string& inputKey);
};


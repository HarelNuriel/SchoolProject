#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string.h>

#define KEY_SIZE 176
#define NUMBER_OF_ROUNDS 9
#define BLOCK_LENGTH 16
#define COLUMN_LENGTH 4

class AES128
{
public:
	~AES128();
	std::string AES_Decrypt(const std::string& encryptedFileContent, const std::string& inputKey);
	std::string AES_Encrypt(const std::string& encryptedFileContent, const std::string& inputKey);

private:
	unsigned char* encryptedMessage;
	unsigned char* key;
	unsigned char* message;

};

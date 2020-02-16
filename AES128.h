#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string.h>
#include <vector>

#define KEY_SIZE 176
#define NUMBER_OF_ROUNDS 9
#define BLOCK_LENGTH 16
#define COLUMN_LENGTH 4

class AES128
{
public:
	~AES128();
	std::string AES_Decrypt(std::vector<unsigned char> fileContent, std::vector<unsigned char> key);
	std::string AES_Encrypt(std::vector<unsigned char> fileContent, std::vector<unsigned char> inputKey);

private:
	unsigned char* encryptedMessage;
	unsigned char* message;

};

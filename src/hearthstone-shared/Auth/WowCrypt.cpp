/***
 * Demonstrike Core
 */

#include "WowCrypt.h"
#include <algorithm>

WowCrypt::WowCrypt()
{
	_initialized = false;
}

void GenerateKey(uint8* key, uint8* K)
{
	const uint8 SeedKeyLen = 16;
	uint8 SeedKey[SeedKeyLen] = { 0x38, 0xA7, 0x83, 0x15, 0xF8, 0x92, 0x25, 0x30, 0x71, 0x98, 0x67, 0xB1, 0x8C, 0x4, 0xE2, 0xAA };

	uint8 *firstBuffer = new uint8[64];
	uint8 *secondBuffer = new uint8[64];

	memset(firstBuffer, 0x36, 64);
	memset(secondBuffer, 0x5C, 64);
    
	for(uint8 i = 0; i < SeedKeyLen; ++i)
	{
		firstBuffer[i] = (uint8)(SeedKey[i] ^ firstBuffer[i]);
		secondBuffer[i] = (uint8)(SeedKey[i] ^ secondBuffer[i]);
	}

	Sha1Hash sha1;
	sha1.UpdateData(firstBuffer, 64);
	sha1.UpdateData(K, 40);
	sha1.Finalize();

	uint8 *tempDigest = sha1.GetDigest();
	Sha1Hash sha2;
	sha2.UpdateData(secondBuffer, 64);
	sha2.UpdateData(tempDigest, 20);
	sha2.Finalize();

	memcpy(key, sha2.GetDigest(), 20);

	delete firstBuffer;
	delete secondBuffer;
}

void WowCrypt::Init(uint8 *K)
{
	_send_i = _send_j = _recv_i = _recv_j = 0;
	uint8 *newKey = new uint8[20];
	GenerateKey(newKey, K);

	_key.resize(20);
	std::copy(newKey, newKey+20, _key.begin());
	delete [] newKey;
	_initialized = true;
}

void WowCrypt::DecryptRecv(uint8 *data)
{
	if (!_initialized)
		return;

	uint8 x, KeySize = (uint8)_key.size();;

	// 0
	_recv_i %= KeySize;
	x = (data[0] - _recv_j) ^ _key[_recv_i];
	++_recv_i;
	_recv_j = data[0];
	data[0] = x;

	// 1
	_recv_i %= KeySize;
	x = (data[1] - _recv_j) ^ _key[_recv_i];
	++_recv_i;
	_recv_j = data[1];
	data[1] = x;

	// 2
	_recv_i %= KeySize;
	x = (data[2] - _recv_j) ^ _key[_recv_i];
	++_recv_i;
	_recv_j = data[2];
	data[2] = x;

	// 3
	_recv_i %= KeySize;
	x = (data[3] - _recv_j) ^ _key[_recv_i];
	++_recv_i;
	_recv_j = data[3];
	data[3] = x;

	// 4
	_recv_i %= KeySize;
	x = (data[4] - _recv_j) ^ _key[_recv_i];
	++_recv_i;
	_recv_j = data[4];
	data[4] = x;

	// 5
	_recv_i %= KeySize;
	x = (data[5] - _recv_j) ^ _key[_recv_i];
	++_recv_i;
	_recv_j = data[5];
	data[5] = x;
}

void WowCrypt::EncryptSend(uint8 *data)
{
	if (!_initialized)
		return;

	uint8 KeySize = (uint8)_key.size();

	_send_i %= KeySize;
	data[0] = _send_j = (data[0] ^ _key[_send_i]) + _send_j;
	++_send_i;

	_send_i %= KeySize;
	data[1] = _send_j = (data[1] ^ _key[_send_i]) + _send_j;
	++_send_i;

	_send_i %= KeySize;
	data[2] = _send_j = (data[2] ^ _key[_send_i]) + _send_j;
	++_send_i;

	_send_i %= KeySize;
	data[3] = _send_j = (data[3] ^ _key[_send_i]) + _send_j;
	++_send_i;
}

WowCrypt::~WowCrypt()
{
}

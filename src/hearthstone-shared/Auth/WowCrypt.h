/***
 * Demonstrike Core
 */

#pragma once

#include <stdlib.h>
#include "../Common.h"
#include "Sha1.h"
#include "BigNumber.h"
#include <vector>

class WowCrypt 
{
public:
	WowCrypt();
	~WowCrypt();

	void Init(uint8 *K);

	void DecryptRecv(uint8 * data);
	void EncryptSend(uint8 * data);

	bool IsInitialized() { return _initialized; }

private:
	std::vector<uint8> _key;
	uint8 _send_i, _send_j, _recv_i, _recv_j;
	bool _initialized;
};

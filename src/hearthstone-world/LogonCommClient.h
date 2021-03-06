/***
 * Demonstrike Core
 */

#pragma once

class LogonCommClientSocket : public TcpSocket
{
	uint32 remaining;
	uint16 opcode;
	RC4Engine _sendCrypto;
	RC4Engine _recvCrypto;
public:
	LogonCommClientSocket(SOCKET fd, const sockaddr_in * peer);
	~LogonCommClientSocket();

	void OnRecvData();
	void SendPacket(WorldPacket * data, bool no_crypto = false);
	void HandlePacket(WorldPacket & recvData);
	void SendPing(uint32 diff = 0);
	void SendChallenge();
	void HandleAuthResponse(WorldPacket & recvData);

	void HandleRegister(WorldPacket & recvData);
	void HandlePong(WorldPacket & recvData);
	void HandleServerPing(WorldPacket &recvData);
	void HandleSessionInfo(WorldPacket & recvData);
	void HandleRequestAccountMapping(WorldPacket & recvData);
	void UpdateAccountCount(uint32 account_id, int8 add);
	void HandleDisconnectAccount(WorldPacket & recvData);
	void HandleConsoleAuthResult(WorldPacket & recvData);
	void HandlePopulationRequest(WorldPacket & recvData);

	void OnDisconnect();
	void CompressAndSend(ByteBuffer & uncompressed);
	uint32 last_ping;
	uint32 last_pong;

	uint32 latency;
	uint32 _id;
	uint32 authenticated;
	bool use_crypto;
	set<uint32> realm_ids;
};

typedef void (LogonCommClientSocket::*logonpacket_handler)(WorldPacket&);

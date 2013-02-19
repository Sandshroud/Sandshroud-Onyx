/***
 * Demonstrike Core
 */

#pragma once

typedef std::list< Channel* > ChannelList;
typedef HM_NAMESPACE::hash_map<uint32, Channel*> ChannelMap;

class SERVER_DECL ChannelMgr :  public Singleton < ChannelMgr >
{
public:
	ChannelMgr();
	~ChannelMgr();

	Channel *GetCreateChannel(const char *name, Player* p, uint32 type_id);
	Channel *GetChannel(const char *name, Player* p);
	Channel * GetChannel(const char * name, uint32 team);
	Channel *GetChannel(uint32 id);
	void RemoveChannel(Channel * chn);
	bool seperatechannels;

private:
	//team 0: aliance, team 1 horde
	Mutex lock;
	uint32 m_idHigh;
	ChannelList Channels[2];
	ChannelMap m_idToChannel;
};

#define channelmgr ChannelMgr::getSingleton()

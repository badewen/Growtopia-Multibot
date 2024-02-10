#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

int distance(int x1, int y1, int x2, int y2) {
return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

struct Tile
{
	int fg, bg, x, y, flags;
};
struct DBox
{
	int x, y;
};

struct itemobj
{
	int itemid, amount, x, y;
	int uid = -1;
};
struct worldPlayer
{
	int netid, userid, x, y;
	string rawname;
	string name;
};

class World
{
public:
	World()
	{
	}
	int owner;
	std::vector<int> admins;
	int lastobjid = 0;
	int width;
	int height;
	int tilecount;
	std::string name;
	std::vector<Tile> m_tiles;
	std::vector<DBox> m_dboxes;

	std::vector<itemobj> m_objs;
	int gtmap[60][100];

	std::vector<worldPlayer> m_worldplayers;
	int getplayernetid(std::string findname) {
		for (const auto& obj : m_worldplayers)
		{
			if (obj.rawname == findname)
			{
				return obj.netid;
			}
		}
		return -1;
	}
	bool loaded = false;
	bool successload = false;
	itemobj getobj(int id)
	{
		for (auto obj : m_objs)
		{
			if (obj.uid == id)
			{
				return obj;
			}
		}
	}
	bool isobjinrange(int id, int botx, int boty)
	{
		for (auto obj : m_objs)
		{
			if (obj.uid == id)
			{
				int dis = distance(obj.x, obj.y, botx, boty);
				std::cout << "objisrange return, Object X: " << obj.x << " Object Y: " << obj.y << " Bot X: " << botx << " Bot Y: " << boty << endl;
				if (dis < (32 * 5))
				{
					return true;
				}
				else
					return false;
			}
		}
		return false;
	}
	void addobj(itemobj obj)
	{
		m_objs.push_back(obj);
	}
	void addobj(int item, int amount, int x, int y)
	{

		lastobjid++;
		cout << "add obj id " << lastobjid << endl;

		itemobj obj;
		obj.amount = amount;
		obj.itemid = item;
		obj.x = x;
		obj.y = y;
		obj.uid = lastobjid;
		m_objs.push_back(obj);
	}
	void removeobj(int id)
	{
		m_objs.erase(std::remove_if(m_objs.begin(), m_objs.end(), [=](itemobj& ele) {
			return ele.uid == id;
			}),
			m_objs.end());
	}
	void updateobj(int id, int amount)
	{
		for (auto& obj : m_objs)
		{
			if (obj.uid == id)
			{
				obj.amount = amount;
			}
		}
	}

	void deserialize(uint8_t* data)
	{
		this->reset();
		int pos = 0;
		auto name_length = *(short*)data;
		data += 2;
		string worldname((char*)data, name_length);
		data += name_length;

		int width = *(int*)data;
		data += 4;
		int height = *(int*)data;
		data += 4;
		int tilecount = *(int*)data;
		data += 4;
		printf("world name : %s size %dx%d %d\n", worldname.c_str(), width, height, tilecount);

		this->name = worldname;
		this->width = width;
		this->height = height;
		this->tilecount = tilecount;

		bool stop = false;
		data += 5;

		for (int i = 0; i < tilecount; i++)
		{
			if (stop)
				break;
			int x = i % width;
			int y = i / width;
			int fg = *(uint16_t*)data;
			data += 2;
			int bg = *(uint16_t*)data;
			data += 2;
			int lockindex = *(uint16_t*)data;
			data += 2;
			int flag = *(uint16_t*)data;
			data += 2;
			if (lockindex != 0)
				data += 2;
			if (fg == 3760)
			{
				data += 22; //bypass data bedrock TODO: move bypass to action type
				Tile asd;
				asd.fg = fg;
				asd.bg = bg;
				asd.x = x;
				asd.y = y;
				asd.flags = flag;
				m_tiles.push_back(asd);
				continue;

			}
			if (flag & 0x01)
			{
				int type = *(uint8_t*)data;
				data++;
				switch (type)
				{
				case 0:
					break;
				case 1:
				{ // door
					int len = *(uint16_t*)data;
					data += 2;
					string doorlabel((char*)data, len);
					data += len;
					data++;
				}
				break;
				case 2:
				{ // sign
					int len = *(uint16_t*)data;
					data += 2;
					string signlabel((char*)data, len);
					data += len;
					data += 4; // -1
				}
				break;
				case 3:
				{			// lock
					data++; // lock flag
					int owner = *(uint32_t*)data;
					data += 4;
					int adminsize = *(uint16_t*)data;
					data += 4;
					data += adminsize * 4; // or loop
					data += 8;
					if (fg == 5814)
					{ //guild lock
						data += 16;
					}
				}
				break;
				case 4:
				{ // tree
					data += 5;
				}
				break;
				case 6: // mailbox not blue
				case 7: // Bulletin board
					break;
				case 8: // dice block
					data++;
					break;
				case 9: //provider
					data += 4;
					break;
				case 10: // Achievement Block
					data += 5;
					break;
				case 11:
				{ // heart monitor
					data += 4;
					int len = *(uint16_t*)data;
					data += 2 + len;
					break;
				}
				case 14:
				{ // mannequin
					int len = *(uint16_t*)data;
					data += 25 + len;
					break;
				}
				case 15: 

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
				case 15: // bunny egg
					data += 4;
					break;
				case 16: //
					data++;
					break;
				case 18: //Xenonite Crystal
					data += 5;
					break;
				case 19:
				{
					if (fg == 9170)
					{ //transmute
						data += 18;
					}
					else
					{ //phonebooth
						data += 18;
					}
					break;
				}
				case 20:
				{ // all color crystal
					int len = *(uint16_t*)data;
					data += 2 + len;
					break;
				}
				case 21:
				{ //crime in proces
					int len = *(uint16_t*)data;
					data += 7 + len;
					break;
				}
				case 23: //display block
					data += 4;
					break;
				case 24: //vend
					data += 8;
					break;
				case 25:
				{ // Fish Tank Port
					data++;
					int len = *(uint16_t*)data;
					data += 4;
					data += 4 * len;
					break;
				}
				case 26: //  Solar Collector
					data += 5;
					break;
				case 27: // Forge
					data += 4;
					break;
				case 28: /// giving tree
					data += 6;
					break;
				case 30: //Steam Organ
					data += 5;
					break;
				case 31:
				{ // silkworm
					data++;
					int len = *(uint16_t*)data;
					data += 31 + len;
					break;
				}
				case 32: //Sewing Machine
					data += 4;
					break;
				case 33:
				{ //  challenge flag end
					if (fg == 3808 || fg == 3806)
					{
					}
					else if (fg == 3394)
					{
						int len = *(uint16_t*)data;
						data += 2 + len;
					}
					else
					{
					}
					break;
				}
				case 34: // Lobster Trap
					break;
				case 35:
				{ // Painting Easel
					data += 4;
					int len = *(uint16_t*)data;
					data += 2 + len;
					break;
				}
				case 36:
				{ // battle pet cage
					int len = *(uint16_t*)data;
					data += len + 14;
					break;
				}
				case 37:
				{ //  Pet Trainer
					int len = *(uint16_t*)data;
					data += 2 + len + 32;
					break;
				}
				case 38: // Steam Engine
					data += 4;
					break;
				case 39: // locke bot
					data += 4;
					break;
				case 40: // st partrick weather | background weather
					data += 4;
					break;
				case 41: // Spirit Storage Unit
					data += 4;
					break;
				case 42:
				{ // data bedrock
					data++;
					if (fg == 6546)
						data += 28; //starship hull
					break;
				}
				case 43:
				{ // display shelf
					data += 16;
					break;
				}
				case 44:
				{ //vip entrance
					data++;
					data += 4;
					int adminsize = *(uint16_t*)data;
					data += 4;
					data += (adminsize * 4);
					break;
				}

				case 47:
				{ // fish wall mount
					int len = *(uint16_t*)data;
					data += 7 + len;
					break;
				}
				case 48:
				{ //portrait
					int len = *(uint16_t*)data;
					data += 2;
					data += len;
					data += 26;
					break;
				}
				case 49: //stuff weather
					data += 9;
					break;
				case 50: // Fossil Prep Station
					data += 4;
					break;
				case 53:
					data += 8;
					break;
				case 54:
				{ // Storage Box Xtreme
					int itemsSize = *(uint16_t*)data;
					data += 2;
					data += itemsSize;
					break;
				}
				case 55:
				{ // Home Oven
					data += 4;
					int ss = *(uint8_t*)data;
					data += 16 + (4 * ss);
				}
				break;
				case 56:
				{ //  Audio Rack
					int len = *(uint16_t*)data;
					data += 2;
					data += len;
					data += 4;
					break;
				}
				case 57: // Geiger Charger
					data += 4;
					break;
				case 60:
					data += 5;
					break;
				case 61: // Training Port
					data += 35;
					break;
				case 62: // MAGPLANT 5000
					data += 14;
					break;
				case 63:
				{ // cybot
					int len = *(uint16_t*)data;
					data += 4;
					data += (len * 15);
					data += 8;
					break;
				}
				case 65: //all Guild Banner
					data += 17;
					break;
				case 66: // GrowScan 9000
					data++;
					break;
				case 69: //Tesseract Manipulator
					data += 16;
					break;
				case 72: // storm cloud
					data += 12;
					break;
				case 73: // Cracked Stone Slab
					data += 4;
					break;

				case 17: // game generator
				case 22: //  Spotlight
				case 45: // Challenge Timer
				case 51: // DNA Processor
				case 52: // Trickster ?
				case 58: // adventure begin
				case 59: // Tomb Robber
				case 74: //  Safe Vault
					break;

				default:
				{
					printf("not supported tile extra %d block %d x%d y%d \n", type, fg, x, y);
					stop = true; // avoid crash
				}
				break;
				}
			}
			else
			{
			}

			Tile tile;
			tile.fg = fg;
			tile.bg = bg;
			tile.x = x;
			tile.y = y;
			tile.flags = flag;
			if (tile.fg == 1452)
			{
				DBox asd;
				asd.x = x;
				asd.y = y;
				m_dboxes.push_back(asd);
				std::cout << "Found Dbox at X:" << x << " Y: " << y << endl;
			}
			m_tiles.push_back(tile);
		}
		data += 12;

		this->loaded = true;
		successload = !stop;
		if (!successload)
		{
			printf("fail to load world %s\n", worldname.c_str());
			return;
		}

		int dropsize = *(uint16_t*)data;
		data += 4;
		int lastdropid = *(uint16_t*)data;
		data += 4;
		this->lastobjid = lastdropid;
		for (int i = 0; i < dropsize; i++)
		{
			int id = *(uint16_t*)data;
			data += 2;
			float x = *(float*)data;
			data += 4;
			float y = *(float*)data;
			data += 4;
			int count = *(uint8_t*)data;
			data++;
			data++;
			int uid = *(uint16_t*)data;
			data += 4;
			itemobj obj;
			obj.amount = count;
			obj.itemid = id;
			obj.x = x;
			obj.y = y;
			obj.uid = uid;
			m_objs.push_back(obj);
		}
		data += 4;

		printf("world %s succesfuly loaded\n", worldname.c_str());
		printf("item obj size %d\n", m_objs.size());

	}

	void reset()
	{
		owner = -1;
		admins.clear();
		width = -1;
		height = -1;
		tilecount = -1;
		name = "";
		m_tiles.clear();
		m_dboxes.clear();
		loaded = false;
		successload = false;
		m_objs.clear();
	}
};	

#pragma once

struct buffStatus
{
	Status state;				//	���� ����
	double continueTimer;		//	���ӽð�
	bool isOver;				//	��������?

	buffStatus()
		: isOver(false), continueTimer(0.0)
	{
		memset(&state, 0, sizeof(Status));
	}

	void update_Buff(double delta)
	{
		continueTimer -= delta;

		if (continueTimer < 0.0)
			isOver = true;
	}
};

class StatusManager
{
private:
	const UINT MAX_BUFF_SIZE = 40;

private:
	vector<buffStatus> vBuffLsit;
	Status* mainStatus;
	Status* maxStatus;

	unordered_map<EQUIPTYPE, EquipBase*>* mEquip;

public:
	StatusManager(Status* cur, Status* max, unordered_map<EQUIPTYPE, EquipBase*>* equip)
		: mainStatus(cur), maxStatus(max), mEquip(equip)
	{
		vBuffLsit.reserve(MAX_BUFF_SIZE);
	}

	~StatusManager();

	void create(Status state, double conTime);
	void update(double delta);
	void allClear();
};
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

class BuffManager
{
private:
	const float MAX_BUFF_SIZE = 40;

private:
	vector<buffStatus> vBuffLsit;
	Status* mainStatus;
	Status* maxStatus;

public:
	BuffManager(Status* cur, Status* max)
		: mainStatus(cur), maxStatus(max) {}

	~BuffManager();

	void create(Status state, double conTime);
	void update(double delta);
	void allClear();
	//void 
};
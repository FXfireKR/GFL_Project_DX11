#pragma once
#include "EquipBase.h"
#include "singleton.hpp"

class EquipLoader : public singleton<EquipLoader>
{
private:
#ifdef _DEBUG
	const string EQUIP_DATA_PATH = "_TextTable/equip2.txt";
	const string EQUIP_IMAGE_PATH = "Equip/";
#else
	const string EQUIP_DATA_PATH = "_TextTable/equip2.txt";
	const string EQUIP_IMAGE_PATH = "Equip/";
#endif

	//	private Value
private:
	unordered_map<string, EquipBase*> mEquipDataBase;
	unordered_map<string, EquipBase*>::iterator EquipIterator;

	//	private Function
private:
	void release();
	void readDataBase();

public:
	EquipLoader();
	EquipLoader(const EquipLoader& other) = delete;				//	복사 연산자 삭제
	EquipLoader operator=(const EquipLoader& other) = delete;	//	대입 연산자 삭제
	~EquipLoader();


	inline const EquipBase* getEquipInfo(__in string _key){
		return (EquipIterator = mEquipDataBase.find(_key)) != mEquipDataBase.end() ? EquipIterator->second : nullptr;
	}

	inline const unordered_map<string, EquipBase*>& getEquipInfoBase() { 
		return mEquipDataBase; 
	}

};
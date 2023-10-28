#pragma once
#include "LevelData.h"

class LevelMediator {
private:
	LevelData currentLevelData;
	static LevelMediator* instance; // The static singleton instance

	LevelMediator() {} // Private constructor to ensure no other instances can be created.

public:
	static LevelMediator* getInstance() {
		if (!instance) {
			instance = new LevelMediator();
		}
		return instance;
	}

	void setLevelData(const LevelData& data) {
		currentLevelData = data;
	}

	LevelData getLevelData() const {
		return currentLevelData;
	}
};

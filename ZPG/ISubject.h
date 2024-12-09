#pragma once
#include <vector>
#include <memory>
#include "IObserver.h"

class ISubject {
private:
	std::vector<std::shared_ptr<IObserver>> observers;
public:
	ISubject() {}

	inline void addObserver(std::shared_ptr<IObserver> observer) {
		observers.push_back(observer);
	}
	inline void removeObserver(std::shared_ptr<IObserver> observer) {
		observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
	}

	inline void notifyObservers() {
		for (auto& observer : observers) {
			observer->updateObserver();
		}
	}
};
#pragma once
class Money {
private:
	int money;

public:
	Money(const int& StartMoney) :money(StartMoney) { }
	void Give(const int& amount) { money += amount; };
	void Lose(const int& amount) { money -= amount; };
	const int get() const { return money; };
};
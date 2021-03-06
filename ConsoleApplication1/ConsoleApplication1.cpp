// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <ctime>

struct sPos {
	sPos() { x = 0; y = 0; }
	sPos(int aX, int aY) { x = aX; y = aY; }
	int x;
	int y;
};

struct sSize
{
	sSize() { width = 0; height = 0; }
	sSize(int aW, int aH) { width = aW; height = aW; }
	int width;
	int height;
};

class sRect
{
	sPos pos;
	sSize size;
public:
	sRect() {};
	sRect(int x, int y, int w, int h) { this->pos.x = x; this->pos.y = y; this->size.width = w; this->size.height = h; }

	sPos get_Pos() { return pos; }
	sSize get_Size() { return size; }
	void set_Pos(int x,int y) { this->pos.x = x; this->pos.y = y;}
	void set_Size(int width, int height) { this->size.width = width; this->size.height = height;}
	bool intersects(const sRect& other) {
		return (((this->pos.x > other.pos.x + other.size.width)||(other.pos.x > this->pos.x + this->size.width))||
			((this->pos.y > other.pos.y + other.size.height )||(other.pos.y > this->pos.y + this->size.height)));
	}
	sRect &operator = (const sRect & other) {
		this->pos.x = other.pos.x; 
		this->pos.y = other.pos.y;
		this->size.width = other.size.width; 
		this->size.height = other.size.height;
		return *this;
	}
};

enum class eDirection {
	UP,
	LEFT,
	RIGHT,
	DOWN
};

class sCar {
	sRect rect;
	eDirection dir;
	int speed;
public:
	sRect get_Rect(){return rect;}
	void set_Rect(sRect rect){this->rect = rect;}
	eDirection get_Dir(){return dir;}
	void set_Dir(eDirection dir){this->dir = dir;}
	int get_Speed(){return speed;}
	void set_Speed(int speed){this->speed = speed;}

	virtual void move() {
		switch (dir) {
		case eDirection::UP:
			this->rect.set_Pos((this->rect.get_Pos().x + speed), this->rect.get_Pos().y);
			break;
		case eDirection::DOWN:
			this->rect.set_Pos((this->rect.get_Pos().x - speed), this->rect.get_Pos().y);
			break;
		case eDirection::RIGHT:
			this->rect.set_Pos(this->rect.get_Pos().x ,(this->rect.get_Pos().y - speed));
			break;
		case eDirection::LEFT:
			this->rect.set_Pos(this->rect.get_Pos().x, (this->rect.get_Pos().y + speed));
			break;
		}
	}
	sRect getFuturePos() {
		switch (this->dir) {
		case eDirection::UP:
			return sRect(rect.get_Pos().x, rect.get_Pos().y + speed, rect.get_Size().width, rect.get_Size().height);
		case eDirection::DOWN:
			return sRect(rect.get_Pos().x, rect.get_Pos().y - speed, rect.get_Size().width, rect.get_Size().height);
		case eDirection::RIGHT:
			return sRect(rect.get_Pos().x - speed, rect.get_Pos().y, rect.get_Size().width, rect.get_Size().width);
		case eDirection::LEFT:
			return sRect(rect.get_Pos().x + speed, rect.get_Pos().y, rect.get_Size().width, rect.get_Size().height);
		}
	}

	bool needPassOtherCar(sCar* otherCar) {
		bool result = false;
		eDirection otherdir = otherCar->dir;
		switch (this->dir) {
		case eDirection::UP:
			if (otherdir == eDirection::LEFT)
				result = true;
			break;
		case eDirection::DOWN:
			if (otherdir == eDirection::RIGHT)
				result = true;
			break;
		case eDirection::RIGHT:
			if (otherdir == eDirection::UP)
				result = true;
			break;
		case eDirection::LEFT:
			if (otherdir == eDirection::DOWN)
				result = true;
			break;
		}
		return result;
	}
};

struct sGasEngine : sCar {
	virtual int getFuel() { return fuel; }
	virtual void refill(int count) { fuel += count; }
	void move() { fuel--; sCar::move(); }
protected:
	int fuel;
};

struct sElectroCar : sCar {
	virtual int getFuel() { return charge; }
	virtual void refill(int count) { charge += count; }
	void move() { charge--; sCar::move(); }
protected:
	int charge;
};

struct sHybrid : sGasEngine, sElectroCar,sCar {
	void refill(int count) { charge += count / 2; fuel += count / 2; }
	int getFuel() { return charge + fuel; }
	void move() {
		if (rand() % 2 == 0)
			charge--;
		else
			fuel--;
		sCar::move();
	}
};

std::vector<sCar*> asdasd;
const int initialCarsCount = 10;
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

//checking that machines don't collide
void error_mas_1(sCar &car_1, sCar &car_2)
{
	try 
	{
		bool flag_for_x = ((car_1.get_Rect().get_Pos().x <= car_2.get_Rect().get_Pos().x) && 
			(car_1.get_Rect().get_Pos().x + 100 >= car_2.get_Rect().get_Pos().x));
		bool flag_for_y = ((car_1.get_Rect().get_Pos().y <= car_2.get_Rect().get_Pos().y) && 
			(car_1.get_Rect().get_Pos().y + 100 >= car_2.get_Rect().get_Pos().y));
		if (flag_for_x && flag_for_y)
		{
			throw 1;
		}
	}
	catch (int i)
	{
		std::cout << "error №" << i << " - cars ran over each other" << std::endl;
	}
}

void delete_same_car(std::vector<sCar*> &vector)
{
	bool flag = true;
	while (flag)
	{
		for (std::vector<sCar*>::iterator it = vector.begin(); it != vector.end(); it++)
		{
			for (std::vector<sCar*>::iterator it_1 = it+1; it_1 != vector.end(); it_1++)
			{
				if ((*it_1)->get_Dir() == (*it)->get_Dir())
				{
					vector.erase(it);
					flag = true;
					break;
				}
				else { flag = false; }
			}
			if (flag) { break; }
		}
		if (vector.size() <= 1) { flag = false; }
	}
}

void spawnCarFromTop()
{
	srand(time(NULL));
	sCar* car = new sCar();
	int carType = rand() % 3;
	if (carType == 0) {
		delete car;
		car = new sGasEngine();
	}
	else if (carType == 1) {
		delete car;
		car = new sElectroCar();
	}
	else if (carType == 2) {
		delete car;
		car = new sHybrid();
	}
	car->set_Rect(sRect(SCREEN_WIDTH / 2, 0, 100, 100));
	car->set_Speed(1);
	car->set_Dir(eDirection::UP);
	asdasd.push_back(car);
}

void spawnCarFromBot() {
	sCar* car = new sCar();
	int carType = rand() % 3;
	if (carType  == 0) {
		delete car;
		car = new sGasEngine();
	}
	else if (carType  == 1) {
		delete car;
		car = new sElectroCar();
	}
	else if (carType  == 2) {
		delete car;
		car = new sHybrid();
	}
	car->set_Rect(sRect(SCREEN_WIDTH / 4, SCREEN_HEIGHT, 100, 100));
	car->set_Speed(1);
	car->set_Dir(eDirection::DOWN);
	asdasd.push_back(car);
}

void SpawnCarFromLeft() {
	sCar* car = new sCar();
	int carType = rand() % 3;
	if (carType  == 0) {
		delete car;
		car = new sGasEngine();
	}
	else if (carType  == 1) {
		delete car;
		car = new sElectroCar();
	}
	else if (carType  == 2) {
		delete car;
		car = new sHybrid();
	}
	car->set_Rect(sRect(SCREEN_WIDTH, SCREEN_HEIGHT/2, 100, 100));
	car->set_Speed(1);
	car->set_Dir(eDirection::LEFT);
	asdasd.push_back(car);
}

void spawnCarFromRight() {
	sCar* car = new sCar();
	int carType = rand() % 3;
	if (carType  == 0) {
		delete car;
		car = new sGasEngine();
	}
	else if (carType == 1) {
		delete car;
		car = new sElectroCar();
	}
	else if (carType  == 2) {
		delete car;
		car = new sHybrid();
	}
	car->set_Rect(sRect(SCREEN_WIDTH, SCREEN_HEIGHT / 4, 100, 100));
	car->set_Speed(1);
	car->set_Dir(eDirection::RIGHT);
	asdasd.push_back(car);
}

void spawnCar() {
	int spawnCarfrom = rand() % 4;
	if (spawnCarfrom == 0)
		spawnCarFromRight();
	else if (spawnCarfrom == 1)
		spawnCarFromTop();
	else if (spawnCarfrom == 2)
		spawnCarFromBot();
	else if (spawnCarfrom == 3)
		SpawnCarFromLeft();
}
void main_loop() {
	while (true)
	{
		for (auto car = asdasd.begin(); car != asdasd.end(); car++) {
			for (auto car22 = asdasd.begin(); car22 != asdasd.end(); car22++) {
				if (((*car)->getFuturePos()).intersects((*car22)->getFuturePos())) {
					if (asdasd.size() == 4)
					{
						if ((*car)->get_Dir() == eDirection::DOWN)
						{
							(*car)->move();
							error_mas_1(**car, **car22);
						}
					}
					else if ((*car)->needPassOtherCar(*car22))
					{
						(*car)->move();
						error_mas_1(**car, **car22);
					}
					else
					{
						(*car22)->move();
						error_mas_1(**car22, **car);
					}
				}
			}
			//add new car if one of the cars has left
			if ((*car)->get_Rect().get_Pos().x < 0 || (*car)->get_Rect().get_Pos().x > SCREEN_WIDTH || 
				(*car)->get_Rect().get_Pos().y < 0 || (*car)->get_Rect().get_Pos().y > SCREEN_HEIGHT)
			{ 
				asdasd.erase(car);
				spawnCar();
				delete_same_car(asdasd);
				break;
			}
			if (asdasd.size() == 1)
			{
				spawnCar();
				delete_same_car(asdasd);
				break;
			}
		}
	}
}

int main(int argc, char** argv) {
	for (auto i = 0; i < initialCarsCount; ++i) {
		spawnCar();
	}
	delete_same_car(asdasd);
	main_loop();
	return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.

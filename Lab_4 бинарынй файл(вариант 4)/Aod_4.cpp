#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Car {
public:
	int number;//номер машины
	int brand;//код марки
	string name;//имя владельца
	bool isStolen;//в угоне или нет
	Car() {};
	Car(int number, int brand, bool isStolen, string name)//параметризованный конструктор
	{
		this->number = number;
		this->brand = brand;
		this->isStolen = isStolen;
		this->name = name;
	}
	int getNumber() { return number; }			//
	int getBrand() { return brand; }			//	ГЕТТЕРЫ
	string getName() { return name; }			//
	bool getIsStolen() { return isStolen; }		//

	void write(ostream& os) // Запись объекта в бинарный файл
	{
		os.write((char*)&number, sizeof(number)); // Запись номера машины
		os.write((char*)&brand, sizeof(brand)); // Запись марки
		os.write((char*)&isStolen, sizeof(isStolen));// Запись информации ою угоне
		uint32_t len = name.length() + 1; // Длина с нулевым байтом
		os.write((char*)&len, sizeof(len)); // Запись длины
		os.write((char*)name.c_str(), len); // Запись данных
	}
	bool read(istream& in) // Чтение объекта из бинарного файла
	{
		if (!in.good()) return false;
		in.read((char*)&number, sizeof(number)); // Чтение номера
		in.read((char*)&brand, sizeof(brand)); // Чтение марки
		in.read((char*)&isStolen, sizeof(isStolen));// Чтение информации об угоне
		if (!in.good()) return false;
		size_t len;// Переменная для длины
		in.read((char*)&len, sizeof(len)); // Чтение длины записанной строки
		if (!in.good()) return false;
		char* buf = new char[len]; // Выделение буфера для чтения
		in.read(buf, len); // Чтение (с нулевым байтом)
		name = buf; // Присвоение считанной строки члену
		delete[]buf; // Освобождение памяти
		return true;
	}
};

void AddToBinFile(string path, Car car)// Добавление в бинарный файл
{
	ofstream fout(path, ofstream::app);
	if (!fout.is_open())
	{
		cout << "Failed open";
	}

	car.write(fout);
	fout.close();
}
void GetFromBinFile(string path, vector<Car> *arr)// Получение вектора из бинарного файла
{
	ifstream fin;
	fin.open(path, ifstream::binary);
	if (!fin.is_open())
	{
		cout << "Failed open";
	}
	else {
		Car car;
		while (car.read(fin))
		{
			arr->push_back(car);
		}
	}
	fin.close();
}
void StolenList(string path, vector<Car> *arr) // составление вектора, состоящего из угнанных автомобилей
{
	vector<Car> *arrFull = new vector<Car>();
	GetFromBinFile(path, arrFull);
	for (int i = 0; i < arrFull->size(); i++)
	{
		if ((*arrFull)[i].getIsStolen())
		{
			arr->push_back((*arrFull)[i]);
		}
	}
}

bool isStolen(int number, string path) //Проверка конкретного номера машины на то, угнана она или нет
{
	ifstream fin(path, ofstream::binary);
	if (!fin.is_open())
	{
		cout << "Failed open";
	}
	else {
		Car car;
		while (car.read(fin))
		{
			if (car.getNumber() == number)
			{
				fin.close();
				return car.getIsStolen();
			}
		}
	}
	fin.close();
	return false;
}

int main()
{
	AddToBinFile("bin.bin", Car(1, 100, false, "driver1"));
	AddToBinFile("bin.bin", Car(2, 101, true, "driver2"));
	AddToBinFile("bin.bin", Car(3, 102, false, "driver3"));
	AddToBinFile("bin.bin", Car(4, 104, true, "driver4"));
	AddToBinFile("bin.bin", Car(5, 105, false, "driver5"));
	AddToBinFile("bin.bin", Car(6, 106, true, "driver6"));
	
	vector<Car> *arr = new vector<Car>();
	StolenList("bin.bin", arr);
	cout << "Stolen cars:\n";
	for (int i = 0; i < arr->size(); i++)
	{
		cout << (*arr)[i].getNumber() << " " << (*arr)[i].getBrand() << " " << (*arr)[i].getIsStolen() << "\n";
	}
	cout << "----------------------------------" << endl;
	cout << "is stolen 1: " << isStolen(1, "bin.bin") << "\n";
	cout << "is stolen 2: " << isStolen(2, "bin.bin");
	ofstream fout("bin.bin", ofstream::binary);
	fout.clear();
	fout.close();
}

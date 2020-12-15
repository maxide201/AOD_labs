#include <iostream>
#include <vector>

using namespace std;
//Задача о рюкзаке (Вариант 6)
//Метод решения: метод ветвей и границ

uint16_t maxWeight; //Максимальынй вес

class node //Узел в дереве всевозможных значений укладки предметов в рюкзак
{
private:
    uint16_t weight; //вес
    uint16_t cost; // стоимость
    vector<uint16_t> items; // прдметы, уложенные в рюкзак
    node* left; // левый потомк, в который не положили вещь
    node* right; // правый потомк, в котоырй положили вещь
public:
    node(uint16_t weight, uint16_t cost, vector<uint16_t> items) // параметризованный конструктор
    {
        this->weight = weight;
        this->cost = cost;
        this->items = items;
        left = right = nullptr;
    }
    node* Add(uint16_t weight, uint16_t cost, bool isTake, uint16_t item_number) // метод, добавляющий значение в дерево
    {
        if (isTake)
            if (this->weight + weight <= maxWeight) { // отесиваем далее ветку, если граница пройдена. Или кладём в рюкзак вещь, если она вмещается
                right = new node(this->weight + weight, this->cost + cost, this->items);
                right->items.push_back(item_number);
                return right;
            }
            else return nullptr;

        left = new node(this->weight, this->cost, this->items);
        return left;
    }

    uint16_t getCost() { return this->cost; }
    vector<uint16_t> getItems() { return this->items; }
};

node* maxNode = new node(0, 0, vector<uint16_t>()); // Узел с максимальным значенеим стоимости

void Enumeration(node* parent, vector<pair<uint16_t, uint16_t>> items, uint16_t i) // Рекурсивная функция перебора всех комбринаций, которые не выходят за границы
{   
    if (i > items.size()) return; // выход, когда все предметы уложены
    node* temp;
    temp = parent->Add(0, 0, false, i); // Не кладём предмет, выход за границу невозможен
    Enumeration(temp, items, i + 1);
    temp = parent->Add(items[i-1].first, items[i-1].second, true, i); // Кладём проедмет, если не выходит за границу, то проверяем не макисмальный ли он
    if (temp != nullptr)
    {
        if (temp->getCost() > maxNode->getCost())
            maxNode = temp;
        Enumeration(temp, items, i + 1);
    }
}

void main()
{
    auto items = vector<pair<uint16_t, uint16_t>>(); //е наши предметы
    cout << "Input max weight of backpack: ";
    cin >> maxWeight;
    uint16_t length; // кол-во предметов
    cout << "Input count of items: ";
    cin >> length;
    for (uint16_t i = 1; i <= length; i++) // вводим значения
    {
        items.push_back(pair<uint16_t, uint16_t>());
        cout << "\ninput item #" << i << ":\n\tWeight: ";
        cin >> items[i - 1].first;
        cout << "\tCost: ";
        cin >> items[i - 1].second;
    }
    Enumeration(maxNode, items, 1); //перебираем значения, в итоге максимальное окажется в узле maxNode
    
    auto result = maxNode->getItems();
    uint16_t result_length = result.size();
    cout << "\nResult items:\n {";
    for (uint16_t i = 0;  i < result_length; i++) //Вывод предметов, которые надо положить в рюкзак
        cout << " " << result[i] << " ";
    cout << "}";
}
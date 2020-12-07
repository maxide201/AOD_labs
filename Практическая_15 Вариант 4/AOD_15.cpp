#include <queue>
#include <vector>
#include <iostream>
#include <sstream>
using namespace std;

//ЗАДАНИЕ: Разработать процедуру сжатия данных на основе жадного алгоритма Хаффмана.
//СЛОЖНОСТЬ: o(nlogn)
struct Node {
    string value, code;
    unsigned amount;
    Node* left;
    Node* right;
    // компаратор
    bool operator() (const Node& x, const Node& y) const {
        return x.amount > y.amount;
    };
    // конструктор по умолчанию нужен для создания объекта-компаратора(чтобы реализовать очередь с приоритетом)
    Node(const string& value = "", unsigned amount = 0, Node* left = 0, Node* right = 0);
    //  объединение деревьев
    Node* join(Node x);
    // проход по дереву с генерацией кода
    void insert_code(string code);
  
    // строим дерево по алгоритму Хаффмана
    static Node* builder(priority_queue<Node, vector<Node>, Node> graph) {
        while (graph.size() > 1) {
            Node* n = new Node(graph.top());
            graph.pop();
            graph.push(*n->join(*new Node(graph.top())));
            graph.pop();
        }
        return new Node(graph.top());
    }
};
string codes[256];
unsigned amounts[256]; // массив счетчиков встречаемости символов
int main() {
    string s;
    cout << "Enter a string containing only letters from a to z: ";
    getline(std::cin, s); // читаем строку вместе с пробелами
    string fullStr = s;
    for (auto i : s) amounts[i]++;
    priority_queue<Node, vector<Node>, Node> graph;
    for (int i = 'a'; i <= 'z'; i++) // записываем в очередь с приоритетами 
        if (amounts[i] > 0) graph.emplace(s = (char)i, amounts[i]);
    Node* tree = Node::builder(graph);
    tree->insert_code("");
    cout << "\nResult: " << endl;
    for (int i = 0; i < 256; i++)
        if (codes[i] != "")
            cout << (char)i << " : " << codes[i] << endl;
    cout << fullStr << " = ";
    for (auto i : fullStr) 
        cout << codes[i];
}

// конструктор по умолчанию нужен для создания объекта-компаратора(чтобы реализовать очередь с приоритетом)
Node::Node(const string& value, unsigned amount, Node* left, Node* right) {
    this->value = value; // множество символом узла
    this->code = ""; // строковое представление битового кода узла
    this->amount = amount; // сколько раз втретилось
    this->left = left; // левый потомок
    this->right = right; // правый потомок
}
//  объединение деревьев
Node* Node :: join(Node x) {
    return new Node(x.value + value, x.amount + amount, new Node(x), this);
}
// проход по дереву с генерацией кода
void Node::insert_code(string code) {
    this->code = code;
    if (left != 0 || right != 0) {
        left->insert_code(code + "1");
        right->insert_code(code + "0");
    }
    else
    {
        codes[this->value[0]] = code;
    }
}

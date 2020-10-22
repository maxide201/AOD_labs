#include <iostream>
#include <array>
using namespace std;
class Book
{
private:
	unsigned long long isbn; // 12-значное число
	string author; // автор
	string title; // название
public:
	Book() {}
	Book(unsigned long long isbn, string author, string title)
	{
		this->isbn = isbn;
		this->author = author;
		this->title = title;
	}
	// Геттеры
	unsigned long long getIsbn() { return isbn; } 
	string getAuthor() { return author; }
	string getTitle() { return title; }
	bool isEmpty() { return !isbn; }
};

class Books
{
private:
	static const uint32_t SIZE = 13;// правило хэширования
	array<Book, SIZE> books; // список книг
	array<bool, SIZE> deleted; //список удалённых книг для правильнйо рабоыт двойного хэширвоания
	uint32_t h(unsigned long long key, uint32_t i) { return (h1(key) + i * h2(key)) % SIZE; } //двойное хэширование
	uint32_t h1(unsigned long long key) { return key % SIZE; } // первая хэш-функция
	uint32_t h2(unsigned long long key) { return (key % (SIZE - 2)) + 1; } //втора хэш-функция
public:
	Books()
	{
		Book emtpyBook(0, "unknown", "unknown");
		books.fill(emtpyBook);
		deleted.fill(false);
	}
	void add(Book book) // добавление книги
	{
		for (uint32_t i = 0; i < SIZE; i++)
		{
			uint32_t x = h(book.getIsbn(), i);
			if (books[x].isEmpty() || deleted[x])
			{
				books[x] = book;
				deleted[x] = false;
				return;
			}
		}
	}
	Book find(unsigned long long  key) //поиск книги
	{
		for (uint32_t i = 0; i < SIZE; i++)
		{
			uint32_t x = h(key, i);
			if (!books[x].isEmpty())
			{
				if (books[x].getIsbn() == key && !deleted[x])
					return books[x];
			}
			else
				return Book(0, "unknown", "unknown");
		}
		return Book(0, "unknown", "unknown");
	}
	void del(unsigned long long key) //удаление книги
	{
		for (uint32_t i = 0; i < SIZE; i++)
		{
			uint32_t x = h(key, i);
			if (!books[x].isEmpty())
			{
				if (books[x].getIsbn() == key)
				{
					deleted[x] = true;
					return;
				}
			}
			else
				return;
		}
	}
	void out() //вывод информации о книгах
	{
		for (uint32_t i = 0; i < SIZE; i++)
			if (!books[i].isEmpty() && !deleted[i])
				cout << "Books[" << i << "]:\n" << books[i].getAuthor() <<" " <<books[i].getTitle() << endl << endl;
	}
};
int main()
{
	Books books;
	books.add(Book(123456789012, "author1", "book1"));
	books.add(Book(111111111111, "author2", "book2"));
	books.add(Book(222222222222, "author3", "book3"));
	books.add(Book(333333333333, "author4", "book4"));
	cout << "Books list:\n";
	books.out();
	cout << "---------------------------------\n";
	books.del(111111111111);
	cout << "Books list:\n";
	books.out();
	cout << "---------------------------------\n";
	cout << "Find key(222222222222):\n";
	cout << books.find(222222222222).getAuthor()<< " " << books.find(222222222222).getTitle() << endl;
	cout << "---------------------------------\n";
	cout << "Find key(111111111111):\n";
	cout << books.find(111111111111).getAuthor() << " " << books.find(111111111111).getTitle() << endl;
	cout << "---------------------------------\n";
	return 0;
}
#include "all.h"
#include <sstream>
using namespace std;
#define PORT 12233
/*
 # 图书管理系统说明

 ## 作者
 曲振达

 ## 创建时间
 2025年01月14日

 ## 邮箱
 hackthestone@163.com

 ## 系统概述
 本图书管理系统是一个基于C++和网络编程的简单应用，用于管理图书信息，包括添加、删除、显示和保存图书数据。系统通过网络与客户端通信，支持多用户同时操作。

 ## 优点
 ### 优点（中文）
 1. **功能齐全**：支持添加、删除、显示和保存图书信息。
 2. **用户友好**：提供清晰的命令提示，用户操作简单。
 3. **网络支持**：支持多用户同时连接，通过网络进行数据交互。
 4. **数据持久化**：可以将图书信息保存到文件中，支持数据的持久化存储。
 5. **排序功能**：可以按价格对图书进行排序并显示。

 ### 优点（英文）
 1. **Comprehensive Features**: Supports adding, deleting, displaying, and saving book information.
 2. **User-Friendly**: Provides clear command prompts, making user operations simple.
 3. **Network Support**: Supports multiple users connecting simultaneously, with data interaction over the network.
 4. **Data Persistence**: Can save book information to files, supporting persistent data storage.
 5. **Sorting Function**: Can sort books by price and display them.

 ## 缺点
 ### 缺点（中文）
 1. **耦合度高**：`BookOS`类中的`books`列表与类的其他方法耦合度较高，不利于扩展和维护。
 2. **异常处理不足**：在网络通信和文件操作中，异常处理不够完善，可能导致程序在遇到错误时崩溃。
 3. **功能有限**：目前仅支持基本的图书管理功能，缺乏高级功能如搜索、分类等。
 4. **用户界面简单**：用户界面仅为命令行界面，不够友好，不支持图形用户界面（GUI）。

 ### 缺点（英文）
 1. **High Coupling**: The `books` list in the `BookOS` class is highly coupled with other methods, making it difficult to extend and maintain.
 2. **Insufficient Exception Handling**: Exception handling in network communication and file operations is not comprehensive, which may cause the program to crash when encountering errors.
 3. **Limited Features**: Currently only supports basic book management features, lacking advanced features such as search and categorization.
 4. **Simple User Interface**: The user interface is only a command-line interface, which is not user-friendly and does not support a graphical user interface (GUI).

 ## 要点
 ### 要点（中文）
 1. **数据结构**：使用`std::list`存储图书信息，支持动态添加和删除。
 2. **网络编程**：使用TCP协议进行网络通信，支持多用户连接。
 3. **文件操作**：使用文件流进行数据的读写，支持数据的持久化存储。
 4. **命令行交互**：通过命令行提示用户操作，提供简单的用户界面。
 5. **排序算法**：使用标准库中的`std::sort`函数对图书按价格进行排序。

 ### 要点（英文）
 1. **Data Structure**: Uses `std::list` to store book information, supporting dynamic addition and deletion.
 2. **Network Programming**: Uses the TCP protocol for network communication, supporting multiple user connections.
 3. **File Operations**: Uses file streams for data reading and writing, supporting persistent data storage.
 4. **Command-Line Interaction**: Provides a simple user interface through command-line prompts.
 5. **Sorting Algorithm**: Uses the `std::sort` function from the standard library to sort books by price.
*/
class Book{
public:
    string name;
    string publ;
    string isbn;
    string auth;
    int    num ;
    int   price;

    Book(){}
    Book(string name,string publ,string isbn,string auth,int num,int price) :name(name),publ(publ),isbn(isbn),auth(auth),num(num),price(price){}
    void show(){
        cout << "[" << this->name << "] {"<<this->isbn << "} [还剩 "<< this->num << "] [价格" <<this-> price <<" $]";
    }
    string shows(){
        ostringstream oss;
        oss << "《 "<< this->name <<" 》" << " 价格 " << this->price << " 剩余数量 " << this->num << endl;
        return oss.str();
    }
    // 序列化和反序列化
    void serialize(std::ofstream& out) const {
        out << name << std::endl;
        out << publ << std::endl;
        out << isbn << std::endl;
        out << auth << std::endl;
        out << num << std::endl;
        out << price << std::endl;
    }
    void deserialize(std::ifstream& in) {
            std::getline(in, name);
            std::getline(in, publ);
            std::getline(in, isbn);
            std::getline(in, auth);
            in >> num;
            in.ignore(); // 忽略换行符
            in >> price;
            in.ignore(); // 忽略换行符
    }
    void input() {
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //防缓冲区残留
            cout << "请输入书名: ";
            getline(cin, name);
            cout << "请输入出版社: ";
            getline(cin, publ);
            cout << "请输入ISBN: ";
            getline(cin, isbn);
            cout << "请输入作者: ";
            getline(cin, auth);
            cout << "请输入库存数量: ";
            while (!(cin >> num) || num < 0) {
                cout << "输入错误，请输入一个非负整数: ";
                cin.clear(); // 清除错误标志
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 忽略错误输入
            }
            cout << "请输入价格: ";
            while (!(cin >> price) || price < 0) {
                cout << "输入错误，请输入一个非负整数: ";
                cin.clear(); // 清除错误标志
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 忽略错误输入
            }
            cin.ignore(); // 清除输入缓冲区中的换行符
    }
};
bool compareBooks(const Book& b1, const Book& b2) {
    return b1.price < b2.price;
}

class BookOS{
public:
    int cnt;
    list<Book> books;//耦合变高了，但是不想增加复杂度了

    BookOS(){
        cnt = 0;
    }
    int lookup(string isbn){
        int c = 0;
        for(auto it = books.begin();it != books.end();++it){
            if(isbn == it->isbn){
                return c;
            }
            c++;
        }
        return -1;
    }
    int add(const Book b){
        if(lookup(b.isbn) != -1){
            return -1;
        }else{
            cnt++;
            books.push_back(b);
            return 1;
        }
    }
    int del(const Book b){
        int c = lookup(b.isbn);
        if(c < 0){
            return -1;
        }else{
            cnt--;
            auto it = books.begin();
            advance(it,c);
            books.erase(it);
        }
    }
    int del(const string isbn){
        int c = lookup(isbn);
        if(c < 0){
            return -1;
        }else{
            cnt--;
            auto it = books.begin();
            advance(it,c);
            books.erase(it);
        }
    }
    template <typename LIST>
    void show(LIST a){
        for(auto b : a){
            b.show();
            cout << endl;
        }
    }
    void show(){
        show(books);
    }
    void show_price(){
        int c=0;
        float v=0;
        for(auto it = books.begin();it != books.end();++it){
            v += it->price;
            c++;
        }
        v = v/c;
        cout << "平均价格" << v <<endl;
        vector<Book> tmp(books.begin(),books.end());
        cout << "排序" <<endl;
        sort(tmp.begin(),tmp.end(),compareBooks);
        show(tmp);
    }
};
class BOS{
public:
    static void hint(){
        cout << "1. ADD"<<endl;
        cout << "2. DEL"<<endl;
        cout << "3. SHOW ALL"<<endl;
        cout << "4. SHOW BY PRICE "<<endl;
        cout << "5. SAVE "<<endl;
        cout << "6. LOAD " <<endl;
    }
    static void oswarn(string a){
        cout << "\033[31m[OS Problem]\033[0m" << a <<endl;
    }
    static void osgood(string a){
        cout << "\033[32m[  Success ]\033[0m" << a << endl;
    }
};
BookOS bookos;

void saveBooksToFile(const list<Book>& books, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "无法打开文件: " << filename << endl;
        BOS::oswarn("保存所需的配置文件不存在");
        return;
    }

    for (const auto& book : books) {
        book.serialize(file);
    }

    file.close();
    BOS::osgood("保存成功");
}
std::list<Book> loadBooksFromFile(const std::string& filename) {
    std::list<Book> books;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "无法打开文件: " << filename << std::endl;
        BOS::oswarn("加载失败");
        return books;
    }

    Book book;
    while (true) {
        book.deserialize(file);
        if (file.eof()) break;
        books.push_back(book);
    }

    file.close();
    BOS::osgood("加载成功");
    return books;
}
#define BUFFERSIZE 128
class Server {
private:
	int port;
	int client,server;
	struct sockaddr_in serverADDr,clientADDr;
	socklen_t len;
public:
    bool sends(const string& data) {
        int sockfd = client;
        ssize_t sentBytes = send(sockfd, data.c_str(), data.size(), 0);
        if (sentBytes < 0) {
            std::cerr << "Error sending data." << std::endl;
            return false; // 发送失败
        }
        return true; // 发送成功
    }

	Server(int port){
		this->port = port;
		len = sizeof(clientADDr);
		server = socket(AF_INET, SOCK_STREAM, 0);
		if(server == -1){
				std::cerr << "Failed to create socket" << std::endl;
			exit(0);
		}
		memset(&serverADDr,0,sizeof(serverADDr));
		serverADDr.sin_family = AF_INET;
		serverADDr.sin_addr.s_addr = INADDR_ANY;
		serverADDr.sin_port = htons(port);

		if(bind(server,(struct sockaddr *)&serverADDr ,sizeof(serverADDr)) == -1){
						std::cerr << "Failed to bind" << std::endl;
						close(server);
						exit(0);
		}

		if(listen(server, 5) == -1){
						std::cerr << "Failed to listen" << std::endl;
						close(server);
						exit(0);
		}

		std::cout << "Server listen on port " << port << std::endl;

		while(true){
 			client = accept(server , (struct sockaddr *)&clientADDr , &len);
 			if(client == -1){
   					continue;
 			}
 			std::cout << "Connect with IP " << inet_ntoa(clientADDr.sin_addr) << std::endl;
 			char buf[BUFFERSIZE];
 			ssize_t nbytes;
 			const char* message = "Hello , I am server\n";
 			send(client , message , strlen(message) , 0);
 			while ( (nbytes = recv(client, buf, BUFFERSIZE, 0)) > 0 ) {
                string data(buf, nbytes);
                cout << "data: " << data <<endl;
                if(data == "ls\n"){
                    for(auto a : bookos.books){
                        sends(a.shows());
                    }
                    sends("ls is click");
                }else{
                    sends("? is click");
                }
 			}
 			if (nbytes == -1) {
				std::cerr << "disconnect .. " << std::endl;
				close(server);
            }
		}
	}
};

int main(){
	pid_t pid = fork();
	if(pid == -1){
		BOS::oswarn("Fork()失败");
		return 1;
	}else if(pid == 0){
		BOS::osgood("server start...");
		Server server(PORT);
	}else{
		while(1){
			BOS::hint();
			cout << "bookos >";
			int command;
			while (true) {
				cin >> command;

				if (cin.fail()) {
					cout << "错误输入";
					cin.clear();
					cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //防缓冲区残留
				} else {
					break;
				}
			}
			switch(command){
				case 1:{
					Book book;
					book.input();
					int bit = bookos.add(book);
					if(bit < 0){
						BOS::oswarn("添加失败，重复ISBN");
					}else{
						BOS::osgood("添加成功");
					}
					break; }
				case 2:{
					string isbn_tmp;
					cin >> isbn_tmp;
					int v2 = bookos.del(isbn_tmp);
					if(v2 < 0){
						BOS::oswarn("删除失败，没有这本书");
					}else{
						BOS::osgood("删除成功");
					}
					break; }
				case 3:{
					bookos.show();
					break; }
				case 4:{
					bookos.show_price();
					break; }
				case 5:{
					//save
					saveBooksToFile(bookos.books, "books.txt");
					break;
				}
				case 6:{
					//load
					bookos.books = loadBooksFromFile("books.txt");
					break;
				}
				default:{
					BOS::oswarn("没有这个功能");
					break; }
			}// 测试通过
		}
	}
}

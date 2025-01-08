#include <cinttypes>
#include <iostream>
#include <iterator>
#include <ostream>
#include <type_traits>
#include <vector>
#include <list>
#include <algorithm>
#include <limits>
#include <fstream>
using namespace std;
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
int main(){
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

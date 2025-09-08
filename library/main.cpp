#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

// Forward declaration
class Member;

class Book {
private:
    string id;
    string title;
    string author;
    bool isAvailable;

public:
    Book(string id, string title, string author)
        : id(id), title(title), author(author), isAvailable(true) {}

    string getId() const { return id; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    bool available() const { return isAvailable; }

    void borrowBook() { isAvailable = false; }
    void returnBook() { isAvailable = true; }
};

class Member {
private:
    string memberId;
    string name;
    vector<Book*> borrowedBooks;  // Raw pointers to books
    const int MAX_BORROW_LIMIT = 3;

public:
    Member(string id, string name) : memberId(id), name(name) {}

    string getId() const { return memberId; }
    string getName() const { return name; }

    bool borrowBook(Book* book) {
        if (borrowedBooks.size() >= MAX_BORROW_LIMIT || !book->available())
            return false;
        borrowedBooks.push_back(book);
        book->borrowBook();
        return true;
    }

    bool returnBook(Book* book) {
        for (auto it = borrowedBooks.begin(); it != borrowedBooks.end(); ++it) {
            if ((*it)->getId() == book->getId()) {
                borrowedBooks.erase(it);
                book->returnBook();
                return true;
            }
        }
        return false;
    }

    void listBorrowedBooks() const {
        cout << "Books borrowed by " << name << ":\n";
        for (const auto& book : borrowedBooks)
            cout << " - " << book->getTitle() << " by " << book->getAuthor() << "\n";
    }
};

class Library {
private:
    unordered_map<string, Book*> books;  // Raw pointers to books
    unordered_map<string, Member*> members;  // Raw pointers to members

public:
    ~Library() {
        // Clean up memory (manual deletion of allocated objects)
        for (auto& [id, book] : books)
            delete book;

        for (auto& [id, member] : members)
            delete member;
    }

    void addBook(string id, string title, string author) {
        books[id] = new Book(id, title, author);  // Dynamically allocated Book
    }

    void removeBook(string id) {
        delete books[id];  // Free the memory before erasing
        books.erase(id);
    }

    void registerMember(string id, string name) {
        members[id] = new Member(id, name);  // Dynamically allocated Member
    }

    Book* searchBookByTitle(string title) {
        for (auto& [id, book] : books) {
            if (book->getTitle() == title)
                return book;
        }
        return nullptr;
    }

    bool borrowBook(string memberId, string bookId) {
        if (members.count(memberId) && books.count(bookId)) {
            return members[memberId]->borrowBook(books[bookId]);
        }
        return false;
    }

    bool returnBook(string memberId, string bookId) {
        if (members.count(memberId) && books.count(bookId)) {
            return members[memberId]->returnBook(books[bookId]);
        }
        return false;
    }

    void showAllBooks() {
        cout << "\nLibrary Book List:\n";
        for (auto& [id, book] : books) {
            cout << book->getTitle() << " by " << book->getAuthor()
                 << (book->available() ? " [Available]" : " [Issued]") << "\n";
        }
    }
};

int main() {
    Library lib;

    // Adding books
    lib.addBook("B1", "The Hobbit", "J.R.R. Tolkien");
    lib.addBook("B2", "1984", "George Orwell");

    // Registering members
    lib.registerMember("U1", "Alice");
    lib.registerMember("U2", "Bob");

    // Borrow a book
    if (lib.borrowBook("U1", "B1")) {
        cout << "Book borrowed successfully.\n";
    } else {
        cout << "Book borrow failed.\n";
    }

    // Try to borrow the same book again
    if (!lib.borrowBook("U2", "B1")) {
        cout << "Book already issued.\n";
    }

    // Return the book
    lib.returnBook("U1", "B1");

    // Show all books
    lib.showAllBooks();

    return 0;
}

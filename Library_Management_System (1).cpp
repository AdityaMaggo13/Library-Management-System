// ============================================================================
//  PROJECT : LIBRARY MANAGEMENT SYSTEM  (Console based, C++)
//  SUBJECT : Object Oriented Programming (OOPS)
//  B.Tech 3rd Semester, MAIT (IPU), Delhi
// ----------------------------------------------------------------------------
//  OOP CONCEPTS SHOWN IN THIS FILE
//   1. Class and Object ..... Book, Member, Student, Faculty, Library
//   2. Encapsulation ........ private data + public getters / setters
//   3. Inheritance .......... Student and Faculty inherit from Member
//   4. Polymorphism ......... virtual functions (overriding)
//                             + searchBook() function overloading
//   5. Abstraction .......... abstract class Member (pure virtual functions)
//   6. Constructor / Destructor in every class
//   7. File Handling ........ books.txt and members.txt save data permanently
// ----------------------------------------------------------------------------
//  COMPILE : g++ Library_Management_System.cpp -o lms      RUN : ./lms
// ============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <cctype>

using namespace std;

// Helper: today's date as a day number (days since 1 Jan 1970). Used so that
// "days late" = (today - due day), which is simple subtraction of two numbers.
long long todayNumber() { return (long long)(time(0) / 86400); }

// Helper: splits one line of a file on the '|' character into its fields.
vector<string> splitLine(string line)
{
    vector<string> parts;
    string part = "";
    for (size_t i = 0; i < line.size(); i++)
    {
        if (line[i] == '|') { parts.push_back(part); part = ""; }
        else                  part += line[i];
    }
    parts.push_back(part);
    return parts;
}

// ============================================================================
//  CLASS 1 : Book                [CONCEPT: CLASS + ENCAPSULATION]
//  All data is private, so it can be read or changed only through the public
//  getter and setter functions given below.
// ============================================================================
class Book
{
private:
    int       bookId;      // unique number of the book
    string    title;       // name of the book
    string    author;      // writer of the book
    bool      issued;      // true = book is with some member
    int       issuedTo;    // id of member holding the book (-1 = nobody)
    long long dueDay;      // day number on which it must be returned

public:
    // [CONSTRUCTOR] runs automatically when we write:  Book b;
    Book() { bookId = 0; title = ""; author = ""; issued = false; issuedTo = -1; dueDay = 0; }

    // [CONSTRUCTOR OVERLOADING] second way to create an object (same name,
    // different parameters).
    Book(int id, string t, string a)
    {
        bookId = id; title = t; author = a;
        issued = false; issuedTo = -1; dueDay = 0;
    }

    // [DESTRUCTOR] runs automatically when the object is destroyed.
    ~Book() { }

    // ---------- GETTERS : safely read the private data ----------
    int       getBookId()   const { return bookId;   }
    string    getTitle()    const { return title;    }
    string    getAuthor()   const { return author;   }
    bool      isIssued()    const { return issued;   }
    int       getIssuedTo() const { return issuedTo; }
    long long getDueDay()   const { return dueDay;   }

    // ---------- SETTERS : safely change the private data ----------
    void setTitle(string t)  { title = t;  }
    void setAuthor(string a) { author = a; }

    void markIssued(int memberId, long long due) { issued = true; issuedTo = memberId; dueDay = due; }
    void markReturned()                          { issued = false; issuedTo = -1; dueDay = 0; }

    // Prints one book as a neat table row.
    void display() const
    {
        cout << "  " << left << setw(6) << bookId << setw(36) << title << setw(22) << author
             << setw(12) << (issued ? "Issued" : "Available") << setw(8) << (issued ? issuedTo : 0) << "\n";
    }
};

// ============================================================================
//  CLASS 2 : Member              [CONCEPT: ABSTRACTION]
//  Abstract class: it has pure virtual functions (= 0), so we can never make
//  an object of Member itself. It only says WHAT every member must be able to
//  do (max books, loan days, fine). The children do the real work.
// ============================================================================
class Member
{
private:                    // [ENCAPSULATION] data is hidden
    int    memberId;
    string name;
    string phone;

public:
    Member() { memberId = 0; name = ""; phone = ""; }              // [CONSTRUCTOR]
    Member(int id, string n, string p) { memberId = id; name = n; phone = p; }  // overloaded

    // [VIRTUAL DESTRUCTOR] needed because we delete members through Member*.
    virtual ~Member() { }

    // ---------- GETTERS / SETTERS ----------
    int    getMemberId() const { return memberId; }
    string getName()     const { return name;     }
    string getPhone()    const { return phone;    }
    void   setName(string n)   { name = n;        }
    void   setPhone(string p)  { phone = p;       }

    // ---------- PURE VIRTUAL FUNCTIONS = ABSTRACTION ----------
    virtual string getType()                       const = 0;  // "Student"/"Faculty"
    virtual int    getMaxBooks()                   const = 0;  // how many books
    virtual int    getLoanDays()                   const = 0;  // days allowed
    virtual double calculateFine(int daysLate)     const = 0;  // rupees to pay

    // [VIRTUAL FUNCTION] has a body, and can be overridden by a child class.
    virtual void display() const
    {
        cout << "  " << left << setw(6) << memberId << setw(22) << name
             << setw(14) << phone << setw(10) << getType() << "\n";
    }
};

// ============================================================================
//  CLASS 3 : Student     [CONCEPTS: INHERITANCE + POLYMORPHISM]
//  A Student "is a" Member, so it inherits Member and OVERRIDES the virtual
//  functions with its own rules: 2 books, 15 days, Rs. 2 per late day.
// ============================================================================
class Student : public Member
{
public:
    Student() : Member() { }                                     // [CONSTRUCTOR]
    Student(int id, string n, string p) : Member(id, n, p) { }   // base constructor
    ~Student() { }                                               // [DESTRUCTOR]

    // ---- function overriding: same name, same parameters, new body ----
    string getType()     const { return "Student"; }
    int    getMaxBooks() const { return 2;  }
    int    getLoanDays() const { return 15; }
    double calculateFine(int daysLate) const { return daysLate * 2.0; }  // Rs.2/day
};

// ============================================================================
//  CLASS 4 : Faculty     [CONCEPTS: INHERITANCE + POLYMORPHISM]
//  Same parent, different rules -> polymorphism. 5 books, 30 days, Rs. 5/day.
// ============================================================================
class Faculty : public Member
{
public:
    Faculty() : Member() { }                                     // [CONSTRUCTOR]
    Faculty(int id, string n, string p) : Member(id, n, p) { }
    ~Faculty() { }                                               // [DESTRUCTOR]

    string getType()     const { return "Faculty"; }
    int    getMaxBooks() const { return 5;  }
    int    getLoanDays() const { return 30; }
    double calculateFine(int daysLate) const { return daysLate * 5.0; }  // Rs.5/day
};

// ============================================================================
//  CLASS 5 : Library            [CONCEPT: one class managing many objects]
//  Keeps a list of books and a list of members, and saves/loads both from
//  text files. Members are stored as Member* (base class pointer), so one
//  list can hold both Students and Faculty -> runtime polymorphism.
// ============================================================================
class Library
{
private:
    vector<Book>    books;      // all books of the library
    vector<Member*> members;    // all members (Student + Faculty together)
    int nextBookId, nextMemberId;

    // ---------- FILE HANDLING : write both lists to disk ----------
    void saveBooks()
    {
        ofstream fout("books.txt");                       // open for writing
        for (size_t i = 0; i < books.size(); i++)
            fout << books[i].getBookId() << "|" << books[i].getTitle() << "|" << books[i].getAuthor()
                 << "|" << books[i].isIssued() << "|" << books[i].getIssuedTo() << "|"
                 << books[i].getDueDay() << "\n";
        fout.close();                                     // always close the file
    }

    void saveMembers()
    {
        ofstream fout("members.txt");
        for (size_t i = 0; i < members.size(); i++)
            fout << members[i]->getType() << "|" << members[i]->getMemberId() << "|"
                 << members[i]->getName() << "|" << members[i]->getPhone() << "\n";
        fout.close();
    }

    // ---------- FILE HANDLING : read both lists back ----------
    void loadBooks()
    {
        ifstream fin("books.txt");
        if (!fin) return;                                 // file not created yet
        string line;
        while (getline(fin, line))
        {
            if (line.empty()) continue;
            vector<string> p = splitLine(line);
            if (p.size() < 6) continue;

            Book b(atoi(p[0].c_str()), p[1], p[2]);
            if (p[3] == "1") b.markIssued(atoi(p[4].c_str()), atoll(p[5].c_str()));
            books.push_back(b);
            if (b.getBookId() >= nextBookId) nextBookId = b.getBookId() + 1;
        }
        fin.close();
    }

    void loadMembers()
    {
        ifstream fin("members.txt");
        if (!fin) return;
        string line;
        while (getline(fin, line))
        {
            if (line.empty()) continue;
            vector<string> p = splitLine(line);
            if (p.size() < 4) continue;

            Member* m;
            if (p[0] == "Faculty") m = new Faculty(atoi(p[1].c_str()), p[2], p[3]);  // [POLYMORPHISM]
            else                   m = new Student(atoi(p[1].c_str()), p[2], p[3]);

            members.push_back(m);
            if (m->getMemberId() >= nextMemberId) nextMemberId = m->getMemberId() + 1;
        }
        fin.close();
    }

    // ---------- small helpers ----------
    Book* findBook(int id)
    {
        for (size_t i = 0; i < books.size(); i++)
            if (books[i].getBookId() == id) return &books[i];
        return NULL;
    }
    const Book* findBook(int id) const          // 'const' copy, used by const functions
    {
        for (size_t i = 0; i < books.size(); i++)
            if (books[i].getBookId() == id) return &books[i];
        return NULL;
    }
    Member* findMember(int id) const
    {
        for (size_t i = 0; i < members.size(); i++)
            if (members[i]->getMemberId() == id) return members[i];
        return NULL;
    }
    bool titleMatches(string title, string word) const   // simple case-insensitive search
    {
        for (size_t k = 0; k < title.size(); k++) title[k] = tolower(title[k]);
        for (size_t k = 0; k < word.size();  k++) word[k]  = tolower(word[k]);
        return title.find(word) != string::npos;
    }
    void printBookHeader() const
    {
        cout << "\n  " << left << setw(6) << "ID" << setw(36) << "Title" << setw(22)
             << "Author" << setw(12) << "Status" << setw(8) << "By" << "\n  "
             << string(84, '-') << "\n";
    }

public:
    // [CONSTRUCTOR] loads old data from files when the program starts.
    Library()
    {
        nextBookId = 1; nextMemberId = 1;
        loadBooks();
        loadMembers();

        // First run only: add a little sample data so the menu is not empty.
        if (books.empty())
        {
            books.push_back(Book(nextBookId++, "Let Us C", "Yashavant Kanetkar"));
            books.push_back(Book(nextBookId++, "Object Oriented Programming in C++", "Robert Lafore"));
            books.push_back(Book(nextBookId++, "Data Structures Using C", "Reema Thareja"));
            saveBooks();
        }
        if (members.empty())
        {
            members.push_back(new Student(nextMemberId++, "Rahul Sharma", "9810012345"));
            members.push_back(new Faculty(nextMemberId++, "Dr. Meena Gupta", "9810098765"));
            saveMembers();
        }
    }

    // [DESTRUCTOR] frees the memory of every member object made with 'new'.
    ~Library()
    {
        for (size_t i = 0; i < members.size(); i++) delete members[i];
        members.clear();
    }

    // FEATURE 1 : ADD BOOK
    void addBook()
    {
        string title, author;
        cout << "\n  Enter book title  : "; getline(cin, title);
        cout << "  Enter author name : "; getline(cin, author);
        if (title.empty() || author.empty()) { cout << "  Title and author cannot be empty.\n"; return; }

        books.push_back(Book(nextBookId++, title, author));
        saveBooks();
        cout << "  Book added successfully. Book ID = " << (nextBookId - 1) << "\n";
    }

    // FEATURE 2 : REMOVE BOOK (an issued book cannot be removed)
    void removeBook()
    {
        int id;
        cout << "\n  Enter book ID to remove : "; cin >> id; cin.ignore(1000, '\n');
        for (size_t i = 0; i < books.size(); i++)
        {
            if (books[i].getBookId() != id) continue;
            if (books[i].isIssued()) { cout << "  This book is issued, so it cannot be removed.\n"; return; }
            books.erase(books.begin() + i);      // remove it from the list
            saveBooks();
            cout << "  Book removed successfully.\n";
            return;
        }
        cout << "  No book found with this ID.\n";
    }

    // FEATURE 3a : SEARCH BY TITLE   [FUNCTION OVERLOADING - one parameter]
    void searchBook(string title)
    {
        bool found = false;
        printBookHeader();
        for (size_t i = 0; i < books.size(); i++)
            if (titleMatches(books[i].getTitle(), title)) { books[i].display(); found = true; }
        if (!found) cout << "  No book found with this title.\n";
    }

    // FEATURE 3b : SEARCH BY TITLE + AUTHOR  [FUNCTION OVERLOADING - two
    // parameters]. Same name, compiler chooses by the arguments given.
    void searchBook(string title, string author)
    {
        bool found = false;
        for (size_t i = 0; i < books.size(); i++)
            if (books[i].getTitle() == title && books[i].getAuthor() == author)
            {
                if (!found) printBookHeader();
                books[i].display();
                found = true;
            }
        if (!found) cout << "  No book found with this title and author.\n";
    }

    // FEATURE 4 : REGISTER MEMBER (Student or Faculty)
    void registerMember()
    {
        int type;
        cout << "\n  Member type: 1 = Student, 2 = Faculty : "; cin >> type; cin.ignore(1000, '\n');
        if (type != 1 && type != 2) { cout << "  Invalid choice.\n"; return; }

        string name, phone;
        cout << "  Enter name  : "; getline(cin, name);
        cout << "  Enter phone : "; getline(cin, phone);
        if (name.empty()) { cout << "  Name cannot be empty.\n"; return; }

        Member* m;
        if (type == 1) m = new Student(nextMemberId++, name, phone);
        else           m = new Faculty(nextMemberId++, name, phone);

        members.push_back(m);          // stored through a Member* pointer
        saveMembers();
        cout << "  Member registered. Member ID = " << m->getMemberId()
             << " (" << m->getType() << ")\n";
    }

    // FEATURE 5 : ISSUE BOOK
    void issueBook()
    {
        int bookId, memberId;
        cout << "\n  Enter book ID   : "; cin >> bookId;
        cout << "  Enter member ID : "; cin >> memberId; cin.ignore(1000, '\n');

        Book*   b = findBook(bookId);
        Member* m = findMember(memberId);
        if (b == NULL) { cout << "  Book not found.\n";   return; }
        if (m == NULL) { cout << "  Member not found.\n"; return; }
        if (b->isIssued()) { cout << "  Sorry, this book is already issued to member "
                                   << b->getIssuedTo() << ".\n"; return; }

        int count = 0;                                  // books this member already has
        for (size_t i = 0; i < books.size(); i++)
            if (books[i].isIssued() && books[i].getIssuedTo() == memberId) count++;

        // Same line works differently for Student and Faculty because
        // getMaxBooks() is virtual -> POLYMORPHISM.
        if (count >= m->getMaxBooks())
        {
            cout << "  Limit reached. A " << m->getType() << " can borrow only "
                 << m->getMaxBooks() << " books.\n";
            return;
        }

        b->markIssued(memberId, todayNumber() + m->getLoanDays());
        saveBooks();
        cout << "  Book \"" << b->getTitle() << "\" issued to " << m->getName()
             << " for " << m->getLoanDays() << " days.\n";
    }

    // FEATURE 6 : RETURN BOOK + FINE CALCULATION
    void returnBook()
    {
        int bookId;
        cout << "\n  Enter book ID to return : "; cin >> bookId; cin.ignore(1000, '\n');

        Book* b = findBook(bookId);
        if (b == NULL)      { cout << "  Book not found.\n";            return; }
        if (!b->isIssued()) { cout << "  This book is not issued.\n";   return; }

        Member*   m        = findMember(b->getIssuedTo());
        long long daysLate = todayNumber() - b->getDueDay();
        if (daysLate < 0) daysLate = 0;

        cout << "  Book \"" << b->getTitle() << "\" returned by "
             << (m ? m->getName() : string("unknown member")) << ".\n";

        if (daysLate > 0 && m != NULL)
        {
            // virtual call: Student pays Rs.2/day, Faculty pays Rs.5/day
            cout << "  Late by " << daysLate << " day(s). Fine = Rs. "
                 << fixed << setprecision(2) << m->calculateFine((int)daysLate) << "\n";
        }
        else cout << "  Returned on time. No fine.\n";

        b->markReturned();
        saveBooks();
    }

    // FEATURE 7 : DISPLAY ALL BOOKS
    void displayAllBooks() const
    {
        printBookHeader();
        for (size_t i = 0; i < books.size(); i++) books[i].display();
        cout << "  Total books : " << books.size() << "\n";
    }

    // FEATURE 8 : DISPLAY ALL MEMBERS (virtual display() is called for each)
    void displayMembers() const
    {
        cout << "\n  " << left << setw(6) << "ID" << setw(22) << "Name" << setw(14)
             << "Phone" << setw(10) << "Type" << "\n  " << string(52, '-') << "\n";
        for (size_t i = 0; i < members.size(); i++) members[i]->display();
        cout << "  Total members : " << members.size() << "\n";
    }

    // FEATURE 9 : DISPLAY ISSUED BOOKS
    void displayIssuedBooks() const
    {
        cout << "\n  " << left << setw(8) << "Book ID" << setw(36) << "Title" << setw(22)
             << "Issued To" << setw(12) << "Days Left" << "\n  " << string(78, '-') << "\n";

        int found = 0;
        for (size_t i = 0; i < books.size(); i++)
        {
            if (!books[i].isIssued()) continue;
            Member*   m        = findMember(books[i].getIssuedTo());
            long long daysLeft = books[i].getDueDay() - todayNumber();
            cout << "  " << left << setw(8) << books[i].getBookId() << setw(36) << books[i].getTitle()
                 << setw(22) << (m ? m->getName() : string("?"))
                 << setw(12) << (daysLeft >= 0 ? to_string(daysLeft) : string("OVERDUE")) << "\n";
            found++;
        }
        if (found == 0) cout << "  No book is currently issued.\n";
        else            cout << "  Total issued books : " << found << "\n";
    }
};

// INPUT VALIDATION: if the user types letters where a number is expected,
// cin fails. We clear the error and ask again, so the menu never gets stuck.
int readInt(string prompt)
{
    int value;
    while (true)
    {
        cout << prompt;
        if (cin >> value) { cin.ignore(1000, '\n'); return value; }
        if (cin.eof()) return 0;                 // input ended -> close safely
        cout << "  Invalid input! Please type a number.\n";
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

// ============================================================================
//  MAIN FUNCTION : menu driven interface
// ============================================================================
int main()
{
    Library lib;        // object created -> constructor loads data from files
    int choice;

    cout << "\n=====================================================\n"
         << "        LIBRARY MANAGEMENT SYSTEM  (C++ OOPS)        \n"
         << "=====================================================\n";

    do
    {
        cout << "\n  ---------------- MAIN MENU ----------------\n"
             << "   1. Add Book\n"
             << "   2. Remove Book\n"
             << "   3. Search Book (by title)\n"
             << "   4. Search Book (by title + author)\n"
             << "   5. Register Member\n"
             << "   6. Issue Book\n"
             << "   7. Return Book\n"
             << "   8. Display All Books\n"
             << "   9. Display All Members\n"
             << "  10. Display Issued Books\n"
             << "   0. Exit\n"
             << "  ------------------------------------------\n";

        choice = readInt("  Enter your choice : ");

        switch (choice)
        {
            case 1:  lib.addBook();        break;
            case 2:  lib.removeBook();     break;
            case 3:
            {
                string t;
                cout << "\n  Enter title (or part of it) : "; getline(cin, t);
                lib.searchBook(t);         // 1-argument version
                break;
            }
            case 4:
            {
                string t, a;
                cout << "\n  Enter exact title  : "; getline(cin, t);
                cout << "  Enter exact author : "; getline(cin, a);
                lib.searchBook(t, a);      // 2-argument version
                break;
            }
            case 5:  lib.registerMember();    break;
            case 6:  lib.issueBook();         break;
            case 7:  lib.returnBook();        break;
            case 8:  lib.displayAllBooks();   break;
            case 9:  lib.displayMembers();    break;
            case 10: lib.displayIssuedBooks();break;
            case 0:  cout << "\n  Data saved. Thank you for using the system!\n"; break;
            default: cout << "  Please enter a number between 0 and 10.\n";
        }
    } while (choice != 0);

    return 0;           // Library destructor runs here and frees memory
}

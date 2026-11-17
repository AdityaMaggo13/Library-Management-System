# Library Management System (C++ / OOP)

A console-based Library Management System built for the OOPS mini project (B.Tech 3rd semester, MAIT, IPU).

## Features
- Add, remove and search books (by title, or by title + author)
- Register members as **Student** or **Faculty**
- Issue and return books with due dates
- Automatic fine on late return (Student Rs. 2/day, Faculty Rs. 5/day)
- Student limit: 2 books / 15 days; Faculty limit: 5 books / 30 days
- Data saved in `books.txt` and `members.txt` and reloaded on every run

## OOP concepts used
| Concept | Where |
|---|---|
| Classes and objects | Book, Member, Student, Faculty, Library |
| Encapsulation | private data with getters/setters in every class |
| Inheritance | Student and Faculty inherit from Member |
| Abstraction | Member is an abstract class (pure virtual functions) |
| Polymorphism | virtual functions overridden in Student/Faculty; overloaded `searchBook()` |
| Constructors / destructors | all classes; Library destructor frees Member objects |
| File handling | ifstream / ofstream in Library |

## Compile and run
```
g++ -std=c++17 Library_Management_System.cpp -o lms
./lms          # Windows: lms.exe
```

## Group
Add group members and enrollment numbers here.

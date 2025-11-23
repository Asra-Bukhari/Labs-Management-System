# Lab Management System

## Project Overview
This is a **Lab Management System** implemented in **C++** for managing university lab information.  
The system stores data about **TAs, instructors, venues (rooms and buildings), and lab sections**. It manages both **regular and makeup lab schedules** and generates reports for academic oversight. The project follows proper **design principles and SOLID guidelines**, ensuring maintainable and modular code.

### Key Features
- Stores information about **TAs, instructors, HOD, academic officers, attendants, rooms, and buildings**.
- Manages **regular lab schedules** as well as **makeup lab schedules**.
- Generates various reports for the Head of Department:
  - Weekly schedule report for all labs
  - Filled timesheet report for all labs in a given week
  - Semester-wise summary report for all lab sessions, including total contact hours and leaves
- Provides **separate interfaces** for:
  - **TAs**
  - **Instructors**
  - **Head of Department (HOD)**
  - **Academic Officer**
  - **Attendants**
- Uses **binary files** for storing data and authentication — no DBMS required.
- Implements **object-oriented principles**, keeping classes cohesive and loosely coupled.

---

## How to Run the Project
1. Open the project in your preferred C++ IDE (e.g., Code::Blocks, Visual Studio).  
2. Compile `main.cpp`.  
3. Run the executable.  
4. Follow on-screen menus for:
   - Academic Officer: populate schedules and labs  
   - Attendants: enter actual timings  
   - HOD: generate reports  
   - Instructors: request makeup labs  
   - TAs: view assigned lab schedules  

---

## Design Principles
- Object-Oriented Programming (OOP) used throughout.  
- Classes are **cohesive**, with minimal coupling.  
- **SOLID principles** followed:
  - **S**: Single Responsibility (classes have single purpose)  
  - **O**: Open/Closed (extensible without modifying existing code)  
  - **L**: Liskov Substitution (subclasses can replace parent classes)  
  - **I**: Interface Segregation (separate interfaces for TAs, instructors, HOD, academic officer, attendants)  
  - **D**: Dependency Inversion (high-level modules do not depend on low-level modules directly)  

---

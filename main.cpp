#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>
#include <algorithm>
#include <stdexcept>

using namespace std;

// helpers

class Time {
private:
    int hour;
    int minute;

public:
    Time(int h = 0, int m = 0) : hour(h), minute(m) {}

    void setTime(int h, int m) { hour = h; minute = m; }
    int getHour() const { return hour; }
    int getMinute() const { return minute; }

    string toString() const {
        stringstream ss;
        ss << setfill('0') << setw(2) << hour << ":" << setfill('0') << setw(2) << minute;
        return ss.str();
    }
    bool operator>(const Time& other) const { return hour != other.hour ? hour > other.hour : minute > other.minute; }
    bool operator<(const Time& other) const { return hour != other.hour ? hour < other.hour : minute < other.minute; }
    bool operator==(const Time& other) const { return hour == other.hour && minute == other.minute; }
    double calculateDurationHours(const Time& endTime) const {
        int startMinutes = hour * 60 + minute;
        int endMinutes = endTime.hour * 60 + endTime.minute;
        return (double)(endMinutes - startMinutes) / 60.0;
    }
};

class Date {
private:
    int day;
    int month;
    int year;
    int weekday; // 0=Sunday, 1=Monday... 6=Saturday

public:
    Date(int d = 1, int m = 1, int y = 2024, int w = 1) : day(d), month(m), year(y), weekday(w) {}

    void setDate(int d, int m, int y, int w) { day = d; month = m; year = y; weekday = w; }
    int getDay() const { return day; }
    int getMonth() const { return month; }
    int getYear() const { return year; }
    int getWeekday() const { return weekday; }

    string getWeekdayString() const {
        switch (weekday) {
        case 1: return "Monday"; case 2: return "Tuesday"; case 3: return "Wednesday";
        case 4: return "Thursday"; case 5: return "Friday"; case 6: return "Saturday";
        case 0: return "Sunday"; default: return "Unknown";
        }
    }

    string toString() const {
        stringstream ss;
        ss << setfill('0') << setw(2) << day << "/" << setfill('0') << setw(2) << month << "/" << year;
        return ss.str();
    }
    bool operator<(const Date& other) const {
        if (year != other.year) return year < other.year;
        if (month != other.month) return month < other.month;
        return day < other.day;
    }
    bool operator==(const Date& other) const { return day == other.day && month == other.month && year == other.year; }
};

// venue

class Building {
public:
    int buildingId;
    string name;
    string address;
    int attendantId; // 1 attendant per building

    Building(int id = 0, const string& n = "", const string& a = "", int aId = 0)
        : buildingId(id), name(n), address(a), attendantId(aId) {
    }

    string getName() const { return name; }
};


class Room {
public:
    int roomId;
    string roomName;
    int buildingId; // References the Building class ID

    Room(int id = 0, const string& rName = "", int bId = 0)
        : roomId(id), roomName(rName), buildingId(bId) {
    }

    int getBuildingId() const { return buildingId; }
    string getRoomName() const { return roomName; }
};

// users

class Person {
public:
    int personId;
    string name;
    string role; 
    string password;

    Person(int id = 0, const string& n = "", const string& r = "", const string& p = "pass")
        : personId(id), name(n), role(r), password(p) {
    }

    virtual void displayInfo() const {
        cout << "ID: " << personId << ", Name: " << name << ", Role: " << role;
    }

    virtual ~Person() {}

    string getName() const { return name; }
    string getRole() const { return role; }
};


class AcademicOfficer : public Person {
public:
    AcademicOfficer(int id = 0, const std::string& n = "", const string& p = "pass")
        : Person(id, n, "AcademicOfficer", p) {
    }
};
class Instructor : public Person {
public:
    Instructor(int id = 0, const string& n = "", const string& p = "pass")
        : Person(id, n, "Instructor", p) {
    }
};
class TA : public Person {
public:
    TA(int id = 0, const string& n = "", const string& p = "pass")
        : Person(id, n, "TA", p) {
    }
};
class HoD : public Person {
public:
    HoD(long long id = 0, const std::string& n = "", const std::string& p = "pass")
        : Person(id, n, "HoD", p) {
    }
};
class Attendant : public Person {
public:
    Attendant(int id = 0, const string& n = "", const string& p = "pass")
        : Person(id, n, "Attendant", p) {
    }
};


//labs

class Course {
public:
    int courseId;
    string courseCode;
    string courseName; 

    Course(long long id = 0, const string& cCode = "", const string& cName = "")
        : courseId(id), courseCode(cCode), courseName(cName) {
    }

    // Getters
    string getCourseCode() const { return courseCode; }
    string getCourseName() const { return courseName; }
};

class LabSection : public Course {
public:
    int sectionId;
    string sectionName; 
    int instructorId;
    vector<int> taIds; 

    LabSection(int cId = 0, const string& cCode = "", const string& cName = "",
        int sId = 0, const string& sName = "", long long insId = 0)
        : Course(cId, cCode, cName),
        sectionId(sId), sectionName(sName), instructorId(insId) {
    }

    void addTA(int taId) {
        if (taIds.size() < 5) { 
            taIds.push_back(taId);
        }
    }

    string getFullSectionCode() const { return courseCode + "-" + sectionName; }
    int getInstructorId() const { return instructorId; }
};

class ScheduleEntry {
public:
    int scheduleId;
    int sectionId;
    int roomId;
    Date scheduledDate;
    Time expectedStart;
    Time expectedEnd;
    Time actualStart;
    Time actualEnd;
    bool isMakeup;
    bool isCanceled;
    int status; // 0: Scheduled, 1: Timesheet Filled, 2: Canceled

    ScheduleEntry(long long id = 0, long long secId = 0, long long rId = 0,
        const Date& sDate = Date(), const Time& eS = Time(), const Time& eE = Time(),
        bool makeup = false)
        : scheduleId(id), sectionId(secId), roomId(rId), scheduledDate(sDate),
        expectedStart(eS), expectedEnd(eE), actualStart(Time(0, 0)), actualEnd(Time(0, 0)),
        isMakeup(makeup), isCanceled(false), status(0) {
    }

    double getActualContactHours() const {
        if (status != 1) return 0.0;
        return actualStart.calculateDurationHours(actualEnd);
    }
};

class MakeupRequest {
public:
    long long requestId;
    long long sectionId;
    long long instructorId;
    Date requestedDate;
    Time requestedStart;
    Time requestedEnd;
    std::string reason;
    int status; // 0: Pending, 1: Approved, 2: Disapproved

    MakeupRequest(long long id = 0, long long secId = 0, long long insId = 0,
        const Date& rDate = Date(), const Time& rS = Time(), const Time& rE = Time(),
        const std::string& rsn = "")
        : requestId(id), sectionId(secId), instructorId(insId), requestedDate(rDate),
        requestedStart(rS), requestedEnd(rE), reason(rsn), status(0) {
    }
};


// manager classes

class DataManager {
private:

    static int nextPersonId;
    static int nextLabSectionId;
    static int nextRoomId;
    static int nextScheduleId;
    static int nextMakeupId;
    static int nextBuildingId;

    // Filepaths
    const string PERSONS_FILE = "persons.dat";
    const string LABS_FILE = "labs.dat";
    const string ROOMS_FILE = "rooms.dat";
    const string SCHEDULES_FILE = "schedules.dat";
    const string MAKEUP_FILE = "makeup_requests.dat";
    const string BUILDINGS_FILE = "buildings.dat";


    // to write a string to binary file
    void writeString(std::ofstream& ofs, const std::string& s) {
        size_t len = s.size();
        ofs.write(reinterpret_cast<const char*>(&len), sizeof(size_t));
        ofs.write(s.data(), len);
    }

    // to read a string from binary file
    void readString(std::ifstream& ifs, std::string& s) {
        size_t len = 0;
        ifs.read(reinterpret_cast<char*>(&len), sizeof(size_t));
        if (len > 0) {
            std::vector<char> buffer(len);
            ifs.read(buffer.data(), len);
            s.assign(buffer.begin(), buffer.end());
        }
        else {
            s.clear();
        }
    }

    // to write a vector 
    template <typename T>
    void writeVector(std::ofstream& ofs, const std::vector<T>& v) {
        size_t size = v.size();
        ofs.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
        if (size > 0) {
            ofs.write(reinterpret_cast<const char*>(v.data()), size * sizeof(T));
        }
    }

    // to read a vector 
    template <typename T>
    void readVector(std::ifstream& ifs, std::vector<T>& v) {
        size_t size = 0;
        ifs.read(reinterpret_cast<char*>(&size), sizeof(size_t));
        v.resize(size);
        if (size > 0) {
            ifs.read(reinterpret_cast<char*>(v.data()), size * sizeof(T));
        }
    }


    // Writes/Reads a single Person object 
    void writeData(std::ofstream& ofs, const Person& p) {
        ofs.write(reinterpret_cast<const char*>(&p.personId), sizeof(long long));
        writeString(ofs, p.name);
        writeString(ofs, p.role);
        writeString(ofs, p.password);
    }
    void readData(std::ifstream& ifs, Person& p) {
        ifs.read(reinterpret_cast<char*>(&p.personId), sizeof(long long));
        readString(ifs, p.name);
        readString(ifs, p.role);
        readString(ifs, p.password);
    }

    // Writes/Reads a single Building object 
    void writeData(std::ofstream& ofs, const Building& b) {
        ofs.write(reinterpret_cast<const char*>(&b.buildingId), sizeof(long long));
        writeString(ofs, b.name);
        writeString(ofs, b.address);
        ofs.write(reinterpret_cast<const char*>(&b.attendantId), sizeof(long long));
    }
    void readData(std::ifstream& ifs, Building& b) {
        ifs.read(reinterpret_cast<char*>(&b.buildingId), sizeof(long long));
        readString(ifs, b.name);
        readString(ifs, b.address);
        ifs.read(reinterpret_cast<char*>(&b.attendantId), sizeof(long long));
    }

    // Writes/Reads a single Room object 
    void writeData(std::ofstream& ofs, const Room& r) {
        ofs.write(reinterpret_cast<const char*>(&r.roomId), sizeof(long long));
        writeString(ofs, r.roomName);
        ofs.write(reinterpret_cast<const char*>(&r.buildingId), sizeof(long long));
    }
    void readData(std::ifstream& ifs, Room& r) {
        ifs.read(reinterpret_cast<char*>(&r.roomId), sizeof(long long));
        readString(ifs, r.roomName);
        ifs.read(reinterpret_cast<char*>(&r.buildingId), sizeof(long long));
    }

    // Writes/Reads LabSection 
    void writeData(std::ofstream& ofs, const LabSection& ls) {
 
        ofs.write(reinterpret_cast<const char*>(&ls.courseId), sizeof(long long));
        writeString(ofs, ls.courseCode);
        writeString(ofs, ls.courseName);


        ofs.write(reinterpret_cast<const char*>(&ls.sectionId), sizeof(long long));
        writeString(ofs, ls.sectionName);
        ofs.write(reinterpret_cast<const char*>(&ls.instructorId), sizeof(long long));
        writeVector(ofs, ls.taIds);
    }
    void readData(std::ifstream& ifs, LabSection& ls) {

        ifs.read(reinterpret_cast<char*>(&ls.courseId), sizeof(long long));
        readString(ifs, ls.courseCode);
        readString(ifs, ls.courseName);

        ifs.read(reinterpret_cast<char*>(&ls.sectionId), sizeof(long long));
        readString(ifs, ls.sectionName); 
        ifs.read(reinterpret_cast<char*>(&ls.instructorId), sizeof(long long));
        readVector(ifs, ls.taIds);
    }

    // Writes/Reads ScheduleEntry 
    void writeData(std::ofstream& ofs, const ScheduleEntry& se) {
        ofs.write(reinterpret_cast<const char*>(&se), sizeof(ScheduleEntry));
    }
    void readData(std::ifstream& ifs, ScheduleEntry& se) {
        ifs.read(reinterpret_cast<char*>(&se), sizeof(ScheduleEntry));
    }

    // Writes/Reads MakeupRequest 
    void writeData(std::ofstream& ofs, const MakeupRequest& mr) {
        ofs.write(reinterpret_cast<const char*>(&mr.requestId), sizeof(long long));
        ofs.write(reinterpret_cast<const char*>(&mr.sectionId), sizeof(long long));
        ofs.write(reinterpret_cast<const char*>(&mr.instructorId), sizeof(long long));
        ofs.write(reinterpret_cast<const char*>(&mr.requestedDate), sizeof(Date));
        ofs.write(reinterpret_cast<const char*>(&mr.requestedStart), sizeof(Time));
        ofs.write(reinterpret_cast<const char*>(&mr.requestedEnd), sizeof(Time));
        writeString(ofs, mr.reason);
        ofs.write(reinterpret_cast<const char*>(&mr.status), sizeof(int));
    }
    void readData(std::ifstream& ifs, MakeupRequest& mr) {
        ifs.read(reinterpret_cast<char*>(&mr.requestId), sizeof(long long));
        ifs.read(reinterpret_cast<char*>(&mr.sectionId), sizeof(long long));
        ifs.read(reinterpret_cast<char*>(&mr.instructorId), sizeof(long long));
        ifs.read(reinterpret_cast<char*>(&mr.requestedDate), sizeof(Date));
        ifs.read(reinterpret_cast<char*>(&mr.requestedStart), sizeof(Time));
        ifs.read(reinterpret_cast<char*>(&mr.requestedEnd), sizeof(Time));
        readString(ifs, mr.reason);
        ifs.read(reinterpret_cast<char*>(&mr.status), sizeof(int));
    }



    template <typename T>
    std::vector<T> readAllRecords(const std::string& filename) {
        std::vector<T> records;
        std::ifstream ifs(filename, std::ios::binary | std::ios::in);
        if (!ifs) return records;

        while (ifs.peek() != std::ifstream::traits_type::eof()) {
            T record;
            readData(ifs, record);
            if (!ifs.fail()) {
                records.push_back(record);
            }
            else {
                break; 
            }
        }
        ifs.close();
        return records;
    }

    template <typename T>
    bool saveRecord(const std::string& filename, const T& record) {
        std::ofstream ofs(filename, std::ios::binary | std::ios::app);
        if (!ofs.is_open()) {
            std::cerr << "ERROR: Could not open file " << filename << " for saving." << std::endl;
            return false;
        }
        writeData(ofs, record);
        ofs.close();
        return true;
    }

    template <typename T>
    bool saveAllRecords(const std::string& filename, const std::vector<T>& records) {
        std::ofstream ofs(filename, std::ios::binary | std::ios::out | std::ios::trunc);
        if (!ofs.is_open()) {
            std::cerr << "ERROR: Could not open file " << filename << " for overwriting." << std::endl;
            return false;
        }
        for (const auto& record : records) {
            writeData(ofs, record);
        }
        ofs.close();
        return true;
    }



public:
    
    static int nextCourseId;

    vector<Person> persons;
    vector<Room> rooms;
    vector<LabSection> labSections;
    vector<ScheduleEntry> schedules;
    vector<MakeupRequest> requests;
    vector<Building> buildings;


    int getNextId(int& staticIdCounter) {
        return ++staticIdCounter;
    }

    DataManager() {
        loadAllData();
    }

    void loadAllData() {
        persons = readAllRecords<Person>(PERSONS_FILE);
        rooms = readAllRecords<Room>(ROOMS_FILE);
        labSections = readAllRecords<LabSection>(LABS_FILE);
        schedules = readAllRecords<ScheduleEntry>(SCHEDULES_FILE);
        requests = readAllRecords<MakeupRequest>(MAKEUP_FILE);
        buildings = readAllRecords<Building>(BUILDINGS_FILE);

        // Update static ID counters based on loaded data
        for (const auto& p : persons) if (p.personId > nextPersonId) nextPersonId = p.personId;
        for (const auto& r : rooms) if (r.roomId > nextRoomId) nextRoomId = r.roomId;
        for (const auto& l : labSections) {
            if (l.sectionId > nextLabSectionId) nextLabSectionId = l.sectionId;
            // Update nextCourseId based on the highest CourseId found in sections
            if (l.courseId > nextCourseId) nextCourseId = l.courseId;
        }
        for (const auto& s : schedules) if (s.scheduleId > nextScheduleId) nextScheduleId = s.scheduleId;
        for (const auto& m : requests) if (m.requestId > nextMakeupId) nextMakeupId = m.requestId;
        for (const auto& b : buildings) if (b.buildingId > nextBuildingId) nextBuildingId = b.buildingId;
    }


    static void initializeStaticIds() {
        nextPersonId = 1000;
        nextLabSectionId = 2000;
        nextRoomId = 3000;
        nextScheduleId = 4000;
        nextMakeupId = 5000;
        nextBuildingId = 6000;
        nextCourseId = 7000; \
    }

    // person management
    int addPerson(const string& name, const string& role, const string& password) {
        int newId = getNextId(nextPersonId);
        Person p(newId, name, role, password);
        persons.push_back(p);
        saveRecord(PERSONS_FILE, p);
        return newId;
    }

    // venue management
    int addBuilding(const string& name, const string& address, int attendantId) {
        int newId = getNextId(nextBuildingId);
        Building b(newId, name, address, attendantId);
        buildings.push_back(b);
        saveRecord(BUILDINGS_FILE, b);
        return newId;
    }

    int addRoom(const string& roomName, int buildingId) {
        int newId = getNextId(nextRoomId);
        Room r(newId, roomName, buildingId);
        rooms.push_back(r);
        saveRecord(ROOMS_FILE, r);
        return newId;
    }

    // lab section management


    int addLabSection(int courseId, const string& courseCode, const string& courseName, const string& sectionName) {
        int newId = getNextId(nextLabSectionId);
        LabSection ls(courseId, courseCode, courseName, newId, sectionName);
        labSections.push_back(ls);
        saveRecord(LABS_FILE, ls);
        return newId;
    }

    bool assignInstructor(long long sectionId, long long insId) {
        for (auto& ls : labSections) {
            if (ls.sectionId == sectionId) {
                ls.instructorId = insId;
                return saveAllRecords(LABS_FILE, labSections);
            }
        }
        return false;
    }

    bool assignTA(int sectionId, int taId) {
        for (auto& ls : labSections) {
            if (ls.sectionId == sectionId) {
                ls.addTA(taId);
                return saveAllRecords(LABS_FILE, labSections);
            }
        }
        return false;
    }

    //Scheduling Management
    int addScheduleEntry(int sectionId, int roomId, const Date& date, const Time& start, const Time& end, bool isMakeup = false) {
        int newId = getNextId(nextScheduleId);
        ScheduleEntry se(newId, sectionId, roomId, date, start, end, isMakeup);
        schedules.push_back(se);
        saveRecord(SCHEDULES_FILE, se);
        return newId;
    }

    bool updateScheduleActualTime(int scheduleId, const Time& actualStart, const Time& actualEnd) {
        for (auto& se : schedules) {
            if (se.scheduleId == scheduleId) {
                se.actualStart = actualStart;
                se.actualEnd = actualEnd;
                se.status = 1; // 1: Timesheet Filled
                return saveAllRecords(SCHEDULES_FILE, schedules);
            }
        }
        return false;
    }

    // Makeup Request Management
    int addMakeupRequest(int sectionId, int instructorId, const Date& date, const Time& start, const Time& end, const string& reason) {
        int newId = getNextId(nextMakeupId);
        MakeupRequest mr(newId, sectionId, instructorId, date, start, end, reason);
        requests.push_back(mr);
        saveRecord(MAKEUP_FILE, mr);
        return newId;
    }

    bool updateMakeupRequestStatus(int requestId, int status) {
        for (auto& mr : requests) {
            if (mr.requestId == requestId) {
                mr.status = status;
                return saveAllRecords(MAKEUP_FILE, requests);
            }
        }
        return false;
    }

    const Person* getPersonById(int id) const {
        for (const auto& p : persons) {
            if (p.personId == id) return &p;
        }
        return nullptr;
    }

    const LabSection* getLabSectionById(int id) const {
        for (const auto& ls : labSections) {
            if (ls.sectionId == id) return &ls;
        }
        return nullptr;
    }

    const Room* getRoomById(int id) const {
        for (const auto& r : rooms) {
            if (r.roomId == id) return &r;
        }
        return nullptr;
    }

    const Building* getBuildingById(int id) const {
        for (const auto& b : buildings) {
            if (b.buildingId == id) return &b;
        }
        return nullptr;
    }

    const vector<Room>& getRooms() const { return rooms; }
    const vector<Building>& getBuildings() const { return buildings; }
    const vector<LabSection>& getLabSections() const { return labSections; }
    const vector<ScheduleEntry>& getSchedules() const { return schedules; }
    const vector<MakeupRequest>& getRequests() const { return requests; }

    bool isRoomAvailable(int roomId, const Date& date, const Time& start, const Time& end) const {
        for (const auto& se : schedules) {
            if (se.roomId == roomId && se.scheduledDate == date) {
                if (se.expectedStart < end && start < se.expectedEnd && !se.isCanceled) {
                    return false; 
                }
            }
        }
        return true; 
    }
};


int DataManager::nextPersonId = 1000;
int DataManager::nextLabSectionId = 2000;
int DataManager::nextRoomId = 3000;
int DataManager::nextScheduleId = 4000;
int DataManager::nextMakeupId = 5000;
int DataManager::nextBuildingId = 6000;
int DataManager::nextCourseId = 7000; 

// authentication
class Authentication {
private:
    DataManager& dm;

public:
    Authentication(DataManager& dataManager) : dm(dataManager) {}

    const Person* authenticate(long long id, const std::string& password) const {
        const Person* p = dm.getPersonById(id);
        if (p && password == p->password) {
            return p;
        }
        return nullptr;
    }
};

class LabManagementSystem;

// reports

class HoDReportGenerator {
private:
    DataManager& dm;

    bool writeReportFile(const std::string& filename, const string& content) const {
        time_t now = time(0);
        tm temp{};
        tm* ltm = &temp;

        if (localtime_s(ltm, &now) != 0) {
            std::cerr << "\n[ERROR] Failed to get local time." << std::endl;
            return false;
        }

        stringstream datedName;
        datedName << filename << "_" << ltm->tm_year + 1900 << "-" << ltm->tm_mon + 1 << "-" << ltm->tm_mday << ".txt";

        ofstream file(datedName.str());
        if (file.is_open()) {
            file << content;
            file.close();
            cout << "\n[SUCCESS] Report generated: " << datedName.str() << endl;
            return true;
        }
        else {
            cout << "\n[ERROR] Could not generate report file: " << datedName.str() << endl;
            return false;
        }
    }

public:
    HoDReportGenerator(DataManager& dataManager) : dm(dataManager) {}

    string getPersonName(int id) const {
        const Person* p = dm.getPersonById(id);
        return p ? p->getName() : "N/A";
    }

    // UPDATED: Use courseName and getFullSectionCode
    string getLabInfo(int id) const {
        const LabSection* ls = dm.getLabSectionById(id);
        return ls ? ls->courseName + " (" + ls->getFullSectionCode() + ")" : "N/A";
    }

    string getRoomInfo(long long id) const {
        const Room* r = dm.getRoomById(id);
        if (!r) return "N/A Room";

        const Building* b = dm.getBuildingById(r->buildingId);
        string buildingName = b ? b->getName() : "Unknown Building";
        return r->roomName + " in " + buildingName;
    }

    void generateLabScheduleReport() const {
        stringstream report;
        report << "--- COMPLETE LAB SCHEDULE FOR THE WEEK ---\n";
        report << "Generated on: " << Date().toString() << "\n\n";

        const auto& schedules = dm.getSchedules();

        vector<ScheduleEntry> sortedSchedules = schedules;
        sort(sortedSchedules.begin(), sortedSchedules.end(), [](const ScheduleEntry& a, const ScheduleEntry& b) {
            if (!(a.scheduledDate == b.scheduledDate)) return a.scheduledDate < b.scheduledDate;
            return a.expectedStart < b.expectedStart;
            });

        if (sortedSchedules.empty()) {
            report << "No labs are currently scheduled.\n";
        }
        else {
            report << left << setw(15) << "Date" << setw(12) << "Day" << setw(15) << "Start"
                << setw(15) << "End" << setw(25) << "Lab Section" << setw(30) << "Venue"
                << setw(20) << "Instructor" << endl;
            report << string(132, '-') << endl;

            for (const auto& se : sortedSchedules) {
                const LabSection* ls = dm.getLabSectionById(se.sectionId);
                report << left << setw(15) << se.scheduledDate.toString()
                    << setw(12) << se.scheduledDate.getWeekdayString()
                    << setw(15) << se.expectedStart.toString()
                    << setw(15) << se.expectedEnd.toString()
                    << setw(25) << (ls ? ls->getFullSectionCode() : "N/A")
                    << setw(30) << getRoomInfo(se.roomId)
                    << setw(20) << getPersonName(ls ? ls->getInstructorId() : 0) << std::endl;
            }
        }

        writeReportFile("LabScheduleReport", report.str());
    }

    void generateTimeSheetReport(const string& targetWeek) const {
        stringstream report;
        report << "--- FILLED TIME SHEET REPORT (Week: " << targetWeek << ") ---\n";
        report << "Generated on: " << Date().toString() << "\n\n";

        const auto& schedules = dm.getSchedules();

        report << left << setw(15) << "Date" << setw(25) << "Lab Section"
            << setw(15) << "Actual Start" << setw(15) << "Actual End"
            << setw(20) << "Duration (hrs)" << setw(30) << "Venue" << std::endl;
        report << string(120, '-') << endl;

        bool found = false;
        for (const auto& se : schedules) {
            if (se.status == 1) { // Only filled timesheets
                found = true;
                double duration = se.getActualContactHours();
                report << left << setw(15) << se.scheduledDate.toString()
                    << setw(25) << getLabInfo(se.sectionId)
                    << setw(15) << se.actualStart.toString()
                    << setw(15) << se.actualEnd.toString()
                    << setw(20) << std::fixed << setprecision(2) << duration
                    << setw(30) << getRoomInfo(se.roomId) << endl;
            }
        }

        if (!found) {
            report << "No timesheets filled for the specified week or in the system yet.\n";
        }

        writeReportFile("TimeSheetReport_" + targetWeek, report.str());
    }

    void generateLabSummaryReport(int sectionId) const {
        const LabSection* ls = dm.getLabSectionById(sectionId);
        if (!ls) {
            cout << "[ERROR] Invalid lab section ID." << endl;
            return;
        }

        stringstream report;
        report << "--- LAB CONTACT HOURS SUMMARY ---\n";
        report << "Lab Section: " << ls->courseName << " (" << ls->getFullSectionCode() << ")\n";
        report << "Instructor: " << getPersonName(ls->getInstructorId()) << "\n";
        report << "Generated on: " << Date().toString() << "\n\n";

        const auto& schedules = dm.getSchedules();
        double totalContactHours = 0.0;
        int leavesOrCancellations = 0;

        report << std::left << std::setw(15) << "Date" << std::setw(15) << "Exp. Start" << std::setw(15) << "Exp. End"
            << std::setw(15) << "Act. Start" << std::setw(15) << "Act. End"
            << std::setw(15) << "Duration" << std::setw(15) << "Status" << std::endl;
        report << std::string(105, '-') << std::endl;

        for (const auto& se : schedules) {
            if (se.sectionId == sectionId) {
                double duration = se.getActualContactHours();
                totalContactHours += duration;

                std::string statusStr;
                std::string actStart = "N/A";
                std::string actEnd = "N/A";
                std::string durationStr = "N/A";

                if (se.isCanceled) {
                    statusStr = "Canceled/Leave";
                    leavesOrCancellations++;
                }
                else if (se.status == 1) {
                    statusStr = "Completed";
                    actStart = se.actualStart.toString();
                    actEnd = se.actualEnd.toString();
                    durationStr = std::to_string(duration);
                }
                else {
                    statusStr = "Scheduled";
                }

                report << std::left << std::setw(15) << se.scheduledDate.toString()
                    << std::setw(15) << se.expectedStart.toString()
                    << std::setw(15) << se.expectedEnd.toString()
                    << std::setw(15) << actStart
                    << std::setw(15) << actEnd
                    << std::setw(15) << durationStr
                    << std::setw(15) << statusStr << std::endl;
            }
        }

        report << "\n\n--- SUMMARY ---\n";
        report << "Total Contact Hours Logged: " << std::fixed << std::setprecision(2) << totalContactHours << " hours\n";
        report << "Total Canceled/Leaves: " << leavesOrCancellations << " sessions\n";

        writeReportFile("LabSummaryReport_" + ls->getFullSectionCode(), report.str());
    }
};

// main

class LabManagementSystem {
private:
    DataManager dm;
    Authentication auth;
    HoDReportGenerator reporter;
    const Person* loggedInUser = nullptr;


    long long getLongInput(const string& prompt) {
        long long val;
        cout << prompt;
        while (!(cin >> val)) {
            cout << "Invalid input. Please enter a number: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }
        cin.ignore(10000, '\n');
        return val;
    }

    string getStringInput(const string& prompt) {
        string val;
        cout << prompt;
        getline(std::cin, val);
        return val;
    }

    int getIntInput(const string& prompt) {
        int val;
        cout << prompt;
        while (!(std::cin >> val)) {
            cout << "Invalid input. Please enter an integer: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }
        cin.ignore(10000, '\n');
        return val;
    }

    Time getTimeInput(const string& prompt) {
        int h, m;
        cout << prompt << " (HH:MM format, e.g., 14 30): ";
        while (!(cin >> h >> m) || h < 0 || h > 23 || m < 0 || m > 59) {
            cout << "Invalid time. Please use HH MM format (00-23 00-59): ";
            cin.clear();
            cin.ignore(10000, '\n');
        }
        cin.ignore(10000, '\n');
        return Time(h, m);
    }

    Date getDateInput(const string& prompt) {
        int d, m, y, w;
        cout << prompt << " (DD MM YYYY Weekday(1-7, 1=Mon)): ";
        while (!(cin >> d >> m >> y >> w) || d < 1 || d > 31 || m < 1 || m > 12 || y < 2024 || w < 1 || w > 7) {
            cout << "Invalid date. Please use DD MM YYYY W format: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }
        cin.ignore(10000, '\n');
        return Date(d, m, y, (w == 7) ? 0 : w); 
    }

    // Academic Officer Functions 
    void aoMenu() {
        while (true) {
            cout << "\n--- Academic Officer Menu ---\n";
            cout << "1. Add Person (TA, Ins, HoD, Attendant)\n";
            cout << "2. Add Lab Section\n"; 
            cout << "3. Add Building\n";
            cout << "4. Add Room to Building\n";
            cout << "5. Assign TA to Lab Section\n";
            cout << "6. Assign Instructor to Lab Section\n";
            cout << "7. Schedule a Lab Session\n";
            cout << "8. View All Scheduled Labs\n";
            cout << "9. View Section Assignments\n";
            cout << "10. View/Approve Makeup Lab Requests\n";
            cout << "0. Logout\n";
            int choice = getIntInput("Enter choice: ");

            if (choice == 0) return;
            switch (choice) {
            case 1: ao_addPerson(); break;
            case 2: ao_addLabSection(); break; 
            case 3: ao_addBuilding(); break;
            case 4: ao_addRoomToBuilding(); break;
            case 5: ao_assignTA(); break;
            case 6: ao_assignInstructor(); break;
            case 7: ao_scheduleLab(); break;
            case 8: ao_viewScheduledLabs(); break;
            case 9: ao_viewSectionAssignments(); break;
            case 10: ao_handleMakeupRequests(); break;
            default: cout << "Invalid choice.\n";
            }
        }
    }

    void ao_addPerson() {
        std::string name = getStringInput("Enter person's name: ");
        std::string role = getStringInput("Enter role (Instructor, TA, HoD, Attendant): ");
        std::string pass = getStringInput("Enter initial password: ");
        if (role == "Instructor" || role == "TA" || role == "HoD" || role == "Attendant" || role == "AcademicOfficer") {
            long long newId = dm.addPerson(name, role, pass);
            cout << "Added " << role << ". ID: " << newId << ", Password: " << pass << endl;
        }
        else {
            cout << "Invalid role specified.\n";
        }
    }


    void ao_addLabSection() {
        string courseCode = getStringInput("Enter Course Code (e.g., CS101): ");
        string sectionName = getStringInput("Enter Section Name (e.g., A, B, CS-A): ");
        string courseName;
        int courseId = 0;

        // Check if Course already exists by courseCode (by checking existing sections)
        for (const auto& ls : dm.getLabSections()) {
            if (ls.courseCode == courseCode) {
                courseId = ls.courseId;
                courseName = ls.courseName;
                cout << "Course '" << courseName << "' already exists (ID: " << courseId << "). Adding section to it.\n";
                break;
            }
        }

        // If Course doesn't exist, prompt for name and create synthetic ID
        if (courseId == 0) {
            cout << "Course code " << courseCode << " not found. Please provide details to create the Course.\n";
            courseName = getStringInput("Enter full Course Name (e.g., Data Structures Lab): ");
            courseId = dm.getNextId(dm.nextCourseId);
        }

        // Check if this specific section already exists for this course
        for (const auto& ls : dm.getLabSections()) {
            if (ls.courseCode == courseCode && ls.sectionName == sectionName) {
                cout << "Section " << sectionName << " for Course " << courseCode << " already exists (ID: " << ls.sectionId << ").\n";
                return;
            }
        }

        // Add the Lab Section
        int newId = dm.addLabSection(courseId, courseCode, courseName, sectionName);
        cout << "Added Lab Section. Full Code: " << courseCode << "-" << sectionName << ", Course Name: " << courseName << ", Section ID: " << newId << endl;
    }

    void ao_addBuilding() {
        string name = getStringInput("Enter Building Name: ");
        string address = getStringInput("Enter Building Address: ");
        int aId = getLongInput("Enter Attendant ID responsible for this building: ");

        const Person* attendant = dm.getPersonById(aId);
        if (!attendant || attendant->getRole() != "Attendant") {
            cout << "Attendant ID not found or is not an Attendant. Aborting building creation.\n";
            return;
        }

        int newId = dm.addBuilding(name, address, aId);
        cout << "Added Building. ID: " << newId << ", Attendant ID: " << aId << endl;
    }

    void ao_addRoomToBuilding() {
        cout << "\n--- Existing Buildings ---\n";
        cout << left << setw(10) << "ID" << std::setw(20) << "Name" << setw(20) << "Attendant ID" << endl;
        cout << string(50, '-') << endl;
        for (const auto& b : dm.getBuildings()) {
            cout << left << setw(10) << b.buildingId << setw(20) << b.name << std::setw(20) << b.attendantId << std::endl;
        }

        int bId = getLongInput("Enter Building ID to add room to: ");
        const Building* b = dm.getBuildingById(bId);
        if (!b) {
            cout << "Invalid Building ID.\n";
            return;
        }

        string rName = getStringInput("Enter Room Name (e.g., G-101): ");
        int newId = dm.addRoom(rName, bId);
        cout << "Added Room " << rName << " to Building " << b->getName() << ". Room ID: " << newId << endl;
    }

    void ao_assignTA() {
        int secId = getLongInput("Enter Lab Section ID to assign TA: ");
        int taId = getLongInput("Enter TA ID: ");

        if (!dm.getLabSectionById(secId)) { cout << "[ERROR] Invalid Lab Section ID.\n"; return; }
        const Person* ta = dm.getPersonById(taId);
        if (!ta || ta->role != "TA") { cout << "[ERROR] Invalid TA ID.\n"; return; }

        if (dm.assignTA(secId, taId)) {
            cout << "TA ID " << taId << " assigned to Section ID " << secId << endl;
        }
        else {
            cout << "Assignment failed (perhaps too many TAs already).\n";
        }
    }

    void ao_assignInstructor() {
        int secId = getLongInput("Enter Lab Section ID to assign Instructor: ");
        int insId = getLongInput("Enter Instructor ID: ");

        if (!dm.getLabSectionById(secId)) { cout << "[ERROR] Invalid Lab Section ID.\n"; return; }
        const Person* ins = dm.getPersonById(insId);
        if (!ins || ins->role != "Instructor") { cout << "[ERROR] Invalid Instructor ID.\n"; return; }

        if (dm.assignInstructor(secId, insId)) {
            cout << "Instructor ID " << insId << " assigned to Section ID " << secId << endl;
        }
        else {
            cout << "Assignment failed.\n";
        }
    }

    void ao_scheduleLab() {
        int secId = getLongInput("Enter Lab Section ID to schedule: ");
        Date date = getDateInput("Enter scheduled date/time ");
        Time start = getTimeInput("Enter expected start time ");
        Time end = getTimeInput("Enter expected end time ");

        if (!dm.getLabSectionById(secId)) { cout << "[ERROR] Invalid Lab Section ID.\n"; return; }

        // Find available rooms
        vector<const Room*> availableRooms;
        for (const auto& room : dm.getRooms()) {
            if (dm.isRoomAvailable(room.roomId, date, start, end)) {
                availableRooms.push_back(&room);
            }
        }

        if (availableRooms.empty()) {
            cout << "No rooms available for the specified time slot.\n";
            return;
        }

        cout << "\n--- Available Rooms ---\n";
        cout << left << setw(10) << "ID" << setw(15) << "Room Name" << setw(20) << "Building" << std::endl;
        cout << string(45, '-') << std::endl;
        for (const auto* room : availableRooms) {
            const Building* b = dm.getBuildingById(room->buildingId);
            cout << left << setw(10) << room->roomId << setw(15) << room->roomName << setw(20) << (b ? b->getName() : "N/A") << std::endl;
        }

        int rId = getLongInput("Enter the Room ID you wish to assign: ");
        bool roomExists = false;
        for (const auto* room : availableRooms) {
            if (room->roomId == rId) {
                roomExists = true;
                break;
            }
        }

        if (roomExists) {
            int newId = dm.addScheduleEntry(secId, rId, date, start, end);
            cout << "Lab scheduled! Schedule ID: " << newId << endl;
        }
        else {
            cout << "Invalid or unavailable Room ID selected.\n";
        }
    }

    void ao_viewScheduledLabs() {
        cout << "\n--- ALL SCHEDULED LAB SESSIONS ---\n";
        const auto& schedules = dm.getSchedules();

        cout << left << setw(12) << "ID" << setw(15) << "Section Code" << setw(15) << "Date"
            << setw(10) << "Day" << setw(10) << "Start" << setw(10) << "End"
            << setw(30) << "Venue" << setw(15) << "Status" << endl;
        cout << string(107, '-') << endl;

        for (const auto& se : schedules) {
            const LabSection* ls = dm.getLabSectionById(se.sectionId);
            string venueInfo = reporter.getRoomInfo(se.roomId);
            string status = se.isMakeup ? "Makeup" : (se.isCanceled ? "Canceled" : (se.status == 1 ? "Filled" : "Scheduled"));

            cout << left << std::setw(12) << se.scheduleId
                << setw(15) << (ls ? ls->getFullSectionCode() : "N/A")
                << setw(15) << se.scheduledDate.toString()
                << setw(10) << se.scheduledDate.getWeekdayString().substr(0, 3)
                << setw(10) << se.expectedStart.toString()
                << setw(10) << se.expectedEnd.toString()
                << setw(30) << venueInfo
                << setw(15) << status << std::endl;
        }
    }

    void ao_viewSectionAssignments() {
        cout << "\n--- LAB SECTION ASSIGNMENTS ---\n";
        const auto& labSections = dm.getLabSections();

        cout << std::left << std::setw(10) << "ID" << std::setw(25) << "Section Code" << std::setw(25) << "Course Name" << std::setw(15) << "Instructor" << std::setw(50) << "TA(s)" << std::endl;
        cout << std::string(125, '-') << std::endl;

        for (const auto& ls : labSections) {
            std::string insName = reporter.getPersonName(ls.instructorId);
            std::stringstream taNames;
            for (size_t i = 0; i < ls.taIds.size(); ++i) {
                const Person* ta = dm.getPersonById(ls.taIds[i]);
                taNames << (ta ? ta->getName() : "N/A") << (i < ls.taIds.size() - 1 ? ", " : "");
            }

            cout << std::left << std::setw(10) << ls.sectionId
                << std::setw(25) << ls.getFullSectionCode()
                << std::setw(25) << ls.getCourseName()
                << std::setw(15) << insName
                << std::setw(50) << taNames.str() << std::endl;
        }
    }

    void ao_handleMakeupRequests() {
        cout << "\n--- PENDING MAKEUP REQUESTS ---\n";
        const auto& requests = dm.getRequests();
        vector<const MakeupRequest*> pendingRequests;

        cout << left << std::setw(10) << "Req ID" << std::setw(15) << "Section Code" << std::setw(15) << "Instructor"
            << std::setw(15) << "Date" << setw(10) << "Start" << std::setw(10) << "End" << std::setw(30) << "Reason" << std::endl;
        cout << std::string(105, '-') << endl;

        for (const auto& mr : requests) {
            if (mr.status == 0) {
                pendingRequests.push_back(&mr);
                const LabSection* ls = dm.getLabSectionById(mr.sectionId);
                cout << std::left << std::setw(10) << mr.requestId
                    << setw(15) << (ls ? ls->getFullSectionCode() : "N/A")
                    << setw(15) << reporter.getPersonName(mr.instructorId)
                    << setw(15) << mr.requestedDate.toString()
                    << setw(10) << mr.requestedStart.toString()
                    << setw(10) << mr.requestedEnd.toString()
                    << setw(30) << mr.reason << std::endl;
            }
        }

        if (pendingRequests.empty()) {
            cout << "No pending makeup requests.\n";
            return;
        }

        int reqId = getLongInput("Enter Request ID to process (or 0 to exit): ");
        if (reqId == 0) return;

        const MakeupRequest* selectedReq = nullptr;
        for (const auto* mr : pendingRequests) {
            if (mr->requestId == reqId) {
                selectedReq = mr;
                break;
            }
        }

        if (selectedReq) {
            char action = getStringInput("Approve (A) or Disapprove (D)? ").at(0);
            action = std::toupper(action);

            if (action == 'A') {
                std::vector<const Room*> availableRooms;
                for (const auto& room : dm.getRooms()) {
                    if (dm.isRoomAvailable(room.roomId, selectedReq->requestedDate, selectedReq->requestedStart, selectedReq->requestedEnd)) {
                        availableRooms.push_back(&room);
                    }
                }

                if (availableRooms.empty()) {
                    cout << "[WARNING] Cannot approve: No rooms available for the requested time. Disapproving.\n";
                    dm.updateMakeupRequestStatus(reqId, 2);
                    return;
                }

                cout << "\n--- Available Rooms for Makeup Session ---\n";
                cout << std::left << std::setw(10) << "ID" << std::setw(15) << "Room Name" << std::setw(20) << "Building" << std::endl;
                cout << std::string(45, '-') << std::endl;
                for (const auto* room : availableRooms) {
                    const Building* b = dm.getBuildingById(room->buildingId);
                    cout << std::left << std::setw(10) << room->roomId << std::setw(15) << room->roomName << std::setw(20) << (b ? b->getName() : "N/A") << std::endl;
                }

                long long rId = getLongInput("Enter the Room ID to assign for the makeup: ");
                bool roomExists = false;
                for (const auto* room : availableRooms) {
                    if (room->roomId == rId) {
                        roomExists = true;
                        break;
                    }
                }

                if (roomExists) {
                    dm.updateMakeupRequestStatus(reqId, 1);
                    dm.addScheduleEntry(selectedReq->sectionId, rId, selectedReq->requestedDate, selectedReq->requestedStart, selectedReq->requestedEnd, true);
                    cout << "Makeup Lab Approved and Scheduled in Room ID " << rId << std::endl;
                }
                else {
                    cout << "Invalid Room ID selected. Request Disapproved.\n";
                    dm.updateMakeupRequestStatus(reqId, 2);
                }

            }
            else if (action == 'D') {
                dm.updateMakeupRequestStatus(reqId, 2);
                cout << "Makeup Lab Disapproved.\n";
            }
            else {
                cout << "Invalid action. Request status unchanged.\n";
            }
        }
        else {
            cout << "Request ID not found in pending list.\n";
        }
    }

    
    void instructorMenu(int insId) {
        while (true) {
            cout << "\n--- Instructor Menu (" << loggedInUser->name << ") ---\n";
            cout << "1. View My Schedule\n";
            cout << "2. Request a Makeup Lab\n";
            cout << "0. Logout\n";
            int choice = getIntInput("Enter choice: ");

            if (choice == 0) return;
            switch (choice) {
            case 1: instructor_viewSchedule(insId); break;
            case 2: instructor_requestMakeupLab(insId); break;
            default: cout << "Invalid choice.\n";
            }
        }
    }

    void instructor_viewSchedule(int insId) {
        cout << "\n--- MY LAB SCHEDULE ---\n";
        const auto& sections = dm.getLabSections();
        const auto& schedules = dm.getSchedules();

        cout << left << setw(15) << "Section Code" << setw(15) << "Date"
            << setw(10) << "Start" << setw(10) << "End" << setw(30) << "Venue" << endl;
        cout << string(80, '-') << std::endl;

        for (const auto& ls : sections) {
            if (ls.getInstructorId() == insId) {
                for (const auto& se : schedules) {
                    if (se.sectionId == ls.sectionId) {
                        string venueInfo = reporter.getRoomInfo(se.roomId);
                        string status = se.isCanceled ? "Canceled" : (se.status == 1 ? "(Filled)" : "");

                        cout << left
                            << setw(15) << ls.getFullSectionCode()
                            << setw(15) << se.scheduledDate.toString()
                            << setw(10) << se.expectedStart.toString()
                            << setw(10) << se.expectedEnd.toString()
                            << setw(30) << venueInfo
                            << status << endl;
                    }
                }
            }
        }
    }

    void instructor_requestMakeupLab(int insId) {
        cout << "\n--- Your Sections ---\n";
        const auto& sections = dm.getLabSections();
        vector<int> mySections;
        cout << left << setw(10) << "ID" << setw(20) << "Section Code" << endl;
        cout << string(30, '-') << endl;
        for (const auto& ls : sections) {
            if (ls.getInstructorId() == insId) {
                
                cout << left << setw(10) << ls.sectionId << setw(20) << ls.getFullSectionCode() << endl;
                mySections.push_back(ls.sectionId);
            }
        }

        if (mySections.empty()) {
            cout << "You are not assigned to any lab sections.\n";
            return;
        }

        int secId = getLongInput("Enter Section ID for makeup: ");
        if (find(mySections.begin(), mySections.end(), secId) == mySections.end()) {
            cout << " Invalid Section ID or not assigned to you.\n";
            return;
        }

        Date date = getDateInput("Enter desired Makeup Date/Time ");
        Time start = getTimeInput("Enter desired Start Time ");
        Time end = getTimeInput("Enter desired End Time ");
        string reason = getStringInput("Enter reason for makeup: ");

        int newId = dm.addMakeupRequest(secId, insId, date, start, end, reason);
        cout << "Makeup Request submitted. Request ID: " << newId << ". Awaiting Academic Officer approval.\n";
    }

    void taMenu(int taId) {
        while (true) {
            cout << "\n--- TA Menu (" << loggedInUser->name << ") ---\n";
            cout << "1. View My Schedule\n";
            cout << "0. Logout\n";
            int choice = getIntInput("Enter choice: ");

            if (choice == 0) return;
            switch (choice) {
            case 1: ta_viewSchedule(taId); break;
            default: cout << "Invalid choice.\n";
            }
        }
    }

    void ta_viewSchedule(int taId) {
        cout << "\n--- MY TA SCHEDULE ---\n";
        const auto& sections = dm.getLabSections();
        const auto& schedules = dm.getSchedules();

        cout << left << setw(15) << "Section Code" << setw(15) << "Date"
            << setw(10) << "Start" << setw(10) << "End" << setw(30) << "Venue" << endl;
        cout << string(80, '-') << endl;

        for (const auto& ls : sections) {
            bool isMySection = false;
            for (long long assignedTaId : ls.taIds) {
                if (assignedTaId == taId) {
                    isMySection = true;
                    break;
                }
            }

            if (isMySection) {
                for (const auto& se : schedules) {
                    if (se.sectionId == ls.sectionId) {
                        string venueInfo = reporter.getRoomInfo(se.roomId);
                        string status = se.isCanceled ? "Canceled" : (se.status == 1 ? "(Filled)" : "");

                        cout << left

                            << setw(15) << ls.getFullSectionCode()
                            << setw(15) << se.scheduledDate.toString()
                            << setw(10) << se.expectedStart.toString()
                            << setw(10) << se.expectedEnd.toString()
                            << setw(30) << venueInfo
                            << status << endl;
                    }
                }
            }
        }
    }

    void hodMenu() {
        while (true) {
            cout << "\n--- Head of Department Menu (" << loggedInUser->name << ") ---\n";
            cout << "1. Generate Complete Lab Schedule Report\n";
            cout << "2. Generate Filled Time Sheet Report\n";
            cout << "3. Generate Lab Summary Report (Contact Hours, Leaves)\n";
            cout << "0. Logout\n";
            int choice = getIntInput("Enter choice: ");

            if (choice == 0) return;
            switch (choice) {
            case 1: reporter.generateLabScheduleReport(); break;
            case 2: {
                string week = getStringInput("Enter Target Semester (e.g., Fall 2024): ");
                reporter.generateTimeSheetReport(week);
                break;
            }
            case 3: {
                int secId = getLongInput("Enter Lab Section ID for Summary Report: ");
                reporter.generateLabSummaryReport(secId);
                break;
            }
            default: cout << "Invalid choice.\n";
            }
        }
    }

    void attendantMenu(int attId) {
        while (true) {
            cout << "\n--- Attendant Menu (" << loggedInUser->name << ") ---\n";
            cout << "1. Fill Time Sheet (Actual Lab Timings)\n";
            cout << "0. Logout\n";
            int choice = getIntInput("Enter choice: ");

            if (choice == 0) return;
            switch (choice) {
            case 1: attendant_fillTimeSheet(attId); break;
            default: cout << "Invalid choice.\n";
            }
        }
    }

    void attendant_fillTimeSheet(int attId) {
        cout << "\n--- FILL LAB TIME SHEET ---\n";

        vector<int> myRoomIds;
        for (const auto& building : dm.getBuildings()) {
            if (building.attendantId == attId) {
              
                for (const auto& room : dm.getRooms()) {
                    if (room.buildingId == building.buildingId) {
                        myRoomIds.push_back(room.roomId);
                    }
                }
            }
        }

        if (myRoomIds.empty()) {
            cout << "You are not assigned to manage any buildings/rooms.\n";
            return;
        }

        // Find sessions in those rooms that are scheduled but not yet filled
        vector<const ScheduleEntry*> sessionsToFill;
        cout << left << std::setw(10) << "Sch ID" << setw(15) << "Date" << setw(10) << "Exp. Start"
            << setw(10) << "Exp. End" << setw(30) << "Venue" << setw(15) << "Section" << endl;
        cout << string(90, '-') << endl;

        for (const auto& se : dm.getSchedules()) {
            bool isMyRoom = false;
            for (long long rId : myRoomIds) {
                if (se.roomId == rId) {
                    isMyRoom = true;
                    break;
                }
            }

            if (isMyRoom && se.status == 0) {
                sessionsToFill.push_back(&se);
                const LabSection* ls = dm.getLabSectionById(se.sectionId);

                cout << left << setw(10) << se.scheduleId
                    << setw(15) << se.scheduledDate.toString()
                    << setw(10) << se.expectedStart.toString()
                    << setw(10) << se.expectedEnd.toString()
                    << setw(30) << reporter.getRoomInfo(se.roomId)
                    << setw(15) << (ls ? ls->getFullSectionCode() : "N/A") << endl;
            }
        }

        if (sessionsToFill.empty()) {
            cout << "No scheduled sessions in your assigned rooms to fill timesheet for.\n";
            return;
        }

        long long schId = getLongInput("Enter Schedule ID to fill timesheet for (or 0 to exit): ");
        if (schId == 0) return;

        const ScheduleEntry* selectedEntry = nullptr;
        for (const auto* se : sessionsToFill) {
            if (se->scheduleId == schId) {
                selectedEntry = se;
                break;
            }
        }

        if (selectedEntry) {
            cout << "\n--- Filling Timesheet for Schedule ID: " << schId << " ---\n";
            Time actualStart = getTimeInput("Enter actual start time ");
            Time actualEnd = getTimeInput("Enter actual end time ");

            if (actualStart > actualEnd) {
                cout << "Actual end time cannot be before actual start time. Timesheet not saved.\n";
                return;
            }

            if (dm.updateScheduleActualTime(schId, actualStart, actualEnd)) {
                cout << "Timesheet filled successfully. Actual contact hours logged.\n";
            }
            else {
                cout << "Failed to update schedule entry.\n";
            }
        }
        else {
            cout << "Invalid Schedule ID or session is not assigned to your rooms.\n";
        }
    }

public:
    LabManagementSystem() : auth(dm), reporter(dm) {
        
        if (dm.persons.empty()) {
            cout << "Creating default Academic Officer (ID 1001, Pass: 123).\n";
            dm.addPerson("Default Academic Officer", "AcademicOfficer", "123");
        }
    }

    void run() {
        cout << "--- Welcome to the Lab Management System ---\n";
        while (true) {
            long long id = getLongInput("\nEnter your User ID (0 to exit): ");
            if (id == 0) break;
            string pass = getStringInput("Enter your Password: ");

            loggedInUser = auth.authenticate(id, pass);

            if (loggedInUser) {
                cout << "\nWelcome, " << loggedInUser->name << " (" << loggedInUser->role << ").\n";
                string role = loggedInUser->role;
                if (role == "AcademicOfficer") {
                    aoMenu();
                }
                else if (role == "Instructor") {
                    instructorMenu(loggedInUser->personId);
                }
                else if (role == "TA") {
                    taMenu(loggedInUser->personId);
                }
                else if (role == "HoD") {
                    hodMenu();
                }
                else if (role == "Attendant") {
                    attendantMenu(loggedInUser->personId);
                }
          
                loggedInUser = nullptr;
            }
            else {
                cout << "Invalid User ID or Password. Please try again.\n";
            }
        }
        cout << "\nExiting Lab Management System. Goodbye!\n";
    }
};


int main() {
    
    DataManager::initializeStaticIds();
    LabManagementSystem app;
    app.run();

    return 0;
}
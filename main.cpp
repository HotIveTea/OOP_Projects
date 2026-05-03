#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
using namespace std;
struct Date
{
    int day;
    int month;
    int year;
};
class ContributedMoney
{
public:
    virtual float CalculateMoney() const = 0;
    virtual ~ContributedMoney() {}
};
class People : public ContributedMoney
{
protected:
    string Name;
    string Gender;
    Date DayOfBirth;
    int Age;
    int ID;
    bool isHouseholdHead;

public:
    People(string _name, string _gender, Date _dob, int _id, bool _isHead)
        : Name(_name), Gender(_gender), DayOfBirth(_dob), ID(_id), isHouseholdHead(_isHead)
    {
        UpdateAge();
    }
    void UpdateAge()
    {
        time_t t = time(0);
        tm *now = localtime(&t);
        int currentYear = now->tm_year + 1900;
        int currentMonth = now->tm_mon + 1;
        int currentDay = now->tm_mday;
        Age = currentYear - DayOfBirth.year;
        if (currentMonth < DayOfBirth.month || (currentMonth == DayOfBirth.month && currentDay < DayOfBirth.day))
            Age--;
    }
    string getName() const { return Name; }
    Date getDateOfBirth() const { return DayOfBirth; }
    int getAge() const { return Age; }
    int getID() const { return ID; }
    void setName(string n) { Name = n; }
    void setGender(string g) { Gender = g; }
    void setDOB(Date d)
    {
        DayOfBirth = d;
        UpdateAge();
    }
    void setIsHead(bool h) { isHouseholdHead = h; }
    bool getIsHouseholdHead() const { return isHouseholdHead; }
    virtual string GetType() const = 0;
    string getGender() const { return Gender; }
    virtual float CalculateMoney() const override = 0;
    virtual string GetSchool() const { return "-"; }
    virtual string GetJob() const { return "-"; }
    virtual string GetSalary() const { return "-"; }
    virtual string GetCompanyName() const { return "-"; }
    virtual ~People() {}
};
class Retriee : public People
{
private:
    float pension;

public:
    Retriee(string _name, string _gender, Date _dob, int _id, bool _isHead, float _pension)
        : People(_name, _gender, _dob, _id, _isHead), pension(_pension) {}
    string GetType() const override { return "Nghi Huu"; }
    float CalculateMoney() const override { return pension * 0.02f; }
    string GetSalary() const override { return to_string((long long)pension) + " VND"; }
};
class Student : public People
{
private:
    string school;

public:
    Student(string _name, string _gender, Date _dob, int _id, bool _isHead, string _school)
        : People(_name, _gender, _dob, _id, _isHead), school(_school) {}
    string GetType() const override { return "HSSV"; }
    float CalculateMoney() const override { return 0; }
    string GetSchool() const override { return school; }
};
class Worker : public People
{
private:
    string job;
    float salary;
    string CompanyName;

public:
    Worker(string _name, string _gender, Date _dob, int _id, bool _isHead, string _job, string _company, float _salary)
        : People(_name, _gender, _dob, _id, _isHead), job(_job), CompanyName(_company), salary(_salary) {}
    string GetType() const override { return "Nguoi Lao Dong"; }
    float CalculateMoney() const override { return salary * 0.05f; }
    string GetJob() const override { return job; }
    string GetCompanyName() const override { return CompanyName; }
    string GetSalary() const override { return to_string((long long)salary) + " VND"; }
};
class Household
{
public:
    string HouseholdID;
    string Address;
    vector<People *> members;
    Household(string _id, string _address) : HouseholdID(_id), Address(_address) {}
    void AddMember(People *p) { members.push_back(p); }
    double CalculateTotal() const
    {
        double total = 0;
        for (People *p : members)
            total += p->CalculateMoney();
        return total;
    }
    void ReplaceMember(People *oldPerson, People *newPerson)
    {
        for (size_t i = 0; i < members.size(); i++)
        {
            if (members[i] == oldPerson)
            {
                delete members[i];
                members[i] = newPerson;
                return;
            }
        }
    }
    int GetMemberCount() const { return members.size(); }
    ~Household()
    {
        for (People *p : members)
            delete p;
        members.clear();
    }
};
class HouseholdManager
{
public:
    vector<Household *> householdList;

    Household *FindHousehold(string id)
    {
        for (Household *h : householdList)
        {
            if (h->HouseholdID == id)
                return h;
        }
        return nullptr;
    }
    bool DeleteMember(int cccd)
    {
        for (auto h : householdList)
        {
            for (auto it = h->members.begin(); it != h->members.end(); ++it)
            {
                if ((*it)->getID() == cccd)
                {
                    delete *it;           // Giải phóng bộ nhớ
                    h->members.erase(it); // Xóa khỏi danh sách
                    return true;
                }
            }
        }
        return false; // Không tìm thấy
    }
    vector<People *> GetAllPeople()
    {
        vector<People *> all;
        for (auto h : householdList)
        {
            for (auto p : h->members)
                all.push_back(p);
        }
        return all;
    }
    People *FindExtreme(int type, int criteria)
    {
        vector<People *> all = GetAllPeople();
        if (all.empty())
            return nullptr;

        People *extreme = all[0];
        for (People *p : all)
        {
            if (type == 0)
            { // Theo Tuổi
                if (criteria == 1 && p->getAge() > extreme->getAge())
                    extreme = p;
                else if (criteria == 0 && p->getAge() < extreme->getAge())
                    extreme = p;
            }
            else
            { // Theo Tiền đóng góp
                if (criteria == 1 && p->CalculateMoney() > extreme->CalculateMoney())
                    extreme = p;
                else if (criteria == 0 && p->CalculateMoney() < extreme->CalculateMoney())
                    extreme = p;
            }
        }
        return extreme;
    }
    void SortMembers(vector<People *> &list, int type, int order)
    {
        if (type == 0)
        { // Sắp xếp theo Tuổi
            if (order == 0)
                sort(list.begin(), list.end(), [](People *a, People *b)
                     { return a->getAge() < b->getAge(); });
            else
                sort(list.begin(), list.end(), [](People *a, People *b)
                     { return a->getAge() > b->getAge(); });
        }
        else
        { // Sắp xếp theo Tiền
            if (order == 0)
                sort(list.begin(), list.end(), [](People *a, People *b)
                     { return a->CalculateMoney() < b->CalculateMoney(); });
            else
                sort(list.begin(), list.end(), [](People *a, People *b)
                     { return a->CalculateMoney() > b->CalculateMoney(); });
        }
    }
    void ExportCSV(string filename)
    {
        ofstream f(filename);
        f << "MaHo,DiaChi,PhanLoai,ID,HoTen,GioiTinh,Ngay,Thang,Nam,ChuHo,Truong_Nghe,CongTy,ThuNhap\n";
        for (auto h : householdList)
        {
            for (auto p : h->members)
            {
                f << h->HouseholdID << "," << h->Address << "," << p->GetType() << ","
                  << p->getID() << "," << p->getName() << "," << p->getGender() << ","
                  << p->getDateOfBirth().day << "," << p->getDateOfBirth().month << "," << p->getDateOfBirth().year << ","
                  << p->getIsHouseholdHead() << ","
                  << (p->GetType() == "HSSV" ? p->GetSchool() : p->GetJob()) << ","
                  << p->GetCompanyName() << ","
                  << (p->GetType() == "HSSV" ? "0" : to_string((long long)(p->CalculateMoney() / (p->GetType() == "Lao Dong" ? 0.05 : 0.02)))) << "\n";
            }
        }
        f.close();
    }
    void ClearData()
    {
        for (Household *h : householdList)
        {
            delete h;
        }
        householdList.clear();
    }
    void ImportCSV(string filename, bool overwrite = false)
    {
        ifstream f(filename);
        if (!f.is_open())
            return;
        if (overwrite == true)
        {
            ClearData();
        }

        string line, word;
        getline(f, line); // Bỏ qua dòng tiêu đề
        while (getline(f, line))
        {
            stringstream s(line);
            vector<string> row;
            while (getline(s, word, ','))
                row.push_back(word);

            if (row.size() < 13)
                continue;

            string hhID = row[0], addr = row[1], type = row[2];
            int id = stoi(row[3]);
            string name = row[4], gender = row[5];
            Date dob = {stoi(row[6]), stoi(row[7]), stoi(row[8])};
            bool isHead = stoi(row[9]);
            string extra1 = row[10], extra2 = row[11];
            float money = stof(row[12]);

            People *newP = nullptr;
            if (type == "HSSV")
                newP = new Student(name, gender, dob, id, isHead, extra1);
            else if (type == "Lao Dong")
                newP = new Worker(name, gender, dob, id, isHead, extra1, extra2, money);
            else if (type == "Nghi Huu")
                newP = new Retriee(name, gender, dob, id, isHead, money);

            if (newP)
            {
                Household *h = FindHousehold(hhID);
                if (!h)
                {
                    h = new Household(hhID, addr);
                    householdList.push_back(h);
                }
                h->AddMember(newP);
            }
        }
        f.close();
    }
    bool FindPersonAndHousehold(int targetID, Household *&outHouse, People *&outPerson)
    {
        for (Household *h : householdList)
        {
            for (People *p : h->members)
            {
                if (p->getID() == targetID)
                {
                    outHouse = h;
                    outPerson = p;
                    return true;
                }
            }
        }
        return false;
    }
    ~HouseholdManager()
    {
        ClearData();
    }
    void GUI()
    {
        SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
        InitWindow(1440, 800, "Quan Ly Dan Cu Pro - Web UI Edition");
        SetTargetFPS(60);

        Color sidebarColor = {30, 41, 59, 255};
        Color bgColor = {241, 245, 249, 255};
        Color primaryColor = {59, 130, 246, 255};
        Color textDark = {15, 23, 42, 255};

        GuiSetStyle(DEFAULT, TEXT_SIZE, 18);
        GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(primaryColor));
        GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
        // ==========================================
        // KHAI BÁO TẤT CẢ BIẾN TRẠNG THÁI Ở ĐÂY (NGOÀI VÒNG LẶP)
        // ==========================================
        int activeTab = 0;
        int inputState = 0;
        Household *currentHousehold = nullptr;

        Vector2 scrollList = {0, 0};
        Vector2 scrollInput = {0, 0};
        Rectangle viewList = {0};
        Rectangle viewInput = {0};

        // Các biến cho form Nhập liệu (Tab 1)
        char hhIdInput[32] = "", addressInput[128] = "";
        int isHeadSelect = 0;
        char idInput[32] = "", nameInput[128] = "";
        char dayInput[16] = "", monthInput[16] = "", yearInput[16] = "";
        int residentType = 0;
        int genderSelect = 0;
        char schoolInput[128] = "", jobInput[128] = "", companyInput[128] = "", moneyInput[32] = "";
        bool editMode[15] = {false};

        // Các biến cho Sắp xếp Dropdown (Tab 0)
        int sortActive = 0;
        bool sortDropdownEdit = false;
        int sortOrder = 0;

        // Các biến cho thao tác Xóa (Tab 0)
        char quickDelInput[32] = "";
        bool editQuickDel = false;
        string notifMsg = "";

        // Các biến cho tính năng Sửa (Tab 0 -> Tab 1)
        char searchInput[32] = "";
        bool editSearch = false;
        People *personToEdit = nullptr;

        HouseholdManager manager;

        // TỰ ĐỘNG LOAD DỮ LIỆU KHI MỞ APP
        manager.ImportCSV("data.csv");
        char importFilenameInput[64] = ""; // Ô nhập tên file
        bool editImportFilename = false;   // Trạng thái click
        string fileNotifMsg = "";          // Dòng chữ thông báo thành công/thất bại
        // ==========================================
        // VÒNG LẶP CHÍNH CỦA GIAO DIỆN
        // ==========================================
        while (!WindowShouldClose())
        {
            int currentWidth = GetScreenWidth();
            int currentHeight = GetScreenHeight();

            BeginDrawing();
            ClearBackground(bgColor);

            // --- SIDEBAR ---
            DrawRectangle(0, 0, 250, currentHeight, sidebarColor);
            DrawText("QUAN LY", 25, 40, 28, WHITE);
            DrawText("KHU PHO", 25, 75, 28, primaryColor);

            if (activeTab == 0)
                DrawRectangle(0, 150, 250, 50, {51, 65, 85, 255});
            if (GuiButton((Rectangle){25, 155, 200, 40}, "Danh sach cu dan"))
                activeTab = 0;

            if (activeTab == 1)
                DrawRectangle(0, 210, 250, 50, {51, 65, 85, 255});
            // Nếu đang ở chế độ sửa thì đổi tên nút
            if (GuiButton((Rectangle){25, 215, 200, 40}, (personToEdit ? "Sua thong tin" : "Them moi cu dan")))
                activeTab = 1;

            if (activeTab == 2)
                DrawRectangle(0, 270, 250, 50, {51, 65, 85, 255});
            if (GuiButton((Rectangle){25, 275, 200, 40}, "Luu & Thong ke"))
                activeTab = 2;

            // --- MAIN CONTENT KHUNG TRẮNG ---
            Rectangle cardRect = {280, 40, (float)currentWidth - 310, (float)currentHeight - 80};
            DrawRectangleRounded(cardRect, 0.02f, 10, WHITE);

            // ----------------------------------------------------
            // TAB 0: DANH SÁCH & CÔNG CỤ TÌM KIẾM/SẮP XẾP
            // ----------------------------------------------------
            if (activeTab == 0)
            {
                // TOẠ ĐỘ ĐỘNG
                float startX = 310;
                float usableWidth = (float)currentWidth - startX - 40;
                float colID = startX;
                float colName = startX + usableWidth * 0.05f;
                float colGender = startX + usableWidth * 0.22f;
                float colAge = startX + usableWidth * 0.30f;
                float colDob = startX + usableWidth * 0.35f;
                float colType = startX + usableWidth * 0.45f;
                float colSchool = startX + usableWidth * 0.55f;
                float colJob = startX + usableWidth * 0.66f;
                float colCompany = startX + usableWidth * 0.75f;
                float colIncome = startX + usableWidth * 0.85f;
                float colContrib = startX + usableWidth * 0.94f;

                DrawText("ID", colID, 130, 16, GRAY);
                DrawText("HO TEN", colName, 130, 16, GRAY);
                DrawText("GIOI TINH", colGender, 130, 16, GRAY);
                DrawText("TUOI", colAge, 130, 16, GRAY);
                DrawText("NGAY SINH", colDob, 130, 16, GRAY);
                DrawText("PHAN LOAI", colType, 130, 16, GRAY);
                DrawText("TRUONG", colSchool, 130, 16, GRAY);
                DrawText("NGHE", colJob, 130, 16, GRAY);
                DrawText("CONG TY", colCompany, 130, 16, GRAY);
                DrawText("THU NHAP", colIncome, 130, 16, GRAY);
                DrawText("DONG GOP", colContrib, 130, 16, GRAY);

                Rectangle panelBounds = {295, 155, (float)currentWidth - 305, (float)currentHeight - 210};

                // --- VẼ DANH SÁCH BÊN DƯỚI TRƯỚC ---
                if (sortActive == 0)
                {
                    float totalContentHeight = 0;
                    for (Household *h : manager.householdList)
                        totalContentHeight += 50 + (h->members.size() * 45) + 20;
                    Rectangle contentBounds = {0, 0, (float)currentWidth - 325, (totalContentHeight > panelBounds.height) ? totalContentHeight : panelBounds.height};

                    GuiScrollPanel(panelBounds, NULL, contentBounds, &scrollList, &viewList);
                    BeginScissorMode(viewList.x, viewList.y, viewList.width, viewList.height);
                    float rowY = panelBounds.y + scrollList.y;

                    for (Household *h : manager.householdList)
                    {
                        DrawRectangleRounded({300, rowY, (float)currentWidth - 340, 40}, 0.1f, 10, {226, 232, 240, 255});
                        DrawText(TextFormat("[ MA HO: %s ]  -  Dia chi: %s", h->HouseholdID.c_str(), h->Address.c_str()), 310, rowY + 10, 18, textDark);
                        DrawText(TextFormat("TONG HO DONG GOP: %.0f VND", h->CalculateTotal()), colIncome - 50, rowY + 10, 18, RED);
                        rowY += 45;

                        for (People *p : h->members)
                        {
                            DrawText(TextFormat("%d", p->getID()), colID, rowY + 10, 18, textDark);
                            string displayTitle = p->getName();
                            if (p->getIsHouseholdHead())
                                displayTitle += " (CHU HO)";
                            DrawText(displayTitle.c_str(), colName, rowY + 10, 18, (p->getIsHouseholdHead() ? BLUE : textDark));
                            DrawText(p->getGender().c_str(), colGender, rowY + 10, 18, textDark);
                            DrawText(TextFormat("%d", p->getAge()), colAge, rowY + 10, 18, textDark);
                            Date dob = p->getDateOfBirth();
                            DrawText(TextFormat("%d/%d/%d", dob.day, dob.month, dob.year), colDob, rowY + 10, 18, textDark);
                            DrawText(p->GetType().c_str(), colType, rowY + 10, 18, DARKGRAY);
                            DrawText(p->GetSchool().c_str(), colSchool, rowY + 10, 18, textDark);
                            DrawText(p->GetJob().c_str(), colJob, rowY + 10, 18, textDark);
                            DrawText(p->GetCompanyName().c_str(), colCompany, rowY + 10, 18, textDark);
                            DrawText(p->GetSalary().c_str(), colIncome, rowY + 10, 18, textDark);
                            DrawText(TextFormat("%.0f VND", p->CalculateMoney()), colContrib, rowY + 10, 18, GREEN);
                            rowY += 40;
                        }
                        rowY += 20;
                    }
                    EndScissorMode();
                }
                else
                {
                    vector<People *> flatList = manager.GetAllPeople();
                    int type = (sortActive == 1) ? 0 : 1;
                    manager.SortMembers(flatList, type, sortOrder);

                    float totalContentHeight = flatList.size() * 45 + 20;
                    Rectangle contentBounds = {0, 0, (float)currentWidth - 325, (totalContentHeight > panelBounds.height) ? totalContentHeight : panelBounds.height};

                    GuiScrollPanel(panelBounds, NULL, contentBounds, &scrollList, &viewList);
                    BeginScissorMode(viewList.x, viewList.y, viewList.width, viewList.height);
                    float rowY = panelBounds.y + scrollList.y;

                    for (People *p : flatList)
                    {
                        DrawRectangleRounded({300, rowY, (float)currentWidth - 340, 35}, 0.1f, 10, {248, 250, 252, 255});
                        DrawText(TextFormat("%d", p->getID()), colID, rowY + 10, 18, textDark);
                        DrawText(p->getName().c_str(), colName, rowY + 10, 18, textDark);
                        DrawText(p->getGender().c_str(), colGender, rowY + 10, 18, textDark);
                        DrawText(TextFormat("%d", p->getAge()), colAge, rowY + 10, 18, textDark);
                        Date dob = p->getDateOfBirth();
                        DrawText(TextFormat("%d/%d/%d", dob.day, dob.month, dob.year), colDob, rowY + 10, 18, textDark);
                        DrawText(p->GetType().c_str(), colType, rowY + 10, 18, DARKGRAY);
                        DrawText(p->GetSchool().c_str(), colSchool, rowY + 10, 18, textDark);
                        DrawText(p->GetJob().c_str(), colJob, rowY + 10, 18, textDark);
                        DrawText(p->GetCompanyName().c_str(), colCompany, rowY + 10, 18, textDark);
                        DrawText(p->GetSalary().c_str(), colIncome, rowY + 10, 18, textDark);
                        DrawText(TextFormat("%.0f VND", p->CalculateMoney()), colContrib, rowY + 10, 18, GREEN);
                        rowY += 40;
                    }
                    EndScissorMode();
                }

                // --- VẼ TOP BAR (Search & Sort) NẰM TRÊN CÙNG ĐỂ KHÔNG BỊ DANH SÁCH CHE LẤP ---
                DrawRectangle(280, 40, currentWidth - 310, 80, WHITE);
                DrawLine(310, 120, currentWidth - 60, 120, LIGHTGRAY);

                DrawText("Thao tac (Nhap ID):", 310, 60, 18, textDark);
                if (GuiTextBox((Rectangle){480, 55, 120, 35}, searchInput, 32, editSearch))
                    editSearch = !editSearch;

                // NÚT XÓA
                if (GuiButton((Rectangle){610, 55, 70, 35}, "XOA"))
                {
                    if (manager.DeleteMember(atoi(searchInput)))
                        notifMsg = "Da xoa!";
                    else
                        notifMsg = "Khong tim thay!";
                }

                // NÚT SỬA (CHUYỂN SANG TAB 1 VÀ ĐỔ DỮ LIỆU)
                if (GuiButton((Rectangle){690, 55, 70, 35}, "SUA"))
                {
                    int targetID = atoi(searchInput);
                    if (manager.FindPersonAndHousehold(targetID, currentHousehold, personToEdit))
                    {
                        activeTab = 1;
                        inputState = 1;

                        snprintf(idInput, 32, "%d", personToEdit->getID());
                        snprintf(nameInput, 128, "%s", personToEdit->getName().c_str());
                        snprintf(dayInput, 16, "%d", personToEdit->getDateOfBirth().day);
                        snprintf(monthInput, 16, "%d", personToEdit->getDateOfBirth().month);
                        snprintf(yearInput, 16, "%d", personToEdit->getDateOfBirth().year);

                        isHeadSelect = personToEdit->getIsHouseholdHead() ? 1 : 0;
                        genderSelect = (personToEdit->getGender() == "Nu") ? 1 : 0;

                        string type = personToEdit->GetType();
                        if (type == "HSSV")
                        {
                            residentType = 0;
                            snprintf(schoolInput, 128, "%s", personToEdit->GetSchool().c_str());
                        }
                        else if (type == "Lao Dong")
                        {
                            residentType = 1;
                            snprintf(jobInput, 128, "%s", personToEdit->GetJob().c_str());
                            snprintf(companyInput, 128, "%s", personToEdit->GetCompanyName().c_str());
                            string incomeRaw = personToEdit->GetSalary();
                            snprintf(moneyInput, 32, "%s", incomeRaw.substr(0, incomeRaw.find(' ')).c_str());
                        }
                        else
                        {
                            residentType = 2;
                            string incomeRaw = personToEdit->GetSalary();
                            snprintf(moneyInput, 32, "%s", incomeRaw.substr(0, incomeRaw.find(' ')).c_str());
                        }
                        notifMsg = "";
                    }
                    else
                    {
                        notifMsg = "Khong tim thay!";
                    }
                }
                DrawText(notifMsg.c_str(), 480, 95, 16, RED);

                // NÚT DROPDOWN SẮP XẾP
                DrawText("Sap xep:", 800, 60, 18, textDark);
                if (sortActive != 0)
                {
                    GuiToggleGroup((Rectangle){900, 55, 80, 35}, "Tang;Giam", &sortOrder);
                }
                // DROPDOWN VẼ CUỐI CÙNG TRONG VÙNG NÀY ĐỂ MENU XỔ XUỐNG ĐƯỢC ƯU TIÊN HIỂN THỊ
                if (GuiDropdownBox((Rectangle){1080, 55, 180, 35}, "Theo Ho;Theo Tuoi;Theo Tien", &sortActive, sortDropdownEdit))
                {
                    sortDropdownEdit = !sortDropdownEdit;
                }
            }
            // ----------------------------------------------------
            // TAB 1: THÊM MỚI / CHỈNH SỬA CƯ DÂN
            // ----------------------------------------------------
            else if (activeTab == 1)
            {
                DrawText("Them Moi Cu Dan", 310, 60, 24, textDark);

                Rectangle panelBounds = {310, 100, 800, (float)currentHeight - 150};
                Rectangle contentBounds = {0, 0, 780, 1000};

                GuiScrollPanel(panelBounds, NULL, contentBounds, &scrollInput, &viewInput);
                BeginScissorMode(viewInput.x, viewInput.y, viewInput.width, viewInput.height);

                float startX = panelBounds.x + 15;
                float startY = panelBounds.y + 10 + scrollInput.y;

                if (inputState == 0)
                {
                    DrawText("BUOC 1: XAC NHAN HO GIA DINH", startX, startY, 20, BLUE);

                    startY += 50;
                    DrawText("Nhap Ma Ho:", startX, startY, 18, textDark);
                    if (GuiTextBox((Rectangle){startX, startY + 25, 200, 40}, hhIdInput, 32, editMode[11]))
                        editMode[11] = !editMode[11];

                    DrawText("Dia chi (Bo trong neu Ho da ton tai):", startX + 220, startY, 18, textDark);
                    if (GuiTextBox((Rectangle){startX + 220, startY + 25, 450, 40}, addressInput, 128, editMode[12]))
                        editMode[12] = !editMode[12];

                    startY += 90;
                    if (GuiButton((Rectangle){startX, startY, 250, 45}, "TIEP TUC / TAO MOI HO"))
                    {
                        string hhID = hhIdInput;
                        if (hhID != "")
                        {
                            currentHousehold = manager.FindHousehold(hhID);
                            if (currentHousehold == nullptr)
                            {
                                currentHousehold = new Household(hhID, addressInput);
                                manager.householdList.push_back(currentHousehold);
                            }
                            inputState = 1;
                        }
                    }

                    startY += 100;
                    DrawText("DANH SACH CAC HO DA TON TAI TRONG HE THONG:", startX, startY, 16, DARKGRAY);
                    DrawLine(startX, startY + 25, startX + 600, startY + 25, LIGHTGRAY);
                    startY += 40;

                    for (Household *h : manager.householdList)
                    {
                        DrawText(TextFormat("> Ma Ho: %s   |   Dia chi: %s (%d thanh vien)",
                                            h->HouseholdID.c_str(), h->Address.c_str(), h->members.size()),
                                 startX, startY, 18, GRAY);
                        startY += 30;
                    }
                }
                else if (inputState == 1)
                {
                    DrawText(TextFormat("BUOC 2: THEM THANH VIEN CHO HO [ %s ]", currentHousehold->HouseholdID.c_str()), startX, startY, 20, GREEN);
                    DrawText(TextFormat("Dia chi: %s", currentHousehold->Address.c_str()), startX, startY + 25, 16, GRAY);

                    startY += 70;
                    DrawText("La Chu Ho:", startX, startY, 18, textDark);
                    GuiToggleGroup((Rectangle){startX, startY + 25, 120, 40}, "Khong;Co", &isHeadSelect);

                    startY += 80;
                    DrawText("Chon loai cu dan:", startX, startY, 18, textDark);
                    GuiToggleGroup((Rectangle){startX, startY + 25, 240, 40}, "HSSV;Lao Dong;Nghi Huu", &residentType);

                    startY += 80;
                    DrawText("ID Ca nhan:", startX, startY, 18, textDark);
                    if (GuiTextBox((Rectangle){startX, startY + 25, 200, 40}, idInput, 32, editMode[0]))
                        editMode[0] = !editMode[0];

                    startY += 80;
                    DrawText("Ho va ten:", startX, startY, 18, textDark);
                    if (GuiTextBox((Rectangle){startX, startY + 25, 400, 40}, nameInput, 128, editMode[1]))
                        editMode[1] = !editMode[1];

                    startY += 80;
                    DrawText("Gioi tinh:", startX, startY, 18, textDark);
                    GuiToggleGroup((Rectangle){startX, startY + 25, 100, 40}, "Nam;Nu", &genderSelect);

                    startY += 80;
                    DrawText("Ngay/Thang/Nam sinh:", startX, startY, 18, textDark);
                    if (GuiTextBox((Rectangle){startX, startY + 25, 60, 40}, dayInput, 16, editMode[3]))
                        editMode[3] = !editMode[3];
                    if (GuiTextBox((Rectangle){startX + 70, startY + 25, 60, 40}, monthInput, 16, editMode[4]))
                        editMode[4] = !editMode[4];
                    if (GuiTextBox((Rectangle){startX + 140, startY + 25, 80, 40}, yearInput, 16, editMode[5]))
                        editMode[5] = !editMode[5];

                    startY += 80;
                    DrawLine(startX, startY, startX + 500, startY, GRAY);
                    startY += 20;

                    if (residentType == 0)
                    {
                        DrawText("Ten truong hoc:", startX, startY, 18, textDark);
                        if (GuiTextBox((Rectangle){startX, startY + 25, 400, 40}, schoolInput, 128, editMode[6]))
                            editMode[6] = !editMode[6];
                    }
                    else if (residentType == 1)
                    {
                        DrawText("Nghe nghiep:", startX, startY, 18, textDark);
                        if (GuiTextBox((Rectangle){startX, startY + 25, 400, 40}, jobInput, 128, editMode[7]))
                            editMode[7] = !editMode[7];
                        startY += 80;
                        DrawText("Ten cong ty:", startX, startY, 18, textDark);
                        if (GuiTextBox((Rectangle){startX, startY + 25, 400, 40}, companyInput, 128, editMode[10]))
                            editMode[10] = !editMode[10];
                        startY += 80;
                        DrawText("Muc luong (VND):", startX, startY, 18, textDark);
                        if (GuiTextBox((Rectangle){startX, startY + 25, 300, 40}, moneyInput, 32, editMode[8]))
                            editMode[8] = !editMode[8];
                    }
                    else if (residentType == 2)
                    {
                        DrawText("Luong huu (VND):", startX, startY, 18, textDark);
                        if (GuiTextBox((Rectangle){startX, startY + 25, 300, 40}, moneyInput, 32, editMode[9]))
                            editMode[9] = !editMode[9];
                    }

                    startY += 100;
                    // LƯU Ý: Lớp phải đúng tên HSSV, NguoiLaoDong, NguoiNghiHuu như định nghĩa OOP
                    if (GuiButton((Rectangle){325, startY, 220, 45}, (personToEdit ? "CAP NHAT THANH VIEN" : "LUU THANH VIEN")))
                    {
                        int id = atoi(idInput);
                        Date dob = {atoi(dayInput), atoi(monthInput), atoi(yearInput)};
                        string name = nameInput;
                        string genderStr = (genderSelect == 0) ? "Nam" : "Nu";
                        bool isHead = (isHeadSelect == 1);

                        People *newPerson = nullptr;
                        if (residentType == 0)
                            newPerson = new Student(name, genderStr, dob, id, isHead, schoolInput);
                        else if (residentType == 1)
                            newPerson = new Worker(name, genderStr, dob, id, isHead, jobInput, companyInput, atof(moneyInput));
                        else if (residentType == 2)
                            newPerson = new Retriee(name, genderStr, dob, id, isHead, atof(moneyInput));

                        if (newPerson != nullptr)
                        {
                            if (personToEdit)
                            {
                                currentHousehold->ReplaceMember(personToEdit, newPerson);
                                personToEdit = nullptr;
                            }
                            else
                            {
                                currentHousehold->AddMember(newPerson);
                            }
                        }

                        idInput[0] = nameInput[0] = '\0';
                        dayInput[0] = monthInput[0] = yearInput[0] = '\0';
                        schoolInput[0] = jobInput[0] = companyInput[0] = moneyInput[0] = '\0';
                        isHeadSelect = 0;
                        scrollInput.y = 0;
                        activeTab = 0;
                        inputState = 0;
                    }

                    if (GuiButton((Rectangle){560, startY, 200, 45}, (personToEdit ? "HUY SUA" : "HOAN TAT HO NAY")))
                    {
                        inputState = 0;
                        currentHousehold = nullptr;
                        personToEdit = nullptr;
                        hhIdInput[0] = addressInput[0] = '\0';
                        scrollInput.y = 0;
                        activeTab = 0;
                    }
                }
                EndScissorMode();
            }
            // ----------------------------------------------------
            // TAB 2: LƯU & THỐNG KÊ
            // ----------------------------------------------------
            else if (activeTab == 2)
            {
                DrawText("Trung Tam Quan Ly Du Lieu", 310, 60, 24, textDark);
                // --- TÍNH NĂNG 1: XUẤT DỮ LIỆU ---
                DrawText("1. XUAT DU LIEU (BACKUP)", 310, 110, 18, BLUE);
                DrawText("Luu toan bo danh sach hien tai vao he thong (Mac dinh: data.csv)", 310, 140, 16, GRAY);
                if (GuiButton((Rectangle){310, 170, 250, 45}, "GHI DE VAO FILE DATA.CSV"))
                {
                    manager.ExportCSV("data.csv");
                    fileNotifMsg = ">> Da xuat du lieu ra file data.csv thanh cong!";
                }
                // --- TÍNH NĂNG 2: NHẬP DỮ LIỆU TỪ FILE KHÁC ---
                DrawLine(310, 250, currentWidth - 60, 250, LIGHTGRAY);
                DrawText("2. NHAP DU LIEU (IMPORT)", 310, 280, 18, BLUE);
                DrawText("Nhap ten file CSV can doc (VD: khupho2.csv). Chon che do Nhap thich hop:", 310, 310, 16, GRAY);
                // Ô nhập tên file
                if (GuiTextBox((Rectangle){310, 340, 300, 40}, importFilenameInput, 64, editImportFilename))
                {
                    editImportFilename = !editImportFilename;
                }
                // NÚT 1: ĐỌC VÀ GHÉP THÊM (overwrite = false)
                if (GuiButton((Rectangle){630, 340, 180, 40}, "DOC GHEP THEM"))
                {
                    string fileToLoad = importFilenameInput;
                    if (fileToLoad != "")
                    {
                        manager.ImportCSV(fileToLoad, false); // Tham số false = KHÔNG ghi đè
                        fileNotifMsg = ">> Da nap GHEP THEM du lieu tu [" + fileToLoad + "] vao he thong!";
                        importFilenameInput[0] = '\0';
                    }
                    else
                    {
                        fileNotifMsg = ">> Loi: Vui long nhap ten file!";
                    }
                }
                // NÚT 2: ĐỌC VÀ GHI ĐÈ (overwrite = true)
                if (GuiButton((Rectangle){830, 340, 180, 40}, "DOC GHI DE"))
                {
                    string fileToLoad = importFilenameInput;
                    if (fileToLoad != "")
                    {
                        manager.ImportCSV(fileToLoad, true); // Tham số true = GHI ĐÈ (Xóa hết cũ)
                        fileNotifMsg = ">> Da xoa du lieu cu. Nap MỚI HOÀN TOÀN tu [" + fileToLoad + "]!";
                        importFilenameInput[0] = '\0';
                    }
                    else
                    {
                        fileNotifMsg = ">> Loi: Vui long nhap ten file!";
                    }
                }
                // --- HIỂN THỊ THÔNG BÁO CHUNG ---
                DrawText(fileNotifMsg.c_str(), 310, 420, 18, RED);
            }
            EndDrawing();
        }
        CloseWindow();
    }
};
int main()
{
    HouseholdManager p;
    p.GUI();
    return 0;
}
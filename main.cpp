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
    void ImportCSV(string filename)
    {
        ifstream f(filename);
        if (!f.is_open())
            return;
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
    ~HouseholdManager()
    {
        for (Household *h : householdList)
            delete h;
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

        int activeTab = 0;
        Vector2 scrollList = {0, 0};
        Vector2 scrollInput = {0, 0};
        Rectangle viewList = {0};
        Rectangle viewInput = {0};

        // Các biến cho form
        char hhIdInput[32] = "", addressInput[128] = "";
        int isHeadSelect = 0;
        char idInput[32] = "", nameInput[128] = "";
        char dayInput[16] = "", monthInput[16] = "", yearInput[16] = "";
        int residentType = 0;
        int genderSelect = 0;
        char schoolInput[128] = "", jobInput[128] = "", companyInput[128] = "", moneyInput[32] = "";
        bool editMode[15] = {false};

        int inputState = 0;
        Household *currentHousehold = nullptr;

        // Các biến cho Web-like UI (Sắp xếp & Xóa)
        int sortMode = 0; // 0: Theo Hộ, 1: Tuổi Tăng, 2: Tuổi Giảm, 3: Tiền Tăng, 4: Tiền Giảm
        char quickDelInput[32] = "";
        bool editQuickDel = false;
        string notifMsg = "";

        HouseholdManager manager;

        // TỰ ĐỘNG LOAD DỮ LIỆU KHI MỞ APP
        manager.ImportCSV("data.csv");

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
            if (GuiButton((Rectangle){25, 215, 200, 40}, "Them moi cu dan"))
                activeTab = 1;

            // Vẫn giữ tab công cụ để Ghi file nếu bro muốn
            if (activeTab == 2)
                DrawRectangle(0, 270, 250, 50, {51, 65, 85, 255});
            if (GuiButton((Rectangle){25, 275, 200, 40}, "Luu & Thong ke"))
                activeTab = 2;

            // --- MAIN CONTENT ---
            Rectangle cardRect = {280, 40, (float)currentWidth - 310, (float)currentHeight - 80};
            DrawRectangleRounded(cardRect, 0.02f, 10, WHITE);

            if (activeTab == 0)
            {
                // --- TOP BAR: THANH CÔNG CỤ NHƯ WEB ---
                DrawText("Xoa theo CCCD:", 310, 60, 18, textDark);
                if (GuiTextBox((Rectangle){460, 55, 150, 35}, quickDelInput, 32, editQuickDel))
                    editQuickDel = !editQuickDel;
                if (GuiButton((Rectangle){620, 55, 80, 35}, "XOA"))
                {
                    if (manager.DeleteMember(atoi(quickDelInput)))
                        notifMsg = "Da xoa xoa thanh cong!";
                    else
                        notifMsg = "Khong tim thay CCCD!";
                }
                DrawText(notifMsg.c_str(), 460, 95, 16, RED);

                DrawText("Sap xep theo:", 750, 60, 18, textDark);
                GuiToggleGroup((Rectangle){880, 55, 100, 35}, "Theo Ho;Tuoi Tang;Tuoi Giam;Tien Tang;Tien Giam", &sortMode);

                DrawLine(310, 120, currentWidth - 60, 120, LIGHTGRAY);

                // ==============================================================
                // THUẬT TOÁN TOẠ ĐỘ ĐỘNG (TỰ ĐỘNG DÃN CỘT RA MÉP MÀN HÌNH)
                // ==============================================================
                float startX = 310;
                float usableWidth = (float)currentWidth - startX - 40; // Trừ lề phải 40px cho đẹp

                float colID = startX;
                float colName = startX + usableWidth * 0.05f;   // Tên bắt đầu sau ID 5%
                float colGender = startX + usableWidth * 0.22f; // Dành 17% siêu rộng cho Tên + (CHỦ HỘ)
                float colAge = startX + usableWidth * 0.30f;
                float colDob = startX + usableWidth * 0.35f;
                float colType = startX + usableWidth * 0.45f;
                float colSchool = startX + usableWidth * 0.55f;
                float colJob = startX + usableWidth * 0.66f;
                float colCompany = startX + usableWidth * 0.75f;
                float colIncome = startX + usableWidth * 0.85f;
                float colContrib = startX + usableWidth * 0.94f;

                // HEADER CỘT DANH SÁCH (Sử dụng biến động)
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

                if (sortMode == 0)
                {
                    // CHẾ ĐỘ 1: HIỂN THỊ THEO HỘ GIA ĐÌNH
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

                        // Chữ Tổng đóng góp cũng đẩy động ra sát mép phải luôn
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
                    // CHẾ ĐỘ 2: PHẲNG HÓA VÀ SẮP XẾP
                    vector<People *> flatList = manager.GetAllPeople();
                    int type = (sortMode == 1 || sortMode == 2) ? 0 : 1;
                    int order = (sortMode == 1 || sortMode == 3) ? 0 : 1;
                    manager.SortMembers(flatList, type, order);

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
            }
            else if (activeTab == 2)
            {
                // TAB LƯU THỐNG KÊ (Để Ghi đè file CSV)
                DrawText("Luu tru Du lieu he thong", 310, 60, 24, textDark);
                if (GuiButton((Rectangle){310, 120, 250, 45}, "GHI DE VAO FILE DATA.CSV"))
                {
                    manager.ExportCSV("data.csv");
                }
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
}
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
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
    Date DayOfBirth;
    int Age;
    int ID;

public:
    People(string _name, Date _dob, int _id) : Name(_name), DayOfBirth(_dob), ID(_id)
    {
        time_t t = time(0);
        tm *now = localtime(&t);
        int currentYear = now->tm_year + 1900;
        int currentMonth = now->tm_mon + 1;
        int currentDay = now->tm_mday;

        Age = currentYear - DayOfBirth.year;
        if (currentMonth < DayOfBirth.month || (currentMonth == DayOfBirth.month && currentDay < DayOfBirth.day))
        {
            Age--;
        }
    }
    string getName() const { return Name; }
    Date getDateOfBirth() const { return DayOfBirth; }
    int getAge() const { return Age; }
    int getID() const { return ID; }
    void setName(string name)
    {
        Name = name;
    }
    void setDate(int d, int m, int y)
    {
        DayOfBirth.day = d;
        DayOfBirth.month = m;
        DayOfBirth.year = y;
    }
    void setAge(int age)
    {
        Age = age;
    }
    void setID(int id)
    {
        ID = id;
    }
    /*Các hàm như là input, nhưng mà mọi ngời để ý là mình in terminal nhé,
    mọi ngời chỉ cần ghi thuật toán, cách nhập ra, còn phần in ra thì mình sẽ áp dụng lên app
    Các class cũng tương tự */
    virtual void Input(string _name, int _id, int _age, int _d, int _m, int _y)
    {
        Name = _name;
        ID = _id;
        Age = _age;
        DayOfBirth.day = _d;
        DayOfBirth.month = _m;
        DayOfBirth.year = _y;
    }
    virtual string Output() const
    {
        return to_string(ID) + "," + Name + "," + to_string(Age) + "," +
               to_string(DayOfBirth.day) + "/" + to_string(DayOfBirth.month) + "/" + to_string(DayOfBirth.year);
    }
    virtual string GetType() const = 0;
    virtual float CalculateMoney() const override = 0;
    virtual string GetTruong() const { return "-"; }
    virtual string GetNghe() const { return "-"; }
    virtual string GetThuNhap() const { return "-"; }
    virtual string GetCompanyName() const { return "-"; }
    virtual ~People() {}
};
/*Các class khác ở đây*/
class NguoiNghiHuu : public People
{
private:
    float pension; // Thuộc tính: Lương hưu
public:
    NguoiNghiHuu(string _name, Date _dob, int _id, float _pension)
        : People(_name, _dob, _id), pension(_pension) {}

    // 1. Phương thức Nhập
    void Input(string _name, int _id, int _age, int _d, int _m, int _y, float _pen)
    {
        People::Input(_name, _id, _age, _d, _m, _y);
        pension = _pen;
    }
    // 3. Phương thức Tính tiền đóng góp (Đa hình)
    float CalculateMoney() const override
    {
        return pension * 0.02f; // Thuật toán: Người nghỉ hưu đóng góp 2% lương hưu
    }
    // 2. Phương thức Xuất
    string Output() const override
    {
        return People::Output() + ", Luong huu: " + to_string(pension) + " VND, Dong gop: " + to_string(CalculateMoney()) + " VND";
    }
    // 4. Phương thức GetType (Dùng để phân loại khi đọc/ghi file CSV)
    string GetType() const override
    {
        return "NghiHuu";
    }
    string GetThuNhap() const override { return to_string((long long)pension) + " VNĐ"; } // Hưu trí có lương hưu
};
class HSSV : public People
{
private:
    string school;

public:
    HSSV(string _name, Date _dob, int _id, string _school)
        : People(_name, _dob, _id), school(_school) {}
    string GetType() const override { return "HSSV"; }
    float CalculateMoney() const override { return 0; } // HSSV miễn đóng góp
    string Output() const override { return People::Output() + " | Truong: " + school; }
    string GetTruong() const override { return school; }
};

// --- LỚP CON 2: NGƯỜI LAO ĐỘNG ---
class NguoiLaoDong : public People
{
private:
    string job;
    float salary;
    string CompanyName;

public:
    NguoiLaoDong(string _name, Date _dob, int _id, string _job, string _company, float _salary)
        : People(_name, _dob, _id), job(_job), CompanyName(_company), salary(_salary) {}
    string GetType() const override { return "Nguoi Lao Dong"; }
    float CalculateMoney() const override { return salary * 0.05f; } // Đóng 5% lương
    string Output() const override { return People::Output() + " | Nghe: " + job; }
    string GetNghe() const override { return job; }
    string GetCompanyName() const override { return CompanyName; }
    string GetThuNhap() const override { return to_string((long long)salary) + " VND"; }
};
class PeopleManager
{
public:
    vector<People *> list;
    ~PeopleManager()
    {
        for (People *p : list)
            delete p;
    }
    void GUI()
    {
        SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
        InitWindow(1280, 720, "Quan Ly Dan Cu Pro - C++ OOP");
        SetTargetFPS(60);

        Color sidebarColor = {30, 41, 59, 255};
        Color bgColor = {241, 245, 249, 255};
        Color primaryColor = {59, 130, 246, 255};
        Color textDark = {15, 23, 42, 255};

        GuiSetStyle(DEFAULT, TEXT_SIZE, 18);
        GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(primaryColor));
        GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(WHITE));

        int activeTab = 0;
        Vector2 scroll = {0, 0};
        Rectangle view = {0};
        char idInput[32] = "", nameInput[128] = "";
        char dayInput[16] = "", monthInput[16] = "", yearInput[16] = "";
        int residentType = 0;
        char schoolInput[128] = "", jobInput[128] = "", companyInput[128] = "", moneyInput[32] = "";
        bool editMode[11] = {false};

        PeopleManager manager;

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

            // --- MAIN CONTENT ---
            Rectangle cardRect = {280, 40, (float)currentWidth - 310, (float)currentHeight - 80};
            DrawRectangleRounded(cardRect, 0.02f, 10, WHITE);
            if (activeTab == 0)
            {
                DrawText("Danh Sach Cu Dan Hien Tai", 310, 60, 24, textDark);
                DrawText("ID", 310, 120, 16, GRAY);
                DrawText("HO TEN", 350, 120, 16, GRAY);
                DrawText("TUOI", 480, 120, 16, GRAY);
                DrawText("NGAY SINH", 540, 120, 16, GRAY);
                DrawText("PHAN LOAI", 650, 120, 16, GRAY);
                DrawText("TRUONG", 810, 120, 16, GRAY);
                DrawText("NGHE", 910, 120, 16, GRAY);
                DrawText("CONG TY", 1010, 120, 16, GRAY);
                DrawText("THU NHAP", 1110, 120, 16, GRAY);
                DrawText("DONG GOP", 1250, 120, 16, GRAY);
                for (int i = 0; i < manager.list.size(); i++)
                {
                    People *p = manager.list[i];
                    float rowY = 150 + (i * 50);
                    DrawRectangleRounded({295, rowY, (float)currentWidth - 305, 40}, 0.1f, 10, {248, 250, 252, 255});
                    DrawText(TextFormat("%d", p->getID()), 310, rowY + 10, 18, textDark);
                    DrawText(p->getName().c_str(), 350, rowY + 10, 18, textDark);
                    DrawText(TextFormat("%d", p->getAge()), 480, rowY + 10, 18, textDark);
                    Date dob = p->getDateOfBirth();
                    DrawText(TextFormat("%d/%d/%d", dob.day, dob.month, dob.year), 540, rowY + 10, 18, textDark);
                    DrawText(p->GetType().c_str(), 650, rowY + 10, 18, BLUE);
                    DrawText(p->GetTruong().c_str(), 810, rowY + 10, 18, textDark);
                    DrawText(p->GetNghe().c_str(), 910, rowY + 10, 18, textDark);
                    DrawText(p->GetCompanyName().c_str(), 1010, rowY + 10, 18, textDark);
                    DrawText(p->GetThuNhap().c_str(), 1110, rowY + 10, 18, textDark);
                    DrawText(TextFormat("%.0f VND", p->CalculateMoney()), 1250, rowY + 10, 18, GREEN);
                }
            }
            else if (activeTab == 1)
            {
                DrawText("Them Moi Cu Dan", 310, 60, 24, textDark);

                Rectangle panelBounds = {310, 100, 700, (float)currentHeight - 150};
                Rectangle contentBounds = {0, 0, 680, 720};

                GuiScrollPanel(panelBounds, NULL, contentBounds, &scroll, &view);
                BeginScissorMode(view.x, view.y, view.width, view.height);

                float startX = panelBounds.x + 15;
                float startY = panelBounds.y + 10 + scroll.y;

                DrawText("Chon loai cu dan:", startX, startY, 18, textDark);
                GuiToggleGroup((Rectangle){startX, startY + 25, 150, 40}, "HSSV;Lao Dong;Nghi Huu", &residentType);

                startY += 80;
                DrawText("ID:", startX, startY, 18, textDark);
                if (GuiTextBox((Rectangle){startX, startY + 25, 200, 40}, idInput, 32, editMode[0]))
                    editMode[0] = !editMode[0];

                startY += 80;
                DrawText("Ho va ten:", startX, startY, 18, textDark);
                if (GuiTextBox((Rectangle){startX, startY + 25, 400, 40}, nameInput, 128, editMode[1]))
                    editMode[1] = !editMode[1];

                // MỚI: Ô nhập tuổi đã được xoá bỏ hoàn toàn ở đây

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
                if (GuiButton((Rectangle){startX, startY, 180, 45}, "LUU THONG TIN"))
                {
                    int id = atoi(idInput);
                    Date dob = {atoi(dayInput), atoi(monthInput), atoi(yearInput)};
                    string name = nameInput;

                    People *newPerson = nullptr;

                    // MỚI: Tham số truyền vào chỉ còn name, dob, id. Hệ thống tự tính age.
                    if (residentType == 0)
                        newPerson = new HSSV(name, dob, id, schoolInput);
                    else if (residentType == 1)
                        newPerson = new NguoiLaoDong(name, dob, id, jobInput, companyInput, atof(moneyInput));
                    else if (residentType == 2)
                        newPerson = new NguoiNghiHuu(name, dob, id, atof(moneyInput));

                    if (newPerson != nullptr)
                        manager.list.push_back(newPerson);

                    idInput[0] = nameInput[0] = '\0';
                    dayInput[0] = monthInput[0] = yearInput[0] = '\0';
                    schoolInput[0] = jobInput[0] = companyInput[0] = moneyInput[0] = '\0';

                    scroll.y = 0;
                    activeTab = 0;
                }
                EndScissorMode();
            }
            EndDrawing();
        }
        CloseWindow();
    }
};

int main()
{
    PeopleManager p;
    p.GUI();
}
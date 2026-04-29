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
    People(string _name, Date _dob, int _age, int _id)
    {
        this->Name = _name;
        this->DayOfBirth = _dob;
        this->Age = _age;
        this->ID = _id;
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
    virtual ~People() {}
};
/*Các class khác ở đây*/
class NguoiNghiHuu : public People
{
private:
    float pension; // Thuộc tính: Lương hưu
public:
    NguoiNghiHuu(string _name, Date _dob, int _age, int _id, float _pension)
        : People(_name, _dob, _age, _id) // Lớp cha giờ chỉ nhận 4 tham số: Tên, Ngày sinh, Tuổi, CCCD
    {
        this->pension = _pension;
    }

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
    HSSV(string _name, Date _dob, int _age, int _id, string _school)
        : People(_name, _dob, _age, _id), school(_school) {}
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

public:
    NguoiLaoDong(string _name, Date _dob, int _age, int _id, string _job, float _salary)
        : People(_name, _dob, _age, _id), job(_job), salary(_salary) {}
    string GetType() const override { return "Nguoi Lao Dong"; }
    float CalculateMoney() const override { return salary * 0.05f; } // Đóng 5% lương
    string Output() const override { return People::Output() + " | Nghe: " + job; }
    string GetNghe() const override { return job; }
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
};

int main()
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

    char idInput[32] = "", nameInput[128] = "", ageInput[32] = "";
    char dayInput[16] = "", monthInput[16] = "", yearInput[16] = "";
    int residentType = 0;
    char schoolInput[128] = "", jobInput[128] = "", moneyInput[32] = "";
    bool editMode[10] = {false};

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

            // --- ĐÃ CĂN CHỈNH LẠI TOẠ ĐỘ ĐỂ THÊM CỘT MỚI ---
            DrawText("ID", 310, 120, 16, GRAY);
            DrawText("HO TEN", 370, 120, 16, GRAY);
            DrawText("TUOI", 540, 120, 16, GRAY);
            DrawText("NGAY SINH", 610, 120, 16, GRAY);
            DrawText("PHAN LOAI", 730, 120, 16, GRAY);
            DrawText("TRUONG HOC", 850, 120, 16, GRAY);
            DrawText("NGHE NGHIEP", 990, 120, 16, GRAY);
            DrawText("THU NHAP", 1140, 120, 16, GRAY);
            DrawText("DONG GOP", 1260, 120, 16, GRAY);

            // Vòng lặp in danh sách
            for (int i = 0; i < manager.list.size(); i++)
            {
                People *p = manager.list[i];
                float rowY = 150 + (i * 50);

                // Khung nền cho từng dòng (kéo dài thêm 1 chút)
                DrawRectangleRounded({295, rowY, (float)currentWidth - 305, 40}, 0.1f, 10, {248, 250, 252, 255});

                // In nội dung các cột tương ứng
                DrawText(TextFormat("%d", p->getID()), 310, rowY + 10, 18, textDark);
                DrawText(p->getName().c_str(), 370, rowY + 10, 18, textDark);
                DrawText(TextFormat("%d", p->getAge()), 540, rowY + 10, 18, textDark);

                Date dob = p->getDateOfBirth();
                DrawText(TextFormat("%d/%d/%d", dob.day, dob.month, dob.year), 610, rowY + 10, 18, textDark);

                DrawText(p->GetType().c_str(), 730, rowY + 10, 18, BLUE);

                // Gọi 2 hàm riêng biệt ra
                DrawText(p->GetTruong().c_str(), 850, rowY + 10, 18, textDark);
                DrawText(p->GetNghe().c_str(), 990, rowY + 10, 18, textDark);

                DrawText(p->GetThuNhap().c_str(), 1140, rowY + 10, 18, textDark);
                DrawText(TextFormat("%.0f VND", p->CalculateMoney()), 1260, rowY + 10, 18, GREEN);
            }
        }
        else if (activeTab == 1)
        {
            // [ PHẦN INPUT GIỮ NGUYÊN ]
            DrawText("Them Moi Cu Dan", 310, 60, 24, textDark);

            Rectangle panelBounds = {310, 100, 700, (float)currentHeight - 150};
            Rectangle contentBounds = {0, 0, 680, 800};

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

            startY += 80;
            DrawText("Tuoi:", startX, startY, 18, textDark);
            if (GuiTextBox((Rectangle){startX, startY + 25, 150, 40}, ageInput, 32, editMode[2]))
                editMode[2] = !editMode[2];

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
                int age = atoi(ageInput);
                Date dob = {atoi(dayInput), atoi(monthInput), atoi(yearInput)};
                string name = nameInput;

                People *newPerson = nullptr;

                if (residentType == 0)
                    newPerson = new HSSV(name, dob, age, id, schoolInput);
                else if (residentType == 1)
                    newPerson = new NguoiLaoDong(name, dob, age, id, jobInput, atof(moneyInput));
                else if (residentType == 2)
                    newPerson = new NguoiNghiHuu(name, dob, age, id, atof(moneyInput));

                if (newPerson != nullptr)
                    manager.list.push_back(newPerson);

                idInput[0] = nameInput[0] = ageInput[0] = '\0';
                dayInput[0] = monthInput[0] = yearInput[0] = '\0';
                schoolInput[0] = jobInput[0] = moneyInput[0] = '\0';

                scroll.y = 0;
                activeTab = 0;
            }
            EndScissorMode();
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
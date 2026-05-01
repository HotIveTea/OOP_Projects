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
    string Gender;
    Date DayOfBirth;
    int Age;
    int ID;
    bool isHouseholdHead;

public:
    People(string _name, string _gender, Date _dob, int _id, bool _isHead)
        : Name(_name), Gender(_gender), DayOfBirth(_dob), ID(_id), isHouseholdHead(_isHead)
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
    ~HouseholdManager()
    {
        for (Household *h : householdList)
            delete h;
    }
    void GUI()
    {
        SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
        InitWindow(1440, 800, "Quan Ly Dan Cu Pro - Theo Ho Gia Dinh");
        SetTargetFPS(60);

        Color sidebarColor = {30, 41, 59, 255};
        Color bgColor = {241, 245, 249, 255};
        Color primaryColor = {59, 130, 246, 255};
        Color textDark = {15, 23, 42, 255};

        GuiSetStyle(DEFAULT, TEXT_SIZE, 18);
        GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(primaryColor));
        GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(WHITE));

        int activeTab = 0;

        // Hai bộ biến cho thanh cuộn: 1 cho Tab Danh sách, 1 cho Tab Nhập liệu
        Vector2 scrollList = {0, 0};
        Vector2 scrollInput = {0, 0};
        Rectangle viewList = {0};
        Rectangle viewInput = {0};

        // Các biến đầu vào GUI
        char hhIdInput[32] = "", addressInput[128] = "";
        int isHeadSelect = 0; // 0: Không, 1: Có

        char idInput[32] = "", nameInput[128] = "";
        char dayInput[16] = "", monthInput[16] = "", yearInput[16] = "";
        int residentType = 0;
        int genderSelect = 0; // 0: Nam, 1: Nữ
        char schoolInput[128] = "", jobInput[128] = "", companyInput[128] = "", moneyInput[32] = "";

        bool editMode[15] = {false};
        int inputState = 0;
        Household *currentHousehold = nullptr;
        HouseholdManager manager;

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
                DrawText("Danh Sach Cu Dan Theo Ho Gia Dinh", 310, 60, 24, textDark);

                // --- ĐÃ DÃN CÁC CỘT RA XA NHAU HƠN ĐỂ KHÔNG BỊ ÉP ---
                DrawText("ID", 310, 110, 16, GRAY);
                DrawText("HO TEN", 350, 110, 16, GRAY);
                DrawText("GIOI TINH", 590, 110, 16, GRAY); // +50px khoảng cách cho chữ (CHU HO)
                DrawText("TUOI", 680, 110, 16, GRAY);
                DrawText("NGAY SINH", 730, 110, 16, GRAY);
                DrawText("PHAN LOAI", 840, 110, 16, GRAY);
                DrawText("TRUONG", 950, 110, 16, GRAY);
                DrawText("NGHE", 1030, 110, 16, GRAY);
                DrawText("CONG TY", 1100, 110, 16, GRAY);
                DrawText("THU NHAP", 1190, 110, 16, GRAY);
                DrawText("DONG GOP", 1330, 110, 16, GRAY);

                Rectangle panelBounds = {295, 135, (float)currentWidth - 305, (float)currentHeight - 190};
                float totalContentHeight = 0;
                for (Household *h : manager.householdList)
                {
                    totalContentHeight += 50 + (h->members.size() * 45) + 20;
                }
                Rectangle contentBounds = {0, 0, (float)currentWidth - 325, (totalContentHeight > panelBounds.height) ? totalContentHeight : panelBounds.height};

                GuiScrollPanel(panelBounds, NULL, contentBounds, &scrollList, &viewList);
                BeginScissorMode(viewList.x, viewList.y, viewList.width, viewList.height);

                float rowY = panelBounds.y + scrollList.y;

                for (Household *h : manager.householdList)
                {
                    DrawRectangleRounded({300, rowY, (float)currentWidth - 340, 40}, 0.1f, 10, {226, 232, 240, 255});
                    DrawText(TextFormat("[ MA HO: %s ]  -  Dia chi: %s", h->HouseholdID.c_str(), h->Address.c_str()), 310, rowY + 10, 18, textDark);
                    DrawText(TextFormat("TONG HO DONG GOP: %.0f VND", h->CalculateTotal()), 1050, rowY + 10, 18, RED);
                    rowY += 45;

                    for (People *p : h->members)
                    {
                        DrawText(TextFormat("%d", p->getID()), 310, rowY + 10, 18, textDark);

                        string displayTitle = p->getName();
                        if (p->getIsHouseholdHead())
                            displayTitle += " (CHU HO)";
                        DrawText(displayTitle.c_str(), 350, rowY + 10, 18, (p->getIsHouseholdHead() ? BLUE : textDark));
                        DrawText(p->getGender().c_str(), 590, rowY + 10, 18, textDark);
                        DrawText(TextFormat("%d", p->getAge()), 680, rowY + 10, 18, textDark);
                        Date dob = p->getDateOfBirth();
                        DrawText(TextFormat("%d/%d/%d", dob.day, dob.month, dob.year), 730, rowY + 10, 18, textDark);
                        DrawText(p->GetType().c_str(), 840, rowY + 10, 18, DARKGRAY);
                        DrawText(p->GetSchool().c_str(), 950, rowY + 10, 18, textDark);
                        DrawText(p->GetJob().c_str(), 1030, rowY + 10, 18, textDark);
                        DrawText(p->GetCompanyName().c_str(), 1100, rowY + 10, 18, textDark);
                        DrawText(p->GetSalary().c_str(), 1190, rowY + 10, 18, textDark);
                        DrawText(TextFormat("%.0f VND", p->CalculateMoney()), 1330, rowY + 10, 18, GREEN);

                        rowY += 40;
                    }
                    rowY += 20;
                }
                EndScissorMode();
            }
            else if (activeTab == 1)
            {
                DrawText("Them Moi Cu Dan", 310, 60, 24, textDark);

                Rectangle panelBounds = {310, 100, 700, (float)currentHeight - 150};
                Rectangle contentBounds = {0, 0, 680, 1000};

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
                }
                // ==========================================
                // TRẠNG THÁI 1: THÊM THÀNH VIÊN VÀO HỘ
                // ==========================================
                else if (inputState == 1)
                {
                    // Tiêu đề cho biết đang thao tác trên Hộ nào
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

                    // Các form nhập thông tin riêng biệt
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

                    // NÚT 1: LƯU VÀ THÊM NGƯỜI TIẾP THEO
                    if (GuiButton((Rectangle){startX, startY, 200, 45}, "LUU THANH VIEN"))
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
                            currentHousehold->AddMember(newPerson); // Nhét thẳng vào hộ hiện tại
                        }

                        // Chỉ xoá form thông tin cá nhân, giữ nguyên Hộ
                        idInput[0] = nameInput[0] = '\0';
                        dayInput[0] = monthInput[0] = yearInput[0] = '\0';
                        schoolInput[0] = jobInput[0] = companyInput[0] = moneyInput[0] = '\0';
                        isHeadSelect = 0;
                        scrollInput.y = 0;
                    }

                    // NÚT 2: HOÀN TẤT VÀ THOÁT RA
                    if (GuiButton((Rectangle){startX + 220, startY, 200, 45}, "HOAN TAT HO NAY"))
                    {
                        inputState = 0;                        // Trả về trạng thái Bước 1
                        currentHousehold = nullptr;            // Giải phóng con trỏ
                        hhIdInput[0] = addressInput[0] = '\0'; // Xoá form nhập hộ

                        scrollInput.y = 0;
                        activeTab = 0; // Về màn hình Danh sách
                    }
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
    HouseholdManager p;
    p.GUI();
}
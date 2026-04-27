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
    virtual float CalculateMoney() = 0;
    virtual ~ContributedMoney() {}
};
class People : public ContributedMoney
{
protected:
    string Name;
    string Job;
    Date DayOfBirth;
    int Age;
    int ID;

public:
    People(string _name, string _job, Date _dob, int _age, int _id)
    {
        this->Name = _name;
        this->Job = _job;
        this->DayOfBirth = _dob;
        this->Age = _age;
        this->ID = _id;
    }
    string getName() const { return Name; }
    string getJob() const { return Job; }
    Date getDateOfBirth() const { return DayOfBirth; }
    int getAge() const { return Age; }
    int getID() const { return ID; }
    void setName(string name)
    {
        Name = name;
    }
    void setJob(string job)
    {
        Job = job;
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
    virtual ~People() {}
};
/*Các class khác ở đây*/
class PeopleManger
{
public:
    vector<People *> list;
    int n;
    /*Các hàm của class quản lý*/
};

int main()
{

    // 1. Thiet lap cua so mo full man hinh (Maximized) hoac co the keo gian (Resizable)
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
    InitWindow(1280, 720, "Quan Ly Dan Cu Pro - C++ OOP (Full Screen)");
    SetTargetFPS(60);

    // --- BANG MAU ---
    Color sidebarColor = {30, 41, 59, 255};
    Color bgColor = {241, 245, 249, 255};
    Color cardColor = WHITE;
    Color primaryColor = {59, 130, 246, 255};
    Color textDark = {15, 23, 42, 255};
    Color textLight = {148, 163, 184, 255};

    // --- TUY CHINH STYLE RAYGUI ---
    GuiSetStyle(DEFAULT, TEXT_SIZE, 18);
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(primaryColor));
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
    GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, ColorToInt(primaryColor));
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 0x2563ebff);
    GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, ColorToInt(WHITE));
    GuiSetStyle(TEXTBOX, BORDER_COLOR_NORMAL, ColorToInt({203, 213, 225, 255}));

    int activeTab = 0;

    // Cac bien cho Scroll Panel (Tab Them Nguoi)
    Vector2 scroll = {0, 0};
    Rectangle view = {0};

    char idInput[32] = "";
    char nameInput[128] = "";
    char jobInput[128] = "";
    char ageInput[32] = "";
    char dayInput[16] = "", monthInput[16] = "", yearInput[16] = "";
    bool editMode[7] = {false};

    while (!WindowShouldClose())
    {
        // 2. Lien tuc lay kich thuoc man hinh hien tai de ve khung cho chuan
        int currentWidth = GetScreenWidth();
        int currentHeight = GetScreenHeight();

        BeginDrawing();
        ClearBackground(bgColor);
        DrawRectangle(0, 0, 250, currentHeight, sidebarColor); // Chieu cao full man hinh
        DrawText("QUAN LY", 25, 40, 28, WHITE);
        DrawText("KHU PHO", 25, 75, 28, primaryColor);
        DrawLine(25, 120, 225, 120, textLight);

        if (activeTab == 0)
            DrawRectangle(0, 150, 250, 50, {51, 65, 85, 255});
        if (GuiButton((Rectangle){25, 155, 200, 40}, "Danh sach cu dan"))
            activeTab = 0;

        if (activeTab == 1)
            DrawRectangle(0, 210, 250, 50, {51, 65, 85, 255});
        if (GuiButton((Rectangle){25, 215, 200, 40}, "Them moi cu dan"))
            activeTab = 1;

        Rectangle cardRect = {280, 40, (float)currentWidth - 310, (float)currentHeight - 80};
        DrawRectangleRounded(cardRect, 0.02f, 10, cardColor);
        DrawRectangleRoundedLines(cardRect, 0.02f, 10, {226, 232, 240, 255});

        if (activeTab == 0)
        {
            // TAB 0: DANH SACH (HIEN THI FULL THONG TIN)
            DrawText("Danh Sach Cu Dan Hien Tai", 310, 60, 24, textDark);
            DrawText("Quan ly toan bo thong tin cua dan cu trong khu vuc", 310, 95, 16, textLight);
            DrawLine(310, 130, currentWidth - 60, 130, {226, 232, 240, 255});

            // Tieu de cot da duoc chia khoang cach rong ra cho full man hinh
            DrawText("ID", 330, 150, 16, textLight);
            DrawText("HO VA TEN", 450, 150, 16, textLight);
            DrawText("NGHE NGHIEP", 750, 150, 16, textLight);
            DrawText("TUOI", 1000, 150, 16, textLight);
            DrawText("NGAY SINH", 1100, 150, 16, textLight);

            // [DEMO DONG 1]
            DrawRectangleRounded({310, 180, (float)currentWidth - 340, 45}, 0.1f, 10, bgColor);
            DrawText("001", 330, 193, 18, textDark);
            DrawText("Nguyen Van A", 450, 193, 18, textDark);
            DrawText("Ky su IT", 750, 193, 18, textDark);
            DrawText("28", 1000, 193, 18, textDark);
            DrawText("15/05/1996", 1100, 193, 18, textDark);

            // [DEMO DONG 2]
            DrawRectangleRounded({310, 235, (float)currentWidth - 340, 45}, 0.1f, 10, bgColor);
            DrawText("002", 330, 248, 18, textDark);
            DrawText("Tran Thi B", 450, 248, 18, textDark);
            DrawText("Giao vien", 750, 248, 18, textDark);
            DrawText("32", 1000, 248, 18, textDark);
            DrawText("20/11/1992", 1100, 248, 18, textDark);
        }
        else if (activeTab == 1)
        {
            // TAB 1: THEM NGUOI (GIU NGUYEN SCROLL BAR)
            DrawText("Them Moi Cu Dan", 310, 60, 24, textDark);
            DrawText("Dien day du thong tin vao bieu mau (Cuon xuong de xem them)", 310, 95, 16, textLight);
            DrawLine(310, 130, currentWidth - 60, 130, {226, 232, 240, 255});

            Rectangle panelBounds = {310, 140, 640, (float)currentHeight - 200};
            Rectangle contentBounds = {0, 0, 620, 650};

            GuiScrollPanel(panelBounds, NULL, contentBounds, &scroll, &view);
            BeginScissorMode(view.x, view.y, view.width, view.height);

            float startX = panelBounds.x + 15;
            float startY = panelBounds.y + 10 + scroll.y;

            DrawText("ID Cu Dan:", startX, startY, 18, textDark);
            if (GuiTextBox((Rectangle){startX, startY + 25, 400, 40}, idInput, 32, editMode[0]))
                editMode[0] = !editMode[0];

            startY += 80;
            DrawText("Ho va ten day du:", startX, startY, 18, textDark);
            if (GuiTextBox((Rectangle){startX, startY + 25, 400, 40}, nameInput, 128, editMode[1]))
                editMode[1] = !editMode[1];

            startY += 80;
            DrawText("Nghe nghiep:", startX, startY, 18, textDark);
            if (GuiTextBox((Rectangle){startX, startY + 25, 400, 40}, jobInput, 128, editMode[2]))
                editMode[2] = !editMode[2];

            startY += 80;
            DrawText("Tuoi:", startX, startY, 18, textDark);
            if (GuiTextBox((Rectangle){startX, startY + 25, 150, 40}, ageInput, 32, editMode[3]))
                editMode[3] = !editMode[3];

            startY += 80;
            DrawText("Ngay / Thang / Nam sinh:", startX, startY, 18, textDark);
            if (GuiTextBox((Rectangle){startX, startY + 25, 80, 40}, dayInput, 16, editMode[4]))
                editMode[4] = !editMode[4];
            if (GuiTextBox((Rectangle){startX + 90, startY + 25, 80, 40}, monthInput, 16, editMode[5]))
                editMode[5] = !editMode[5];
            if (GuiTextBox((Rectangle){startX + 180, startY + 25, 100, 40}, yearInput, 16, editMode[6]))
                editMode[6] = !editMode[6];

            startY += 90;
            if (GuiButton((Rectangle){startX, startY, 180, 45}, "LUU THONG TIN"))
            {
                // Xu ly data OOP o day...
                idInput[0] = nameInput[0] = jobInput[0] = ageInput[0] = '\0';
                dayInput[0] = monthInput[0] = yearInput[0] = '\0';
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
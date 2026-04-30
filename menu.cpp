#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <ctime>
#include <sstream>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Date
{
    int day;
    int month;
    int year;
};

bool isValidDate(int d, int m, int y)
{
    if (y <= 0 || m < 1 || m > 12 || d < 1)
        return false;
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (y % 400 == 0 || (y % 4 == 0 && y % 100 != 0))
    {
        daysInMonth[2] = 29;
    }

    return d <= daysInMonth[m];
}

// ==========================================
// CÁC CLASS ĐỐI TƯỢNG CƠ BẢN
// ==========================================
class ContributedMoney
{
public:
    virtual double CalculateMoney() const = 0;
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
    People() : Name(""), Gender(""), DayOfBirth({1, 1, 2000}), Age(0), ID(0), isHouseholdHead(false) {}

    string getName() const { return Name; }
    string getGender() const { return Gender; }
    Date getDateOfBirth() const { return DayOfBirth; }
    int getAge() const { return Age; }
    int getID() const { return ID; }
    bool getIsHouseholdHead() const { return isHouseholdHead; }

    void setIsHouseholdHead(bool isHead) { isHouseholdHead = isHead; }
    void setGender(string g) { Gender = g; }

    void setDate(int d, int m, int y)
    {
        DayOfBirth.day = d;
        DayOfBirth.month = m;
        DayOfBirth.year = y;
        CalculateAge();
    }

    void CalculateAge()
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

    virtual void InputData(string _name, string _gender, int _id, int _d, int _m, int _y)
    {
        Name = _name;
        Gender = _gender;
        ID = _id;
        setDate(_d, _m, _y);
    }

    virtual void Input(int _validatedID)
    {
        ID = _validatedID;
        cin.ignore(10000, '\n');
        cout << "Input Ho Ten: ";
        getline(cin, Name);

        int genderChoice;
        do
        {
            cout << "Chon Gioi tinh (1-Nam, 2-Nu, 3-Khac): ";
            cin >> genderChoice;
            if (genderChoice == 1)
                Gender = "Nam";
            else if (genderChoice == 2)
                Gender = "Nu";
            else if (genderChoice == 3)
                Gender = "Khac";
            else
                cout << "=> [LOI]: Lua chon khong hop le!\n";
        } while (genderChoice < 1 || genderChoice > 3);

        do
        {
            cout << "Input Ngay Sinh (dd mm yyyy): ";
            cin >> DayOfBirth.day >> DayOfBirth.month >> DayOfBirth.year;
            if (!isValidDate(DayOfBirth.day, DayOfBirth.month, DayOfBirth.year))
            {
                cout << "=> [LOI]: Ngay thang nam khong hop le hoac sai logic. Vui long nhap lai!\n";
            }
        } while (!isValidDate(DayOfBirth.day, DayOfBirth.month, DayOfBirth.year));

        CalculateAge();
    }

    virtual void Print() const
    {
        cout << "CCCD: " << ID << " | Ho ten: " << Name << " (" << Gender << ")";
        if (isHouseholdHead)
            cout << " [CHU HO]";
        cout << " | Ngay sinh: " << DayOfBirth.day << "/" << DayOfBirth.month << "/" << DayOfBirth.year
             << " | Tuoi: " << Age << endl;
    }

    virtual string GetType() const = 0;
    virtual double CalculateMoney() const override = 0;
    virtual string GetSchool() const { return "-"; }
    virtual string GetOccupation() const { return "-"; }
    virtual string GetIncome() const { return "-"; }
    virtual string GetCompanyName() const { return "-"; }
    virtual ~People() {}
};

class Retiree : public People
{
private:
    double Pension;

public:
    Retiree() : People(), Pension(0.0) {}
    void setPension(double l) { Pension = l; }

    void Input(int _id) override
    {
        cout << "\n--- Input thong tin Nguoi Nghi Huu ---" << endl;
        People::Input(_id);

        do
        {
            cout << "Input luong huu: ";
            cin >> Pension;
            if (Pension < 0)
                cout << "=> [LOI]: So tien khong duoc am!\n";
        } while (Pension < 0);
    }

    void Print() const override
    {
        cout << "[ NGUOI NGHI HUU ]" << endl;
        People::Print();
        cout << "Luong huu: " << (long long)Pension << " VND" << endl;
        cout << "Tien dong gop: " << (long long)CalculateMoney() << " VND\n";
    }

    double CalculateMoney() const override { return Pension * 0.02; }
    string GetType() const override { return "NghiHuu"; }
    string GetIncome() const override { return to_string(Pension); }
};

class Student : public People
{
private:
    string School;
    double Allowance;

public:
    Student() : People(), School(""), Allowance(0.0) {}
    void setSchool(string s) { School = s; }
    void setAllowance(double a) { Allowance = a; }

    void Input(int _id) override
    {
        cout << "\n--- Input thong tin Hoc Sinh / Sinh Vien ---\n";
        People::Input(_id);
        cin.ignore(10000, '\n');
        cout << "Input ten truong hoc: ";
        getline(cin, School);

        do
        {
            cout << "Input tien tro cap: ";
            cin >> Allowance;
            if (Allowance < 0)
                cout << "=> [LOI]: So tien khong duoc am!\n";
        } while (Allowance < 0);
    }

    void Print() const override
    {
        cout << "[ HOC SINH / SINH VIEN ]" << endl;
        People::Print();
        cout << "Truong hoc: " << School << endl;
        cout << "Tro cap: " << (long long)Allowance << " VND" << endl;
        cout << "Tien dong gop: " << (long long)CalculateMoney() << " VND\n";
    }

    double CalculateMoney() const override { return 0.0; }
    string GetType() const override { return "HSSV"; }
    string GetSchool() const override { return School; }
    string GetIncome() const override { return to_string(Allowance); }
};

class Worker : public People
{
private:
    string Job;
    double Salary;
    string Company;

public:
    Worker() : People(), Job(""), Salary(0.0), Company("") {}
    void setJob(string j) { Job = j; }
    void setSalary(double s) { Salary = s; }
    void setCompany(string c) { Company = c; }

    void Input(int _id) override
    {
        cout << "\n--- Input thong tin Nguoi Lao Dong ---\n";
        People::Input(_id);
        cin.ignore(10000, '\n');
        cout << "Input nghe nghiep: ";
        getline(cin, Job);

        do
        {
            cout << "Input luong: ";
            cin >> Salary;
            if (Salary < 0)
                cout << "=> [LOI]: So tien khong duoc am!\n";
        } while (Salary < 0);

        cin.ignore(10000, '\n');
        cout << "Input ten cong ty: ";
        getline(cin, Company);
    }

    void Print() const override
    {
        cout << "[ NGUOI LAO DONG ]" << endl;
        People::Print();
        cout << "Nghe nghiep: " << Job << endl;
        cout << "Cong ty: " << Company << endl;
        cout << "Luong: " << (long long)Salary << " VND" << endl;
        cout << "Tien dong gop: " << (long long)CalculateMoney() << " VND\n";
    }

    double CalculateMoney() const override { return Salary * 0.05; }
    string GetType() const override { return "LaoDong"; }
    string GetOccupation() const override { return Job; }
    string GetCompanyName() const override { return Company; }
    string GetIncome() const override { return to_string(Salary); }
};

// ==========================================
// CLASS HỘ GIA ĐÌNH VÀ QUẢN LÝ DÂN CƯ
// ==========================================
class Household
{
public:
    string HouseholdID;
    string Address;
    vector<People *> members;

    void Print() const
    {
        cout << "\n=====================================";
        cout << "\n[Ma Ho]: " << HouseholdID << " | [Dia chi]: " << Address;
        cout << "\nSo thanh vien: " << members.size() << endl;

        bool hasHead = false;
        double totalContribution = 0;

        cout << "-------------------------------------" << endl;
        for (size_t i = 0; i < members.size(); i++)
        {
            if (members[i]->getIsHouseholdHead())
                hasHead = true;
            members[i]->Print();
            totalContribution += members[i]->CalculateMoney();
            cout << "------------------------" << endl;
        }

        if (!hasHead)
        {
            cout << "=> [CANH BAO]: Ho nay chua co Chu Ho!\n";
        }
        cout << "=> TONG TIEN HO [" << HouseholdID << "] DONG GOP: " << (long long)totalContribution << " VND" << endl;
    }

    ~Household()
    {
        for (People *p : members)
            delete p;
        members.clear();
    }
};

class PopulationManager
{
private:
    vector<Household *> households;

public:
    bool isDuplicateID(int id, int excludeID = -1)
    {
        for (Household *ho : households)
        {
            for (People *p : ho->members)
            {
                if (p->getID() == id && p->getID() != excludeID)
                    return true;
            }
        }
        return false;
    }

    void ClearData()
    {
        for (Household *ho : households)
            delete ho;
        households.clear();
    }

    void LoadFromCSV(string filename)
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            cout << "Khong the mo file " << filename << "!\n";
            return;
        }

        ClearData();
        string line;
        getline(file, line);

        while (getline(file, line))
        {
            if (line.empty())
                continue;
            stringstream ss(line);
            string householdID, diaChi, loai, vaiTroStr, cccdStr, hoTen, gioiTinh, ngayStr, thangStr, namStr, truong_congty, ngheNghiep, tienStr;

            getline(ss, householdID, ',');
            getline(ss, diaChi, ',');
            getline(ss, loai, ',');
            getline(ss, vaiTroStr, ',');
            getline(ss, cccdStr, ',');
            getline(ss, hoTen, ',');
            getline(ss, gioiTinh, ',');
            getline(ss, ngayStr, ',');
            getline(ss, thangStr, ',');
            getline(ss, namStr, ',');
            getline(ss, truong_congty, ',');
            getline(ss, ngheNghiep, ',');
            getline(ss, tienStr, ',');

            Household *currentHo = nullptr;
            for (Household *ho : households)
            {
                if (ho->HouseholdID == householdID)
                {
                    currentHo = ho;
                    break;
                }
            }
            if (currentHo == nullptr)
            {
                currentHo = new Household();
                currentHo->HouseholdID = householdID;
                currentHo->Address = diaChi;
                households.push_back(currentHo);
            }

            try
            {
                int id = stoi(cccdStr);
                int d = stoi(ngayStr);
                int m = stoi(thangStr);
                int y = stoi(namStr);
                double tien = (tienStr == "-" || tienStr.empty()) ? 0.0 : stod(tienStr);
                bool isHead = (vaiTroStr == "ChuHo" || vaiTroStr == "1");

                if (loai == "HSSV")
                {
                    Student *s = new Student();
                    s->InputData(hoTen, gioiTinh, id, d, m, y);
                    s->setIsHouseholdHead(isHead);
                    s->setSchool(truong_congty);
                    s->setAllowance(tien);
                    currentHo->members.push_back(s);
                }
                else if (loai == "LaoDong")
                {
                    Worker *w = new Worker();
                    w->InputData(hoTen, gioiTinh, id, d, m, y);
                    w->setIsHouseholdHead(isHead);
                    w->setCompany(truong_congty);
                    w->setJob(ngheNghiep);
                    w->setSalary(tien);
                    currentHo->members.push_back(w);
                }
                else if (loai == "NghiHuu")
                {
                    Retiree *r = new Retiree();
                    r->InputData(hoTen, gioiTinh, id, d, m, y);
                    r->setIsHouseholdHead(isHead);
                    r->setPension(tien);
                    currentHo->members.push_back(r);
                }
            }
            catch (...)
            {
                cout << "Loi tai dong: " << line << endl;
            }
        }
        file.close();
        cout << "=> Doc du lieu tu file " << filename << " thanh cong!\n";
    }

    void SaveToCSV(string filename) const
    {
        ofstream file(filename);
        if (!file.is_open())
        {
            cout << "Khong the ghi file!\n";
            return;
        }
        file << "HouseholdID,DiaChi,Loai,VaiTro,CCCD,HoTen,GioiTinh,Ngay,Thang,Nam,Truong_CongTy,NgheNghiep,TroCap_Luong_LuongHuu\n";
        for (Household *ho : households)
        {
            for (People *p : ho->members)
            {
                file << ho->HouseholdID << "," << ho->Address << "," << p->GetType() << ","
                     << (p->getIsHouseholdHead() ? "ChuHo" : "ThanhVien") << "," << p->getID() << "," << p->getName() << ","
                     << p->getGender() << ","
                     << p->getDateOfBirth().day << "," << p->getDateOfBirth().month << "," << p->getDateOfBirth().year << ",";
                if (p->GetType() == "HSSV")
                    file << p->GetSchool() << ",-," << p->GetIncome() << "\n";
                else if (p->GetType() == "LaoDong")
                    file << p->GetCompanyName() << "," << p->GetOccupation() << "," << p->GetIncome() << "\n";
                else if (p->GetType() == "NghiHuu")
                    file << "-,-," << p->GetIncome() << "\n";
            }
        }
        file.close();
        cout << "=> Ghi thanh cong vao " << filename << "!\n";
    }

    void AddMember()
    {
        string householdID;
        cout << "Input Ma Ho: ";
        cin >> householdID;
        Household *currentHo = nullptr;
        for (Household *ho : households)
        {
            if (ho->HouseholdID == householdID)
            {
                currentHo = ho;
                break;
            }
        }
        if (!currentHo)
        {
            currentHo = new Household();
            currentHo->HouseholdID = householdID;
            cout << "Ho chua ton tai! Moi nhap Dia chi cho ho nay: ";
            cin.ignore(10000, '\n');
            getline(cin, currentHo->Address);
            households.push_back(currentHo);
        }

        int type;
        cout << "Chon loai (1-HSSV, 2-LaoDong, 3-NghiHuu): ";
        cin >> type;
        People *p = nullptr;
        if (type == 1)
            p = new Student();
        else if (type == 2)
            p = new Worker();
        else
            p = new Retiree();

        int id;
        while (true)
        {
            cout << "Input CCCD/ID: ";
            cin >> id;
            if (isDuplicateID(id))
            {
                cout << "=> [LOI]: CCCD nay da ton tai trong danh sach! Vui long nhap lai.\n";
            }
            else
            {
                break;
            }
        }

        p->Input(id);

        int isHead;
        cout << "La chu ho? (1-Co, 0-Khong): ";
        cin >> isHead;
        if (isHead == 1)
        {
            People *currentHead = nullptr;
            for (People *existingP : currentHo->members)
            {
                if (existingP->getIsHouseholdHead())
                {
                    currentHead = existingP;
                    break;
                }
            }

            if (currentHead != nullptr)
            {
                cout << "\n!! CANH BAO: Ho nay da co chu ho la [" << currentHead->getName() << "]\n";
                cout << "Ban muon chon ai lam Chu ho chinh thuc?\n";
                cout << "1. Giu nguyen (" << currentHead->getName() << ")\n";
                cout << "2. Thay the thanh nguoi moi (" << p->getName() << ")\n";
                cout << "=> Lua chon: ";
                int headChoice;
                cin >> headChoice;
                if (headChoice == 2)
                {
                    currentHead->setIsHouseholdHead(false);
                    p->setIsHouseholdHead(true);
                }
                else
                {
                    p->setIsHouseholdHead(false);
                }
            }
            else
            {
                p->setIsHouseholdHead(true);
            }
        }
        else
        {
            p->setIsHouseholdHead(false);
        }

        currentHo->members.push_back(p);
        cout << "=> Da them thanh vien thanh cong!\n";
    }

    void DeleteMember()
    {
        int id;
        cout << "Input CCCD can xoa: ";
        cin >> id;
        for (auto itHo = households.begin(); itHo != households.end(); ++itHo)
        {
            for (auto itP = (*itHo)->members.begin(); itP != (*itHo)->members.end(); ++itP)
            {
                if ((*itP)->getID() == id)
                {
                    delete *itP;
                    (*itHo)->members.erase(itP);
                    cout << "=> Da xoa thanh vien co CCCD " << id << "\n";
                    if ((*itHo)->members.empty())
                    {
                        cout << "=> Ho nay da trong, tu dong xoa Ho khoi danh sach!\n";
                        delete *itHo;
                        households.erase(itHo);
                    }
                    return;
                }
            }
        }
        cout << "=> Khong tim thay thanh vien co CCCD " << id << "\n";
    }

    void EditMember()
    {
        int id;
        cout << "Input CCCD can chinh sua: ";
        cin >> id;
        for (Household *ho : households)
        {
            for (size_t i = 0; i < ho->members.size(); i++)
            {
                People *oldP = ho->members[i];

                if (oldP->getID() == id)
                {
                    cout << "\nDa tim thay thanh vien! Tien hanh nhap lai thong tin.\n";

                    int type;
                    cout << "Chon Loai thanh vien moi (1-HSSV, 2-LaoDong, 3-NghiHuu): ";
                    cin >> type;
                    People *newP = nullptr;
                    if (type == 1)
                        newP = new Student();
                    else if (type == 2)
                        newP = new Worker();
                    else
                        newP = new Retiree();

                    int newId;
                    while (true)
                    {
                        cout << "Input CCCD/ID moi (Nhap " << id << " de giu nguyen): ";
                        cin >> newId;
                        if (newId != id && isDuplicateID(newId))
                        {
                            cout << "=> [LOI]: CCCD nay da ton tai trong danh sach! Vui long nhap lai.\n";
                        }
                        else
                        {
                            break;
                        }
                    }

                    newP->Input(newId);

                    int isHead;
                    cout << "La chu ho? (1-Co, 0-Khong): ";
                    cin >> isHead;
                    if (isHead == 1)
                    {
                        People *currentHead = nullptr;
                        for (People *existingP : ho->members)
                        {
                            if (existingP != oldP && existingP->getIsHouseholdHead())
                            {
                                currentHead = existingP;
                                break;
                            }
                        }

                        if (currentHead != nullptr)
                        {
                            cout << "\n!! CANH BAO: Ho nay dang co chu ho khac la [" << currentHead->getName() << "]\n";
                            cout << "Ban muon chon ai lam Chu ho chinh thuc?\n";
                            cout << "1. Giu nguyen (" << currentHead->getName() << ")\n";
                            cout << "2. Giao cho nguoi dang sua (" << newP->getName() << ")\n";
                            cout << "=> Lua chon: ";
                            int headChoice;
                            cin >> headChoice;
                            if (headChoice == 2)
                            {
                                currentHead->setIsHouseholdHead(false);
                                newP->setIsHouseholdHead(true);
                            }
                            else
                            {
                                newP->setIsHouseholdHead(false);
                            }
                        }
                        else
                        {
                            newP->setIsHouseholdHead(true);
                        }
                    }
                    else
                    {
                        newP->setIsHouseholdHead(false);
                    }

                    ho->members[i] = newP;
                    delete oldP;

                    cout << "=> Cap nhat thong tin thanh cong!\n";
                    return;
                }
            }
        }
        cout << "=> Khong tim thay thanh vien co CCCD " << id << "\n";
    }

    void SearchMemberAdvanced()
    {
        if (households.empty())
        {
            cout << "Danh sach trong!\n";
            return;
        }

        int choice;
        cout << "\n--- TIM KIEM CU THE ---\n";
        cout << "1. Tim theo CCCD\n";
        cout << "2. Tim theo Ten\n";
        cout << "3. Tim theo Ma Ho\n";
        cout << "=> Lua chon: ";
        cin >> choice;

        if (choice == 1)
        {
            int id;
            cout << "Nhap CCCD can tim: ";
            cin >> id;
            for (Household *ho : households)
            {
                for (People *p : ho->members)
                {
                    if (p->getID() == id)
                    {
                        cout << "\n=> [KET QUA]: Tim thay thuoc Ho [" << ho->HouseholdID << "]\n";
                        p->Print();
                        return;
                    }
                }
            }
            cout << "=> Khong tim thay nguoi co CCCD: " << id << "\n";
        }
        else if (choice == 2)
        {
            string name;
            cout << "Nhap ten can tim (Co the nhap 1 phan ten): ";
            cin.ignore(10000, '\n');
            getline(cin, name);
            bool found = false;

            cout << "\n=> [KET QUA TIM KIEM]:\n";
            for (Household *ho : households)
            {
                for (People *p : ho->members)
                {
                    if (p->getName().find(name) != string::npos)
                    {
                        cout << "Ho [" << ho->HouseholdID << "] - ";
                        p->Print();
                        found = true;
                    }
                }
            }
            if (!found)
                cout << "Khong tim thay ai co ten chua: " << name << "\n";
        }
        else if (choice == 3)
        {
            string maHo;
            cout << "Nhap Ma Ho can tim: ";
            cin >> maHo;
            for (Household *ho : households)
            {
                if (ho->HouseholdID == maHo)
                {
                    ho->Print();
                    return;
                }
            }
            cout << "=> Khong tim thay Ma Ho: " << maHo << "\n";
        }
        else
        {
            cout << "=> Lua chon khong hop le!\n";
        }
    }

    void SearchMemberHighLow()
    {
        if (households.empty())
        {
            cout << "Danh sach trong!\n";
            return;
        }
        int criteria, highLow;
        cout << "Tieu chi (1-Tuoi, 2-Tien dong gop): ";
        cin >> criteria;
        cout << "Muc do   (1-Cao nhat, 2-Thap nhat): ";
        cin >> highLow;

        People *best = nullptr;
        double bestVal = (highLow == 1) ? -1 : 1e18;

        for (Household *ho : households)
        {
            for (People *p : ho->members)
            {
                double val = (criteria == 1) ? p->getAge() : p->CalculateMoney();
                if (highLow == 1)
                {
                    if (val > bestVal)
                    {
                        bestVal = val;
                        best = p;
                    }
                }
                else
                {
                    if (val < bestVal)
                    {
                        bestVal = val;
                        best = p;
                    }
                }
            }
        }
        if (best)
        {
            cout << "\n=> KET QUA TIM KIEM:\n";
            best->Print();
        }
        else
        {
            cout << "=> Khong co du lieu phu hop!\n";
        }
    }

    void SortList()
    {
        int criteria, ascending;
        cout << "Tieu chi sap xep (1-Tuoi, 2-Tien dong gop): ";
        cin >> criteria;
        cout << "Thu tu         (1-Tang dan, 2-Giam dan): ";
        cin >> ascending;

        vector<People *> all;
        for (Household *ho : households)
        {
            for (People *p : ho->members)
                all.push_back(p);
        }

        sort(all.begin(), all.end(), [criteria, ascending](People *a, People *b)
             {
            double valA = (criteria == 1) ? a->getAge() : a->CalculateMoney();
            double valB = (criteria == 1) ? b->getAge() : b->CalculateMoney();
            if (ascending == 1) return valA < valB;
            return valA > valB; });

        cout << "\n=> DANH SACH SAU KHI SAP XEP:\n";
        for (People *p : all)
        {
            p->Print();
            cout << "---------------------\n";
        }
    }

    void PrintMemberCount()
    {
        string householdID;
        cout << "Input Ma Ho can xem (vd: HD01): ";
        cin >> householdID;
        for (Household *ho : households)
        {
            if (ho->HouseholdID == householdID)
            {
                cout << "=> Ho [" << householdID << "] hien co: " << ho->members.size() << " thanh vien.\n";
                return;
            }
        }
        cout << "=> Khong tim thay Ma Ho nay!\n";
    }

    void PrintAll() const
    {
        cout << "\n*********** DANH SACH DAN CU ***********\n";
        if (households.empty())
            cout << "(Danh sach trong)\n";
        for (Household *ho : households)
            ho->Print();
        cout << "****************************************\n";
    }

    void ShowStatistics() const
    {
        if (households.empty())
        {
            cout << "\n=> Danh sach hien tai dang trong. Khong co du lieu de thong ke!\n";
            return;
        }

        int totalHouseholds = households.size();
        int totalPeople = 0;
        int countNam = 0, countNu = 0, countKhac = 0;
        int countHSSV = 0, countLaoDong = 0, countNghiHuu = 0;
        double totalContribution = 0;

        for (Household *ho : households)
        {
            totalPeople += ho->members.size();
            for (People *p : ho->members)
            {
                // Thống kê giới tính
                if (p->getGender() == "Nam")
                    countNam++;
                else if (p->getGender() == "Nu")
                    countNu++;
                else
                    countKhac++;

                // Thống kê đối tượng
                if (p->GetType() == "HSSV")
                    countHSSV++;
                else if (p->GetType() == "LaoDong")
                    countLaoDong++;
                else if (p->GetType() == "NghiHuu")
                    countNghiHuu++;

                // Thống kê tài chính
                totalContribution += p->CalculateMoney();
            }
        }

        // In kết quả thống kê
        cout << "\n============= BANG THONG KE DU LIEU =============\n";
        cout << "[1]. Thong tin chung:\n";
        cout << "   - Tong so ho gia dinh  : " << totalHouseholds << " ho\n";
        cout << "   - Tong so nhan khau    : " << totalPeople << " nguoi\n";

        if (totalPeople > 0)
        {
            cout << "\n[2]. Phan loai theo Gioi tinh:\n";
            cout << fixed << setprecision(2); // Cài đặt lấy 2 chữ số thập phân
            cout << "   - Nam  : " << countNam << " nguoi (" << (countNam * 100.0 / totalPeople) << "%)\n";
            cout << "   - Nu   : " << countNu << " nguoi (" << (countNu * 100.0 / totalPeople) << "%)\n";
            cout << "   - Khac : " << countKhac << " nguoi (" << (countKhac * 100.0 / totalPeople) << "%)\n";

            cout << "\n[3]. Phan loai theo Doi tuong:\n";
            cout << "   - Hoc sinh / Sinh vien : " << countHSSV << " nguoi\n";
            cout << "   - Nguoi lao dong       : " << countLaoDong << " nguoi\n";
            cout << "   - Nguoi nghi huu       : " << countNghiHuu << " nguoi\n";

            cout << "\n[4]. Thong ke Tai chinh (Tien dong gop):\n";
            cout << "   - TONG TIEN THU DUOC   : " << (long long)totalContribution << " VND\n";
            cout << "   - Trung binh moi ho    : " << (long long)(totalContribution / totalHouseholds) << " VND\n";
            cout << "   - Trung binh moi nguoi : " << (long long)(totalContribution / totalPeople) << " VND\n";
        }
        cout << "=================================================\n";
    }

    void Menu()
    {
        int choice;
        do
        {
            cout << "\n=========== MENU QUAN LY ===========\n";
            cout << "1. Doc danh sach tu file CSV\n";
            cout << "2. Ghi danh sach ra file CSV\n";
            cout << "3. Them thanh vien\n";
            cout << "4. Chinh sua thong tin thanh vien\n";
            cout << "5. Xoa thanh vien\n";
            cout << "6. Tim thanh vien (Cao/Thap nhat)\n";
            cout << "7. Tim kiem cu the (CCCD/Ten/Ma Ho)\n";
            cout << "8. Sap xep danh sach\n";
            cout << "9. In ra so luong thanh vien cua ho\n";
            cout << "10. Print toan bo danh sach hien tai\n";
            cout << "11. Thong ke du lieu\n"; // Thêm chức năng vào Menu
            cout << "0. Thoat\n";
            cout << "====================================\n";
            cout << "=> Chon tinh nang: ";
            if (!(cin >> choice))
            {
                cin.clear();
                cin.ignore(10000, '\n');
                choice = -1;
            }

            switch (choice)
            {
            case 1:
                LoadFromCSV("data.csv");
                break;
            case 2:
                SaveToCSV("data.csv");
                break;
            case 3:
                AddMember();
                break;
            case 4:
                EditMember();
                break;
            case 5:
                DeleteMember();
                break;
            case 6:
                SearchMemberHighLow();
                break;
            case 7:
                SearchMemberAdvanced();
                break;
            case 8:
                SortList();
                break;
            case 9:
                PrintMemberCount();
                break;
            case 10:
                PrintAll();
                break;
            case 11:
                ShowStatistics();
                break; // Gọi hàm thống kê
            case 0:
                cout << "Dang thoat chuong trinh...\n";
                break;
            default:
                cout << "=> Lua chon khong hop le, vui long thu lai!\n";
            }
        } while (choice != 0);
    }

    ~PopulationManager() { ClearData(); }
};

int main()
{
    PopulationManager manager;
    manager.Menu();
    return 0;
}

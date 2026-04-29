#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>
using namespace std;
struct Date
{
    int day,month,year;
};
class PEOPLE //lớp cha
{
protected:
    int ID;
    string Name;
    Date DayofBirth;
    int Age;
public:
    virtual void nhap()
    {
        cout<<"Nhap so CCCD: ";
        cin>>ID;
        cin.ignore();
        cout<<"Nhap ho ten: ";
        getline(cin,Name);
        cout<<"Nhap ngay sinh: ";
        cin>>DayofBirth.day;
        cout<<"Nhap thang sinh: ";
        cin>>DayofBirth.month;
        cout<<"Nhap nam sinh: ";
        cin>>DayofBirth.year;
    }
    virtual void xuat()
    {
        cout<<"CCCD: "<<ID<<endl;
        cout<<"Ho ten: "<<Name<<endl;
        cout<<"Ngay sinh: "<<DayofBirth.day<<"/"<<DayofBirth.month<<"/"<<DayofBirth.year<<endl;
        cout<<"Tuoi: "<<Age<<endl;
    }
    void TinhTuoi()
    {
        int currentDay = 1, currentMonth = 1, currentYear = 2026;
        Age = currentYear - DayofBirth.year;   
        if (currentMonth < DayofBirth.month || (currentMonth == DayofBirth.month && currentDay < DayofBirth.day))
        {
            Age--;
        }
    }
    virtual double ContributedMoney() = 0;
};
class STUDENT : public PEOPLE //phần này của Khải
{

};
class WORKER : public PEOPLE
{
private:
    string Job;
    double Salary;
    string Company;
public:
    void nhap()
    {
        PEOPLE::nhap();
        cin.ignore();
        cout<<"Nhap nghe nghiep: ";
        getline(cin,Job);
        cout<<"Nhap luong: ";
        cin>>Salary;
        cin.ignore();
        cout<<"Nhap ten cong ty: ";
        getline(cin,Company);
    }
    void xuat() override
    {
        PEOPLE::xuat();
        cout<<"Nghe nghiep: "<<Job<<endl;
        cout<<"Luong: "<<formatMoney(Salary)<<" dong"<<endl;
        cout<<"Cong ty: "<<Company<<endl;
        cout<<"Tien dong gop: "<<formatMoney(ContributedMoney())<<" dong"<<endl;
    }
    double ContributedMoney() override
    {
        return Salary*0.05;
    }
    string formatMoney(double money)
    {
        string s = to_string((long long)money);
        int n = s.length();
        string res = "";
        int count = 0;
        for(int i = n-1; i >= 0; i--)
        {
            res = s[i] + res;
            count++;
            if(count == 3 && i != 0)
            {
                res = "," + res;
                count = 0;
            }
        }
        return res;
    }
};
class RETIRE : public PEOPLE //phần này của Phàm
{

};
class QUANLYHODAN
{
private:
    string Address;
public:
    vector <PEOPLE*> ds;
    int n;
    void nhap()
    {
        cout<<"Nhap dia chi nha: ";
        getline(cin,Address);
        cout<<"Nhap so thanh vien: ";
        cin>>n;
        for(int i = 0; i < n; i++)
        {
            /*cout<<"Chon (1.HSSV 2.Nguoi lao dong 3.Nghi huu): ";
            int choice;
            cin>>choice;*/
            cout <<"\n--- Thanh vien thu "<<i+1<<" ---\n";
            PEOPLE* p;
            /*if (choice == 1)
                p = new STUDENT;
            else if (choice == 2)
                p = new WORKER;
            else
                p = new RETIRE;*/
            p = new WORKER;
            p->nhap();
            p->TinhTuoi();
            ds.push_back(p);
        }
    }
    void xuat()
    {
        cout<<"\n----- THONG TIN HO DAN -----\n";
        cout<<"Dia chi: "<<Address<<endl;
        for(int i = 0; i < ds.size(); i++)
        {
            cout<<"\n--- Thanh vien thu "<<i+1<<" ---\n";
            ds[i]->xuat();
        }
    }
};
int main()
{
    /*QUANLYHODAN ho1;
    ho1.nhap();
    ho1.xuat();*/
    return 0;
}
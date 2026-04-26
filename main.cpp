#include <iostream>
#include <string>
#include <fstream>
#include <vector>
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
}
class STUDENT : public PEOPLE {
private:
    string School;
    double Allowance;

public:
    STUDENT() : School(""), Allowance(0.0) {}

    void nhap() override {
        cout << "\n--- Nhap thong tin Hoc Sinh / Sinh Vien ---\n";
        PEOPLE::nhap(); 
        cin.ignore(); 
        cout << "Nhap ten truong hoc: ";
        getline(cin, School);
        cout << "Nhap tien tro cap: ";
        cin >> Allowance;
    }

    void xuat() override {
        PEOPLE::xuat(); 
        cout << "Truong hoc: " << School << endl;
        cout << "Tro cap: " << (long long)Allowance << " dong" << endl; 
        
        cout << "Tien dong gop: " << (long long)CalculateMoney() << " dong" << endl;
    }

    
    float CalculateMoney() override {
        return 0.0f;
    }

    string GetType() const override {
        return "HSSV";
    }
    string getSchool() const { return School; }
    double getAllowance() const { return Allowance; }
    void setSchool(string s) { School = s; }
    void setAllowance(double a) { Allowance = a; }
};
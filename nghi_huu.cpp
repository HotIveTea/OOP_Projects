class NguoiNghiHuu : public People 
{
private:
    float LuongHuu; // Thuộc tính: Lương hưu

public:
    NguoiNghiHuu(string _name = "", Date _dob = {1, 1, 1950}, int _age = 0, int _id = 0, float _luonghuu = 0.0f)
        : People(_name, _dob, _age, _id) // Lớp cha giờ chỉ nhận 4 tham số: Tên, Ngày sinh, Tuổi, CCCD
    {
        this->LuongHuu = _luonghuu;
    }

    // 1. Phương thức Nhập
    void Nhap() override
    {
        cout << "--- Nhap thong tin Nguoi Nghi Huu ---" << endl;
        People::Nhap(); // Gọi hàm nhập của lớp cha (Chỉ nhập Họ tên, Tuổi, Ngày sinh, CCCD)

        cout << "Nhap luong huu: ";
        cin >> LuongHuu;
    }

    // 2. Phương thức Xuất
    void Xuat() override
    {
        cout << "\n[ NGUOI NGHI HUU ]" << endl;
        People::Xuat();
        cout << "Luong huu: " << LuongHuu << " VND" << endl;
        cout << "Tien dong gop: " << CalculateMoney() << " VND" << endl;
    }

    // 3. Phương thức Tính tiền đóng góp (Đa hình)
    float CalculateMoney() override
    {
        return LuongHuu * 0.02f; // Thuật toán: Người nghỉ hưu đóng góp 2% lương hưu
    }

    // 4. Phương thức GetType (Dùng để phân loại khi đọc/ghi file CSV)
    string GetType() override
    {
        return "NghiHuu"; 
    }
};
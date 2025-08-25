******************************************************************************************************
************************************Logging for Application Debug*************************************
******************************************************************************************************
# Tính năng chính
1.  **Tối ưu cho Linux**: Hoạt động hiệu quả trên các bản phân phối Linux (Ubuntu, Fedora, v.v.).
2.  **An toàn luồng (Thread-Safe)**: Sử dụng `std::mutex` của C++11 để đảm bảo an toàn khi ghi log từ nhiều luồng.
3.  **Thiết kế Singleton**: Đảm bảo chỉ có một thực thể (instance) duy nhất của Logger trong toàn bộ ứng dụng.
4.  **Giao diện Stream**: Sử dụng toán tử `<<` quen thuộc để ghi log một cách linh hoạt.
5.  **Tự động ghi Ngữ cảnh**: Tự động thêm các thông tin quan trọng vào mỗi dòng log:
    *   Dấu thời gian (Timestamp)
    *   Cấp độ log (INFO, WARNING, ERROR)
    *   Tên hàm và số dòng nơi log được gọi (`__FUNCTION__`, `__LINE__`)
6.  **Tự động quản lý File**:
    *   **Tạo file theo ngày**: Tự động tạo file log mới mỗi ngày với định dạng tên `YYYY_MM_DD_logsys.txt`.
    *   **Dọn dẹp file cũ**: Tự động xóa 5 file log cũ nhất khi tổng số file đạt 15, giúp tiết kiệm dung lượng.

******************************************************************************************************
# Yêu cầu dependency
1.  Một trình biên dịch C++ hỗ trợ C++11 hoặc mới hơn (ví dụ: GCC, Clang).
2.  Khi biên dịch, cần thêm cờ `-pthread`.
3.  Thư viện chuẩn C++.

******************************************************************************************************
# Hướng dẫn sử dụng
1.  Include file log vào dự án
    ```cpp
    #include "Logger.h"
    ```
2.  Ghi log
    Bạn không cần cấu hình tên file. Logger sẽ tự động tạo file theo ngày hiện tại.
    ```cpp
    int main() {
        LOG_INFO << "Ứng dụng đang khởi động...";
        
        int value = 42;
        LOG_WARNING << "Phát hiện giá trị đáng ngờ: " << value;

        LOG_ERROR << "Không thể kết nối đến cơ sở dữ liệu.";

        return 0;
    }
    ```
******************************************************************************************************
# Định dạng Log Output
Các dòng log trong file (ví dụ: `2025_09_26_logsys.txt`) sẽ có định dạng như sau:
`Timestamp [Cấp Độ] [TênHàm:SốDòng] Nội dung thông điệp`
******************************************************************************************************
# Biên dịch
```bash
g++ your_main_file.cpp Logger.cpp -o my_app -std=c++11 -pthread
```

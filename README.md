******************************************************************************************************
************************************Logging for Application Debug*************************************
******************************************************************************************************
# Tính năng chính
1. **Thread-Safe**: Sử dụng `CRITICAL_SECTION` của Windows API để đảm bảo an toàn khi ghi log từ nhiều luồng.
2. **Thiết kế Singleton**: Đảm bảo chỉ có một thực thể (instance) duy nhất của Logger trong toàn bộ ứng dụng.
3. **Giao diện Stream**: Sử dụng toán tử `<<` quen thuộc để ghi log một cách linh hoạt.
4. **Tự động ghi Ngữ cảnh**: Tự động thêm các thông tin quan trọng vào mỗi dòng log:
  - Dấu thời gian (Timestamp)
  - Cấp độ log (INFO, WARNING, ERROR)
  - Tên hàm và số dòng nơi log được gọi (__FUNCTION__, __LINE__)
5. **Tự động quản lý File**:
  - **Tạo file theo ngày**: Tự động tạo file log mới mỗi ngày với định dạng tên `YYYY_MM_DD_logsys.txt`.
  - **Dọn dẹp file cũ**: Tự động xóa 5 file log cũ nhất khi tổng số file đạt 15, giúp tiết kiệm dung lượng.
******************************************************************************************************
# Yêu cầu dependency
Thư viện này được viết cho môi trường Windows và có các phụ thuộc sau:  
1.Windows API (windows.h): Cho việc xử lý đa luồng và lấy thời gian hệ thống.  
2. C++ Builder/VCL (System.hpp): Cho kiểu dữ liệu System::UnicodeString. Nếu bạn không dùng C++ Builder, bạn cần xóa hoặc sửa lại phần operator<< cho UnicodeString.  
3. Thư viện chuẩn C++: fstream, sstream, string, v.v.  
******************************************************************************************************
# Hướng dẫn sử dụng
1. Include file log vào dự án
  #include "Logger.h"  
2. Ghi log
   Bạn không cần cấu hình tên file. Logger sẽ tự động tạo file theo ngày hiện tại.
  int main() {  
      LOG_INFO << "Ứng dụng đang khởi động...";
      
      int value = 42;
      LOG_WARNING << "Phát hiện giá trị đáng ngờ: " << value;

      LOG_ERROR << "Không thể kết nối đến cơ sở dữ liệu.";

      return 0;  
  }
******************************************************************************************************
# Định dạng Log Output
Các dòng log trong file (ví dụ: `2025_09_26_logsys.txt`) sẽ có định dạng như sau:  
`Timestamp [Cấp Độ] [TênHàm:SốDòng] Nội dung thông điệp`

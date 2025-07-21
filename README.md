******************************************************************************************************
************************************Logging for Application Debug*************************************
******************************************************************************************************
# Tính năng chính
1. Thread-Safe: Sử dụng CRITICAL_SECTION của Windows API để đảm bảo rằng các thông điệp log từ nhiều luồng khác nhau không bị ghi đè lên nhau.
2. Thiết kế Singleton: Đảm bảo chỉ có một thực thể (instance) duy nhất của Logger trong toàn bộ ứng dụng, dễ dàng truy cập từ mọi nơi.
3. Giao diện Stream: Sử dụng toán tử << quen thuộc để ghi log, cho phép nối chuỗi và các kiểu dữ liệu khác nhau một cách linh hoạt.
4. Tự động ghi Ngữ cảnh: Tự động thêm các thông tin quan trọng vào mỗi dòng log, bao gồm:
  - Dấu thời gian (Timestamp)
  - Cấp độ log (INFO, WARNING, ERROR)
  - Tên hàm và số dòng nơi log được gọi (__FUNCTION__, __LINE__)
Nhiều cấp độ Log: Hỗ trợ các cấp độ khác nhau để phân loại mức độ quan trọng của thông điệp.
******************************************************************************************************
# Yêu cầu dependency
Thư viện này được viết cho môi trường Windows và có các phụ thuộc sau:
1.Windows API (windows.h): Cho việc xử lý đa luồng và lấy thời gian hệ thống.
2. C++ Builder/VCL (System.hpp): Cho kiểu dữ liệu System::UnicodeString. Nếu bạn không dùng C++ Builder, bạn cần xóa hoặc sửa lại phần operator<< cho UnicodeString.
3. Thư viện chuẩn C++: <fstream>, <sstream>, <string>, v.v.
******************************************************************************************************
# Hướng dẫn sử dụng
1. Include file log vào dự án
  #include "Logger.h"  
2. Ghi log cơ bản  
  int main() {  
      // Thay đổi file log thành "my_app_log.txt"  
      Logger::get_instance().set_log_file("my_app_log.txt");  
      LOG_INFO << "Log này sẽ được ghi vào file my_app_log.txt";  
      return 0;  
  }
******************************************************************************************************
# Định dạng Log Output
Các dòng log trong file sẽ có định dạng như sau:  
Timestamp [Cấp Độ] [TênHàm:SốDòng] Nội dung thông điệp

  

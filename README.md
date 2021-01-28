# ESP32_SIM7600
library module SIM7600
Kit Wifi ESP32 4G SIM7600CE
Là phiên bản tiếp nối trong chuỗi mạch ESP32 SIM, KIT Wifi ESP32 4G SIM7600CE sử dụng module SIM7600CE-T hộ trợ các băng tần 2G/3G/4G LTE giúp người dùng linh hoạt trong các ứng dụng cần kết nối Internet tốc độ cao, cũng như thay thế cho các bo mạch băng tần 2G đã cũ

Thông số kỹ thuật
Vi điều khiển trung tâm: ESP-WROOM-32 based on Espressif ESP32 dual core Tensilica LX6 processor with 802.11 b/g/n WiFi and Bluetooth 4.2 LE.
Module SIM7600CE-T: SIM7600CE là module truyền thông 2G/3G/4G và định vị vị trí, hỗ trợ LTE CAT4 tải xuống lên tới 150Mbs, tiêu thụ năng lượng khá thấp.  Xem thêm thông tin về sim7600CE tại đây: Link
Điện áp sử dụng: 4.5-20VDC nếu dùng apdaptor hoặc qua cable USB. Tích hợp mạch giảm áp stepdown cho hiệu suất cao mà không bị nóng như các board arduino thông thường.
Tích hợp mạch nạp USB-UART sử dụng driver CP2102 hiệu năng tốt và dễ nhận driver hơn so với dòng CH340
Chuẩn kích thước Arduino UNO
Sử dụng cổng Micro USB nhỏ gọn , tiện dụng
Khe cắm nano sim ( Push pull ) nhỏ gọn, tiện lợi
Hỗ trợ jack anten SMA với  anten gsm 3dBi đi kem tăng cường khả năng kết nối mạng GSM
Hỗ trợ anten AUX giúp người dùng kết nối thêm Anten LTE để đặt tốc độ tối ưu khi sử dụng băng tần 4G LTE
Mạch nguồn 4V/2A chuẩn cũng cấp đầy đủ năng lượng cho module SIM ( nếu dùng module sim thì nên cấp nguồn từ adaptor ngoài để đảm bảo hoạt động ổn định ).
Giao tiếp với MCU qua UART 2 của ESP32 ( GPIO16,GPIO17 )
Led báo trạng thái mạng
Đặc điểm KIT WIFI ESP32 4G SIM7600CE
KIT WIFI ESP32 4G SIM7600CE là một sự kết hợp hoàn hảo với thiết kế tich hợp nhỏ gọn, đơn giản dễ sử dụng. Hỗ trợ LTE Cat4 tốc độ cao, năng lượng thấp kết hợp với bộ xử lý mạnh mẽ ESP32 hỗ trợ sẵn kết nối Wifi và BLE

Thiết kế dạng chuẩn Shield Arduino UNO thông dụng nên chỉ cần kết hợp thêm 1 shield RELAY là bạn có thể hoàn toàn có một sản phẩm điều khiển thiết bị điện 220V qua SMS, gọi điện hoặc qua internet,… rất nhỏ gọn mà không cần đi dây phức tạp. Và còn rất nhiều ứng dụng khác với WIFI ESP32 4G SIM7600CE  này

Đóng gói
Mạch ESP32 4G SIM7600CE : 1
Anten GSM 3dBi: 1
Tài liệu tham khảo:
Thư viện Arduino : https://github.com/aht-lab/esp32-sim800
Driver mạch nạp: USB-UART CP2102

Kết nối SIM7600CE với ESP32 thông qua hardware UART ( Serial2, chân 16,17 )
 







 

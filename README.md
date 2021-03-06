# Tomato Classification by Color and Size

Sau khi thu hoạch và rửa sạch, cà chua được đưa qua băng chuyền để phân loại thành các loại khác nhau. Dự án giúp thay con người phân loại cà chua dựa trên kích thước và màu sắc sử dụng thư viện OpenCV. Sơ đồ dây chuyền như sau:

![Dây chuyền phân loại cà chua](https://github.com/hieunvce/tomato_classification/blob/master/images/screenshot/steps.png)

## Getting Started

Sau đây là hướng dẫn để chạy dự án trên máy tính cá nhân của bạn, sử dụng cho mục đích kiểm thử và phát triển hoàn thiện thêm phần mềm. Phần mềm đang ở giai đoạn III, chạy trên máy tính cá nhân và có thể xử lí hình ảnh cũng như video hoặc camera. Mục đích cuối cùng là phần mềm có thể chạy trên máy tính nhúng để ứng dụng vào thực tế.

## Features

- Code đơn giản và đang trong quá trình optimize
- Chạy nhanh trên hình ảnh, video, camera
- Đang thiết lập các thông số để có thể chỉnh sửa được
- Đang phát triển GUI cho sản phẩm

## Yêu cầu phần cứng và phần mềm

Để chạy được phần mềm, máy tính cá nhân của bạn cần có:

1. Webcam hoặc camera nếu muốn phần mềm chạy trên camera
2. Phần cứng đang được dùng để phát triển và thử nghiệm:
   * CPU Intel Core i3 2.27Ghz x 2
   * RAM: 4 GB
   * Card đồ họa: Onboard
3. Hệ điều hành: Hiện tại, phần mềm đang được thử nghiệm và phát triển trên hệ điều hành **```Linux Mint 18.3 Cinnamon 64-bit ```** sử dụng **IDE Clion**, tuy nhiên các hệ điều hành dựa trên nhân **Linux** nói chung đều có thể chạy được.
4. Các thư viện và gói cài đặt cần có:
   * **Open CV 3.4.0** . Có thể xem cách cài đặt ở đây: [Install OpenCV](https://www.learnopencv.com/install-opencv3-on-ubuntu/)
   * **cmake, g++**,... Tất cả các gói thư viện và công cụ này sẽ được cài đặt khi bạn cài đặt **OpenCV** theo hướng dẫn bên trên.
   * **git** Cài đặt và cách sử dụng ở đây: [Simple Git Guide](http://rogerdudler.github.io/git-guide/)

## Cài đặt phần mềm

1. Clone dự án về máy tính cá nhân của bạn:

   ```bash
   git clone https://github.com/hieunvce/tomato_classification.git
   ```

2. Di chuyển vào thư mục dự án:

   ```bash
   cd tomato_classification
   ```

3. Build phần mềm

   ```bash
   mkdir build
   cd build/
   cmake ..
   make
   ```

4. Chạy thử phần mềm:

   ```bash
   ./TomatoClassification ../images/tomatos/1.jpg
   ```


## Hình ảnh Demo qua các version

1. Version 1.0: Chạy trên hình ảnh sử dụng ngưỡng chặn:

   ![Version 1.0](https://github.com/hieunvce/tomato_classification/blob/master/images/screenshot/screenshot_v1.0.png)

2. Version 2.0: Chạy trên hình ảnh sử dụng đặc điểm của không gian màu **```L*a*b```**:

   ![Version 2.0](https://github.com/hieunvce/tomato_classification/blob/master/images/screenshot/screenshot_v2.0.png)

3. Version 3.0: Chạy trên video/camera:

   ![Version 3.0](https://github.com/hieunvce/tomato_classification/blob/master/images/screenshot/screenshot_v3.0.png)

4. Version 4.0: Thay đổi lớn về thuật toán. Chạy trên hình ảnh mô phỏng môi trường làm việc công nghiệp đạt tốc độ < 0.02 s/frame

   ![Version 1.0](https://github.com/hieunvce/tomato_classification/blob/master/images/screenshot/screenshot_v4.0.png)

## Contribute

- Issue Tracker: https://github.com/hieunvce/tomato_classification/issues
- Source Code: https://github.com/hieunvce/tomato_classification

## Support

Nếu gặp phải bất cứ vấn đề nào, xin hãy vui lòng cho chúng  tôi biết qua email:

**Email**: hieunv.ce@gmail.com

Hoặc để lại vấn đề trên trang Github này.

## License

The project is licensed under the BSD license.
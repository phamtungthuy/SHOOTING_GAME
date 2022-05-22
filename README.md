# SHOOTING_GAME
1. Hướng dẫn cách lấy file code.  
    - Bước 1: Chọn Fork ở góc trên bên phải để liên kết nơi lưu trữ này đến tài khoản của bạn.  
    - Bước 2: Đến thư mục bạn muốn lưu trữ, nhấn chuột phải chọn terminal hoặc gitBash xong gõ câu lệnh:  
        - *git clone https://github.com/[YOUR_USERNAME]/SHOOTING_GAME.git*  
    - Bước 3: Thoát cửa sổ lệnh. Giờ đây file đã được copy vào nơi lưu trữ của bạn.  
2. Mô tả Game:  
    - Trò chơi tương tự như game Contra cổ điển nhưng đơn giản hơn.   
    - Bạn điều khiển một nhân vật có thể di chuyển tự do, hãy cẩn thận với những quái vật cản đường bạn và những khe vực sâu.  
    - Chú ý bạn chỉ có 3 mạng hãy cẩn thận!  
    - Giới hạn thời gian là 300s, bạn hãy cố gắng đánh bại quái vật 1 cách nhanh nhất có thể, nếu không bạn sẽ thua!  
    - Hãy dùng khẩu súng của bạn một cách hợp lý để tiêu diệt quái vật và tích điểm nhiều nhất có thể.  
    - Trên đường đi có những đồng tiền, hãy ăn nó, nó có thể giúp ích bạn nhiều đấy!  
    - Đích đến là 1 lá cờ, bạn chỉ cần đến đó => bạn sẽ chiến thắng.  
    - Chú ý: có 4 loại quái vật:  
      - Những con quái vật không thể di chuyển được nhưng bắn laser từ miệng mình <=> 10 điểm.  
      - Những con quái vật di chuyển quanh một khu vực nhất định <=> 5 điểm.  
      - Những chiếc máy bay có thể thả bom trên bầu trời <=> 20 điểm.  
      - Boss cuối nằm ở gần vạch đích.
    - Hãy cẩn thận vì nếu bạn va phải quái vật hay trúng đạn, bạn sẽ mất 1 mạng.  
    - Hãy đánh bại và chinh phục trò chơi!  
3. Chức năng của Game:  
   - (1) Phần menu: có 3 lựa chọn:  
        -  ô "Play Game": bắt đầu game khi ấn vào.
        -  ô "Tutorial": hướng dẫn chơi game.  
        -  ô "Exit" nếu bạn không muốn chơi nữa, hãy ấn vào nó.  
   - (2) Dấu "X" bên phải màn hình nếu bạn muốn thoát trò chơi.  
   - (3) Thanh trên cùng hiển thị chỉ số mạng, số coin, điểm và giới hạn thời gian.  
   - (4) Các con quái vật xuất hiện ngẫu nhiên trên bản đồ, tiêu diệt => cộng điểm.  
   - (5) Các khối block hình đồng tiền, có thể ăn.  
   - (6) Nếu chết có thể ấn nút SPACE để chơi lại hoặc nút ESC để thoát game.  
   - (7) Dùng dấu mũi tên LEFT, RIGHT, UP, DOWN trên bàn phím để di chuyển nhân vật.  
   - (8) Nút A dùng để bắn và nút SPACE dùng để nhảy.
   - (9) Nút ESC dùng để tạm dừng trò chơi
   - (10) Có thể bắn sang trái, phải, hoặc bắn 1 góc 45 độ phía trên hoặc dưới.  
   - (11) Chạm vào cây cờ cuối bản đồ để giành chiến thắng.  
4. Kỹ thuật được sử dụng trong dự án:  
    - (1) thư viện SDL, SDL_image, SDL_mixer và SDL_ttf.  
    - (2) sử dụng thêm các file hình ảnh, âm thanh, font chữ bên ngoài.  
    - (3) mảng, mảng con trỏ, mảng vector.  
    - (4) sử dụng truy xuất tệp dữ diệu bên ngoài (file .txt, .dat ...).  
    - (5) sử dụng lớp class: tính đóng gói, tính kế thừa,... để tạo các đối tượng.  
    - (6) sử dụng tách file .h, .cpp để dễ quản lý và phân ra từng chức năng.  
    - (7) sử dụng các vòng lặp, biến toàn cục, biến static,...  
    - (8) thuật toán xử lý va chạm giữa các đối tượng.  
5.   
 - (1)Kết luận: Về cơ bản mình thấy game khá ổn định, mình đã test rất nhiều lần, bạn sẽ ít khi gặp những lỗi cơ bản khi chơi game  
   - Tuy nhiên, bên cạnh đó trò chơi mình vẫn còn một vài chỗ thiếu xót:
      - Hình ảnh vẫn còn chưa đẹp.
      - Bản đồ vẫn còn quá nhỏ.  
      - Ăn tiền chưa biết làm gì.  
      - Hiệu ứng âm thanh vẫn còn cần cải thiện.  
      - Chưa thể lưu điểm của người chơi sau mỗi lần chơi.  
      - Chưa thể lưu tiến độ của người chơi.
 - (2)Hướng phát triển:  
    - Mình dự kiến thêm tính năng khác:  
      - Lưu tiến độ của người chơi => Có thể chơi tiếp phần đang chơi dở lần trước, không phải chơi lại từ đầu nếu bạn thoát game.  
      - Mở rộng bản đồ => thêm nhiều loại quát vật đa dạng.  
      - Tạo nhiều bản đồ => có nhiều cửa ải.  
      - Có cửa hàng, đồng tiền bạn ăn được bạn có thể mua súng, trang bị áo giáp khác xịn hơn.  
      - Bảng xếp hạng lưu điểm và tên của người chơi có thành tích cao.  
 - (3)Tâm đắc:  
    - Khi làm dự án, mình đã rất khó khăn giai đoạn đầu vì lần đầu tiếp xúc đến thư viện SDL và phải xây dựng một tựa game cho riêng mình.  
    - Nhưng bên cạnh đó đem lại cho mình nhiều lợi ích:  
      - Học quy trình tạo ra sản phẩm.  
      - Giúp mình có khả năng tự học, tự tìm tòi những kiến thức mới và những nguồn trên mạng.  
      - Xây dựng mình một tính cách cẩn thận, chu đáo, làm việc có kế hoạch, biết chia nhỏ từng phần công việc để xử lý:  
        - Tách file => Xây dựng từng đối tượng đơn lẻ => Gộp chung lại thành 1 đối tượng lớn hơn.  
      - Việc làm bài tập lớn này giúp cho khả năng cũng như hiểu biết của mình về ngôn ngữ C++ được cải thiện đáng kể khi phải làm quen với nhiều dữ liệu mới.  
      - Dần dần học được cách clean code, refactor.  
        
**ĐÂY LÀ NHỮNG CHIA SẺ MÌNH SAU MỘT THỜI GIAN DÀI HOÀN THÀNH BÀI TẬP LỚN NÀY. MONG SẼ GIÚP ÍCH BẠN ĐƯỢC PHẦN NÀO.   
CẢM ƠN BẠN VÌ ĐÃ QUAN TÂM!**
  
      
    
  
   
 

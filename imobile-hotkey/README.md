這是一個相當複雜的問題，先來看看整個 source tree，# 號後面是說明
imobile-hotkey-0.3.0/
├── DEBIAN                   # 這個目錄是關鍵，但是最重要的檔只有一個，底下會說明
│   ├── changelog.gz         # 版本進版的差異/修改說明，我是懶得寫了，可以跳過不看
│   ├── control              # 最重要的檔，如果有機會，後面再專章說明, 主要修改 Version, Depends 兩個
│   ├── copyright.gz         # 同 changelog.gz, 版權聲明，目前使用 GPLv3
│   ├── md5sums              # 套件的 md5sum, 後面會提到怎麼產生
│   ├── postinst             # 安裝完套件會自動執行此檔，主要是啟動背景服務
│   ├── prerm                # 同前，套件移除前自動執行，主要是刪除一些系統產生的檔，這邊是 /etc/rc*.d
│   └── README               # 算相當重要，但是跟套件無關，請看到這裡時，先移駕過去看看這個檔
  ############ 底下的檔，都是會被安裝到系統中的，肯定是每一個檔都很重要 ##############
├── etc
│   ├── buttond.conf         # buttond 背景服務啟動時會讀它，
│   │                        # 用來設定監聽的 IO port/mask 以及要往上傳的訊息，
│   │                        # 若 BIOS 有更換的話，要適當的修改此檔
│   └── init.d
│       └── buttond-init.d   # 啟動背景服務的方法: sudo service buttond-init.d start
│                            # 停止的方法猜得到：  sudo service buttond-init.d stop
└── usr
    ├── bin
    │   ├── buttond          # 真正的 button daemon, 建議不要手動執行它，請透過上一個檔
    │   ├── buttons_client   # 桌面使用者會執行的客戶端工具，但是也不要手動執行它，見後面的說明
    │   ├── cappind.py       # 工具，見 README
    │   └── hotkey_config.sh # 桌面使用者的設定熱鍵工具，
    │                        # 請登入後至少要執行一次，才會自動呼叫 buttons_client
    │                        # 這個工具會產生 $HOME/.config/upstart/buttons_client.conf
    │                        # 它是使用者登入桌面自動執行的設定檔，啟動方法是：
    │                        # start buttons_client; 停止是 stop buttons_client
    └── share                # 底下都只是備份，含真正的 source code，
        │                    # 一般用戶只需要前面的資訊
        │                    # 開發者一定要研讀下面的檔
        └── doc
            └── imobile-hotkey
                ├── buttond.cpp        # buttond 原始檔
                ├── buttons_client.c   # buttons_client 原始檔
                ├── buttons_client.rc  # 供 buttons_client 使用的設定檔，會放在 $HOME/.buttons_client.rc
                │                      # 請透過 hotkey_config.sh 修改此檔，以免產生錯誤
                ├── buttons_client.sh  # 這是開發過程的工具，沒維護，
                │                      # 大致相當於編譯好的 buttons_client 的 script，研究 dbus 可以看此檔
                ├── init.d-buttond     # 啟動背景服務用的
                └── Makefile           # 可以用來編譯 buttond, buttons_client

最後還有一個 make-deb.sh, 用來編譯成 .deb....其實已經很簡單了


用法:
    三個工具: buttond, buttons_client, hotkey_config.sh
buttond:
  1. /usr/bin/buttond
  2. 監聽按鈕，並透過 dbus 傳送訊息給上層應用程式
  3. 透過 /etc/init.d/buttond-init.d 來啟動/停止,
    3.1. 正常的啟動是 sudo service buttond-init.d start
    3.2. 正常的停止是 sudo service buttond-init.d stop
    3.3. 本來我想把名稱寫成 buttond, 但是又怕別人跟 /usr/bin/buttond 搞混
    3.4. 反正不重要，因為開機後會自動執行，別管這段了
    3.5. 自動執行的原因是安裝套件時，有呼叫 update-rc.d buttond-init.d defaults,
  4. 監聽的方式，是不斷的讀 IO port, 其設定值放在 /etc/buttond.conf
    4.1. /etc/buttond.conf 有三個欄位，IO Port:bit mask:要上傳的訊息
    4.2. 理論上只要 BIOS 沒變動，這個設定檔就不必改
    4.3. 目前按鈕的順序是由下往上為 B1...B5, 與 buttons_client.rc 相呼應即可自行修改
buttons_client:
  1. /usr/bin/buttons_client
  2. buttons_client 是接收來自 buttond 的按鈕事件，並執行相對應的程式
  3. 會讀 $HOME/.buttons_client.rc，第一次（它不存在的時候）請先用 hotkey_config.sh 產生
  4. 請勿直接修改此檔
  5. 這個工具因為必須在用戶登入桌面後自動執行，所以也由 hotkey_config.sh 負責產生一個設定檔,
   5.1. 設定檔在 $HOME/.config/upstart/buttons_client.conf
   5.2. 設定檔不太需要修改，因為我也不會改
   5.3. 有了這個設定檔，登入桌面後會自動啟動 buttons_client
   5.4. 手動啟動的方法是 start buttons_client
   5.5. 手動停止的方法是 stop buttons_client
hotkey_config.sh:
  1. /usr/bin/hotkey_config.sh
  2. 讓用戶設定熱鍵對應的命令，只能使用絕對路徑
  3. 使用者第一次使用時，會產生兩個檔: $HOME/.buttons_client.rc, $HOME/.config/upstart/buttons_client.conf
    3.1. $HOME/.buttons_client.rc 是用來設定按鍵與要執行的應用程式對應檔，相當重要
    3.2. $HOME/.config/upstart/buttons_client.conf 則是使用者登入桌面自動執行的設定檔，也是相當重要，但是不必修改
  4. 執行後，會在桌面的最上方狀態列產生一個 Hotkey 字樣+圖示，下拉後可以設定熱鍵

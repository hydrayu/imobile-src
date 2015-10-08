用途: 將本機設定成「開機時自動登入」或取消它

語法:
1. 取消自動登入
  WindowsAutoLogin.exe -noautologin

2. 設定「開機時自動登入」
  注意：需要配合帳號/密碼, 

  a) 下例是沒有密碼的用法，兩個的結果應該是一樣的
  WindowsAutoLogin.exe -autologin -u U45J
  WindowsAutoLogin.exe -autologin -u U45J -p

  b) 下例是有密碼的狀況
  WindowsAutoLogin.exe -autologin -u U45J -p jj

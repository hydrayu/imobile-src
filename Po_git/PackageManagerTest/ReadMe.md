### Test Environment
Devices: MT8382
OS: android 4.4.2
---

### Remark:
>裡面有 3 個功能
>test1()                       --> 可以透過此 function 找尋 package name (例如com.android.nfc)
>kill_me()                     --> 可以透過此 function 找尋自己的 package name 並 kill 自己 (可透過 ps aux | grep packagemanager 驗證)
>getRunningAppProcessInfo()    --> 可以透過此 function 找尋 package name (包含 pid 等，參考內地的 code)
>getRunningAppProcessInfo_Po() --> 可以透過此 function 找尋 package name 並 kill 自己或別人 (經過驗證無法 kill com.android.nfc)
>
---

### result picture:
> 1. How to USE
![alt tag](https://github.com/showoowohs/Po_git/blob/master/xxx/demo/result001.png.gif)

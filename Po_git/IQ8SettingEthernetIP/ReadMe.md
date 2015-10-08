Test Environment
Devices: IQ8 
OS: not root

need to file:
A. /system/local_script/dns.sh(branch: No_IO_DNS)

Remark:
1. This APP can write data to /mnt/shell/emulated/0/IQ8_EthernetIP.sh(uesr define LAN IP)
P.S IQ8_EthernetIP.sh is user define

2. This APP can read /mnt/shell/emulated/0/IQ8_IP_info
P.S IQ8_IP_info is current LAN IP address

note
1. 2015/05/08 add DNS2 setting function

result:
1. save user setting IP
![alt tag](https://github.com/showoowohs/Po_git/blob/master/IQ8SettingEthernetIP/demo/save.gif)

2. read current IP
![alt tag](https://github.com/showoowohs/Po_git/blob/master/IQ8SettingEthernetIP/demo/read.gif)

3. restore default IP
![alt tag](https://github.com/showoowohs/Po_git/blob/master/IQ8SettingEthernetIP/demo/default.gif)
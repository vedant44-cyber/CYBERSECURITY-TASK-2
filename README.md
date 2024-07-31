# CYBERSECURITY-TASK-2

## Name :- Vedant Dorlikar <br> Roll no :- 107123133 <br>Dept :- EEE

### Disclaimer: My code is not compatible with every system. (It may work in KERNAL_VERSION>4.7 & KERNAL_VERSION<5.7).
The code is not working correctly due to manipulating the cr0 register in ARM64, which might be incorrect.


My Setup <br>
ubuntu 20.04.6 LTS<br>
5.4.0-190-generic <br>
ARM-64 arch<br>
GNU/Linux <br>
UTM (VM) <br>

## 1) Environment Setup:<br>
  ```bash
 sudo apt update & sudo apt upgrade
```
Make sure to use Kernel Version in between 4.7.x to 5.7.x ( Reason is given in report)
  ```bash
 sudo apt-get install build-essential linux-headers-$(uname -r)

```
   <br>
  
  ( Keep rootkit.c and Makefile in same directory)
  
### 2)To compile the code 
Run this command in same Directory
```bash
make
``` 



### 3) Load and Unload the Module<br>
To load the module :<br>
```command
 sudo insmod rootkit.ko
```
To unload the module :<br>
```command
 sudo rmmod rootkit.ko
```
<br>

To check the messages:<br>
```command
 sudo dmesg | tail
```
To Cheak Loaded module:<br>
```bash
lsmod
```
<img width="731" alt="Screenshot 2024-07-31 at 6 21 43 PM" src="https://github.com/user-attachments/assets/6eacbfd2-70de-4138-80fa-c71c642f9efc">






  

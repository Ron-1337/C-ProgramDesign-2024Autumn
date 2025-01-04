@echo off
echo [*] 清理编译残留
if exist bin rmdir /s /q bin
if exist main.exe del main.exe

echo [+] 开始编译
mkdir bin

echo [+] 编译源文件
gcc -c src/main.c -o bin/main.o -Isrc
gcc -c src/utils.c -o bin/utils.o -Isrc
gcc -c src/define.c -o bin/define.o -Isrc
gcc -c src/destination.c -o bin/destination.o -Isrc
gcc -c src/order.c -o bin/order.o -Isrc

echo [+] 链接目标文件
gcc bin/main.o bin/utils.o bin/define.o bin/destination.o bin/order.o -o bin/CourseDesign.exe -lwinhttp -lz
echo [+] 编译完成
echo [+] 开始运行
bin\CourseDesign.exe
echo [+] 运行完成
pause
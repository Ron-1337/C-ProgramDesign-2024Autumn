@echo off
echo [*] ����������
if exist bin rmdir /s /q bin
if exist main.exe del main.exe

echo [+] ��ʼ����
mkdir bin

echo [+] ����Դ�ļ�
gcc -c src/main.c -o bin/main.o -Isrc
gcc -c src/utils.c -o bin/utils.o -Isrc
gcc -c src/define.c -o bin/define.o -Isrc
gcc -c src/destination.c -o bin/destination.o -Isrc
gcc -c src/order.c -o bin/order.o -Isrc

echo [+] ����Ŀ���ļ�
gcc bin/main.o bin/utils.o bin/define.o bin/destination.o bin/order.o -o bin/CourseDesign.exe -lwinhttp -lz
echo [+] �������
echo [+] ��ʼ����
bin\CourseDesign.exe
echo [+] �������
pause
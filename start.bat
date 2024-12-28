echo "清理编译残留"
rm main.exe
echo "开始编译"
gcc .\src\main.c .\src\utils.c .\src\define.c .\src\destination.c .\src\order.c -o main.exe
echo "编译完成"
echo "========开始运行========"
.\main.exe
echo "========运行完成========"
pause
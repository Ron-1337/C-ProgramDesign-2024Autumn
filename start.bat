echo "����������"
rm main.exe
echo "��ʼ����"
gcc .\src\main.c .\src\utils.c .\src\define.c .\src\destination.c .\src\order.c -o main.exe
echo "�������"
echo "========��ʼ����========"
.\main.exe
echo "========�������========"
pause
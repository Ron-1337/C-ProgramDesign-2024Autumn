#define _CRT_SECURE_NO_WARNINGS

#include "utils.h"

const int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

int show_menu(char *title, MenuList menu_list[], int count) {
  // 菜单项
  int currentChoice = 0;
  char key;

  while (1) {
    system("cls");
    printf("\n========%s========\n", title);

    // 显示菜单项
    for (int i = 0; i < count; i++) {
      if (i == currentChoice) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x70);
        printf("[%c] %s", 'A' + i, menu_list[i].name);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
        printf("\n");
      } else {
        printf("[%c] %s\n", 'A' + i, menu_list[i].name);
      }
    }
    printf("\n");
    printf("使用方向键↑↓选择, 回车确认, 或直接按选项字母, ESC返回\n");

    // 读取按键
    key = _getch();
    if (key == KEY_ESC) {
      return -1;
    } else if (_kbhit()) {
      key = _getch();
      switch (key) {
        case KEY_UP:
          currentChoice = (currentChoice - 1 + count) % count;
          break;
        case KEY_DOWN:
          currentChoice = (currentChoice + 1) % count;
          break;
      }
    } else if (key == KEY_ENTER) {
      return menu_list[currentChoice].value;
    } else if (toupper(key) >= 'A' && toupper(key) <= 'A' + count - 1) {
      currentChoice = toupper(key) - 'A';
      return menu_list[currentChoice].value;
    }
    continue;
  }
  return 0;
}

Date get_today() {
  time_t now = time(NULL);
  struct tm *tm = localtime(&now);
  return (Date){
      .year = tm->tm_year + 1900, .month = tm->tm_mon + 1, .day = tm->tm_mday};
}

Date get_next_day(Date date) {
  Date next_day = date;
  next_day.day++;

  int is_leap_year =
      (date.year % 4 == 0 && date.year % 100 != 0) || (date.year % 400 == 0);

  int max_days = days_in_month[next_day.month - 1];
  if (next_day.month == 2 && is_leap_year) {
    max_days++;
  }

  if (next_day.day > max_days) {
    next_day.day = 1;
    next_day.month++;
    if (next_day.month > 12) {
      next_day.month = 1;
      next_day.year++;
    }
  }

  return next_day;
}

int time_diff(Date date) {
  Date today = get_today();
  struct tm tm1 = {.tm_year = today.year - 1900,
                   .tm_mon = today.month - 1,
                   .tm_mday = today.day,
                   .tm_hour = 0,
                   .tm_min = 0,
                   .tm_sec = 0};
  struct tm tm2 = {.tm_year = date.year - 1900,
                   .tm_mon = date.month - 1,
                   .tm_mday = date.day,
                   .tm_hour = 0,
                   .tm_min = 0,
                   .tm_sec = 0};

  time_t time1 = mktime(&tm1);
  time_t time2 = mktime(&tm2);

  return difftime(time2, time1) / 86400;
}

char *http_get(const wchar_t *server, const wchar_t *path, size_t *resultSize) {
  // 初始化 WinHTTP 会话
  HINTERNET hSession =
      WinHttpOpen(L"WinHTTP Example/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                  WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

  if (!hSession) {
    wprintf(L"WinHttpOpen failed with error: %d\n", GetLastError());
    return NULL;
  }

  // 连接到服务器
  HINTERNET hConnect =
      WinHttpConnect(hSession, server, INTERNET_DEFAULT_HTTPS_PORT, 0);
  if (!hConnect) {
    wprintf(L"WinHttpConnect failed with error: %d\n", GetLastError());
    WinHttpCloseHandle(hSession);
    return NULL;
  }

  // 创建请求句柄
  HINTERNET hRequest =
      WinHttpOpenRequest(hConnect, L"GET", path, NULL, WINHTTP_NO_REFERER,
                         WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);

  if (!hRequest) {
    wprintf(L"WinHttpOpenRequest failed with error: %d\n", GetLastError());
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
    return NULL;
  }

  // 发送请求
  if (!WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
                          WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
    wprintf(L"WinHttpSendRequest failed with error: %d\n", GetLastError());
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
    return NULL;
  }

  // 接收响应
  if (!WinHttpReceiveResponse(hRequest, NULL)) {
    wprintf(L"WinHttpReceiveResponse failed with error: %d\n", GetLastError());
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
    return NULL;
  }

  // 读取数据
  DWORD dwSize = 0, dwDownloaded = 0;
  BYTE *responseBuffer = NULL;
  DWORD totalSize = 0;

  do {
    // 查询响应数据块大小
    if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) {
      wprintf(L"WinHttpQueryDataAvailable failed with error: %d\n",
              GetLastError());
      break;
    }

    if (dwSize == 0) break;

    // 分配缓冲区
    BYTE *tempBuffer = (BYTE *)malloc(dwSize);
    if (!tempBuffer) {
      wprintf(L"Memory allocation failed\n");
      break;
    }

    // 读取数据
    if (!WinHttpReadData(hRequest, tempBuffer, dwSize, &dwDownloaded)) {
      wprintf(L"WinHttpReadData failed with error: %d\n", GetLastError());
      free(tempBuffer);
      break;
    }

    // 将数据拼接到响应缓冲区
    responseBuffer = (BYTE *)realloc(responseBuffer, totalSize + dwDownloaded);
    if (!responseBuffer) {
      wprintf(L"Memory reallocation failed\n");
      free(tempBuffer);
      break;
    }

    memcpy(responseBuffer + totalSize, tempBuffer, dwDownloaded);
    totalSize += dwDownloaded;

    free(tempBuffer);
  } while (dwSize > 0);

  // 关闭句柄
  WinHttpCloseHandle(hRequest);
  WinHttpCloseHandle(hConnect);
  WinHttpCloseHandle(hSession);

  if (totalSize == 0) {
    wprintf(L"No response data received.\n");
    free(responseBuffer);
    return NULL;
  }

  // UTF-8 转换为 GBK
  int utf8Length = (int)totalSize;
  int wideLength = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)responseBuffer,
                                       utf8Length, NULL, 0);

  if (wideLength == 0) {
    wprintf(L"MultiByteToWideChar failed with error: %d\n", GetLastError());
    free(responseBuffer);
    return NULL;
  }

  wchar_t *wideBuffer = (wchar_t *)malloc(wideLength * sizeof(wchar_t));
  if (!wideBuffer) {
    wprintf(L"Memory allocation failed\n");
    free(responseBuffer);
    return NULL;
  }

  MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)responseBuffer, utf8Length,
                      wideBuffer, wideLength);

  int gbkLength = WideCharToMultiByte(CP_ACP, 0, wideBuffer, wideLength, NULL,
                                      0, NULL, NULL);
  if (gbkLength == 0) {
    wprintf(L"WideCharToMultiByte failed with error: %d\n", GetLastError());
    free(responseBuffer);
    free(wideBuffer);
    return NULL;
  }

  char *gbkBuffer = (char *)malloc(gbkLength + 1);  // +1 for null terminator
  if (!gbkBuffer) {
    wprintf(L"Memory allocation failed\n");
    free(responseBuffer);
    free(wideBuffer);
    return NULL;
  }

  WideCharToMultiByte(CP_ACP, 0, wideBuffer, wideLength, gbkBuffer, gbkLength,
                      NULL, NULL);

  // 释放内存
  free(responseBuffer);
  free(wideBuffer);

  // 添加 null terminator
  gbkBuffer[gbkLength] = '\0';

  // 设置返回大小
  *resultSize = gbkLength;

  return gbkBuffer;
}

char *url_encode(const char *str) {
  if (str == NULL) return NULL;

  // 计算编码后的长度
  int len = strlen(str);
  char *encoded =
      (char *)malloc(len * 3 + 1);  // 最坏情况：每个字符都需要编码（变成%XX）
  if (!encoded) return NULL;

  int j = 0;
  for (int i = 0; i < len; i++) {
    unsigned char c = str[i];
    if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
      encoded[j++] = c;
    } else {
      sprintf(&encoded[j], "%%%02X", c);
      j += 3;
    }
  }
  encoded[j] = '\0';

  return encoded;
}

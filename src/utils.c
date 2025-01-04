#define _CRT_SECURE_NO_WARNINGS

#include "utils.h"

const int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

int show_menu(char* title, MenuList menu_list[], int count) {
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
    printf("使用方向键↑↓选择, 回车确认, 或直接按选项字母\n");

    // 读取按键
    key = _getch();
    if (_kbhit()) {
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
  struct tm* tm = localtime(&now);
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

char* http_get(const char* host, const char* path) {
  DWORD data_len = 0;
  char* response_data = NULL;

  // 初始化WinHTTP
  HINTERNET h_session =
      WinHttpOpen(L"Weather API Client", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                  WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
  if (!h_session) {
    return NULL;
  }

  // 转换host到宽字符
  int host_len = MultiByteToWideChar(CP_UTF8, 0, host, -1, NULL, 0);
  wchar_t* wide_host = (wchar_t*)malloc(host_len * sizeof(wchar_t));
  MultiByteToWideChar(CP_UTF8, 0, host, -1, wide_host, host_len);

  // 转换path到宽字符
  int path_len = MultiByteToWideChar(CP_UTF8, 0, path, -1, NULL, 0);
  wchar_t* wide_path = (wchar_t*)malloc(path_len * sizeof(wchar_t));
  MultiByteToWideChar(CP_UTF8, 0, path, -1, wide_path, path_len);

  // 连接到服务器
  HINTERNET h_connect =
      WinHttpConnect(h_session, wide_host, INTERNET_DEFAULT_HTTPS_PORT, 0);
  if (h_connect) {
    // 创建请求句柄
    HINTERNET h_request = WinHttpOpenRequest(
        h_connect, L"GET", wide_path, NULL, WINHTTP_NO_REFERER,
        WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
    if (h_request) {
      // 添加Accept-Encoding头
      LPCWSTR headers = L"Accept-Encoding: gzip";
      WinHttpAddRequestHeaders(h_request, headers, -1L,
                               WINHTTP_ADDREQ_FLAG_ADD);

      // 发送请求
      if (WinHttpSendRequest(h_request, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
                             WINHTTP_NO_REQUEST_DATA, 0, 0, 0) &&
          WinHttpReceiveResponse(h_request, NULL)) {
        // 检查是否是gzip压缩的响应
        DWORD size = 0;
        DWORD index = 0;
        BOOL is_gzipped = FALSE;
        WinHttpQueryHeaders(h_request, WINHTTP_QUERY_CONTENT_ENCODING,
                            WINHTTP_HEADER_NAME_BY_INDEX, NULL, &size, &index);
        if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
          wchar_t* encoding = (wchar_t*)malloc(size);
          if (WinHttpQueryHeaders(h_request, WINHTTP_QUERY_CONTENT_ENCODING,
                                  WINHTTP_HEADER_NAME_BY_INDEX, encoding, &size,
                                  &index)) {
            is_gzipped = (wcscmp(encoding, L"gzip") == 0);
          }
          free(encoding);
        }

        // 读取响应数据
        DWORD available_size = 0;
        DWORD downloaded_size = 0;
        DWORD buffer_size = 0;
        char* temp_buffer = NULL;

        do {
          available_size = 0;
          WinHttpQueryDataAvailable(h_request, &available_size);

          if (available_size > 0) {
            temp_buffer =
                (char*)realloc(response_data, data_len + available_size + 1);
            if (temp_buffer) {
              response_data = temp_buffer;
              buffer_size = available_size;

              if (WinHttpReadData(h_request, response_data + data_len,
                                  buffer_size, &downloaded_size)) {
                data_len += downloaded_size;
                response_data[data_len] = '\0';
              }
            }
          }
        } while (available_size > 0);
      }
      WinHttpCloseHandle(h_request);
    }
    WinHttpCloseHandle(h_connect);
  }

  // 清理资源
  free(wide_host);
  free(wide_path);
  WinHttpCloseHandle(h_session);

  return response_data;
}

char* url_encode(const char* str) {
  if (str == NULL) return NULL;

  // 计算编码后的长度
  int len = strlen(str);
  char* encoded =
      (char*)malloc(len * 3 + 1);  // 最坏情况：每个字符都需要编码（变成%XX）
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

// 天气
#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  // panic("Not implemented");
  va_list ap;
  va_start(ap, fmt);
  int i = 0;
  int cnt = 0;
  while (fmt[i] != 0) {
    if (fmt[i] == '%' && fmt[i+1] == 'd') {
        int t = va_arg(ap, int);
        int t2 = t;
        if (t < 0) {
          putch('-');
          cnt++;
          t = -t;
        }
        int n = 1;
        do {
          t /= 10;
          n *= 10;
        } while (t != 0);
        n /= 10;
        t = t2;
        do {
          putch((t / n) + '0');
          cnt++;
          t %= n;
          n /= 10;
        }while (n != 0); 
        i += 2;
    }
    else if (fmt[i] == '%' && fmt[i + 1] == 'u') {
      unsigned int t = va_arg(ap, unsigned int);
        unsigned int t2 = t;
        
       unsigned int n = 1;
        do {
          t /= 10;
          n *= 10;
        } while (t != 0);
        n /= 10;
        t = t2;
        do {
          putch((t / n) + '0');
          cnt++;
          t %= n;
          n /= 10;
        }while (n != 0); 
        i += 2;
    }
    else if (fmt[i] == '%' && fmt[i+1] == 's') {
      char * t = va_arg(ap, char*);
      int k = 0; 
      while (t[k] != 0) {
        putch(t[k]);
        k++;
      }
      i += 2;
    }
    else if (fmt[i] == '%' && fmt[i + 1] == 'p') {
      putstr("0x");
      uint32_t t = va_arg(ap, uint32_t);
      uint32_t t2 = t;
        int n = 1;
        do {
          t /= 16;
          n *= 16;
        } while (t != 0);
        n /= 16;
        t = t2;
        do {
          if (t / n < 10) {
            putch((t / n) + '0');
          }
          else {
            putch((t / n - 10) + 'a');
          }
          cnt++;
          t %= n;
          n /= 16;
        }while (n != 0); 
        i += 2;
    }
    else {
      putch(fmt[i]);
      cnt++;
      i++;
    }
  }
  va_end(ap);
  return cnt;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
  // panic("Not implemented");
  va_list ap;
  va_start(ap, fmt);
  int i = 0;
  int j = 0;
  while (fmt[i] != 0) {
    if (fmt[i] == '%' && fmt[i+1] == 'd') {
        int t = va_arg(ap, int);
        int t2 = t;
        if (t < 0) {
          out[j++] = '-';
          t = -t;
        }
        int n = 1;
        do {
          t /= 10;
          n *= 10;
        } while (t != 0);
        n /= 10;
        t = t2;
        do {
          out[j++] = t / n + '0';
          t %= n;
          n /= 10;
        }while (n != 0); 
        i += 2;
    }
    else if (fmt[i] == '%' && fmt[i+1] == 's') {
      char * t = va_arg(ap, char*);
      strcat(out + j, t);
      j += strlen(t);
      i += 2;
    }
    else {
      out[j++] = fmt[i];
      i++;
    }
  }
  va_end(ap);
  out[j] = 0;
  return strlen(out);
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  // panic("Not implemented");

  va_list ap;
  va_start(ap, fmt);
  int j = 0, i = 0;
  while (j < n - 1 && fmt[i] != 0){

   if (fmt[i] == '%' && fmt[i+1] == 'd') {
        int t = va_arg(ap, int);
        int t2 = t;
        if (t < 0) {
          out[j++] = '-';
          if (j >= n - 1) break;
          t = -t;
        }
        int n = 1;
        do {
          t /= 10;
          n *= 10;
        } while (t != 0);
        n /= 10;
        t = t2;
        do {
          out[j++] = t / n + '0';
          if (j >= n - 1) break;
          t %= n;
          n /= 10;
        }while (n != 0); 
        i += 2;
    }
    else if (fmt[i] == '%' && fmt[i+1] == 's') {
      char * t = va_arg(ap, char*);
      int x = n - j - 1;
      int k = 0;
      for (k = 0; k < x && t[k] != 0; k++) {
        out[j++] = t[k];
      }
      j += k; 
      i += 2;
    }
    else {
      out[j++] = fmt[i];
      i++;
    }
  }
  va_end(ap);
  out[j] = 0;
  return strlen(out);
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  // panic("Not implemented");
  int j = 0, i = 0;
  while (j < n - 1 && fmt[i] != 0){

   if (fmt[i] == '%' && fmt[i+1] == 'd') {
        int t = va_arg(ap, int);
        int t2 = t;
        if (t < 0) {
          out[j++] = '-';
          if (j >= n - 1) break;
          t = -t;
        }
        int n = 1;
        do {
          t /= 10;
          n *= 10;
        } while (t != 0);
        n /= 10;
        t = t2;
        do {
          out[j++] = t / n + '0';
          if (j >= n - 1) break;
          t %= n;
          n /= 10;
        }while (n != 0); 
        i += 2;
    }
    else if (fmt[i] == '%' && fmt[i+1] == 's') {
      char * t = va_arg(ap, char*);
      int x = n - j - 1;
      int k = 0;
      for (k = 0; k < x && t[k] != 0; k++) {
        out[j++] = t[k];
      }
      j += k; 
      i += 2;
    }
    else {
      out[j++] = fmt[i];
      i++;
    }
  }
  va_end(ap);
  out[j] = 0;
  return strlen(out);
}

#endif

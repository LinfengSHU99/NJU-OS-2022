#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  // panic("Not implemented");
  if (s == NULL) return 0;
  int i = 0; 
  for (i = 0; s[i] != 0; i++) {
    
  }
  return i;
}

char *strcpy(char *dst, const char *src) {
  // panic("Not implemented");
  int i = 0;
  for (i = 0; src[i] != 0; i++) {
    dst[i] = src[i];
  }
  dst[i] = 0;
  return dst; 
}

char *strncpy(char *dst, const char *src, size_t n) {
  // panic("Not implemented");
  int i = 0;
  for (i = 0; src[i] != 0 && i < n; i++) {
    dst[i] = src[i];
  }
  while (i < n) {
    dst[i] = 0;
    i++;
  }
  return dst;
}

char *strcat(char *dst, const char *src) {
  // panic("Not implemented");
  int i = 0;
  int j = 0;
  for (i = 0; dst[i] != 0; i++) {
    
  }
  for (j = 0; src[j] != 0; j++) {
    dst[i + j] = src[j];
  }
  dst[i + j] = 0;
  return dst;
}

int strcmp(const char *s1, const char *s2) {
  // panic("Not implemented");
  int i = 0;
  for (i = 0; s1[i] != 0 && s2[i] != 0; i++) {
    if (s1[i] < s2[i]) return -1;
    else if (s1[i] > s2[i]) return 1; 
  }
  if (s1[i] == s2[i]) return 0;
  return 0;
}

int strncmp(const char *s1, const char *s2, size_t n) {
  // panic("Not implemented");
  int i = 0; 
  for (i = 0; s1[i] != 0 && s2[i] != 0 && i < n; i++) {
    if (s1[i] < s2[i]) return -1;
    else if (s1[i] > s2[i]) return 1;
  }
  if (s1[i] == s2[i]) return 0;
  else if (s1[i] < s2[i]) return -1;
  else return 1;
  return 0;
}

void *memset(void *s, int c, size_t n) {
  // panic("Not implemented");
  unsigned char* p=s;
    while(n--)
    {
        *p++ = (unsigned char)c;
    }
    return s;
}

void *memmove(void *dst, const void *src, size_t n) {
  // panic("Not implemented");
  char t[n];
  int i;
  char *p = (char*) src;
  char *p2 = (char*) dst;
  for (i = 0; i < n; i++) {
    t[i] = *p;
    p++;
  }
  p = (char*) src;
  for (i = 0; i < n; i++) {
    *p2 = t[i];
    p2++;
  }
  return dst;
}

void *memcpy(void *out, const void *in, size_t n) {
  // panic("Not implemented");

//  int i;
//   char *p = (char*) out;
//   char *p2 = (char*) in;
 
//   for (i = 0; i < n; i++) {
//     *p2 = *p; wrong!!!
//     p++; 
//     p2++;
//   }
//   return out;
char *pszDest = (char *)out;
    const char *pszSource =( const char*)in;
    if((pszDest!= NULL) && (pszSource!= NULL))
    {
        while(n) //till cnt
        {
            //Copy byte by byte
            *(pszDest++)= *(pszSource++);
            --n;
        }
    }
    return out;
}


int memcmp(const void *s1, const void *s2, size_t n) {
  // panic("Not implemented");

  // int i = 0;
  // char *p1 = (char*) s1;
  // char *p2 = (char*) s2;
  // for (i = 0; i < n; i++) {
  //   if ((unsigned char)*p1 < (unsigned char) *p2) return (unsigned char) *p1 - (unsigned char) *p2;
  //   else if ((unsigned char) *p1 > (unsigned char) *p2) return (unsigned char) *p1 - (unsigned char) *p2;
  // }
  // return 0;
  unsigned char *p = (unsigned char*) s1;
    unsigned char *q = (unsigned char*) s2;
    int charCompareStatus = 0;
    //If both pointer pointing same memory block
    if (s1 == s2)
    {
        return charCompareStatus;
    }
    while (n > 0)
    {
        if (*p != *q)
        {  //compare the mismatching character
            charCompareStatus = (*p >*q)?1:-1;
            break;
        }
        n--;
        p++;
        q++;
    }
    return charCompareStatus;
}
// size_t strlen(const char *s) {
//   size_t i = 0;
//   while(s[i] && s[i] != '\0') i++;
//   return i;
// }

// char *strcpy(char* dst,const char* src) {
//   /* 错误的实现方法
//    * return strncpy(dst, src, strlen(dst) );
//    *
//    * 正确的实现方法
//    * return strncpy(dst, src, strlen(src));但这种方法效率比较低下
//    */

//   size_t i;
//   for(i=0;src[i]!='\0';i++) 
//     dst[i]=src[i];
  
//   dst[i]='\0';
//   return dst;
// }

// char *strncpy(char* dst, const char* src, size_t n) {
//   size_t i;
//   for (i = 0; i < n && src[i]!= '\0'; i++) dst[i] = src[i];
//   for (; i < n; i++) dst[i] = '\0';
//   return dst;
// }

// char* strcat(char* dst, const char* src) {
  
//   /* 虽然这种方法也可以通过，但是性能非常非常的低下
//    * strcpy (dst + strlen(src), src);
//    * return dst;
//    */
  
//   char *str=dst;
//   assert(dst!=NULL && src!=NULL);
//   while(*dst!='\0') {
//     dst++;
//   }
//   while((*dst++ = *src++) != '\0');
//   return str;
// }

// int strcmp(const char* s1, const char* s2) {
//   // size_t s1_size = strlen(s1);
//   // size_t s2_size = strlen(s2);
//   // size_t n = s1_size < s2_size ? s1_size : s2_size;
//   // return strncmp(s1, s2, n);

//   uint32_t i = 0;
//   for (i = 0; s1[i] == s2[i]; i++) if(s1[i] == '\0') return 0;

//   return ((signed char *)s1)[i] - ((signed char *)s2)[i];
// }

// int strncmp(const char* s1, const char* s2, size_t n) {
  
//   /* 虽然可以通过测试，但是性能太低了
//   // size_t s1_size = strlen(s1);
//   // size_t s2_size = strlen(s2);
  
//   // if (s1_size < n) n = s1_size;
//   // if (s2_size < n) n = s2_size;
  
//   // uint32_t i = 0;
//   // for (i = 0; i < n; i++) {
//   // 	int D_value = s1[i] - s2[i];
//   // 	if (D_value) return D_value;
//   // }
//   // return 0;
//   */
//   int D_value = 0;
//   size_t i    = 0;
  
//   for (i = 0; *s1 && *s2 && i < n && D_value != 0; i++) {
//     s1++; s2++;
//   	D_value = *s1 - *s2;
//   }

//   return D_value;
// }

// void* memset(void* v,int c,size_t n) {
//   const unsigned char uc = c; /* only cat the lowest c */
//   unsigned char *vu;
//   for (vu = v; 0 < n; ++vu, --n)
// 	  *vu = uc;
//   return v;
// }

// void* memcpy(void* out, const void* in, size_t n) {
//   const char *src = in;
//   char *dst = out;

//   while (n--) *dst++ = *src++;

//   return out;
// }

// int memcmp(const void* s1, const void* s2, size_t n){
//   if(!n) return 0;
//   while(--n && *(char *)s1 == *(char *)s2) {
// 	s1 = (char *)s1 + 1;
// 	s2 = (char *)s2 + 1;
//   }
//   return (*((unsigned char *)s1) - *((unsigned char *)s2));
// }

#endif

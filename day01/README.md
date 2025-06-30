# 📁 파일 디스크립터 및 엔디안 실습 예제 모음

---

## 📂 파일: `fdTest.c`

### 🔧 개요

이 프로그램은 리눅스/유닉스 시스템에서 **표준 입출력 스트림**(`stdin`, `stdout`, `stderr`)이  
각각 어떤 **파일 디스크립터(File Descriptor)** 번호를 가지는지 확인하는 간단한 예제입니다.

### 🧠 핵심 개념: 파일 디스크립터

| 스트림 | 설명                   | 번호 |
|--------|------------------------|------|
| stdin  | 표준 입력 (키보드 등)  | 0    |
| stdout | 표준 출력 (터미널 등)  | 1    |
| stderr | 표준 에러 출력         | 2    |

### 📝 코드

```c
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    int fd1, fd2, fd3;

    fd1 = fileno(stdin);
    fd2 = fileno(stdout);
    fd3 = fileno(stderr);

    printf("fd 1: %d\n", fd1);
    printf("fd 2: %d\n", fd2);
    printf("fd 3: %d\n", fd3);

    close(fd1);
    close(fd2);
    close(fd3);

    return 0;
}
```

### 💻 실행 예시

```bash
$ gcc fdTest.c -o fdTest
$ ./fdTest
fd 1: 0
fd 2: 1
fd 3: 2
```

---

## 📂 파일: `fdTest2.c`

### 🔧 개요

`open()` 시스템 호출로 텍스트 파일 `a.txt`를 생성하고, 해당 파일의 **파일 디스크립터 번호**를 출력하는 예제입니다.

### 📝 코드

```c
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int fd;
    fd = open("a.txt", O_CREAT | O_WRONLY);
    if(fd < 0){
        perror("could not open a.txt");
        exit(1);
    } else {
        printf("open success!!\n");
        printf("file descriptor: %d\n", fd);
    }

    close(fd);
    return 0;
}
```

### 📌 open() 플래그 설명

| 플래그        | 설명                           |
|---------------|--------------------------------|
| `O_RDONLY`    | 읽기 전용                       |
| `O_WRONLY`    | 쓰기 전용                       |
| `O_RDWR`      | 읽기 + 쓰기                     |
| `O_CREAT`     | 파일이 없으면 새로 생성          |
| `O_TRUNC`     | 기존 내용을 비우고 새로 작성     |
| `O_APPEND`    | 항상 파일 끝에 덧붙이기          |

### 💻 실행 예시

```bash
$ gcc fdTest2.c -o fdTest2
$ ./fdTest2
open success!!
file descriptor: 3
```

---

## 📂 파일: `fdTest3.c`

### 🔧 개요

이 프로그램은 `open()`으로 파일을 열고, `write()`를 이용해 `"Hello socket!!"` 문자열을 `hello.txt`에 기록합니다.

### 📝 코드

```c
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
    int fd;
    char str[] = "Hello socket!!";

    fd = open("hello.txt", O_CREAT | O_WRONLY, 0644);
    if(fd < 0) {
        perror("could not open hello.txt");
        exit(1);
    }

    if(write(fd, str, strlen(str) + 1) < 0) {
        perror("Failed to write to file");
        exit(1);
    }

    close(fd);
    return 0;
}
```

### 📌 권한 예시 (`0644`)

| 권한 값 | 의미                             |
|---------|----------------------------------|
| `6`     | 읽기 + 쓰기 (소유자)             |
| `4`     | 읽기 (그룹)                      |
| `4`     | 읽기 (기타 사용자)               |

### 💻 실행 예시

```bash
$ gcc fdTest3.c -o fdTest3
$ ./fdTest3
```

---

## 📂 파일: `fdTest4.c`

### 🔧 개요

이 프로그램은 `read()` 시스템 호출을 사용해 `hello.txt` 파일의 내용을 읽고 출력합니다.

### 📝 코드

```c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void main()
{
    int fd;
    char buf[20];
    ssize_t read_sz;

    fd = open("hello.txt", O_RDONLY);
    if(fd < 0) {
        perror("could not open");
        exit(1);
    }

    read_sz = read(fd, buf, sizeof(buf));
    if(read_sz < 0) {
        perror("Failed to read from file");
        exit(1);
    }

    printf("read from file: %s", buf);

    close(fd);
}
```

### 📌 read() 함수 설명

| 파라미터 | 설명                     |
|----------|--------------------------|
| `fd`     | 파일 디스크립터           |
| `buf`    | 데이터를 저장할 버퍼      |
| `count`  | 최대 읽을 바이트 수       |
| 반환값   | 읽은 바이트 수 (또는 -1)  |

### 💻 실행 예시

```bash
$ gcc fdTest4.c -o fdTest4
$ ./fdTest4
read from file: Hello socket!!
```

---

## 📂 파일: `order.c`

### 🔧 개요

메모리에 저장된 정수의 **바이트 순서**를 출력하여 **리틀 엔디안 구조**를 확인하는 예제입니다.

### 🧠 개념 정리: 엔디안(Endian)

| 종류            | 설명                                                 |
|------------------|------------------------------------------------------|
| 리틀 엔디안       | 가장 작은 바이트가 가장 낮은 주소에 저장됨 (Intel/AMD) |
| 빅 엔디안         | 가장 큰 바이트가 가장 낮은 주소에 저장됨 (네트워크 등) |

### 📝 코드

```c
#include <stdio.h>

void main()
{
    int n = 0x1234567;
    char *pn = (char*)&n;

    printf("1st: %p, %#x\n", &(pn[0]), pn[0]);
    printf("2st: %p, %#x\n", &(pn[1]), pn[1]);
    printf("3st: %p, %#x\n", &(pn[2]), pn[2]);
    printf("4st: %p, %#x\n", &(pn[3]), pn[3]);
}
```

### 💻 출력 예시 (리틀 엔디안 기준)

```bash
1st: 0x7ffeefbff57c, 0x67
2st: 0x7ffeefbff57d, 0x45
3st: 0x7ffeefbff57e, 0x23
4st: 0x7ffeefbff57f, 0x1
```

→ 메모리에는 `0x1234567`이 `67 45 23 01` 순서로 저장됨

---

## 📂 파일: `order1.c`

### 🔧 개요

`htons()` 및 `htonl()`을 사용해 **호스트 바이트 오더**(리틀 엔디안)를  
**네트워크 바이트 오더**(빅 엔디안)로 변환하는 예제입니다.

### 🧠 개념 정리

| 개념                  | 설명                                     |
|------------------------|------------------------------------------|
| 호스트 바이트 오더     | 현재 시스템의 바이트 순서 (대부분 리틀 엔디안) |
| 네트워크 바이트 오더   | 전송 시 사용하는 표준 (항상 빅 엔디안)        |

### 📌 변환 함수

| 함수     | 설명                        | 대상 자료형            |
|----------|-----------------------------|-------------------------|
| `htons()`| host to network short       | `unsigned short` (16비트) |
| `htonl()`| host to network long        | `unsigned int` (32비트)  |

### 📝 코드

```c
#include <stdio.h>
#include <arpa/inet.h>

void main()
{
    unsigned short port = 0x1234;
    unsigned int ip = 0x12345678;

    unsigned short network_port;
    unsigned int network_ip;

    network_port = htons(port);
    network_ip = htonl(ip);

    printf("ip: %x\n", ip);
    printf(" port: %#x\n", port);
    printf("conversion ip: %#x\n", network_ip);
    printf(" conversion port: %#x\n", network_port);
}
```

### 💻 실행 예시 (리틀 엔디안 기준)

```bash
ip: 12345678
 port: 0x1234
conversion ip: 0x78563412
 conversion port: 0x3412
```

→ 전송 시:
- 0x12345678 → `0x78 0x56 0x34 0x12`
- 0x1234 → `0x34 0x12`

---

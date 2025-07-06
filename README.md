# Linux 기반 TCP/IP 소켓 프로그래밍
***
# VMware + Ubuntu 개발 환경 구성 가이드

## 1. VMware Workstation 설치

- VMware 공식 사이트 접속  
  👉 [https://www.vmware.com/products/desktop-hypervisor/workstation-and-fusion](https://www.vmware.com/products/desktop-hypervisor/workstation-and-fusion)

- `Download Now` 클릭
- **Broadcom 회원가입 및 로그인 필요**
- `VMware Workstation Pro for Windows (v17.6.3)` 다운로드 및 설치

## 2. Ubuntu 설치

- Ubuntu LTS 버전 다운로드  
  👉 [https://ubuntu.com/download/desktop](https://ubuntu.com/download/desktop)

- ISO 파일 다운로드 후 VMware에서 가상 머신 생성
- Ubuntu 설치 진행 후 재시작
- 사용자 계정 생성 및 비밀번호 설정

## 3. PuTTY 설치 및 Ubuntu 접속

- PuTTY 다운로드  
  👉 [https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html)

- 설치 후 실행
- Ubuntu의 IP 주소 확인 후 SSH 접속

## 4. 기본 네트워크 개념

| 용어         | 설명                                                                 |
|--------------|----------------------------------------------------------------------|
| **IP 주소**   | 네트워크에서 각 기기를 구분하는 주소 (예: 인터넷 상의 집 주소)           |
| **Port 번호**| 하나의 IP 내에서 실행 중인 프로그램을 구분하는 번호 (예: 아파트의 호수)    |
| **HTTP**     | 웹 브라우저와 서버 간 데이터 통신 시 사용되는 프로토콜                    |
| **Socket**   | 통신의 시작점이자 끝점. 일종의 '전화기' 역할                             |

### 소켓 통신 절차 (전화기 비유)

| 단계              | 설명                | 비유           |
|-------------------|---------------------|----------------|
| 소켓 생성         | socket() 호출       | 전화기 만들기   |
| 주소 할당         | bind() 호출         | 전화번호 설정   |
| 연결 요청 대기    | listen() 호출       | 전화 받을 준비 |
| 연결 수락         | accept() 호출       | 전화 받기       |
| 데이터 송수신     | send()/recv()       | 전화로 대화     |
| 연결 종료         | close() 호출        | 전화 끊기       |

## 5. Ubuntu 기본 명령어

```bash
# 시스템 정보 확인
uname -a

# 디렉토리 이동 및 탐색
cd [폴더명]       # 폴더로 이동
cd ..             # 상위 디렉토리로 이동
ls                # 현재 폴더 내 목록 출력
pwd               # 현재 경로 출력

# 파일/폴더 작업
mkdir [폴더명]    # 폴더 생성
rm -fr [대상]     # 파일/폴더 강제 삭제
touch [파일명]    # 빈 파일 생성
cp [원본] [복사본]# 파일 복사
cat [파일명]      # 파일 내용 출력
clear             # 터미널 화면 정리

# nano 설정 파일 수정 및 gcc 설치
sudo nano /etc/nanorc
sudo apt install gcc

# 실행 명령
./[파일명]        # 현재 폴더에서 실행

```
## 6. IP 주소 클래스 체계

| 클래스 | 구성 (Network + Host) | 주소 범위                         | 용도               |
|--------|------------------------|-----------------------------------|--------------------|
| A      | 1바이트 + 3바이트      | 0.0.0.0 ~ 127.255.255.255         | 대규모 네트워크    |
| B      | 2바이트 + 2바이트      | 128.0.0.0 ~ 191.255.255.255       | 중간 규모 네트워크 |
| C      | 3바이트 + 1바이트      | 192.0.0.0 ~ 223.255.255.255       | 소규모 네트워크    |
| D      | Multicast              | 224.0.0.0 ~ 239.255.255.255       | 멀티캐스트 용도    |
| E      | Reserved (실험용)      | 240.0.0.0 ~ 255.255.255.255       | 예약 주소          |


## 7. 프로토콜 체계 및 소켓 종류

### 7-1. 프로토콜 체계

| 프로토콜 패밀리 | 설명                       |
|------------------|----------------------------|
| PF_INET          | IPv4 인터넷 프로토콜 체계  |
| PF_INET6         | IPv6 인터넷 프로토콜 체계  |

### 7-2. 전송 방식

| 전송 방식 | 특징                               | 소켓 타입     |
|------------|------------------------------------|----------------|
| TCP         | 연결 지향, 신뢰성 있음             | SOCK_STREAM    |
| UDP         | 비연결 지향, 빠르지만 신뢰성 없음 | SOCK_DGRAM     |

***

# 📁 파일 디스크립터 및 엔디안 실습 예제 모음

---

## 📂 파일: `fdTest.c`

### 🔧 개요

이 프로그램은 리눅스/유닉스 시스템에서 **표준 입출력 스트림**(`stdin`, `stdout`, `stderr`)이  
각각 어떤 **파일 디스크립터(File Descriptor)** 번호를 가지는지 확인하는 간단한 예제

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

`open()` 시스템 호출로 텍스트 파일 `a.txt`를 생성하고, 해당 파일의 **파일 디스크립터 번호**를 출력하는 예제

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

이 프로그램은 `open()`으로 파일을 열고, `write()`를 이용해 `"Hello socket!!"` 문자열을 `hello.txt`에 기록

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

이 프로그램은 `read()` 시스템 호출을 사용해 `hello.txt` 파일의 내용을 읽고 출력

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

메모리에 저장된 정수의 **바이트 순서**를 출력하여 **리틀 엔디안 구조**를 확인하는 예제

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
**네트워크 바이트 오더**(빅 엔디안)로 변환하는 예제

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

***
# 📘 SOCKET 프로그래밍 전체 정리

## 📑 Slide 1: TCP/IP socket

- TCP/IP 소켓은 네트워크 상의 두 프로세스 간 통신을 위한 소프트웨어 인터페이스
- 소켓은 IP 주소와 포트 번호로 식별
- TCP는 연결 지향형, 신뢰성 있는 전송을 제공

---

## 📑 Slide 2: socket 통신

- 소켓 통신은 TCP 또는 UDP 기반으로 수행되는 실시간 양방향 통신 기술
- **클라이언트(Client)**: 데이터를 요청하는 측
- **서버(Server)**: 데이터를 제공하는 측
- 네트워크(Network) = Net(그물망) + Work(작업)
- 소켓은 **응용 계층(Application Layer)**과 **전송 계층(Transport Layer)** 사이의 인터페이스 역할 수행

---

## 📑 Slide 3: socket 개요

- 소켓은 떨어져 있는 두 호스트(컴퓨터) 간에 데이터를 주고받기 위한 **논리적 통신 채널**
- 운영체제에서 제공하는 API 형태로 구현
- **IP는 비신뢰성 / 비연결성** 프로토콜이기 때문에 TCP 같은 신뢰성 계층이 필요

---

## 📑 Slide 4: Protocol이란?

- 프로토콜(Protocol): 데이터 송수신 시 **정해진 통신 규칙**
- 예시:
  - **IP**: 목적지까지 패킷을 전달만 함 (비신뢰적)
  - **TCP**: 수신 확인, 순서 보장, 재전송 등 신뢰성 보장
  - **UDP**: 빠르지만 신뢰성 없음 (예: 실시간 스트리밍)
  - **HTTP**: 웹에서 클라이언트-서버 간 데이터 통신

---

## 📑 Slide 5: 서버 함수 흐름

| 함수 | 역할 | 전화기 비유 |
|------|------|--------------|
| `socket()` | 소켓 생성 | 전화기 구입 |
| `bind()` | 소켓에 IP/포트 지정 | 전화번호 등록 |
| `listen()` | 연결 요청 대기 상태 진입 | 전화 개통 |
| `accept()` | 클라이언트 연결 수락 | 전화 받기 |
| `read()` / `write()` | 데이터 송수신 | 대화 진행 |
| `close()` | 연결 종료 | 통화 종료 |

🧠 서버는 다수의 클라이언트 요청을 수락하기 위해 `listen()` 상태에서 `accept()`를 반복 수행

---

## 📑 Slide 6: 클라이언트 함수 흐름

| 함수 | 역할 | 전화기 비유 |
|------|------|--------------|
| `socket()` | 소켓 생성 | 전화기 구입 |
| `connect()` | 서버에 연결 요청 | 전화 걸기 |
| `read()` / `write()` | 데이터 송수신 | 대화 진행 |
| `close()` | 연결 종료 | 통화 종료 |

📌 클라이언트는 서버 IP와 포트 번호를 알고 있어야 `connect()` 가능

---

## 📑 Slide 7: main()의 인자

```c
int main(int argc, char *argv[])
```

- `argc`: 인자의 개수
- `argv`: 인자의 문자열 배열
  - `argv[0]`: 실행 파일명
  - `argv[1]~argv[n]`: 추가 인자

🧪 예시 실행: `./server 9000`  
→ `argc = 2`, `argv[0] = "./server"`, `argv[1] = "9000"`

---

## 📑 Slide 8: main 인자 테스트 예시

```c
int main(int argc, char *argv[]) {
    for (int i = 0; i < argc; i++) {
        printf("argv[%d] : %s\n", i, argv[i]);
    }
}
```

🧪 출력 예시:
```
argv[0] : ./a.out
argv[1] : hello
argv[2] : world
```

---

## 📑 Slide 9: IP 주소 구조체 (IPv4)

```c
struct sockaddr_in {
    sa_family_t    sin_family;  // 주소 체계 (AF_INET)
    in_port_t      sin_port;    // 포트 번호 (16bit)
    struct in_addr sin_addr;    // IP 주소
    char           sin_zero[8]; // 구조체 크기 맞춤용 padding
};
```

- 소켓 주소 설정 시 사용하는 구조체
- IPv4 환경에서 `bind()`, `connect()`, `accept()` 등에 사용됨

---

## 📑 Slide 10: 주소 체계 설명

- `sin_family`: `AF_INET` (IPv4), `AF_INET6` (IPv6)
- `sin_port`: 16비트 포트번호 (`htons()`로 big-endian 변환 필요)
- `sin_addr`: IP 주소 (`inet_addr()` 또는 `inet_pton()`으로 설정)
- `sin_zero`: 0으로 채우는 패딩 영역 (사용 안함)

---

## 📑 Slide 11: 바이트 순서 변환 함수

| 함수 | 역할 | 사용 위치 |
|------|------|------------|
| `htons()` | short → network | 포트 번호 설정 |
| `htonl()` | long → network | IP 주소 설정 |
| `ntohs()` | network → short | 수신된 포트 해석 |
| `ntohl()` | network → long | 수신된 IP 해석 |

📌 네트워크 표준은 Big Endian이므로 변환 필수

---

## 📑 Slide 12: 바이트 순서 예시

- **Little Endian**: 하위 바이트 먼저 (x86 아키텍처)
- **Big Endian**: 상위 바이트 먼저 (네트워크)
- IP/Port 정보를 전달할 땐 반드시 `htons()`, `htonl()` 사용

🧪 예: `htons(0x1234)` → `0x3412`로 저장됨

---

## 📑 Slide 13: TCP vs UDP

| 항목 | TCP (Transmission Control Protocol) | UDP (User Datagram Protocol) |
|------|-------------------------------|---------------------------|
| 연결 방식 | 연결 지향 (3-way handshake) | 비연결 지향 |
| 신뢰성 | 높음 (재전송, 순서보장, 오류검사) | 낮음 |
| 속도 | 느림 | 빠름 |
| 순서 보장 | O | X |
| 헤더 크기 | 20바이트 이상 | 8바이트 |
| 용도 예시 | 웹, 이메일, 파일전송(FTP) | 스트리밍, DNS, 게임 |

---

## 📑 Slide 14: socket()

```c
int socket(int domain, int type, int protocol);
```

- `domain`: 주소 체계 (`AF_INET`, `AF_INET6`)
- `type`: `SOCK_STREAM`(TCP), `SOCK_DGRAM`(UDP)
- `protocol`: 보통 0 (자동 선택됨)

🧪 예:
```c
int sock = socket(AF_INET, SOCK_STREAM, 0);  // TCP 소켓 생성
```

---

## 📑 Slide 15: bind()

```c
int bind(int sockfd, struct sockaddr *addr, socklen_t addrlen);
```

- 소켓에 IP주소 및 포트 번호를 할당
- 반드시 `struct sockaddr_in`을 `struct sockaddr*`로 형변환

🧪 예:
```c
struct sockaddr_in serv_addr;
serv_addr.sin_family = AF_INET;
serv_addr.sin_port = htons(9000);
serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
```

📌 **서버 측에서 필수** 호출이며, 클라이언트는 생략 가능 (시스템이 자동 지정)

---

## 📑 Slide 16: listen()

```c
int listen(int sockfd, int backlog);
```

- 클라이언트 연결 요청을 **수신 대기 상태**로 설정
- `sockfd`: socket() + bind()로 생성한 서버 소켓
- `backlog`: 동시에 수용 가능한 최대 대기 클라이언트 수  
  (커널에서 큐로 관리되며 초과 시 연결 거절됨)

🧠 **주의**: `listen()` 호출 없이는 서버가 `accept()` 호출해도 연결되지 않음

```c
listen(sockfd, 5);  // 최대 5개까지 연결 요청 대기
```

---

## 📑 Slide 17: accept()

```c
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```

- 연결 요청이 들어오면 이를 **수락하고** 새 소켓을 반환
- `sockfd`: 서버가 listen 중인 소켓
- `addr`: 연결한 클라이언트의 주소 정보를 담음
- `addrlen`: addr 구조체 크기 (입력값 겸 출력값)

📌 **반환값**은 실제 통신에 사용할 **클라이언트 전용 소켓 fd**

```c
int clnt_sock;
struct sockaddr_in clnt_addr;
socklen_t clnt_addr_size = sizeof(clnt_addr);

clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
```

---

## 📑 Slide 18: connect()

```c
int connect(int sockfd, struct sockaddr *addr, socklen_t addrlen);
```

- 클라이언트가 **서버에 연결 요청**을 보내는 함수
- 서버는 반드시 `listen()` 상태여야 함
- TCP의 경우 3-way handshake 수행

```c
connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
```

💡 연결 실패 시 -1 반환 → `perror()` 또는 `strerror(errno)`로 확인

---

## 📑 Slide 19: read() / write()

```c
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
```

- 소켓에서 **데이터 수신/송신** (TCP: 바이트 스트림 기반)
- `fd`: `accept()` 또는 `connect()`로 얻은 소켓
- `buf`: 버퍼 포인터
- `count`: 읽거나 쓸 최대 바이트 수
- **반환값**: 실제 전송된 바이트 수, 오류 시 -1

```c
write(sock, "Hello", 5);
read(sock, buffer, 1024);
```

---

## 📑 Slide 20: close()

```c
int close(int fd);
```

- 열린 소켓을 닫고 연결을 종료
- TCP의 경우 FIN 패킷 전송 → 정상 연결 종료

📌 리눅스 기준: `shutdown()` 사용 시 송/수신 방향만 종료 가능  
📌 반드시 마지막에 `close()`로 리소스 해제 필요

---

## 📑 Slide 21: fork()

```c
pid_t fork(void);
```

- 새로운 프로세스를 생성 (자식 프로세스)
- 자식: 0 리턴 / 부모: 자식 PID 리턴 / 실패: -1 리턴
- 클라이언트마다 `fork()`로 프로세스를 분리하면 **동시 처리 가능**

---

## 📑 Slide 22: fork() 예시

```c
pid_t pid = fork();

if (pid == 0) {
    // 자식 프로세스: 클라이언트 요청 처리
} else if (pid > 0) {
    // 부모 프로세스: 다른 클라이언트 대기
} else {
    perror("fork failed");
}
```

📌 소켓 서버에서 멀티 클라이언트 처리 시 널리 사용됨

---

## 📑 Slide 23: zombie 프로세스

- 자식 프로세스가 종료되었으나, 부모가 `wait()` 안 하면 **좀비(zombie)** 상태가 됨
- `ps` 명령어로 확인 가능 (stat: Z)

📌 해결 방법
- 부모가 `wait()` 또는 `waitpid()` 호출
- 또는 `SIGCHLD` 시그널 핸들러 등록

---

## 📑 Slide 24: wait()

```c
int status;
pid_t pid = wait(&status);
```

- 종료된 **자식 프로세스의 상태 정보 수거**
- 여러 자식 프로세스가 있다면, 아무거나 먼저 종료된 것 수거
- `status` 통해 종료 코드 확인 가능 (`WEXITSTATUS(status)`)

---

## 📑 Slide 25: waitpid()

```c
pid_t waitpid(pid_t pid, int *status, int options);
```

- 특정 자식 프로세스의 종료를 기다림
- `pid`: -1이면 모든 자식 / 양수면 특정 PID
- `options`: `WNOHANG` 사용 시 블로킹 없이 검사

```c
while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
    printf("Child %d exited\n", pid);
}
```

---

## 📑 Slide 26: signal()

- 좀비 프로세스를 방지하기 위해 `SIGCHLD` 시그널에 핸들러 등록 가능

```c
void handler(int sig) {
    pid_t pid;
    while ((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
        printf("Child %d terminated\n", pid);
    }
}
signal(SIGCHLD, handler);
```

📌 여러 자식이 동시에 종료될 수 있으므로 루프 사용

---

## 📑 Slide 27: gethostbyname()

```c
struct hostent *gethostbyname(const char *name);
```

- 도메인명을 **IP 주소로 변환** (IPv4 전용)
- 반환값 구조체는 전역 메모리 → 복사 필요

```c
struct hostent *host = gethostbyname("www.naver.com");
```

📌 최신 환경에서는 `getaddrinfo()` 사용 권장

---

## 📑 Slide 28: gethostbyaddr()

```c
struct hostent *gethostbyaddr(const void *addr, socklen_t len, int type);
```

- IP 주소를 **호스트명으로 변환**
- `addr`: struct in_addr 등
- `type`: `AF_INET` or `AF_INET6`

📌 반대로 IP → 도메인 조회 가능 (역방향 DNS)

---

## 📑 Slide 29: gethostent 구조체

```c
struct hostent {
    char *h_name;      // 공식 호스트명
    char **h_aliases;  // 별칭 리스트
    int h_addrtype;    // 주소 타입 (AF_INET 등)
    int h_length;      // 주소 길이
    char **h_addr_list;// IP 주소 리스트
};
```

- 하나의 호스트가 여러 개의 IP를 가질 수 있음
- h_addr_list는 NULL로 끝남

🧪 사용 예:
```c
inet_ntoa(*(struct in_addr*)host->h_addr_list[0])
```

---

## 📑 Slide 30: setsockopt()

```c
int setsockopt(int sockfd, int level, int optname,
               const void *optval, socklen_t optlen);
```

- 소켓에 다양한 동작 옵션을 설정할 수 있음

| 파라미터 | 설명 |
|----------|------|
| `sockfd` | 대상 소켓 디스크립터 |
| `level` | 설정할 계층 (`SOL_SOCKET`, `IPPROTO_TCP` 등) |
| `optname` | 옵션 이름 (`SO_REUSEADDR`, `SO_KEEPALIVE` 등) |
| `optval` | 설정할 값의 포인터 |
| `optlen` | optval의 크기 |

📌 자주 쓰는 옵션:
- `SO_REUSEADDR`: TIME_WAIT 회피
- `SO_BROADCAST`: 브로드캐스트 허용
- `SO_RCVBUF`, `SO_SNDBUF`: 버퍼 크기 조정

🧪 예:
```c
int opt = 1;
setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
```

---

## 📑 Slide 31: setsockopt() 주요 옵션 요약

`setsockopt()` 함수에서 자주 사용되는 옵션

| 옵션 이름 | 설명 | 사용 예 |
|-----------|------|---------|
| `SO_REUSEADDR` | 포트 즉시 재사용 가능 (TIME_WAIT 회피) | 서버 재시작 시 필수 |
| `SO_BROADCAST` | 브로드캐스트 메시지 전송 허용 | UDP 브로드캐스트 |
| `SO_RCVBUF` | 수신 버퍼 크기 설정 | 버퍼 오버플로 방지 |
| `SO_SNDBUF` | 송신 버퍼 크기 설정 | 대용량 데이터 전송 시 |

📌 옵션은 `setsockopt()`로 설정, `getsockopt()`로 확인 가능

---

## 📑 Slide 32: getsockopt()

```c
int getsockopt(int sockfd, int level, int optname,
               void *optval, socklen_t *optlen);
```

- 소켓에 설정된 **옵션값을 조회**
- `optval`: 출력 버퍼
- `optlen`: 버퍼 크기 (in/out)

🧪 예:
```c
int optval;
socklen_t optlen = sizeof(optval);
getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &optval, &optlen);
```

---

## 📑 Slide 33: 소켓 옵션 설정 예제

```c
int optval = 1;
setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
```

- `SOL_SOCKET`: 소켓 레벨
- `SO_REUSEADDR`: 서버 재시작 시 TIME_WAIT 포트 사용 가능
- 반드시 `bind()` 이전에 호출해야 함

---

## 📑 Slide 34: 브로드캐스트 주소란?

- IPv4의 브로드캐스트 주소: `255.255.255.255`  
  또는 `192.168.1.255`와 같은 서브넷 브로드캐스트

📌 **브로드캐스트 특징**
- 한 번의 송신으로 같은 서브넷 내 모든 호스트에게 전송
- UDP 프로토콜로만 지원
- 기본적으로 차단되어 있으므로 `SO_BROADCAST` 옵션 필요

---

## 📑 Slide 35: 브로드캐스트 사용 예

```c
int opt = 1;
setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));
```

- 설정 후 `sendto()` 함수로 브로드캐스트 가능

📌 주의: 브로드캐스트 남용 시 네트워크 혼잡 유발

---

## 📑 Slide 36: SO_RCVBUF / SO_SNDBUF

- 기본적으로 커널이 설정한 버퍼 크기를 사용하지만, 상황에 따라 조정 가능

```c
int size = 8192;
setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size));
```

📌 버퍼 크기를 너무 작게 설정하면 `recv()`/`send()` 성능 저하

---

## 📑 Slide 37: getsockname()

```c
int getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```

- 소켓에 바인딩된 **로컬 IP 주소 및 포트번호** 확인
- 주로 클라이언트에서 임시로 할당된 IP/PORT 확인용으로 사용

---

## 📑 Slide 38: getpeername()

```c
int getpeername(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```

- 소켓에 연결된 **상대방(피어)의 주소 정보** 확인
- 주로 서버에서 접속한 클라이언트의 IP/PORT를 출력할 때 사용

---

## 📑 Slide 39: shutdown()

```c
int shutdown(int sockfd, int how);
```

- 소켓의 **송신/수신**을 선택적으로 종료
- `how` 파라미터:

| 값 | 의미 |
|----|------|
| 0  | 수신 종료 (`SHUT_RD`) |
| 1  | 송신 종료 (`SHUT_WR`) |
| 2  | 송수신 모두 종료 (`SHUT_RDWR`) |

📌 `close()`와 달리 방향 제어 가능

---

## 📑 Slide 40: inet_aton()

```c
int inet_aton(const char *cp, struct in_addr *inp);
```

- 문자열 IP (ex: `"127.0.0.1"`) → `struct in_addr`로 변환
- 성공 시 1, 실패 시 0 반환

---

## 📑 Slide 41: inet_ntoa()

```c
char *inet_ntoa(struct in_addr in);
```

- `struct in_addr` → 문자열 IP로 변환 (ex: `"192.168.0.1"`)
- 반환 문자열은 **정적(static)** → 다중 호출 시 주의

---

## 📑 Slide 42: inet_addr()

```c
in_addr_t inet_addr(const char *cp);
```

- 문자열 IP → 네트워크 바이트 순서 long 값으로 변환
- 오류 시 `INADDR_NONE` 반환
- **비표준 함수**로 현재는 잘 사용하지 않음

---

## 📑 Slide 43: inet_ntop()

```c
const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
```

- 바이너리 IP (IPv4/IPv6) → 문자열로 변환 (IPv6까지 지원)
- `af`: `AF_INET` 또는 `AF_INET6`

🧪 예:
```c
inet_ntop(AF_INET, &in_addr, ip_str, sizeof(ip_str));
```

---

## 📑 Slide 44: inet_pton()

```c
int inet_pton(int af, const char *src, void *dst);
```

- 문자열 IP → 바이너리 IP로 변환
- `af`: 주소 체계 (`AF_INET`, `AF_INET6`)

🧪 예:
```c
inet_pton(AF_INET, "127.0.0.1", &in_addr);
```

📌 IPv6 지원하려면 `inet_ntop`, `inet_pton` 필수

---

## 📑 Slide 45: IP 주소 변환 요약

| 함수 이름 | 변환 방향 | 설명 |
|-----------|------------|------|
| `inet_aton()` | str → addr 구조체 | 문자열 → 구조체 |
| `inet_ntoa()` | addr 구조체 → str | 구조체 → 문자열 |
| `inet_pton()` | str → bin | 문자열 → 네트워크 바이트 |
| `inet_ntop()` | bin → str | 네트워크 바이트 → 문자열 |
| `inet_addr()` | str → long | (비표준, 사용 자제) |

✅ IPv6 호환을 고려하면 `inet_pton()`과 `inet_ntop()` 사용 권장

---

## 📑 Slide 46: TCP 클라이언트/서버 기본 구조 요약

**TCP 서버 구성 순서**
1. `socket()` – 소켓 생성
2. `bind()` – 주소(IP/PORT) 할당
3. `listen()` – 연결 요청 대기
4. `accept()` – 클라이언트 연결 수락
5. `read()/write()` – 데이터 송수신
6. `close()` – 소켓 종료

**TCP 클라이언트 구성 순서**
1. `socket()` – 소켓 생성
2. `connect()` – 서버에 연결 요청
3. `read()/write()` – 데이터 송수신
4. `close()` – 소켓 종료

---

## 📑 Slide 47: 클라이언트/서버 실습 흐름

- 클라이언트: 사용자 입력 → `write()`로 서버 전송
- 서버: `read()`로 메시지 수신 → 가공 후 `write()`로 응답

📌 여러 클라이언트를 처리할 경우 서버는 `fork()` 또는 `multi-thread` 방식 사용

---

## 📑 Slide 48: 서버 실습 코드 예시 (요약)

```c
// 1. 소켓 생성
int serv_sock = socket(AF_INET, SOCK_STREAM, 0);

// 2. 주소 구조체 설정
struct sockaddr_in serv_addr;
serv_addr.sin_family = AF_INET;
serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
serv_addr.sin_port = htons(atoi(argv[1]));

// 3. 바인딩 및 대기
bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
listen(serv_sock, 5);

// 4. 연결 수락
int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_size);

// 5. 통신
read(clnt_sock, buffer, BUF_SIZE);
write(clnt_sock, buffer, strlen(buffer));
```

---

## 📑 Slide 49: 클라이언트 실습 코드 예시 (요약)

```c
int sock = socket(PF_INET, SOCK_STREAM, 0);

struct sockaddr_in serv_addr;
serv_addr.sin_family = AF_INET;
serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
serv_addr.sin_port = htons(atoi(argv[2]));

connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

// 사용자 입력 전송
fgets(message, BUF_SIZE, stdin);
write(sock, message, strlen(message));

// 서버 응답 수신
read(sock, message, BUF_SIZE);
```

---

## 📑 Slide 50: 파일 전송 구현 개요

- 클라이언트가 요청한 파일을 서버가 읽어 전송
- 서버: 파일 open → `read()` → `write()`
- 클라이언트: `read()`로 수신 → local 파일로 저장

📌 구현 시 파일 종료 체크, 에러 체크 중요

---

## 📑 Slide 51: 파일 서버 구조 요약

1. 파일 열기 (`fopen` 또는 `open`)
2. while 루프로 버퍼 단위 읽기
3. `write()`를 통해 클라이언트에 전송
4. 전송 완료 후 파일/소켓 닫기

```c
while ((read_len = fread(buffer, 1, BUF_SIZE, fp)) > 0)
    write(clnt_sock, buffer, read_len);
```

---

## 📑 Slide 52: 파일 클라이언트 구조 요약

1. 서버 연결 요청
2. `read()` 루프로 파일 내용 수신
3. local 파일에 `fwrite()`로 저장
4. 파일 종료 후 닫기

```c
while ((read_len = read(sock, buffer, BUF_SIZE)) > 0)
    fwrite(buffer, 1, read_len, fp);
```

---

## 📑 Slide 53: 멀티 프로세스 서버

- `fork()`를 통해 클라이언트 접속 시마다 자식 프로세스 생성
- 자식은 해당 클라이언트 통신 담당
- 부모는 계속 `accept()` 대기

📌 종료된 자식은 반드시 `wait()` 또는 `SIGCHLD`로 정리

---

## 📑 Slide 54: 좀비 프로세스 방지

```c
void handler(int sig) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}
signal(SIGCHLD, handler);
```

- 자식 종료 시 자동으로 회수하여 좀비 방지

📌 `WNOHANG` 사용 시 블로킹 없이 수거 가능

---

## 📑 Slide 55: 소켓통신 실습 응용

- 실시간 채팅 프로그램
- 간단한 HTTP 서버 구현
- IoT 장치 간 통신
- 파일 서버 / 클라이언트

📌 `select()`, `poll()`, `epoll()` 활용 시 고성능 서버 구현 가능

---

## 📑 Slide 56: 마무리 및 참고

✅ 주요 함수 요약:
- TCP: `socket`, `bind`, `listen`, `accept`, `connect`, `read`, `write`, `close`
- 주소변환: `inet_pton`, `inet_ntop`
- 프로세스: `fork`, `wait`, `signal`

📚 참고:
- Beej's Guide to Network Programming
- man page: `man 2 socket`, `man 7 ip`, `man 2 fork`, `man 2 accept`

---

# ✅ 전체 마무리 요약

| 구분 | 함수 |
|------|------|
| 소켓 생성 | `socket()` |
| 주소 바인딩 | `bind()` |
| 연결 대기 | `listen()` |
| 연결 수락 | `accept()` |
| 연결 요청 | `connect()` |
| 데이터 송수신 | `read()` / `write()` |
| 종료 | `close()`, `shutdown()` |
| 자식 생성 | `fork()`, `wait()`, `signal()` |
| 주소 변환 | `inet_pton()`, `inet_ntop()` |
| 호스트명 변환 | `gethostbyname()`, `gethostbyaddr()` |
| 옵션 설정 | `setsockopt()`, `getsockopt()` |

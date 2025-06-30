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

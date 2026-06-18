# Mini Student Shell (사용자 권한 분리형 학생 정보 관리 시스템)

C 언어를 기반으로 구조체(Structure)와 동적 할당 기반의 단방향 연결 리스트(Singly Linked List)를 활용하여 학생 정보를 관리하는 인메모리 데이터베이스 및 명령행 인터페이스(CLI Shell) 시스템입니다. 전처리기 플래그와 Makefile을 통해 관리자(Admin) 모드와 사용자(Client) 모드로 분리하여 빌드할 수 있도록 설계되었습니다.

## 1. 프로젝트 구조 및 모듈 역할

본 프로그램은 관심사 분리(Separation of Concerns) 원칙에 따라 기능별로 완벽히 모듈화되어 있습니다.

* Makefile : 다중 타겟(admin_shell, client_shell) 빌드 스크립트
* main.c : 프로그램 진입점 및 REPL 쉘 루프 제어
* command.c / .h : 명령어 테이블 정의 및 각 명령어 핸들러 구현
* student.c / .h : Student 구조체 정의 및 연결 리스트 CRUD 로직
* file_io.c / .h : CSV 파일 적재(Load) 및 영속화(Save) 레이어

### 모듈별 상세 역할

* main.c: 실행 인자(argc, argv)를 파싱하여 예외 처리를 수행하고, 디스크로부터 데이터를 마운트한 뒤 REPL(Read-Eval-Print Loop) 기반의 콘솔 인터페이스를 구동합니다. -f 옵션을 통한 배치 파일 스크립트 실행 기능도 포함합니다.
* command.c / .h: 입력된 문자열을 토큰화(strtok)하여 파싱하고, 현재 빌드 모드(Admin/Client)에 부합하는 명령어를 라우팅합니다. 매크로 상수에 따라 명령어 배열을 다르게 빌드하여 물리적인 권한 보안을 달성합니다.
* student.c / .h: 학생 데이터 모델을 정의하고, 학번 중복 검사(isDuplicate), 삽입(add), 투 포인터를 활용한 삭제(deleteList), 성적 수정(updateList), 검색(findList), 통계 계산(calculateList), 메모리 해제(freeList) 알고리즘을 담고 있습니다.
* file_io.c / .h: 표준 파일 입출력(fopen, fgets, fprintf)을 담당합니다. 첫 줄의 CSV 헤더를 처리하고, 콤마(,) 구분자를 기반으로 텍스트 데이터를 정수 및 문자열로 변환하여 메모리에 적재하거나 파일에 기록합니다.

---

## 2. 자료구조 설계

### Student 구조체

학생 개별 레코드의 동적 생성 및 연결을 위해 다음과 같은 자기 참조 구조체를 정의했습니다.

typedef struct Student {
int id;               // 학번 (고유 Key)
char name[32];        // 이름 (최대 31자 및 널 문자)
int score;            // 성적 (0 ~ 100 범위 보장)
struct Student* next; // 다음 노드를 가리키는 포인터
} Student;

### 메모리 관리 및 알고리즘 특징

* 삽입(O(N)): 데이터의 일관성 및 정형 유지를 위해 새 노드를 항상 리스트의 맨 끝(Tail)에 순차적으로 추가합니다.
* 삭제(O(N)): 단방향 리스트 구조에서 헤드 노드가 삭제되는 예외 상황과 일반 노드가 삭제되는 상황을 모두 안전하게 처리하기 위해 Student head 이중 포인터와 prev, now 투 포인터(Two-pointer) 추적 기법을 적용했습니다.
* 자원 해제: 프로그램 종료 시점이나 데이터를 다시 불러오는 reload 명령어 수행 시, 메모리 누수(Memory Leak)를 차단하기 위해 모든 노드를 순회하며 안전하게 free()를 수행합니다.

---

## 3. 기능 및 명령어 명세

컴파일 시점에 결정되는 조건부 플래그에 따라 쉘의 실행 권한 및 사용 가능한 명령어가 물리적으로 제한됩니다.

* help
* 사용 형식: help
* 권한: 공통
* 설명: 사용 가능한 명령어 목록 및 설명 출력


* list
* 사용 형식: list
* 권한: 공통
* 설명: 현재 리스트 내의 모든 학생 정보 출력 (데이터가 없을 시 공백 메시지 처리)


* find
* 사용 형식: find 
* 권한: 공통
* 설명: 학번 기준 특정 학생 검색 및 출력 (학번 미존재 시 STUDENT_NOT_FOUND 에러)


* stats
* 사용 형식: stats
* 권한: 공통
* 설명: 전체 학생 수, 성적 평균, 최고/최저점 출력 (데이터가 없을 시 유효성 체크 및 0점 나누기 방지)


* reload
* 사용 형식: reload
* 권한: 공통
* 설명: 메모리를 초기화하고 CSV 파일 재적재 (파일 오픈 실패 및 추가 인자 주입 차단)


* clear
* 사용 형식: clear
* 권한: 공통
* 설명: ANSI 이스케이프 시퀀스를 이용한 화면 초기화


* exit
* 사용 형식: exit
* 권한: 공통
* 설명: 힙 메모리를 모두 해제하고 쉘 안전 종료


* add
* 사용 형식: add   
* 권한: Admin
* 설명: 새로운 학생 정보 추가 (인자 부족/초과, 학번 중복, 성적 범위(0~100) 이탈 검증)


* update
* 사용 형식: update  
* 권한: Admin
* 설명: 기존 학생의 성적 수정 (대상 학번 부재, 성적 포맷 오류(strtol), 범위 검증)


* delete
* 사용 형식: delete 
* 권한: Admin
* 설명: 학번 기준 특정 학생 정보 삭제 (대상 학번 부재 및 명령어 인자 유효성 검증)


* save
* 사용 형식: save
* 권한: Admin
* 설명: 메모리의 데이터를 CSV 파일로 반영 (디스크 쓰기 권한 오류 및 파일 열기 실패 대응)



※ Client Mode로 빌드 시 관리자 전용 명령어(add, update, delete, save)는 명령어 테이블 배열 자체에서 배제되므로 권한이 원천 차단됩니다.

---

## 4. 빌드 및 실행 방법

이 프로젝트는 Makefile 내에 매크로 상수 주입 방식을 사용하여 다중 바이너리를 빌드합니다.

### 컴파일 환경 요구사항

* GCC 컴파일러
* GNU Make 유틸리티

### 빌드 명령어

* make all : Admin 및 Client 바이너리를 모두 빌드
* make admin : 관리자 모드 전용 쉘 빌드 (-DADMIN_MODE 주입)
* make client : 사용자 모드 전용 쉘 빌드 (-DCLIENT_MODE 주입)
* make clean : 생성된 실행 파일 및 디버그 파일 삭제

### 실행 방법

프로그램 구동 시 첫 번째 인자로 연동할 CSV 파일명을 필수로 지정해야 합니다. 인자가 없거나 규격에 맞지 않으면 사용법(Usage)을 출력합니다.

1. 대화형(Interactive) 콘솔 모드 실행
./admin_shell students.csv
./client_shell students.csv
2. 배치 파일(Batch File) 스크립트 실행 모드
스크립트 내부의 명령어를 순차적으로 실행한 뒤 자동 종료됩니다.
./admin_shell students.csv -f commands.txt

---

## 5. 예외 처리 및 방어적 프로그래밍

안정적인 인터페이스 운영을 위해 시스템 전반에 강력한 예외 처리 로직이 구현되어 있습니다.

1. 인자 개수 유효성 검증(과소/과대 입력 방지)

* 필수 인자가 누락된 경우 (if (id_str == NULL) 등)를 식별하여 Error: Missing arguments.를 출력합니다.
* 요구 인자 파싱 완료 후 strtok(NULL, " ")을 추가 검증하여 사용자가 불필요한 인자를 더 많이 입력한 경우(nam != NULL)에도 Error: Invalid arguments.를 출력하며 실행을 거부합니다.

2. 엄격한 타입 파싱 및 범위 검증

* 성적 입력 시 숫자가 아닌 잘못된 문자열 포맷이 입력되는 현상을 방지하기 위해 strtol 함수를 통해 파싱 후 문자열 종단 포인터(ep)를 정밀 검사합니다.
* 성적 도메인 범위(0 <= score <= 100) 및 학번 양수 제약을 엄격히 적용합니다.

---

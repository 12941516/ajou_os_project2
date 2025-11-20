# 운영체제 프로젝트

ajou_os_project_race_condition(소프트웨어학과 202422117 금민기)

Ajou Univ. OS_Project

## 프로젝트명

**Race Condition과 동기화 문제 실습: Producer/Consumer & Reader/Writer 모델**

---

## 개요

이 프로젝트는 운영체제 수업의 두 번째 과제로, race condition이 발생하는 상황과 이를 해결하는 동기화 기법(semaphore, mutex, condition variable)을 학습하기 위해 제작하였다.

프로젝트는 다음 두 가지 모델로 구성된다:

1. **Producer/Consumer 모델**

   * 동기화 없는(no sync) 버전과 동기화(sync) 버전을 구현
   * 버퍼에 데이터를 넣고 빼는 과정에서 race condition 발생 확인

2. **Reader/Writer 모델**

   * 공유 데이터에 대해 동시 읽기와 쓰기를 수행
   * 동기화 없는(no sync) 버전에서 데이터 충돌(race condition) 발생 확인
   * mutex를 사용한 동기화(sync) 버전에서 문제 해결 확인

이를 통해 동기화의 필요성과 효과를 직접 확인할 수 있으며, 리눅스 환경에서 **pthread, mutex, condition variable** 사용법과 **race condition** 개념을 실습할 수 있다.

---

## 요구 환경

* 운영체제: Linux (Ubuntu 22.04 권장)
* 개발 언어: C
* 컴파일러: `gcc`
* 사용된 시스템 콜/기능: `pthread_create()`, `pthread_join()`, `pthread_mutex_*`, `pthread_cond_*`, `usleep()` 등
* 테스트 환경: 로컬 또는 VM 리눅스

---

## 빌드 방법

터미널에서 다음 명령으로 컴파일:

```bash
# Producer/Consumer
gcc -o pc2_no_sync pc2_no_sync.c -lpthread
gcc -o pc2_with_sync pc2_with_sync.c -lpthread

# Reader/Writer
gcc -o rw2_no_sync rw2_no_sync.c -lpthread
gcc -o rw2_with_sync rw2_with_sync.c -lpthread
```

---

## 실행 순서 및 사용법

### 1. Producer/Consumer 모델

#### 동기화 없는 버전 실행

```bash
./pc2_no_sync
```

* 여러 producer가 버퍼에 동시에 데이터를 쓰고, consumer가 읽는 과정에서 **race condition**이 발생함
* 콘솔에서 빈 슬롯을 접근하거나 덮어쓰는 메시지를 확인할 수 있음

#### 동기화 있는 버전 실행

```bash
./pc2_with_sync
```

* `mutex`와 `condition variable`로 버퍼 접근을 동기화
* race condition 문제 해결 확인

---

### 2. Reader/Writer 모델

#### 동기화 없는 버전 실행

```bash
./rw2_no_sync
```

* writer가 데이터를 갱신하는 중 reader가 읽어 데이터 불일치 발생 가능
* 콘솔에서 값이 건너뛰거나 중복 출력되는 것을 확인할 수 있음

#### 동기화 있는 버전 실행

```bash
./rw2_with_sync
```

* reader/writer mutex 사용으로 공유 데이터 접근을 보호
* race condition 문제 해결, reader는 동시에 읽을 수 있지만 writer는 단독 접근 보장

---

## 데이터 구조 예시

### Producer/Consumer

```c
typedef struct {
    int priority;
    int value;
    int iter;
    int valid; // no-sync only
} job_t;

job_t buffer[BUF_SIZE];
```

### Reader/Writer

* 공유 변수: `int shared_data`
* 동기화된 버전에서는 `pthread_mutex_t mutex`와 `pthread_mutex_t rw_mutex` 사용

---

## 예제 실행 결과

### Producer/Consumer (no sync)

```
[Producer 1] Put job at slot 2 (priority=1, iter=0)
[Producer 2] Put job at slot 2 (priority=2, iter=0)
>>> [Consumer] Took job from slot 2 (priority=2, value=531, iter=0)
>>> [Consumer] WARNING: slot 3 was EMPTY!
```

* 같은 슬롯에 여러 producer가 동시에 접근하여 race condition 발생

### Producer/Consumer (with sync)

```
[Producer 1] Put job at slot 0 (priority=0, iter=0)
>>> [Consumer] Took job from slot 0 (priority=0, value=123, iter=0)
```

* 동기화로 race condition이 발생하지 않음

### Reader/Writer (no sync)

```
[Reader 1] Detected change: -1 -> 0
    [Writer] Updated value to 1
[Reader 2] Detected change: 0 -> 1
    [Writer] Updated value to 2
```

* writer 갱신 중 reader가 읽어 불일치 발생

### Reader/Writer (with sync)

```
[Reader] Detected change: 0 -> 1
    [Writer] Updated value to 1
```

* 동기화로 데이터 일관성 보장

---

## 에러 처리 및 주의사항

* 버퍼 접근 시 동기화를 하지 않으면 race condition 발생
* 콘솔 메시지는 race condition 발생 여부를 확인하는 학습용 예시
* 필요 시 `usleep()`로 context switching을 인위적으로 발생시켜 race condition을 유도

---

## 테스트 케이스

1. **Producer/Consumer**

   * no sync: 빈 슬롯 접근, 중복 쓰기 발생 확인
   * with sync: 모든 생산/소비 동작 정상 처리

2. **Reader/Writer**

   * no sync: reader/writer 중 데이터 불일치 확인
   * with sync: reader는 동시에 읽어도 writer 접근 보호 확인

---

## 참고문헌 및 자료

* W. Richard Stevens, *Advanced Programming in the UNIX Environment*
* Pthreads Documentation: [https://man7.org/linux/man-pages/man7/pthreads.7.html](https://man7.org/linux/man-pages/man7/pthreads.7.html)
* 아주대학교 MOCA 자료 및 수업 교재

---

## 작성자

* 학생: 금민기 (Github ID = 12941516)
* 학과/학번: 소프트웨어학과 202422117

---

## 요약

* 본 프로젝트는 **race condition 발생 및 해결**을 직접 구현하고 학습하기 위해 제작
* Producer/Consumer와 Reader/Writer 모델을 사용하여 동기화 전후 차이를 관찰
* pthread, mutex, condition variable 사용을 통해 **동기화 기법 실습** 및 **데이터 일관성 확보**를 경험

# 운영체제 프로젝트2

ajou_os_project2(소프트웨어학과 202422117 금민기)

Ajou Univ. OS_Project2

## 프로젝트명

**Race Condition과 Syncrinization 문제 실습: Producer/Consumer & Reader/Writer 모델**

---

## 개요

이 프로젝트는 운영체제 수업의 두 번째 과제로, race condition이 발생하는 상황과 이를 해결하는 동기화 기법(semaphore, mutex, condition variable)을 학습하기 위한 것이다.

프로젝트는 다음 두 가지 모델로 구성된다:

1. **Producer/Consumer 모델**
   * 동기화 없는(no sync) 버전과 동기화(sync) 버전을 구현해야 한다.
   * 버퍼에 데이터를 넣고 빼는 과정에서 race condition가 발생함을 확인한다.

2. **Reader/Writer 모델**
   * 공유 데이터에 대해 동시 읽기와 쓰기를 수행한다.
   * 동기화를 구현하지 않은(no sync) 버전에서 race condition가 발생함을 확인한다.
   * mutex를 사용한 동기화(sync) 버전의 코드에서 문제가 해결됨을 확인한다.

이를 통해 동기화의 필요성과 효과를 직접 확인할 수 있으며, 리눅스 환경에서 **pthread, mutex, condition variable** 사용법과 **race condition** 개념을 실습할 수 있다.

---

## 요구 환경

* 운영체제: Linux (Ubuntu 22.04 권장)
* 개발 언어: C
* 컴파일러: `gcc`
* 사용된 시스템 콜/기능: `pthread_create()`, `pthread_join()`, `pthread_mutex_*`, `pthread_cond_*`, `usleep()` 등
* 테스트 환경: 로컬 또는 VM 리눅스

---

## 빌드 방법1

터미널에서 다음 명령으로 git clone한 뒤, 각각 gcc 컴파일한다:

```cmd
~$ git clone https://github.com/12941516/ajou_os_project2.git -b master
~$ cd ajou_os_project2
~/ajou_os_project2$ gcc -o reader_writer2/rw2_no_sync reader_writer2/rw2_no_sync.c -lpthread
~/ajou_os_project2$ gcc -o reader_writer2/rw2_with_sync reader_writer2/rw2_with_sync.c -lpthread
~/ajou_os_project2$ gcc -o producer_consumer2/pc2_no_sync producer_consumer2/pc2_no_sync.c -lpthread
~/ajou_os_project2$ gcc -o producer_consumer2/pc2_with_sync producer_consumer2/pc2_with_sync.c -lpthread
```

---

## 빌드 방법2

터미널에서 다음 순서대로 디렉토리를 생성한 뒤, 각 소스코드를 해당 디렉토리에 삽입하고 컴파일한다.

```cmd
~$ mkdir ajou_os_project2
~$ cd ajou_os_project2

# 코드를 ajou_os_project2 디렉토리로 옮긴 뒤 아래 내용 진행

~/ajou_os_project2$ gcc -o reader_writer2/rw2_no_sync reader_writer2/rw2_no_sync.c -lpthread
~/ajou_os_project2$ gcc -o reader_writer2/rw2_with_sync reader_writer2/rw2_with_sync.c -lpthread
~/ajou_os_project2$ gcc -o producer_consumer2/pc2_no_sync producer_consumer2/pc2_no_sync.c -lpthread
~/ajou_os_project2$ gcc -o producer_consumer2/pc2_with_sync producer_consumer2/pc2_with_sync.c -lpthread
```

---

## 실행 순서 및 사용법

### 1. Producer/Consumer 모델

#### 동기화 없는 버전 실행

```cmd
~/ajou_os_project2$ ./producer_consumer2/pc2_no_sync
```

* 여러 producer가 버퍼에 동시에 데이터를 쓰고, consumer가 읽는 과정에서 race condition이 발생하는 것을 볼 수 있다.
* results의 결과 로그나 실제 코드 실행 결과를 통해 콘솔에서 빈 슬롯에 접근하거나, 이미 내용물이 있는 슬롯에 덮어쓰는 메시지를 확인할 수 있다.

#### 동기화 있는 버전 실행

```cmd
~/ajou_os_project2$ ./producer_consumer2/pc2_with_sync
```

* `mutex`와 `condition variable`로 버퍼 접근을 동기화하는 코드로 race condition 문제가 해결된 것을 확인할 수 있다.
* 즉, 빈 슬롯을 읽거나 이미 채워진 슬롯을 덮어쓰는 작용을 예방한 것이다.

---

### 2. Reader/Writer 모델

#### 동기화 없는 버전 실행

```cmd
~/ajou_os_project2$ ./reader_writer2/rw2_no_sync
```

* writer가 데이터를 갱신하는 중 reader가 읽으면 데이터 불일치가 발생할 수 있다.
* 실제로 콘솔에서 값이 건너뛰어지거나 중복되어 출력되는 것을 확인할 수 있다.

#### 동기화 있는 버전 실행

```cmd
~/ajou_os_project2$ ./reader_writer2/rw2_with_sync
```

* reader/writer mutex를 사용하여 공유 데이터로의 접근을 보호한다.
* race condition 문제를 해결할 수 있는데, reader는 동시에 읽을 수 있지만 writer는 단독 접근을 보장하였다.

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
* 동기화된 버전에서는 `pthread_mutex_t mutex`와 `pthread_mutex_t rw_mutex`를 사용했다.

---

## 예제 실행 결과

### Producer/Consumer (no sync)

```
[Producer 1] Put job at slot 2 (priority=1, iter=0)
[Producer 2] Put job at slot 2 (priority=2, iter=0)
>>> [Consumer] Took job from slot 2 (priority=2, value=531, iter=0)
>>> [Consumer] WARNING: slot 3 was EMPTY!
```

* 같은 슬롯에 여러 producer가 동시에 접근하여 race condition이 발생하는 것을 확인할 수 있다.

### Producer/Consumer (with sync)

```
[Producer 1] Put job at slot 0 (priority=0, iter=0)
>>> [Consumer] Took job from slot 0 (priority=0, value=123, iter=0)
```

* 동기화로 race condition이 발생하지 않음을 확인할 수 있다.

### Reader/Writer (no sync)

```
[Reader 1] Detected change: -1 -> 0
    [Writer] Updated value to 1
[Reader 2] Detected change: 0 -> 1
    [Writer] Updated value to 2
```

* writer 갱신 중 reader가 읽어 데이터 간의 불일치가 발생함을 확인할 수 있다.

### Reader/Writer (with sync)

```
[Reader] Detected change: 0 -> 1
    [Writer] Updated value to 1
```

* 동기화로 데이터 일관성을 보장함을 확인할 수 있다.

---

## 에러 처리 및 주의사항

* 버퍼 접근 시 동기화를 하지 않으면 race condition이 발생한다.
* 콘솔 메시지는 race condition 발생 여부를 확인하기 위한 것으로, 반복 횟수와 함께 표시하였다.
* 강제로 race condition을 돋보이게 만들기 위해 `usleep()`로 context switching을 인위적으로 발생시켜 race condition을 유도하였다.

---

## 테스트 케이스

1. **Producer/Consumer**
   * no sync: 빈 슬롯 접근, 중복 쓰기 발생을 확인한다.
   * with sync: 모든 producer/consumer 동작이 정상적으로 처리됨을 확인한다.

2. **Reader/Writer**
   * no sync: reader/writer 중 데이터 불일치를 확인한다.
   * with sync: reader는 동시에 읽어도 writer 접근이 보호됨을 확인한다.

---

## 참고문헌 및 자료

* W. Richard Stevens, *Advanced Programming in the UNIX Environment*
* Pthreads Documentation: [https://man7.org/linux/man-pages/man7/pthreads.7.html](https://man7.org/linux/man-pages/man7/pthreads.7.html)
* 아주대학교 MOCA 자료(리눅스 관련 자료: [https://moca.ajou.ac.kr](https://moca.ajou.ac.kr)) 및 수업 교재
* GCC 컴파일 방법 자료: https://code-lab1.tistory.com/368
* Thread에서의 동기화 개념 참고 자료: https://wikidocs.net/65529

---

## 작성자

* 학생: 금민기 (Github ID = 12941516)
* 학과/학번: 소프트웨어학과 202422117

---

## 요약

* 본 프로젝트는 race condition 발생 및 해결을 직접 구현하고 학습하기 위한 것이다.
* Producer/Consumer와 Reader/Writer 모델을 사용하여 동기화 전후 차이를 관찰하였으며, 동기화 사용으로 race condition 발생을 해결함을 확인하였다.
* pthread, mutex, condition variable 사용을 통해 동기화 기법 실습 및 데이터 일관성 확보를 경험하였다.

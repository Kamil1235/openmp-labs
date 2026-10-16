# Lab 1 — Задачи 1–3

Язык: C++ (OpenMP). Среда: Visual Studio 2022, конфигурация Release | x64.

---

## Task 1 — Hello World per thread

**Условие:** каждый поток печатает свой идентификатор, общее число потоков и строку «Hello World».

**Исходник:** `lab1_1.cpp`

**Сборка (MSVC):**
    cl /openmp /EHsc /O2 lab1_1.cpp /Fe:lab1_1.exe

**Запуск:**
    lab1_1.exe <num_threads>

**Пример:**
    lab1_1.exe 8

**Ожидаемый вывод:** 8 строк вида `Thread X of 8: Hello World` в произвольном порядке.

---

## Task 2 — Средние значения массива

**Условие:** массив `a[16000]`, где `a[i] = i`. Построить `b[i] = (a[i-1]+a[i]+a[i+1])/3`. Запуск с 8 потоками при разных типах распределения работ.

**Исходник:** `Lab1_2.cpp`

**Сборка (MSVC):**
    cl /openmp /EHsc /O2 Lab1_2.cpp /Fe:Lab1_2.exe

**Запуск:**
    set OMP_SCHEDULE=static
    Lab1_2.exe <num_threads> static

    set OMP_SCHEDULE=dynamic
    Lab1_2.exe <num_threads> dynamic

    set OMP_SCHEDULE=guided
    Lab1_2.exe <num_threads> guided

**Примеры:**
    Lab1_2.exe 8 static
    Lab1_2.exe 8 dynamic
    Lab1_2.exe 8 guided

**Ожидаемый вывод:**
    schedule=static  chunk=0 threads=8 time=... s
    b[1]=1.000 b[100]=100.000 b[15998]=15998.000

---

## Task 3 — Обратный порядок потоков

**Условие:** модифицировать задачу 1 так, чтобы потоки печатали свои идентификаторы в обратном порядке. Реализовано 6 способов.

**Исходник:** `lab1_3.cpp`

**Сборка (MSVC):**
    cl /openmp /EHsc /O2 lab1_3.cpp /Fe:lab1_3.exe

**Запуск:**
    lab1_3.exe <method 1..6> <num_threads>

**Примеры:**
    lab1_3.exe 1 8    (ordered)
    lab1_3.exe 2 8    (lock)
    lab1_3.exe 3 8    (critical)
    lab1_3.exe 4 8    (single)
    lab1_3.exe 5 8    (master)
    lab1_3.exe 6 8    (flag)

**Ожидаемый вывод:** идентификаторы потоков в порядке от 7 до 0.

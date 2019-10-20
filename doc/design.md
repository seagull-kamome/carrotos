---
title: Design note
---


Components
================

```plantuml
@startuml

[User application] ..> Task
[User application] ..> Mutex/Semaphore
[User application] ..> Real time clock
[Kernel] - Task
[Kernel] - Mutex/Semaphore
[Kernel] - Real time clock



[carrotlib] - Atomic
[carrotlib] - Compiler helper
[carrotlib] - Interrupt
[carrotlib] - List
[carrotlib] - Spinlock
[carrotlib] - timespec


[carrotlib-arch]


package ConfigurationSpecificCode {
  [Kernel-port]
  [carrotlib-port]
  [carrotlib-arch-port]
  [Board specific code]
  [MCU specific code]
  [Configuraation specific code]
}

@enduml
```




Kernel
==========

## Real time clock (or Tick)


## SMP scheduler

### CPU affinity



## Memory manager



Plan
======

## carritlib

  - bitops
  - malloc/free
  - lock-free algorithms
  - tracer
  - coverage tracer


## Kernel

  - scheduler
  - SMP
  - cyclic task
  - event flag
  - notify to task
  - message queue
  - virtual IRQ


## Driver

  - I2C driver
  - DMA channel manager
  - UART driver
  - SPI driver
  - GPIO
  - LED
  - Charactor LCD
  - Graphical LCD


## Language wrapper

  - ATS2

## other

  - any CI


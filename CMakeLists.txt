cmake_minimum_required(VERSION 3.9)
project(multithread_1)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(multithread_1 main.cpp ThreadPool.h task.h ThreadJoiner.h RegularType.h universal_call.h isFunctor.h mergeSort.h Semaphore.h shellSort.h radixSort.h heapSort.h quickSort.h bubbleSort.h insertionSort.h selectionSort.h Decany.h typeErasureBasic.h array_view.h PredicatePool.h SafeWrappers.h Server.h User.h)
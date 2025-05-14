/*
*	STL-подобная библиотека queue.
* Если при добавлении элемента буфер заполнен полностью,
* то записи не происходит. Очередь ожидает пока хотя бы
* один элемент будет считан, чтобы перейти в начало
* внутреннего буфера и записать элемент.
*
*	Особенности и отличия от std::queue:
*	1. Фактически здесь представлен функционал циклического буфера;
* 2. Нет функций front() и back();
* 3. Есть функции at(), capacity().
*/

#ifndef _CQUEUE_H_
#define _CQUEUE_H_

#include "stdlib.h"

typedef unsigned char cq_byte;
typedef struct cq_private_s cq_private_t;
typedef struct cqueue_s cqueue;

typedef void    (*cq_func)          (cqueue*);
typedef cq_byte (*cq_func_rb)       (cqueue*);
typedef cq_byte (*cq_func_rb_cpv)   (cqueue*, const void*);
typedef cq_byte (*cq_func_rb_pv)    (cqueue*, void*);
typedef cq_byte (*cq_func_rb_pv_i)  (cqueue*, void*, int);
typedef size_t  (*cq_func_rsize)    (cqueue*);


struct cqueue_s
{
    /// @brief  Запись значения в циклический буфер.
    ///         Если буфер полон (ни один элемент не вычитан),
    ///         то происходит отказ от записи пока не появится место.
    /// @param  item - данные, помещаемые в буфер
    /// @return 0 в случае успешного помещения, иначе 1
    cq_func_rb_cpv  push;

    /// @brief  Извлечение значения из циклического буфера,
    ///         если он не пустой
    /// @param  dst - указатель, куда поместить данные
    /// @return 0 в случае успешного извлечения, иначе 1
    cq_func_rb_pv   pop;

    /// @brief  Извлечение элемента из очереди,
    ///         считая от текущего положения указателя чтения
    /// @param  dst - указатель, куда поместить данные
    /// @param  n - номер элемента в очереди
    /// @return 0 в случае успешного извлечения, иначе 1
    cq_func_rb_pv_i at;

    /// @brief  Определяет количество элементов в очереди
    cq_func_rsize   size;

    /// @brief  Размер выделенной памяти для внутреннего
    ///         буфера очереди в байтах
    cq_func_rsize   capacity;

    /// @brief  Проверка на пустоту очереди
    cq_func_rb      empty;

    /// @brief  Очищение очереди
    cq_func         clear;

    /// @brief  Освобождение памяти
    cq_func         free;

    /// @brief  Приватные переменные
    cq_private_t*   cpriv;
};

/// @brief Функция инициализиация циклического буфера
/// @param item_size - размер одного элемента циклического буфера
/// @param count - количество элементов циклического буфера
/// @return указатель на очередь в случае успешного выделения памяти, иначе NULL
cqueue* cqueue_new(int item_size, int count);

#endif

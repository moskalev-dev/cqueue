# cqueue
 
![Alt Text](https://github.com/moskalev-dev/cqueue/blob/main/target/resources/logo.png)

#
# RU:
STL-подобная библиотека queue.
Поддерживает все типы и пользовательские структуры,
использующует универсальную структуру,
в которой тип передается как cq_byte*.
Если при добавлении элемента буфер заполнен полностью,
то записи не происходит. Очередь ожидает пока хотя бы
один элемент будет считан, чтобы перейти в начало
внутреннего буфера и записать элемент.\
Особенности и отличия от std::queue:
1. Фактически здесь представлен функционал циклического буфера;
2. Нет функций front() и back();
3. Есть функции at(), capacity().

Пример работы библиотеки (CMake проект) находится в директории "target".

### 🚀 Для выделения в памяти вектора необходимо:
```c
cqueue* tqueue  = NULL;  // создать указатель на структуру
tqueue = cqueue_new (sizeof(int), 10); // аллоцировать циклический буфер в памяти 
```
В функцию cqueue_new:
 - Первым параметром (item_size) передается размер элемента в байтах.
 - Вторым параметром (count) передается максимальне количество элементов для накопления.
 
### 🚀 Пример обращения к функциям:
```c
int a = 1;
tqueue->push(tqueue, &a);

int item = 0;
cq_byte res_pop = tqueue->pop(tqueue, &item);

tqueue->size(tqueue);

tqueue->empty(tqueue);

tqueue->free(tqueue);
tqueue = NULL;
```

### ⚙️ Библиотека имеет следующие функции:
######
#### cq_byte push(cqueue* self, const void* item) 
Запись значения в циклический буфер.\
Если буфер полон (ни один элемент не вычитан),
то происходит отказ от записи пока не появится место.\
Параметры: self - указатель на структуру в памяти,
item - данные, помещаемые в буфер.\
Возвращаемые значения: 0 в случае успешного помещения, иначе 1.
######

#### cq_byte cq_pop(cqueue* self, void* dst)
Извлечение значения из циклического буфера, если он не пустой.\
Параметры: self - указатель на структуру в памяти,
dst - указатель, куда поместить данные.\
Возвращаемые значения: 0 в случае успешного извлечения, иначе 1.
######

#### cq_byte cq_at(cqueue* self, void* dst, int n)
Извлечение элемента из очереди, считая от текущего положения указателя чтения.\
Параметры: self - указатель на структуру в памяти,
dst - указатель, куда поместить данные.\
n - номер элемента в очереди.\
Возвращаемые значения: 0 в случае успешного извлечения, иначе 1.
######

#### cq_byte cq_empty(cqueue* self)
Проверка на пустоту очереди.\
Возвращаемые значения: 1 если буфер пуст, иначе 0.
######

#### size_t cq_size(cqueue* self)
Определяет количество элементов в очереди.\
Возвращаемые значения: 0 если буфер пуст, иначе количество элементов.
######

#### size_t cq_capacity(cqueue* self)
Размер выделенной памяти для внутреннего буфера очереди в байтах.\
Возвращаемые значения: 0 если буфер пуст, иначе количество байт в памяти.
######

#### void cq_clear(cqueue* self)
Очищение очереди.
######

#### void cq_free(cqueue* self)
Освобождение памяти.


#
# EN:
An STL-like queue library.
Supports all types and custom structures
that use a universal structure
in which the type is passed as cq_byte*.
If the buffer is completely full when adding an item,
then no writing occurs. The queue waits until at least
one element is read in order to go to the beginning
of the internal buffer and write the element.\
Features and differences from std::queue:
1. In fact, the cyclic buffer functionality is presented here;
2. There are no front() and back() functions.;
3. There are functions at(), capacity().

An example of how the library works (CMake project) is located in the "target" directory.
### 🚀 To allocate a vector in memory, you must:
```c
cqueue* tqueue  = NULL;  // create a pointer to the structure
tqueue = cqueue_new (sizeof(int), 10); // allocate a cyclic buffer in memory 
```
To the queue_new function:
 - The first parameter (item_size) is the size of the item in bytes.
 - The second parameter (count) specifies the maximum number of items to accumulate.
 
### 🚀 Example of accessing functions:
```c
int a = 1;
tqueue->push(tqueue, &a);

int item = 0;
cq_byte res_pop = tqueue->pop(tqueue, &item);

tqueue->size(tqueue);

tqueue->empty(tqueue);

tqueue->free(tqueue);
tqueue = NULL;
```

### ⚙️ The library has the following functions:
######
#### cq_byte push(cqueue* self, const void* item) 
Writing a value to a cyclic buffer.\
If the buffer is full (no element has been subtracted),
then writing is refused until there is space.\
Parameters: self - pointer to the structure in memory,
item - data placed in the buffer.\
Return values: 0 in case of successful placement, otherwise 1.
######

#### cq_byte cq_pop(cqueue* self, void* dst)
Retrieving a value from a cyclic buffer if it is not empty.\
Parameters: self is a pointer to a structure in memory,
dst is a pointer to where to put the data.\
Return values: 0 in case of successful extraction, otherwise 1.
######

#### cq_byte cq_at(cqueue* self, void* dst, int n)
Retrieves an item from the queue, counting from the current position of the read pointer.\
Parameters: self is a pointer to a structure in memory,
dst is a pointer to where to put the data.\
n is the number of the item in the queue.\
Return values: 0 in case of successful extraction, otherwise 1.
######

#### cq_byte cq_empty(cqueue* self)
Checking for an empty queue.\
Return values: 1 if the buffer is empty, otherwise 0.
######

#### size_t cq_size(cqueue* self)
Determines the number of items in the queue.\
Return values: 0 if the buffer is empty, otherwise the number of items.
######

#### size_t cq_capacity(cqueue* self)
The size of the allocated memory for the queue's internal buffer, in bytes.\
Return values: 0 if the buffer is empty, otherwise the number of bytes in memory.
######

#### void cq_clear(cqueue* self)
Clearing the queue.
######

#### void cq_free(cqueue* self)
Freeing up memory.

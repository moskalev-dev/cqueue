#include <string.h>
#include "cqueue.h"

/// Структура приватной информации о циклическом буфере
struct cq_private_s
{
    int wpos;           ///< Текущее положения указателя 'запись'
    int rpos;           ///< Текущее положения указателя 'чтение'
    int max_items;      ///< Максимальное количество элементов в буфере
    int item_size;      ///< Размер элемента
    cq_byte full;       ///< Признак полностью заполненного буфера
    unsigned char* buf; ///< Указатель на буфер
};

cq_byte cq_push(cqueue* self, const void* item);
cq_byte cq_pop(cqueue* self, void* dst);
cq_byte cq_at(cqueue* self, void* dst, int n);
cq_byte cq_empty(cqueue* self);
size_t  cq_size(cqueue* self);
size_t  cq_capacity(cqueue* self);
void    cq_clear(cqueue* self);
void    cq_free(cqueue* self);

cqueue* cqueue_new(int item_size, int count)
{
    if(item_size <= 0 || count <= 0)
        return NULL;

    cqueue* cq = (cqueue*)malloc(sizeof(cqueue));

    if(!cq)
        return NULL;

    cq->cpriv = (cq_private_t*)malloc(sizeof(cq_private_t));
    if(!cq->cpriv)
    {
        free(cq);
        return NULL;
    }

    cq->cpriv->buf = (unsigned char*)malloc(item_size*count);
    if(!cq->cpriv->buf)
    {
        free(cq->cpriv);
        free(cq);
        return NULL;
    }

    // init private:
    cq->cpriv->wpos = 0;
    cq->cpriv->rpos = 0;
    cq->cpriv->full = 0;
    cq->cpriv->max_items = count;
    cq->cpriv->item_size = item_size;

    // init public:
    cq->push = cq_push;
    cq->pop = cq_pop;
    cq->at = cq_at;
    cq->empty = cq_empty;
    cq->size = cq_size;
    cq->capacity = cq_capacity;
    cq->clear = cq_clear;
    cq->free = cq_free;

    return cq;
}

cq_byte cq_push(cqueue *self, const void* item)
{
    if(!self || !self->cpriv || !item)
        return 1;

    /// Используем локальные переменные для лучшей
    /// совместимости с многопоточностью
    int wpos = self->cpriv->wpos;
    int rpos = self->cpriv->rpos;

#if 1
    //! Если буфер полон, отказ от добавления
    if(self->cpriv->full)
        return 1;
#endif

    memcpy(self->cpriv->buf + (wpos*self->cpriv->item_size), item,
           self->cpriv->item_size);

    wpos += 1;
    wpos %= self->cpriv->max_items; // автопереход записи в начало

#if 0 //! Данная логика обеспечивает другое поведение циклического буфера
	//! Если буфер полон, т.е. хвост (wpos) залез на голову (rpos),
	//! то двигаем rpos чтобы освободить место для записи по кругу
	if(self->cpriv->full)
	{
		rpos += 1;
		rpos %= self->cpriv->max_items;
	}
#endif

    self->cpriv->full = (wpos == rpos);
    self->cpriv->wpos = wpos;
    self->cpriv->rpos = rpos;

    return 0;
}

cq_byte cq_pop(cqueue *self, void* dst)
{
    if( !self || !self->cpriv || !dst )
        return 1;

    /// Используем локальные переменные для лучшей
    /// совместимости с многопоточностью
    int rpos = self->cpriv->rpos;

    //! Если буфер пуст
    if( rpos == self->cpriv->wpos && !self->cpriv->full )
        return 1;

    memcpy(dst, self->cpriv->buf + (rpos * self->cpriv->item_size),
           self->cpriv->item_size);

    rpos += 1;
    rpos %= self->cpriv->max_items;  // автопереход чтения
    self->cpriv->rpos = rpos;
    self->cpriv->full = 0;

    return 0;
}

cq_byte cq_at(cqueue *self, void *dst, int n)
{
    if(self && self->cpriv && dst && n >= 0)
    {
        if( cq_size(self) <= (size_t)n )
            return 1;

        int pos = self->cpriv->rpos + n;
        pos %= self->cpriv->max_items;

        memcpy(dst, self->cpriv->buf + (pos * self->cpriv->item_size),
               self->cpriv->item_size);
    }
    else
        return 1;

    return 0;
}

size_t cq_size(cqueue *self)
{
    if(!self)
        return 0;

    int delta = 0;

    if(self->cpriv->full)
        delta = self->cpriv->max_items;
    else
    {
        delta = self->cpriv->wpos - self->cpriv->rpos;
        if(delta < 0)
            delta += self->cpriv->max_items;
    }

    return (size_t)delta;
}

size_t cq_capacity(cqueue *self)
{
    if(self)
        return self->cpriv->max_items*self->cpriv->item_size;
    return 0;
}

cq_byte cq_empty(cqueue *self)
{
    if(self)
        return (cq_byte)(!self->cpriv->full && (self->cpriv->rpos == self->cpriv->wpos));
    return 1;
}

void cq_clear(cqueue *self)
{
    if(self)
    {
        self->cpriv->wpos = self->cpriv->rpos = 0;
        self->cpriv->full = 0;
    }
}

void cq_free(cqueue *self)
{
    if(self)
    {
        self->cpriv->wpos = 0;
        self->cpriv->rpos = 0;
        self->cpriv->full = 0;

        if(self->cpriv->buf)
        {
            free(self->cpriv->buf);
            self->cpriv->buf = NULL;
        }

        if(self->cpriv)
        {
            free(self->cpriv);
            self->cpriv = NULL;
        }

        free(self);
    }
}

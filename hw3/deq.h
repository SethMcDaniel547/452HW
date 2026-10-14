#ifndef DEQ_H
#define DEQ_H

typedef void *Deq;
typedef void *Data;
typedef char *Str;
typedef void (*DeqMapF)(Data d);
typedef Str (*DeqStrF)(Data d);

extern Deq deq_new(void);
extern int deq_len(Deq q);
extern void deq_head_put(Deq q, Data d);
extern Data deq_head_get(Deq q);
extern Data deq_head_ith(Deq q, int i);
extern Data deq_head_rem(Deq q, Data d);
extern void deq_tail_put(Deq q, Data d);
extern Data deq_tail_get(Deq q);
extern Data deq_tail_ith(Deq q, int i);
extern Data deq_tail_rem(Deq q, Data d);
extern void deq_map(Deq q, DeqMapF f);
extern void deq_del(Deq q, DeqMapF f);
extern Str deq_str(Deq q, DeqStrF f);

#endif
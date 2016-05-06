/*   program 11-14   */

#include <stdlib.h>
#include <pthread.h>

struct job
{
	struct job *j_next;
	struct job *j_prev;
	pthread_t j_id;
};

struct quene
{
	struct job *q_head;
	struct job *q_tail;
	pthread_rwlock_t q_lock;
};

/* init the quene */
int quene_init(struct quene *qp)
{
	int err;

	qp->q_head = NULL;
	qp->q_tail = NULL;
	
	err = pthread_rwlock_init(&qp->q_lock, NULL);
	if (err != 0)
		return err;

	return 0;
}

/* insert a job into the quene 从队列前面插入*/
void job_insert(struct quene *qp, struct job *jp)
{
	pthread_rwlock_wrlock(&qp->q_lock);
	jp->j_next = qp->q_head;
	jp->j_prev = NULL;
	if (qp->head != NULL)
		qp->head->j_prev = jp;
	else
		qp->q_tail = jp;
	qp->q_head = jp;
	pthread_rwlock_unlock(&qp->q_lock);
}

/* append a job to the quene 从队列后面插入 */
void job_append(struct quene *qp, struct job *jp)
{
	pthread_rwlock_wrlock(&qp->q_lock);
	jp->j_next = NULL;
	jp->j_prev = qp->tail;
	if (qp->tail != NULL)
		qp->tail->j_next = jp;
	else
		qp->head = jp;
	qp->tail = jp;
	pthread_rwlock_unlock(&qp->q_lock);
}

/* remove a job from the quene somewhere 从队列某处删除一个已经指定的job */
void job_remove(struct quene *qp, struct job *jp)
{
	pthread_rwlock_wrlock(&qp->q_lock);
	if (jp == qp->q_head)
	{
		qp->q_head = jp->j_next;
		if (qp->q_head != NULL)
			qp->q_head->j_prev = NULL;
		else
			qp->q_tail = NULL;
	}
	else if (jp == qp->q_tail)
	{
		qp->q_tail = jp->j_prev;
		if (qp->q_tail != NULL)
			qp->q_tail->j_next = NULL;
		else
			qp->q_head = NULL;
	}
	else
	{
		jp->j_prev->j_next = jp->j_next;
		jp->j_next->j_prev = jp->j_prev;
	}
	pthread_rwlock_unlock(&qp->q_lock);
}

/* find a job from the given thread ID */
struct job *job_find(struct quene *qp, pthread_t id)
{
	if (pthread_rwlock_rdlock(&qp->q_lock) != 0)
		return NULL;
	struct job *jp = qp->q_head;
	while (jp != NULL)
		if (jp->j_id == id)
			break;
	pthread_rwlock_unlock(&qp->q_lock);
	return jp;
}

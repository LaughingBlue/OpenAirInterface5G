/*
  Author: Laurent THOMAS, Open Cells
  copyleft: OpenAirInterface Software Alliance and it's licence
*/
#include <vector>
#include <map>
#include <sys/eventfd.h>


#include <intertask_interface.h>

typedef struct timer_elm_s {
  timer_type_t      type;     ///< Timer type
  long              instance;
  long duration;
  uint64_t timeout;
  void              *timer_arg; ///< Optional argument that will be passed when timer expires
} timer_elm_t ;

typedef struct task_list_s {
  task_info_t admin;
  pthread_t thread;
  pthread_mutex_t queue_cond_lock;
  std::vector<MessageDef *> message_queue;
  std::map<long,timer_elm_t> timer_map;
  uint64_t next_timer=UINT64_MAX;
  struct epoll_event  *events =NULL;
  int nb_fd_epoll=0;
  int nb_events=0;
  int epoll_fd=-1;
  int sem_fd=-1;
} task_list_t;

int timer_expired(int fd);
task_list_t tasks[TASK_MAX];

extern "C" {
  void *pool_buffer_init (void) {
    return 0;
  }

  void *pool_buffer_clean (void *arg) {
    //-----------------------------------------------------------------------------
    return 0;
  }

  void free_mem_block (mem_block_t *leP, const char *caller) {
    AssertFatal(leP!=NULL,"");
    free(leP);
  }

  mem_block_t *get_free_mem_block (uint32_t sizeP, const char *caller) {
    mem_block_t *ptr=(mem_block_t *)malloc(sizeP+sizeof(mem_block_t));
    ptr->next = NULL;
    ptr->previous = NULL;
    ptr->data=((unsigned char *)ptr)+sizeof(mem_block_t);
    ptr->size=sizeP;
    return ptr;
  }

  void *itti_malloc(task_id_t origin_task_id, task_id_t destination_task_id, ssize_t size) {
    void *ptr = NULL;
    AssertFatal ((ptr=malloc (size)) != NULL, "Memory allocation of %zu bytes failed (%d -> %d)!\n",
                 size, origin_task_id, destination_task_id);
    return ptr;
  }

  int itti_free(task_id_t task_id, void *ptr) {
    AssertFatal (ptr != NULL, "Trying to free a NULL pointer (%d)!\n", task_id);
    free (ptr);
    return (EXIT_SUCCESS);
  }

  MessageDef *itti_alloc_new_message_sized(task_id_t origin_task_id, MessagesIds message_id, MessageHeaderSize size) {
    MessageDef *temp = (MessageDef *)itti_malloc (origin_task_id, TASK_UNKNOWN, sizeof(MessageHeader) + size);
    temp->ittiMsgHeader.messageId = message_id;
    temp->ittiMsgHeader.originTaskId = origin_task_id;
    temp->ittiMsgHeader.ittiMsgSize = size;
    return temp;
  }

  MessageDef *itti_alloc_new_message(task_id_t origin_task_id, MessagesIds message_id) {
    int size=sizeof(MessageHeader) + messages_info[message_id].size;
    MessageDef *temp = (MessageDef *)itti_malloc (origin_task_id, TASK_UNKNOWN, size);
    temp->ittiMsgHeader.messageId = message_id;
    temp->ittiMsgHeader.originTaskId = origin_task_id;
    temp->ittiMsgHeader.ittiMsgSize = size;
    return temp;
    //return itti_alloc_new_message_sized(origin_task_id, message_id, messages_info[message_id].size);
  }

  static inline int itti_send_msg_to_task_locked(task_id_t destination_task_id, instance_t instance, MessageDef *message) {
    task_list_t *t=tasks+destination_task_id;
    message->ittiMsgHeader.destinationTaskId = destination_task_id;
    message->ittiMsgHeader.instance = instance;
    message->ittiMsgHeader.lte_time.frame = 0;
    message->ittiMsgHeader.lte_time.slot = 0;
    int message_id = message->ittiMsgHeader.messageId;
    size_t s=t->message_queue.size();

    if ( s > t->admin.queue_size )
      LOG_E(TMR,"Queue for %s task contains %ld messages\n", itti_get_task_name(destination_task_id), s );

    if ( s > 50 )
      LOG_I(TMR,"Queue for %s task size: %ld\n",itti_get_task_name(destination_task_id), s+1);

    t->message_queue.insert(t->message_queue.begin(), message);
    eventfd_t sem_counter = 1;
    AssertFatal ( sizeof(sem_counter) == write(t->sem_fd, &sem_counter, sizeof(sem_counter)), "");
    LOG_D(TMR,"sent messages id=%d to %s\n",message_id, t->admin.name);
    return 0;
  }

  int itti_send_msg_to_task(task_id_t destination_task_id, instance_t instance, MessageDef *message) {
    task_list_t *t=&tasks[destination_task_id];
    pthread_mutex_lock (&t->queue_cond_lock);
    int ret=itti_send_msg_to_task_locked(destination_task_id, instance, message);

    while ( t->message_queue.size()>0 && t->admin.func != NULL ) {
      if (t->message_queue.size()>1)
	LOG_W(TMR,"queue in no thread mode is %ld\n", t->message_queue.size());
      pthread_mutex_unlock (&t->queue_cond_lock);
      t->admin.func(NULL);
      pthread_mutex_lock (&t->queue_cond_lock);
    }
    pthread_mutex_unlock (&t->queue_cond_lock);
    return ret;
  }

  void itti_subscribe_event_fd(task_id_t task_id, int fd) {
    struct epoll_event event;
    task_list_t *t=&tasks[task_id];
    t->nb_fd_epoll++;
    t->events = (struct epoll_event *)realloc((void *)t->events,
                t->nb_fd_epoll * sizeof(struct epoll_event));
    event.events  = EPOLLIN | EPOLLERR;
    event.data.u64 = 0;
    event.data.fd  = fd;
    AssertFatal(epoll_ctl(t->epoll_fd, EPOLL_CTL_ADD, fd, &event) == 0,
                "epoll_ctl (EPOLL_CTL_ADD) failed for task %s, fd %d: %s!\n",
                itti_get_task_name(task_id), fd, strerror(errno));
  }

  void itti_unsubscribe_event_fd(task_id_t task_id, int fd) {
    task_list_t *t=&tasks[task_id];
    AssertFatal (epoll_ctl(t->epoll_fd, EPOLL_CTL_DEL, fd, NULL) == 0,
                 "epoll_ctl (EPOLL_CTL_DEL) failed for task %s, fd %d: %s!\n",
                 itti_get_task_name(task_id), fd, strerror(errno));
    t->nb_fd_epoll--;
  }

  static inline int itti_get_events_locked(task_id_t task_id, struct epoll_event **events) {
    task_list_t *t=&tasks[task_id];
    uint64_t current_time=0;

    do {
      if ( t->next_timer != UINT64_MAX ) {
        struct timespec tp;
        clock_gettime(CLOCK_MONOTONIC, &tp);
        current_time=(uint64_t)tp.tv_sec*1000+tp.tv_nsec/(1000*1000);

        if ( t->next_timer < current_time) {
          t->next_timer=UINT64_MAX;

          // Proceed expired timer
          for ( auto it=t->timer_map.begin() ; it != t->timer_map.end() ; ++it ) {
            if ( it->second.timeout < current_time ) {
              MessageDef *message = itti_alloc_new_message(TASK_TIMER, TIMER_HAS_EXPIRED);
              message->ittiMsg.timer_has_expired.timer_id=it->first;
              message->ittiMsg.timer_has_expired.arg=it->second.timer_arg;

              if (itti_send_msg_to_task_locked(task_id, it->second.instance, message) < 0) {
                LOG_W(TMR,"Failed to send msg TIMER_HAS_EXPIRED to task %u\n", task_id);
                free(message);
                t->timer_map.erase(it);
                return -1;
              }

              if ( it->second.type==TIMER_PERIODIC ) {
                it->second.timeout+=it->second.duration;

                if (it->second.timeout < t->next_timer)
                  t->next_timer=it->second.timeout;
              } else
                t->timer_map.erase(it);
            } else if (it->second.timeout < t->next_timer)
              t->next_timer=it->second.timeout;
          }
        }
      }

      int epoll_timeout = -1;

      if ( t->next_timer != UINT64_MAX )
        epoll_timeout = t->next_timer-current_time;

      pthread_mutex_unlock(&t->queue_cond_lock);
      LOG_D(TMR,"enter blocking wait for %s\n", itti_get_task_name(task_id));
      t->nb_events = epoll_wait(t->epoll_fd,t->events,t->nb_fd_epoll, epoll_timeout);
      if ( t->nb_events  < 0 && (errno == EINTR || errno == EAGAIN ) )
	pthread_mutex_lock(&t->queue_cond_lock);
    } while (t->nb_events  < 0 && (errno == EINTR || errno == EAGAIN ) );

    AssertFatal (t->nb_events >=0,
                 "epoll_wait failed for task %s, nb fds %d, timeout %lu: %s!\n",
                 itti_get_task_name(task_id), t->nb_fd_epoll, t->next_timer != UINT64_MAX ? t->next_timer-current_time : -1, strerror(errno));
    LOG_D(TMR,"receive on %d descriptors for %s\n", t->nb_events, itti_get_task_name(task_id));

    if (t->nb_events == 0)
      /* No data to read -> return */
      return 0;

    for (int i = 0; i < t->nb_events; i++) {
      /* Check if there is an event for ITTI for the event fd */
      if ((t->events[i].events & EPOLLIN) &&
          (t->events[i].data.fd == t->sem_fd)) {
        eventfd_t   sem_counter;
        /* Read will always return 1 */
        AssertFatal( sizeof(sem_counter) == read (t->sem_fd, &sem_counter, sizeof(sem_counter)), "");
        /* Mark that the event has been processed */
        t->events[i].events &= ~EPOLLIN;
      }
    }

    *events = t->events;
    return t->nb_events;
  }

  int itti_get_events(task_id_t task_id, struct epoll_event **events) {
    pthread_mutex_lock(&tasks[task_id].queue_cond_lock);
    return itti_get_events_locked(task_id, events);
  }

  void itti_receive_msg(task_id_t task_id, MessageDef **received_msg) {
    // Reception of one message, blocking caller
    task_list_t *t=&tasks[task_id];
    pthread_mutex_lock(&t->queue_cond_lock);

    // Weird condition to deal with crap legacy itti interface
    if ( t->nb_fd_epoll == 1 ) {
      while (t->message_queue.empty()) {
        itti_get_events_locked(task_id, &t->events);
        pthread_mutex_lock(&t->queue_cond_lock);
      }
    } else {
      if (t->message_queue.empty()) {
        itti_get_events_locked(task_id, &t->events);
        pthread_mutex_lock(&t->queue_cond_lock);
      }
    }

    // Legacy design: we return even if we have no message
    // in this case, *received_msg is NULL
    if (t->message_queue.empty()) {
      *received_msg=NULL;
      LOG_D(TMR,"task %s received even from other fd (total fds: %d), returning msg NULL\n",t->admin.name, t->nb_fd_epoll);
    } else {
      *received_msg=t->message_queue.back();
      t->message_queue.pop_back();
      LOG_D(TMR,"task %s received a message\n",t->admin.name);
    }

    pthread_mutex_unlock (&t->queue_cond_lock);
  }

  void itti_poll_msg(task_id_t task_id, MessageDef **received_msg) {
    //reception of one message, non-blocking
    task_list_t *t=&tasks[task_id];
    pthread_mutex_lock(&t->queue_cond_lock);

    if (!t->message_queue.empty()) {
      LOG_D(TMR,"task %s received a message in polling mode\n",t->admin.name);
      *received_msg=t->message_queue.back();
      t->message_queue.pop_back();
    } else
      *received_msg=NULL;

    pthread_mutex_unlock (&t->queue_cond_lock);
  }

  int itti_create_task(task_id_t task_id, void *(*start_routine)(void *), void *args_p) {
    task_list_t *t=&tasks[task_id];
    AssertFatal ( pthread_create (&t->thread, NULL, start_routine, args_p) ==0,
                  "Thread creation for task %d failed!\n", task_id);
    pthread_setname_np( t->thread, itti_get_task_name(task_id) );
    LOG_I(TMR,"Created Posix thread %s\n",  itti_get_task_name(task_id) );
#if 1 // BMC test RT prio
    {
      int policy;
      struct sched_param sparam;
      memset(&sparam, 0, sizeof(sparam));
      sparam.sched_priority = sched_get_priority_max(SCHED_FIFO)-10;
      policy = SCHED_FIFO ;
      if (pthread_setschedparam(t->thread, policy, &sparam) != 0) {
	LOG_E(TMR,"task %s : Failed to set pthread priority\n",  itti_get_task_name(task_id) );
      }
    }
#endif
    return 0;
  }

  void itti_exit_task(void) {
    pthread_exit (NULL);
  }

  void itti_terminate_tasks(task_id_t task_id) {
    // Sends Terminate signals to all tasks.
    itti_send_terminate_message (task_id);
    usleep(100*1000); // Allow the tasks to receive the message before going returning to main thread
  }

  int itti_init(task_id_t task_max, thread_id_t thread_max, MessagesIds messages_id_max, const task_info_t *tasks_info,
                const message_info_t *messages_info) {
    AssertFatal(TASK_MAX<UINT16_MAX, "Max itti tasks");

    for(int i=0; i<task_max; ++i) {
      LOG_I(TMR,"Starting itti queue: %s as task %d\n", tasks_info[i].name, i);
      pthread_mutex_init(&tasks[i].queue_cond_lock, NULL);
      memcpy(&tasks[i].admin, &tasks_info[i], sizeof(task_info_t));
      AssertFatal( ( tasks[i].epoll_fd = epoll_create1(0) ) >=0, "");
      AssertFatal( ( tasks[i].sem_fd = eventfd(0, EFD_SEMAPHORE) ) >=0, "");
      itti_subscribe_event_fd((task_id_t)i, tasks[i].sem_fd);

      if (tasks[i].admin.threadFunc != NULL)
        itti_create_task((task_id_t)i, tasks[i].admin.threadFunc, NULL);
    }

    return 0;
  }

  int timer_setup(
    uint32_t      interval_sec,
    uint32_t      interval_us,
    task_id_t     task_id,
    int32_t       instance,
    timer_type_t  type,
    void         *timer_arg,
    long         *timer_id) {
    task_list_t *t=&tasks[task_id];

    do {
      // set the taskid in the timer id to keep compatible with the legacy API
      // timer_remove() takes only the timer id as parameter
      *timer_id=(random()%UINT16_MAX) << 16 | task_id ;
    } while ( t->timer_map.find(*timer_id) != t->timer_map.end());

    /* Allocate new timer list element */
    timer_elm_t timer;
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);

    if (interval_us%1000 != 0)
      LOG_W(TMR, "Can't set timer precision below 1ms, rounding it\n");

    timer.duration  = interval_sec*1000+interval_us/1000;
    timer.timeout= ((uint64_t)tp.tv_sec*1000+tp.tv_nsec/(1000*1000)+timer.duration);
    timer.instance  = instance;
    timer.type      = type;
    timer.timer_arg = timer_arg;
    pthread_mutex_lock (&t->queue_cond_lock);
    t->timer_map[*timer_id]= timer;

    if (timer.timeout < t->next_timer)
      t->next_timer=timer.timeout;

    eventfd_t sem_counter = 1;
    AssertFatal ( sizeof(sem_counter) == write(t->sem_fd, &sem_counter, sizeof(sem_counter)), "");
    pthread_mutex_unlock (&t->queue_cond_lock);
    return 0;
  }

  int timer_remove(long timer_id) {
    task_id_t task_id=(task_id_t)(timer_id&0xffff);
    int ret;
    pthread_mutex_lock (&tasks[task_id].queue_cond_lock);
    ret=tasks[task_id].timer_map.erase(timer_id);
    pthread_mutex_unlock (&tasks[task_id].queue_cond_lock);

    if (ret==1)
      return 0;
    else {
      LOG_W(TMR, "tried to remove a non existing timer\n");
      return 1;
    }
  }

  const char *itti_get_message_name(MessagesIds message_id) {
    return messages_info[message_id].name;
  }

  const char *itti_get_task_name(task_id_t task_id) {
    return tasks[task_id].admin.name;
  }

  // void for compatibility
  void itti_send_terminate_message(task_id_t task_id) {
  }

  void itti_wait_tasks_end(void) {
    while(1)
      sleep(24*3600);
  }

  void itti_update_lte_time(uint32_t frame, uint8_t slot) {}
  void itti_set_task_real_time(task_id_t task_id) {}
  void itti_mark_task_ready(task_id_t task_id) {
    // Function meaning is clear, but legacy implementation is wrong
    // keep it void is fine: today implementation accepts messages in the queue before task is ready
  }
  void itti_wait_ready(int wait_tasks) {
    // Stupid function, kept for compatibility (the parameter is meaningless!!!)
  }
  int signal_mask(void) { return 0;}
}

#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

typedef uint32_t u32;
typedef int32_t i32;

struct process
{
  u32 pid;
  u32 arrival_time;
  u32 burst_time;
  u32 time_left; 
  u32 start_time; 
  bool started;

  TAILQ_ENTRY(process) pointers;
};

TAILQ_HEAD(process_list, process);

u32 next_int(const char **data, const char *data_end)
{
  u32 current = 0;
  bool started = false;
  while (*data != data_end)
  {
    char c = **data;

    if (c < 0x30 || c > 0x39)
    {
      if (started)
      {
        return current;
      }
    }
    else
    {
      if (!started)
      {
        current = (c - 0x30);
        started = true;
      }
      else
      {
        current *= 10;
        current += (c - 0x30);
      }
    }

    ++(*data);
  }

  printf("Reached end of file while looking for another integer\n");
  exit(EINVAL);
}

u32 next_int_from_c_str(const char *data)
{
  char c;
  u32 i = 0;
  u32 current = 0;
  bool started = false;
  while ((c = data[i++]))
  {
    if (c < 0x30 || c > 0x39)
    {
      exit(EINVAL);
    }
    if (!started)
    {
      current = (c - 0x30);
      started = true;
    }
    else
    {
      current *= 10;
      current += (c - 0x30);
    }
  }
  return current;
}

void init_processes(const char *path,
                    struct process **process_data,
                    u32 *process_size)
{
  int fd = open(path, O_RDONLY);
  if (fd == -1)
  {
    int err = errno;
    perror("open");
    exit(err);
  }

  struct stat st;
  if (fstat(fd, &st) == -1)
  {
    int err = errno;
    perror("stat");
    exit(err);
  }

  u32 size = st.st_size;
  const char *data_start = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (data_start == MAP_FAILED)
  {
    int err = errno;
    perror("mmap");
    exit(err);
  }

  const char *data_end = data_start + size;
  const char *data = data_start;

  *process_size = next_int(&data, data_end);

  *process_data = calloc(sizeof(struct process), *process_size);
  if (*process_data == NULL)
  {
    int err = errno;
    perror("calloc");
    exit(err);
  }

  for (u32 i = 0; i < *process_size; ++i)
  {
    (*process_data)[i].pid = next_int(&data, data_end);
    (*process_data)[i].arrival_time = next_int(&data, data_end);
    (*process_data)[i].burst_time = next_int(&data, data_end);
    (*process_data)[i].time_left = (*process_data)[i].burst_time;
    (*process_data)[i].started = false;
  }

  munmap((void *)data, size);
  close(fd);
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    return EINVAL;
  }
  struct process *data;
  u32 size;
  init_processes(argv[1], &data, &size);

  u32 quantum_length = next_int_from_c_str(argv[2]);

  struct process_list list;
  TAILQ_INIT(&list);

  u32 total_waiting_time = 0;
  u32 total_response_time = 0;

  // add processes to new list and sort based on arrival time
  for (u32 i = 0; i < size; i++)
  {
    struct process* current_process = &data[i];
    struct process* next_process = TAILQ_FIRST(&list); // start from the beginning of the list

    while (next_process != NULL) {
      // compare current_process with next_process
      if (current_process->arrival_time < next_process->arrival_time) {
        TAILQ_INSERT_BEFORE(next_process, current_process, pointers);
        break; 
      }
      next_process = TAILQ_NEXT(next_process, pointers);
    }

    // if we reached the end of the list, insert at the tail
    if (next_process == NULL) {
      TAILQ_INSERT_TAIL(&list, current_process, pointers);
    }
  }

  u32 current_time = 0;
  struct process* previous_task = NULL;
  // create queue for tasks that have arrived
  struct process_list queue;
  TAILQ_INIT(&queue);

  // loop through the processes if either the queue/list has something or previous_task != NULL
  while (!(TAILQ_EMPTY(&list) && TAILQ_EMPTY(&queue) && previous_task == NULL))
  {
    // while list isn't empty, check if the earliest process in the list has arrived
    while (!TAILQ_EMPTY(&list))
    {
      struct process* p = TAILQ_FIRST(&list);
      if (p->arrival_time <= current_time)
      {
        // remove process from list and add it to the end of the queue
        TAILQ_REMOVE(&list, p, pointers);
        TAILQ_INSERT_TAIL(&queue, p, pointers);
      }
      else
      {
        break;
      }
    }

    // move previous task to the end of queue
    if (previous_task != NULL)
    {
      TAILQ_INSERT_TAIL(&queue, previous_task, pointers);
    }
    
    // if the queue is not empty
    if (!TAILQ_EMPTY(&queue))
    {
      // execute (remove) the first process
      struct process* current_task = TAILQ_FIRST(&queue);
      TAILQ_REMOVE(&queue, current_task, pointers);

      // if the current task just started
      if (!current_task->started)
      {
        // update the current task's start values and overall total_response_time
        current_task->start_time = current_time;
        current_task->started = true;
        total_response_time += current_time - current_task->arrival_time; // response time = start time (current time) - arrival time
      }

      // if the current task's time_left > q
      if (current_task->time_left > quantum_length)
      {
        // run the current task for quantum_length
        current_task->time_left -= quantum_length;
        current_time = current_time + quantum_length - 1;
        previous_task = current_task;
      }

      // if the current task's time_left <= q (finishing a process)
      else
      {
        // run the current task for time_left and update total_waiting_time
        total_waiting_time += ((current_time + current_task->time_left) - current_task->burst_time - current_task->arrival_time); // waiting time = end_time - burst_time - arrival_time; end_time = current_time + time_left
        current_time = current_time + current_task->time_left - 1;
        previous_task = NULL;
      }
    }

    // increment current_time even if there are no processes in the queue
    current_time += 1;
  }

  printf("Average waiting time: %.2f\n", (float)total_waiting_time / (float)size);
  printf("Average response time: %.2f\n", (float)total_response_time / (float)size);

  free(data);
  return 0;
}

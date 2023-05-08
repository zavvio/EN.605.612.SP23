#ifndef __HOMEWORK_H
#define __HOMEWORK_H

/** The homework message. */
#define HOMEWORK_MESSAGE "Hello, Homework!\n"

typedef struct read_request {
  /* Information about incomplete I/O requests is stored here. */
  endpoint_t caller;    /* process that made the call, or NONE */
  cdev_id_t id;		    /* ID of suspended read request */
  cp_grant_id_t grant;	/* grant where data is to go */
  size_t size;          /* size of data to be read */
  int32_t slot_index;   /* index to slot that must be read */
} read_request_t;

#endif /* __HOMEWORK_H */

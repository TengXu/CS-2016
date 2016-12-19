#ifndef __TEAM_H__
#define __TEAM_H__

typedef struct {
  char* teamname; /* ID1+ID2 or ID1 */
  char* name1; /* full name of first member */
  char* id1; /* login ID of first member */
  char* name2; /* full name of second member (if any) */
  char* id2; /* login ID of second member */
} team_t;

extern team_t team;

#endif

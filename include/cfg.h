#ifndef _EKANS_CFG_H_
#define _EKANS_CFG_H_

typedef struct sytem_configs {
   int n_threads;
   const char * port;
}system_configs;

void parse_args(int argc, char * argv[]);

extern system_configs sys_cfg;

#endif /* cfg.h */

#ifndef MODULE_H
#define MODULE_H


typedef struct module_ {
  int features;
} module_t;


typedef struct param_ {
  int nb_dct;
} param_t;
  

struct module_extraction {
  int features;
  int type;
  int iteration;
  int (*init) (void);
  int (*reset) (void);
  int (*compute) (void);
  int (*get_features) (void);
  int data;
};

#endif

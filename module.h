typedef struct module_ {
  int features;
} module_t;


struct module_extraction {
  int features;
  int type;
  int iteration;
  int (*init) (void);
  int (*reset) (void);
  int (*compute) (void);
  int (*extract_features) (void);
  int data;
};

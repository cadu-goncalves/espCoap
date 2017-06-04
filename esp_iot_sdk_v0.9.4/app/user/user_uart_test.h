#include "c_stdint.h"

#define START_OF_PKT  'a'
#define END_OF_PKT  'a'

#define MAX_DATA_SIZE   100



enum {
      RX_SOP=1, RX_DATA, RX_EOP, RX_NULL,
};
